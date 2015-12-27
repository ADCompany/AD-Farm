////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_transactionswidget.h"

// Qt Includes

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CTransactionsWidget
//

// Constructors
CTransactionsWidget::CTransactionsWidget(QWidget* pwParent, std::shared_ptr<db::CDBManager> pDBManager)
	: QWidget(pwParent),
	m_pCustomersData(nullptr),
	m_pStringListModel(nullptr),
	m_pTransactionsData(nullptr),
	m_strCurrentCustomerName("")
{
	ui.setupUi(this);
	m_pNewDealDlg = new CNewDealDlg(this);

	//
	//	Connections
	//
	FM_CONNECT(ui.btnNewDeal, clicked(), m_pNewDealDlg, show());
	FM_CONNECT(ui.listView, clicked(QModelIndex const&), this, onActivatedCustomer(QModelIndex const&));
	SetDBManager(pDBManager);

	ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void CTransactionsWidget::SetDBManager(std::shared_ptr<db::CDBManager> pDBManager)
{
	if (pDBManager == nullptr)
		return;

	m_pTransactionsData = std::static_pointer_cast<db::CTransactionsData>(pDBManager->GetDBComponent(db::CDBManager::component::transactions));
	m_pCustomersData = std::static_pointer_cast<db::CCustomersData>(pDBManager->GetDBComponent(db::CDBManager::component::customers));

	FM_CONNECT(m_pCustomersData.get(), sigChangeData(), this, onChangeData());

	UpdateData(true);
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
