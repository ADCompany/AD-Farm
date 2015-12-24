////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_customerswidget.h"

// Qt Includes
#include <QSqlRecord>
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
	m_pCustomersData(new db::CCustomersData()),
	m_pAddCustomerDlg(new CAddCustomerDlg(this)),
	m_pDBManager(nullptr),
	m_pSqlTableModel(nullptr)
{
	m_uiCustomersWidget.setupUi(this);
	m_pCustomersData->Initialize();

	QObject::connect(m_uiCustomersWidget.btnAddCustomers, SIGNAL(clicked()),
		m_pAddCustomerDlg, SLOT(show()));

	FM_CONNECT(m_pAddCustomerDlg, accepted(), this, onAddCustomer());
}

void CCustomersWidget::SetDBManager(std::shared_ptr<db::CDBManager> pDBManager)
{
	if (pDBManager == nullptr)
		return;

	m_pDBManager = pDBManager;
	m_pSqlTableModel = std::shared_ptr<QSqlTableModel>(new QSqlTableModel(this, m_pDBManager->GetDataBase()));
	m_pSqlTableModel->setTable("customer");
	m_pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	m_pSqlTableModel->select();
	m_pSqlTableModel->removeColumn(0); // don't show the ID
	m_pSqlTableModel->submitAll();

	UpdateData();
}

void CCustomersWidget::AddCustomer(QString const& strFirstName, QString const& strLastName, int nPhoneNumber)
{
	int nRowCount = m_pSqlTableModel->rowCount();
	m_pSqlTableModel->insertRows(nRowCount, 1);
	m_pSqlTableModel->setData(m_pSqlTableModel->index(nRowCount, 0), strFirstName);
	m_pSqlTableModel->setData(m_pSqlTableModel->index(nRowCount, 1), strLastName);
	m_pSqlTableModel->setData(m_pSqlTableModel->index(nRowCount, 2), nPhoneNumber);
	m_pSqlTableModel->submitAll();
}

void CCustomersWidget::RemoveCustomer(int nRow)
{
	m_pSqlTableModel->removeRow(nRow);
	m_pSqlTableModel->submitAll();
}

void CCustomersWidget::RemoveCustomer(QString const& strFirstName, QString const& strLastName)
{
	int nRow = -1;
	nRow = m_pCustomersData->GetId(strFirstName, strLastName);

	if (nRow != -1)
		RemoveCustomer(nRow);
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
	//case QEvent::Paint:
	//	UpdateData();
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
