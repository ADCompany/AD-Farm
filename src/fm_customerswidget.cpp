////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "fm_customerswidget.h"

// Qt Includes

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
	m_pCreateSplDlg(new QDialog(this)),
	m_pTableWidgetItem(new QTableWidgetItem())
{
	m_uiCustomersWidget.setupUi(this);

	m_uiAddCustomersDialog.setupUi(m_pCreateSplDlg);

	QObject::connect(m_uiCustomersWidget.btnAddCustomers, SIGNAL(clicked()),
		m_pCreateSplDlg, SLOT(show()));

	m_pCustomersData->Initialize();

	m_pTableWidgetItem->setData(0, "asdf");
	m_uiCustomersWidget.tableWidget->setColumnCount(10);
	m_uiCustomersWidget.tableWidget->setRowCount(10);
	m_uiCustomersWidget.tableWidget->setItem(1, 1, m_pTableWidgetItem.get());
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
