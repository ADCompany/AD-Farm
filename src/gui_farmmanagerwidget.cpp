////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_farmmanagerwidget.h"

// Qt Includes
#include <QSplitter>
#include <QDialog>

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CFarmManagerWidget
//
CFarmManagerWidget::CFarmManagerWidget(QWidget *parent)
	: QMainWindow(parent),
	m_pDBManager(new db::CDBManager())
{
	ui.setupUi(this);

	m_pDBManager->Initialize();

	m_CreateTrnDlg = std::shared_ptr<QDialog>(new QDialog(this));
	dialogUi.setupUi( m_CreateTrnDlg.get() );

	// Transactions
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/FarmManager/Resources/transactions2.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwTrnsactions = std::shared_ptr<CTransactionsWidget> (new CTransactionsWidget(this, m_pDBManager));
	ui.tabWidget->insertTab(0, m_pwTrnsactions.get(), icon1, QString("Transactions"));

	// Customers
	QIcon icon2;
	icon2.addFile(QString::fromUtf8(":/FarmManager/Resources/suppliers.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwCustomers = std::shared_ptr<CCustomersWidget> (new CCustomersWidget(this, m_pDBManager));
	ui.tabWidget->insertTab(1, m_pwCustomers.get(), icon2, QString("Customers"));

	// Storages
	QIcon icon3;
	icon3.addFile(QString::fromUtf8(":/FarmManager/Resources/farm.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwStorages = std::shared_ptr<CStoragesWidget>(new CStoragesWidget(this, m_pDBManager));
	ui.tabWidget->insertTab(2, m_pwStorages.get(), icon3, QString("Storages"));

	QIcon icon4;
	icon4.addFile(QString::fromUtf8(":/FarmManager/Resources/finances.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwFinances = std::shared_ptr<CFinancesWidget>(new CFinancesWidget(this, m_pDBManager));
	ui.tabWidget->insertTab(3, m_pwFinances.get(), icon4, QString("Finances"));


	// Connections
	//QObject::connect( ui.btnCreateTransaction, SIGNAL( clicked() ),
	//				  m_CreateTrnDlg, SLOT( show() ) );
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
