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

	m_CreateTrnDlg = new QDialog( this );
	dialogUi.setupUi( m_CreateTrnDlg );

	// Transactions
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/FarmManager/Resources/transactions2.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwTrnsactions = new CTransactionsWidget(this, m_pDBManager);
	ui.tabWidget->insertTab(0, m_pwTrnsactions, icon1, QString("Transactions"));

	// Customers
	QIcon icon2;
	icon2.addFile(QString::fromUtf8(":/FarmManager/Resources/suppliers.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwCustomers = new CCustomersWidget(this, m_pDBManager);
	ui.tabWidget->insertTab(1, m_pwCustomers, icon2, QString("Customers"));

	// Storages
	QIcon icon3;
	icon3.addFile(QString::fromUtf8(":/FarmManager/Resources/farm.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwStorages = new CStoragesWidget();
	ui.tabWidget->insertTab(2, m_pwStorages, icon3, QString("Storages"));


	// Connections
	//QObject::connect( ui.btnCreateTransaction, SIGNAL( clicked() ),
	//				  m_CreateTrnDlg, SLOT( show() ) );
}

CFarmManagerWidget::~CFarmManagerWidget()
{

}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
