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
CCustomersWidget::CCustomersWidget(QWidget *pwParent)
	: QWidget(pwParent),
	m_pCustomersData(new db::CCustomersData(this)),
	m_pAddCustomerDlg(new CAddCustomerDlg(this))
{
	m_uiCustomersWidget.setupUi(this);
	m_pCustomersData->Initialize();

	QAction* pAction = new QAction("Heracnel", this);
	m_pMenu = new QMenu("MENU", this);
	m_pMenu->addAction(pAction);

	QObject::connect(m_uiCustomersWidget.btnAddCustomers, SIGNAL(clicked()),
		m_pAddCustomerDlg, SLOT(show()));

	m_uiCustomersWidget.tableView->installEventFilter(this);

	FM_CONNECT(m_pAddCustomerDlg, accepted(), this, onAddCustomer());

	FM_CONNECT(m_uiCustomersWidget.tableView, clicked(QModelIndex const&), this, onSelectModelIndex(QModelIndex const&));

	FM_CONNECT(m_uiCustomersWidget.tableView, clicked(QModelIndex const&), m_pMenu, show());

	FM_CONNECT(pAction, triggered(bool), this, onRemoveCustomer());
}

CCustomersWidget::CCustomersWidget(QWidget* pwParent, std::shared_ptr<db::CDBManager> pDBManager)
	: QWidget(pwParent),
	m_pCustomersData(new db::CCustomersData(this, pDBManager)),
	m_pAddCustomerDlg(new CAddCustomerDlg(this))
{
	m_uiCustomersWidget.setupUi(this);
	m_pCustomersData->Initialize();

	QAction* pAction = new QAction("Heracnel", this);
	m_pMenu = new QMenu("MENU", this);
	m_pMenu->addAction(pAction);

	QObject::connect(m_uiCustomersWidget.btnAddCustomers, SIGNAL(clicked()),
		m_pAddCustomerDlg, SLOT(show()));

	FM_CONNECT(m_pAddCustomerDlg, accepted(), this, onAddCustomer());

	FM_CONNECT(m_uiCustomersWidget.tableView, clicked(QModelIndex const&), this, onSelectModelIndex(QModelIndex const&));

	FM_CONNECT(m_uiCustomersWidget.tableView, clicked(QModelIndex const&), m_pMenu, show());

	FM_CONNECT(pAction, triggered(bool), this, onRemoveCustomer());
}

void CCustomersWidget::SetDBManager(std::shared_ptr<db::CDBManager> pDBManager)
{
	if (pDBManager == nullptr)
		return;

	m_pCustomersData->SetDBManager(pDBManager);

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

	AddCustomer(pAddCustomerDlg->GetFirstName(), pAddCustomerDlg->GetLastName(), pAddCustomerDlg->GetPhoneNumber().toInt());
}

bool CCustomersWidget::event(QEvent* pEvent)
{
	if (pEvent == nullptr)
		return false;

	//switch (pEvent->type())
	//{
	//case QEvent::MouseButtonPress:
	//	{
	//		QMouseEvent* pMouseEvent = (QMouseEvent*)(pEvent);
	//		if (pMouseEvent->button() == Qt::RightButton)
	//			onMenuShow(pMouseEvent->pos());
	//	}
	//default:
	//	break;
	//}

	return QWidget::event(pEvent);
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
