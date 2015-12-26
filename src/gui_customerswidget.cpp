////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_customerswidget.h"

// Qt Includes
#include <QSqlRecord>
#include <QMouseEvent>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CCustomersWidget
//

// Constructors
CCustomersWidget::CCustomersWidget(QWidget* pwParent, std::shared_ptr<db::CDBManager> pDBManager)
	: QWidget(pwParent),
	m_pAddCustomerDlg(new CAddCustomerDlg(this)),
	m_pMenu(new QMenu("menu", this))
{
	m_uiCustomersWidget.setupUi(this);
	SetDBManager(pDBManager);

	QAction* pAction = new QAction("Heracnel", this);
	m_pMenu->addAction(pAction);

	m_uiCustomersWidget.tableView->setContextMenuPolicy(Qt::CustomContextMenu);

	FM_CONNECT(m_uiCustomersWidget.btnAddCustomers, SIGNAL(clicked()), m_pAddCustomerDlg.get(), SLOT(show()));

	FM_CONNECT(m_pAddCustomerDlg.get(), accepted(), this, onAddCustomer());

	FM_CONNECT(m_uiCustomersWidget.tableView, customContextMenuRequested(QPoint const&), this, onMenuShow(QPoint const&));

	FM_CONNECT(pAction, triggered(bool), this, onRemoveCustomer());
}

void CCustomersWidget::SetDBManager(std::shared_ptr<db::CDBManager> pDBManager)
{
	if (pDBManager == nullptr)
		return;

	m_pCustomersData = std::static_pointer_cast<db::CCustomersData>(pDBManager->GetDBComponent(db::CDBManager::component::customers));

	UpdateData();
}

void CCustomersWidget::AddCustomer(QString const& strFirstName, QString const& strLastName, int nDept, int nPhoneNumber)
{
	if (m_pCustomersData == nullptr)
		return;

	m_pCustomersData->AddCustomer(strFirstName, strLastName, nDept, nPhoneNumber);
}

void CCustomersWidget::RemoveCustomer(int nRow)
{
	if (m_pCustomersData == nullptr)
		return;

	m_pCustomersData->RemoveCustomer(nRow);
}

void CCustomersWidget::RemoveCustomer(QString const& strFirstName, QString const& strLastName)
{
	if (m_pCustomersData == nullptr)
		return;

	m_pCustomersData->RemoveCustomer(strFirstName, strLastName);
}

void CCustomersWidget::onAddCustomer()
{
	CAddCustomerDlg* pAddCustomerDlg = qobject_cast<CAddCustomerDlg*>(sender());
	if (pAddCustomerDlg == nullptr)
		return;

	AddCustomer(pAddCustomerDlg->GetFirstName(), pAddCustomerDlg->GetLastName(), 0, pAddCustomerDlg->GetPhoneNumber().toInt());
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
