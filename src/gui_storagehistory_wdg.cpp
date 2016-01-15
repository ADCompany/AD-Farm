////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_storagehistory_wdg.h"

// Qt Includes

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CStorageHistoryWidget
//

// Constructors
CStorageHistoryWidget::CStorageHistoryWidget(QWidget* pwParent, std::shared_ptr<db::CDBManager> pDBManager)
	: QWidget(pwParent),
	m_pStringListModel(nullptr),
	m_strCurrentStorageName("")
{
	ui.setupUi(this);
	ui.splitter->setStretchFactor( 0, 1 );
	ui.splitter->setStretchFactor( 1, 3 );

	FM_CONNECT(ui.listView, clicked(QModelIndex const&), this, onActivatedStorage(QModelIndex const&));
	SetDBManager(pDBManager);

	ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void CStorageHistoryWidget::SetDBManager(std::shared_ptr<db::CDBManager> pDBManager)
{
	if (pDBManager == nullptr)
		return;

	m_pStoragesData = std::static_pointer_cast<db::CStoragesData>(pDBManager->GetDBComponent( db::CDBManager::component::storages ));

	FM_CONNECT(m_pStoragesData.get(), sigChangeData(), this, onChangeData());

	UpdateData(true);
}

// Helper Functions
void CStorageHistoryWidget::UpdateData(bool bFull)
{
	if (bFull)
	{
		QStringList lstStorageNames(m_pStoragesData->GetStorageNames());
		m_pStringListModel = std::shared_ptr<QStringListModel>(new QStringListModel(lstStorageNames));

		ui.listView->setModel(m_pStringListModel.get());
		ui.listView->update();
	}

	if (m_strCurrentStorageName == "")
		return;

	ui.tableView->setModel(m_pStoragesData->GetHistorySqlTableModelByStorageName(m_strCurrentStorageName).get());
	ui.tableView->update();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
