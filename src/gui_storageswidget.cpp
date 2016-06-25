////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_storageswidget.h"

// Qt Includes

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesWidget
//

// Constructors
CStoragesWidget::CStoragesWidget(QWidget* pwParent, std::shared_ptr<db::CDBManager> pDBManager)
	: QWidget(pwParent),
	m_pStringListModel(nullptr),
	m_strCurrentStorageName(""),
	m_pDeclineItemDlg(nullptr),
	m_pAddFarmCostsDlg(nullptr),
	m_pAddStorageCostsDlg(nullptr),
	m_pAddItemDlg(nullptr)
{
	m_uiStorages.setupUi(this);
	m_uiStorages.splitter->setStretchFactor(0, 1);
	m_uiStorages.splitter->setStretchFactor(1, 3);

	m_pMoveStoreItemDlg = std::shared_ptr<CMoveStoreItemDlg>(new CMoveStoreItemDlg (QStringList(), this));
	FM_CONNECT(m_pMoveStoreItemDlg.get(), sigMoveStoreItem(SItemMovingInfo const&), this, onMoveStoreItem(SItemMovingInfo const&));
	m_pNourishStoreItemDlg = std::shared_ptr<CNourishStoreItemDlg>(new CNourishStoreItemDlg(QStringList(), this));
	FM_CONNECT(m_pNourishStoreItemDlg.get(), sigMoveStoreItem(SItemNourishInfo const&), this, onNourishStoreItem(SItemNourishInfo const&));

	m_uiStorages.btnAddItem->setDisabled(true);
	m_uiStorages.btnDecline->setDisabled(true);
	m_uiStorages.btnAddStorageCosts->setDisabled(true);
	FM_CONNECT(m_uiStorages.listView, clicked(QModelIndex const&), this, onSelectStorage(QModelIndex const&));
	FM_CONNECT(m_uiStorages.tableView, clicked(QModelIndex const&), this, onSelectProduct(QModelIndex const&));
	FM_CONNECT(m_uiStorages.btnAddItem, clicked(), this, onAddItemClicked());
	FM_CONNECT(m_uiStorages.btnDecline, clicked(), this, onDeclineItemClicked());
	FM_CONNECT(m_uiStorages.btnAddFarmCosts, clicked(), this, onAddFarmCostsClicked());
	FM_CONNECT(m_uiStorages.btnAddStorageCosts, clicked(), this, onAddStorageCostsClicked());
	FM_CONNECT(m_uiStorages.btnMoveStoreItem, clicked(), this, onShowMoveStoreItemDialog());
	FM_CONNECT(m_uiStorages.btnNourishStoreItem, clicked(), this, onShowNourishStoreItemDialog());
	
	SetDBManager(pDBManager);

	m_uiStorages.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

// Interface Methodes
void CStoragesWidget::SetDBManager(std::shared_ptr<db::CDBManager> pDBManager)
{
	if (pDBManager == nullptr)
		return;

	m_pStoragesData = std::static_pointer_cast<db::CStoragesData>(pDBManager->GetDBComponent(db::CDBManager::component::storages));
	FM_CONNECT(m_pStoragesData.get(), sigChangeData(), this, onChangeData());

	UpdateData(true);
}

// Public Slots
void CStoragesWidget::onShowMoveStoreItemDialog()
{
	m_pMoveStoreItemDlg->Clear();
	m_pMoveStoreItemDlg->AddStorageNames(m_pStoragesData->GetStorageNames());
	m_pMoveStoreItemDlg->show();
}

void CStoragesWidget::onShowNourishStoreItemDialog()
{
	m_pNourishStoreItemDlg->Clear();
	m_pNourishStoreItemDlg->AddStorageNames(m_pStoragesData->GetStorageNames());
	m_pNourishStoreItemDlg->show();
}

// Helper Functions
void CStoragesWidget::UpdateData(bool bFull)
{
	if (bFull)
	{
		m_pStringListModel = std::shared_ptr<QStringListModel>(new QStringListModel(m_pStoragesData->GetStorageNames()));

		m_uiStorages.listView->setModel(m_pStringListModel.get());
		m_uiStorages.listView->update();
	}

	if (m_strCurrentStorageName == "")
		return;

	m_uiStorages.lineEditGroupName->setText(m_pStoragesData->GetLastGroupNameByStorage(m_strCurrentStorageName)); // ??
	m_uiStorages.tableView->setModel(m_pStoragesData->GetSqlTableModelByStorageName(m_strCurrentStorageName).get());
	m_uiStorages.tableView->resizeColumnsToContents();
	m_uiStorages.tableView->update();
	m_pMoveStoreItemDlg->Update();
	m_pNourishStoreItemDlg->Update();
}

// Protected Slots
void CStoragesWidget::onSelectStorage(QModelIndex const& modelIndex)
{
	QString strSelectCustomerName = m_pStringListModel->data(modelIndex, 0).toString();
	if (strSelectCustomerName == m_strCurrentStorageName)
		return;

	m_strCurrentStorageName = strSelectCustomerName;

	m_uiStorages.btnAddItem->setEnabled(true);
	m_uiStorages.btnAddStorageCosts->setEnabled(true);
	m_uiStorages.btnDecline->setDisabled(true);

	UpdateData();
}

void CStoragesWidget::onAddItem()
{
	t_lstProductPriceInfo productInfo = m_pAddItemDlg->GetProductInfo();
	QString strGroupName = m_pAddItemDlg->GetGroupName();
	QStringList strListGroupNames = m_pStoragesData->GetGroupNames();
	if (!strListGroupNames.contains(strGroupName))
		m_pStoragesData->AddNewGroup(m_strCurrentStorageName, strGroupName);

	QList<QString> lstProductName;
	QList<int> lstProductCount;
	QList<double> lstProductCost;
	for (int i = 0; i < productInfo.count(); ++i)
	{
		lstProductName.push_back(productInfo[i].sProductName);
		lstProductCount.push_back(productInfo[i].nCount);
		lstProductCost.push_back(productInfo[i].fPrice);
	}

	m_pStoragesData->AddProductInStorage(m_strCurrentStorageName, strGroupName, lstProductName, lstProductCount, lstProductCost,
		QString::fromUtf8("\324\261\325\276\325\245\325\254\325\241\326\201\325\245\325\254 \325\247"));
}

void CStoragesWidget::onSelectProduct(QModelIndex const& modelIndex)
{
	m_currModelIndex = modelIndex;
	m_uiStorages.btnDecline->setEnabled(true);
}

void CStoragesWidget::onMoveStoreItem(SItemMovingInfo const& itemMovingInfo)
{
	QList<QString> lstproductName;
	lstproductName.push_back(itemMovingInfo.sProductName);

	QList<int> lstproductCount;
	lstproductCount.push_back(itemMovingInfo.nProductCount);

	m_pStoragesData->MoveProductFromStorageInStorage(itemMovingInfo.sTargetStoreName, m_pStoragesData->GetLastGroupNameByStorage(itemMovingInfo.sTargetStoreName),
		itemMovingInfo.sSourceStoreName, m_pStoragesData->GetLastGroupNameByStorage(itemMovingInfo.sSourceStoreName), lstproductName, lstproductCount,
		QString::fromUtf8("\324\262\325\245\326\200\325\276\325\245\325\254 \325\247 ") + itemMovingInfo.sSourceStoreName + QString::fromUtf8("-\325\253\326\201"),
		QString::fromUtf8("\325\217\325\245\325\262\325\241\326\203\325\270\325\255\325\276\325\245\325\254 \325\247 ") + itemMovingInfo.sTargetStoreName);
}

void CStoragesWidget::onNourishStoreItem(SItemNourishInfo const& itemMovingInfo)
{
	QList<QString> lstproductName;
	lstproductName.push_back(itemMovingInfo.sProductName);

	QList<int> lstproductCount;
	lstproductCount.push_back(itemMovingInfo.nProductCount);

	m_pStoragesData->NourishProductFromStorageToStorage(itemMovingInfo.sTargetStoreName, m_pStoragesData->GetLastGroupNameByStorage(itemMovingInfo.sTargetStoreName),
		itemMovingInfo.sSourceStoreName, m_pStoragesData->GetLastGroupNameByStorage(itemMovingInfo.sSourceStoreName), lstproductName, lstproductCount,
		QString::fromUtf8("\324\277\325\245\326\200\325\241\325\257\326\200\325\276\325\245\325\254 \325\247 ") + itemMovingInfo.sSourceStoreName + QString::fromUtf8("-\325\253\326\201"),
		QString::fromUtf8("\324\277\325\245\326\200\325\241\325\257\326\200\325\276\325\245\325\254 \325\247 ") + itemMovingInfo.sTargetStoreName + QString::fromUtf8("-\325\250 "));
}

void CStoragesWidget::onDeclineItem()
{
	int nRow = m_currModelIndex.row();
	int nCount = m_pDeclineItemDlg->GetDeclineCount();
	QSqlRecord record = m_pStoragesData->GetSqlTableModelByStorageName(m_strCurrentStorageName).get()->record(nRow);
	QString strProductName = record.value(0).toString();

	m_pStoragesData->DeclineProductInStorage(m_strCurrentStorageName, m_pStoragesData->GetLastGroupNameByStorage(m_strCurrentStorageName),
		strProductName, nCount, QString::fromUtf8("\324\261\325\266\325\257\325\270\326\202\325\264"));

	UpdateData();
}

void CStoragesWidget::onAddItemClicked() // BAD Solution
{
	m_pAddItemDlg = std::shared_ptr<CAddStoreItem>(new CAddStoreItem(m_pStoragesData->GetProductNames(), this));
	m_pAddItemDlg->AddGroupNames(m_pStoragesData->GetGroupNamesByStorage(m_strCurrentStorageName));

	FM_CONNECT(m_pAddItemDlg.get(), accepted(), this, onAddItem());

	m_pAddItemDlg->show();
}

void CStoragesWidget::onDeclineItemClicked()
{
	int nRow = m_currModelIndex.row();

	QSqlRecord record = m_pStoragesData->GetSqlTableModelByStorageName(m_strCurrentStorageName).get()->record(nRow);
	int nCount = record.value(1).toInt();

	m_pDeclineItemDlg = std::shared_ptr<CDeclineStoreItem>(new CDeclineStoreItem(nCount, this));
	FM_CONNECT(m_pDeclineItemDlg.get(), accepted(), this, onDeclineItem());

	m_pDeclineItemDlg->show();
}

void CStoragesWidget::onAddStorageCostsClicked()
{
	m_pAddStorageCostsDlg = std::shared_ptr<CAddStorageCostsDlg>(new CAddStorageCostsDlg(this));
	FM_CONNECT(m_pAddStorageCostsDlg.get(), accepted(), this, onAddStorageCosts());

	m_pAddStorageCostsDlg->show();
}

void CStoragesWidget::onAddStorageCosts()
{
	double dCosts = m_pAddStorageCostsDlg->GetCosts();
	QString strInfoText = m_pAddStorageCostsDlg->GetInfoText();

	m_pStoragesData->AddStoragesCosts(m_strCurrentStorageName, m_uiStorages.lineEditGroupName->text(), dCosts,
		QString(m_strCurrentStorageName + QString::fromUtf8(" \325\272\325\241\325\260\325\270\326\201\325\253 ") +
		QString::fromUtf8("\325\256\325\241\325\255\325\275: ")), strInfoText);

	UpdateData();
}

void CStoragesWidget::onAddFarmCostsClicked()
{
	m_pAddFarmCostsDlg = std::shared_ptr<CAddFarmCostsDlg>(new CAddFarmCostsDlg(this));
	FM_CONNECT(m_pAddFarmCostsDlg.get(), accepted(), this, onAddFarmCosts());

	m_pAddFarmCostsDlg->show();
}

void CStoragesWidget::onAddFarmCosts()
{
	double dCosts = m_pAddFarmCostsDlg->GetCosts();
	QString strInfoText = m_pAddFarmCostsDlg->GetInfoText();
	m_pStoragesData->AddFarmCosts(dCosts, QString::fromUtf8("\325\226\325\245\326\200\325\264\325\241\325\253 \325\256\325\241\325\255\325\275: ") + strInfoText);

	UpdateData();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
