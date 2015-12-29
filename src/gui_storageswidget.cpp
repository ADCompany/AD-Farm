////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_storageswidget.h"

// Qt Includes

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

// Constructors
CStoragesWidget::CStoragesWidget(QWidget* pwParent, std::shared_ptr<db::CDBManager> pDBManager)
	: QWidget(pwParent),
	m_pStringListModel(nullptr),
	m_strCurrentStorageName(""),
	m_pAddItemDlg( nullptr )
{
	m_uiStorages.setupUi(this);

	//
	//	Connections
	//
	m_uiStorages.btnAddItem->setDisabled(true);
	FM_CONNECT(m_uiStorages.listView, clicked(QModelIndex const&), this, onActivatedStorage(QModelIndex const&));
	FM_CONNECT( m_uiStorages.btnAddItem, clicked(), this, onAddItemClicked() );
	SetDBManager(pDBManager);

	m_uiStorages.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void CStoragesWidget::SetDBManager(std::shared_ptr<db::CDBManager> pDBManager)
{
	if (pDBManager == nullptr)
		return;

	m_pStoragesData = std::static_pointer_cast<db::CStoragesData>(pDBManager->GetDBComponent(db::CDBManager::component::storages));
	FM_CONNECT(m_pStoragesData.get(), sigChangeData(), this, onChangeData());

	UpdateData(true);
}

// onAddItemClicked
void CStoragesWidget::onAddItemClicked() // BAD Solustion
{
	m_pAddItemDlg = std::shared_ptr<CAddStoreItem>(new CAddStoreItem(m_pStoragesData->GetProductNames(), this));
	FM_CONNECT(m_pAddItemDlg.get(), accepted(), this, onAddItem());
	m_pAddItemDlg->show();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
