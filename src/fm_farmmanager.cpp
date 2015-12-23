////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "fm_farmmanager.h"

// Qt Includes
#include <QSplitter>
#include <QDialog>

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CFarmManager
//
CFarmManager::CFarmManager(QWidget *parent)
	: QMainWindow(parent),
	m_pDBManager(new db::CDBManager())
{
	ui.setupUi(this);

	m_pDBManager->Initialize();

	m_CreateTrnDlg = new QDialog( this );
	dialogUi.setupUi( m_CreateTrnDlg );

	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/FarmManager/Resources/transactions2.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwTrnsactions= new gui::CTransactionsWidget();
	ui.tabWidget->insertTab(0, m_pwTrnsactions, icon1, QString("Transactions"));

	QIcon icon2;
	icon2.addFile(QString::fromUtf8(":/FarmManager/Resources/suppliers.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwCustomers = new gui::CCustomersWidget();
	ui.tabWidget->insertTab(1, m_pwCustomers, icon2, QString("Customers"));

	QIcon icon3;
	icon3.addFile(QString::fromUtf8(":/FarmManager/Resources/farm.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwStorages = new gui::CStoragesWidget();
	ui.tabWidget->insertTab(2, m_pwStorages, icon3, QString("Storages"));


	// Connections
	//QObject::connect( ui.btnCreateTransaction, SIGNAL( clicked() ),
	//				  m_CreateTrnDlg, SLOT( show() ) );
}

CFarmManager::~CFarmManager()
{

}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
