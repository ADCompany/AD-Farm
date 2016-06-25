////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_settingswidget.h"

// Qt Includes

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CSettingsWidget
//

// Constructors
CSettingsWidget::CSettingsWidget(QWidget* pwParent, std::shared_ptr<db::CDBManager> pDBManager)
	: QWidget(pwParent),
	m_pStoragesData(nullptr),
	m_pAddProductDlg(new CAddProductDlg(this)),
	m_pProductMenu(new QMenu("productMenu", this)),
	m_pStorageMenu(new QMenu("storageMenu", this))
{
	m_uiSettingsWidget.setupUi(this);

	SetDBManager(pDBManager);

	QAction* pProductMenuAction = new QAction(QString::fromUtf8("\325\200\325\245\325\274\325\241\326\201\325\266\325\245\325\254"), this);
	m_pProductMenu->addAction(pProductMenuAction);

	QAction* pStorageMenuAction = new QAction(QString::fromUtf8("\325\200\325\245\325\274\325\241\326\201\325\266\325\245\325\254"), this);
	m_pStorageMenu->addAction(pStorageMenuAction);

	// Setup Add Storage Dialog
	m_pAddStoreDlg = std::shared_ptr<CAddStoreDlg>(new CAddStoreDlg(this));

	FM_CONNECT( m_uiSettingsWidget.btnAddProduct, clicked(), this, onShowAddProductDlg() );
	FM_CONNECT( m_uiSettingsWidget.btnAddStore, clicked(), this, onShowAddStoreDlg() );

	FM_CONNECT(m_pAddProductDlg.get(), accepted(), this, onAddNewProduct());
	FM_CONNECT(m_pAddStoreDlg.get(), accepted(), this, onAddNewStore());

	FM_CONNECT(m_uiSettingsWidget.tableView, customContextMenuRequested(QPoint const&), this, onProductMenuShow(QPoint const&));
	FM_CONNECT(m_uiSettingsWidget.tableView_2, customContextMenuRequested(QPoint const&), this, onStorageMenuShow(QPoint const&));

	FM_CONNECT(pProductMenuAction, triggered(bool), this, onRemoveProduct());
	FM_CONNECT(pStorageMenuAction, triggered(bool), this, onRemoveStorage());

	m_uiSettingsWidget.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void CSettingsWidget::SetDBManager(std::shared_ptr<db::CDBManager> pDBManager)
{
	if (pDBManager == nullptr)
		return;

	m_pStoragesData = std::static_pointer_cast<db::CStoragesData>(pDBManager->GetDBComponent(db::CDBManager::component::storages));

	FM_CONNECT(m_pStoragesData.get(), sigChangeData(), this, onChangeData());

	UpdateData();
}

void CSettingsWidget::RemoveProduct(int nRow)
{
	if (m_pStoragesData == nullptr)
		return;

	m_pStoragesData->RemoveProduct(nRow);
}

void CSettingsWidget::RemoveStorage(int nRow)
{
	if (m_pStoragesData == nullptr)
		return;

	m_pStoragesData->RemoveStorage(nRow);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
