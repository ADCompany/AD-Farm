#ifndef GUI_STORAGES_WIDGET_H
#define GUI_STORAGES_WIDGET_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef DB_MANAGER_H
#	include "db_manager.h"
#endif
#ifndef DB_STORAGES_DATA_H
#	include "db_storagesdata.h"
#endif
#ifndef GUI_ADD_STORE_ITEM_DLG_H
#	include "gui_addstoreitem_dlg.h"
#endif
#ifndef GUI_SUBTRACT_STORE_ITEM_DLG_H
#	include "gui_subtractstoreitem_dlg.h"
#endif
#ifndef GUI_ADD_FARM_COSTS_DLG_H
#	include "gui_addfarmcostsdlg.h"
#endif
#ifndef GUI_ADD_STORAGE_COSTS_DLG_H
#	include "gui_addstoragecostsdlg.h"
#endif

// Ui
#include "ui_storages.h"
#include "ui_addstoreitem.h"
#include "ui_subtractproduct.h"

// Qt includes
#include <QWidget>
#include <QStringListModel>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesWidget
//
class CStoragesWidget : public QWidget
{
	Q_OBJECT
public:// Constructors
	CStoragesWidget(QWidget* pwParent = nullptr, std::shared_ptr<db::CDBManager> pDBManager = nullptr);
	~CStoragesWidget() = default;

public:// Interface Methodes
	void SetDBManager(std::shared_ptr<db::CDBManager> pDBManager);
	inline std::shared_ptr<QSqlTableModel> GetTableModel();

protected:// Helper Methodes
	inline void UpdateData(bool bFull = false);


protected slots:// Slots
	void onChangeData()
	{
		UpdateData(true);
	}
	void onActivatedStorage(QModelIndex const& modelIndex)
	{
		QString strSelectCustomerName = m_pStringListModel->data(modelIndex, 0).toString();
		if (strSelectCustomerName == m_strCurrentStorageName)
			return;

		m_strCurrentStorageName = strSelectCustomerName;

		m_uiStorages.btnAddItem->setEnabled(true);
		m_uiStorages.btnAddStorageCosts->setEnabled(true);
		m_uiStorages.btnSubItem->setDisabled(true);

		UpdateData();
	}
	void onAddItem()
	{
		t_lstProductPriceInfo productInfo = m_pAddItemDlg->GetProductInfo();
		QList<QString> lstProductName;
		QList<int> lstProductCount;
		QList<double> lstProductCost;
		for (int i = 0; i < productInfo.count(); ++i)
		{
			lstProductName.push_back(productInfo[i].sProductName);
			lstProductCount.push_back(productInfo[i].nCount);
			lstProductCost.push_back(productInfo[i].fPrice);
		}

		m_pStoragesData->AddProductInStorage(m_strCurrentStorageName, lstProductName, lstProductCount, lstProductCost);
	}
	void onSelectProduct(QModelIndex const& modelIndex)
	{
		m_currModelIndex = modelIndex;
		m_uiStorages.btnSubItem->setEnabled(true);
	}
	void onSubItem()
	{
		int nRow = m_currModelIndex.row();
		int nCount = m_pSubtractItemDlg->GetSutractionCount();
		QSqlRecord record = m_pStoragesData->GetSqlTableModelByStorageName(m_strCurrentStorageName).get()->record(nRow);
		QString strProductName = record.value(0).toString();

		m_pStoragesData->SubstractProductInStorage(m_strCurrentStorageName, strProductName, nCount);
		UpdateData();
	}
	//
	void onAddItemClicked();
	void onSubItemClicked()
	{
		int nRow = m_currModelIndex.row();

		QSqlRecord record = m_pStoragesData->GetSqlTableModelByStorageName(m_strCurrentStorageName).get()->record(nRow);
		int nCount = record.value(1).toInt();

		m_pSubtractItemDlg = std::shared_ptr<CSubtractStoreItem>(new CSubtractStoreItem(nCount, this));
		FM_CONNECT(m_pSubtractItemDlg.get(), accepted(), this, onSubItem());

		m_pSubtractItemDlg->show();
	}
	void onAddStorageCostsClicked()
	{
		m_pAddStorageCostsDlg = std::shared_ptr<CAddStorageCostsDlg>(new CAddStorageCostsDlg(this));
		FM_CONNECT(m_pAddStorageCostsDlg.get(), accepted(), this, onAddStorageCosts());

		m_pAddStorageCostsDlg->show();
	}
	void onAddStorageCosts()
	{
		double dCosts = m_pAddStorageCostsDlg->GetCosts();
		m_pStoragesData->AddStoragesCosts(m_strCurrentStorageName, dCosts);

		UpdateData();
	}
	void onAddFarmCostsClicked()
	{
		m_pAddFarmCostsDlg = std::shared_ptr<CAddFarmCostsDlg>(new CAddFarmCostsDlg(this));
		FM_CONNECT(m_pAddFarmCostsDlg.get(), accepted(), this, onAddFarmCosts());

		m_pAddFarmCostsDlg->show();
	}
	void onAddFarmCosts()
	{
		double dCosts = m_pAddFarmCostsDlg->GetCosts();
		m_pStoragesData->AddFarmCosts(dCosts);

		UpdateData();
	}

private:
	Ui::storages m_uiStorages;

	//QDialog* m_pCreateSplDlg;
	std::shared_ptr<CAddStoreItem>			m_pAddItemDlg;
	std::shared_ptr<CSubtractStoreItem>		m_pSubtractItemDlg;
	std::shared_ptr<CAddFarmCostsDlg>		m_pAddFarmCostsDlg;
	std::shared_ptr<CAddStorageCostsDlg>	m_pAddStorageCostsDlg;

	std::shared_ptr<db::CStoragesData> m_pStoragesData;
	std::shared_ptr<db::CDBManager> m_pDBManager;

	std::shared_ptr<QStringListModel> m_pStringListModel;

	QModelIndex m_currModelIndex;

	QString m_strCurrentStorageName;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesWidget
//

// Interface Methodes
inline std::shared_ptr<QSqlTableModel> CStoragesWidget::GetTableModel()
{
	return nullptr;
}

// Helper Functions
inline void CStoragesWidget::UpdateData(bool bFull)
{
	if (bFull)
	{
		m_pStringListModel = std::shared_ptr<QStringListModel>(new QStringListModel(m_pStoragesData->GetStorageNames()));

		m_uiStorages.listView->setModel(m_pStringListModel.get());
		m_uiStorages.listView->update();
	}

	if (m_strCurrentStorageName == "")
		return;

	m_uiStorages.tableView->setModel(m_pStoragesData->GetSqlTableModelByStorageName(m_strCurrentStorageName).get());
	m_uiStorages.tableView->update();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_STORAGES_WIDGET_H