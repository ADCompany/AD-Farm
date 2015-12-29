////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_settingswidget.h"

// Qt Includes

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CSettingsWidget
//

// Constructors
CSettingsWidget::CSettingsWidget(QWidget* pwParent, std::shared_ptr<db::CDBManager> pDBManager)
	: QWidget(pwParent),
	m_pStoragesData(nullptr),
	m_pAddProductDlg(new CAddProductDlg(this))
{
	m_uiSettingsWidget.setupUi(this);

	SetDBManager(pDBManager);

	// Setup Add Storage Dialog
	m_pAddStoreDlg = std::shared_ptr<CAddStoreDlg>(new CAddStoreDlg(this));

	FM_CONNECT( m_uiSettingsWidget.btnAddProduct, clicked(), this, onShowAddProductDlg() );
	FM_CONNECT( m_uiSettingsWidget.btnAddStore, clicked(), this, onShowAddStoreDlg() );

	FM_CONNECT(m_pAddProductDlg.get(), accepted(), this, onAddNewProduct());
	FM_CONNECT(m_pAddStoreDlg.get(), accepted(), this, onAddNewStore());

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

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
