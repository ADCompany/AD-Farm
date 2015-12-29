////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_farmmanagerwidget.h"

// Qt Includes
#include <QSplitter>
#include <QDialog>
#include <QString>
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
	ui.tabWidget->insertTab(0, m_pwTrnsactions.get(), icon1, 
		QString::fromUtf8("\324\263\325\270\326\200\325\256\325\241\326\200\326\204\325\266\325\245\326\200"));

	// Customers
	QIcon icon2;
	icon2.addFile(QString::fromUtf8(":/FarmManager/Resources/customers.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwCustomers = std::shared_ptr<CCustomersWidget> (new CCustomersWidget(this, m_pDBManager));
	ui.tabWidget->insertTab(1, m_pwCustomers.get(), icon2,
		QString::fromUtf8("\324\263\325\266\325\270\326\200\325\244\325\266\325\245\326\200"));

	// Storages
	QIcon icon3;
	icon3.addFile(QString::fromUtf8(":/FarmManager/Resources/barn.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwStorages = std::shared_ptr<CStoragesWidget>(new CStoragesWidget(this, m_pDBManager));
	ui.tabWidget->insertTab(2, m_pwStorages.get(), icon3,
		QString::fromUtf8("\325\212\325\241\325\260\325\270\326\201\325\266\325\245\326\200"));

	// Finances
	QIcon icon4;
	icon4.addFile(QString::fromUtf8(":/FarmManager/Resources/finances4.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwFinances = std::shared_ptr<CFinancesWidget>(new CFinancesWidget(this, m_pDBManager));
	ui.tabWidget->insertTab(3, m_pwFinances.get(), icon4,
		QString::fromUtf8("\325\226\325\253\325\266\325\241\325\266\325\275\325\266\325\245\326\200"));

	QIcon icon5;
	icon5.addFile(QString::fromUtf8(":/FarmManager/Resources/settings.png"), QSize(), QIcon::Normal, QIcon::Off);

	m_pwSettings = std::shared_ptr<CSettingsWidget>(new CSettingsWidget(this, m_pDBManager));
	ui.tabWidget->insertTab(4, m_pwSettings.get(), icon5,
		QString::fromUtf8("\324\277\325\241\326\200\325\243\325\241\325\276\325\270\326\200\325\270\326\202\325\264\325\266\325\245\326\200"));

	// Connections
	//QObject::connect( ui.btnCreateTransaction, SIGNAL( clicked() ),
	//				  m_CreateTrnDlg, SLOT( show() ) );
	FM_CONNECT( ui.actShowDeals, triggered(), this, OnActionDeals() );
	FM_CONNECT( ui.actCustomers, triggered(), this, OnActionCustomers() );
	FM_CONNECT( ui.actStores,    triggered(), this, OnActionStorages() );
	FM_CONNECT( ui.actFinances,  triggered(), this, OnActionFinances() );
	FM_CONNECT( ui.actSettings,  triggered(), this, OnActionSettings() );
	FM_CONNECT(ui.actAddCustomer, triggered(), this, OnActionAddCustomer());
	FM_CONNECT(ui.actNewDeal, triggered(), this, OnActionNewDeal());
	FM_CONNECT( ui.actExit, triggered(), qApp, quit() );
}


//
//	Action Handlers
//

void CFarmManagerWidget::OnActionDeals()
{
	ui.tabWidget->setCurrentWidget( m_pwTrnsactions.get() );
}
void CFarmManagerWidget::OnActionCustomers()
{
	ui.tabWidget->setCurrentWidget( m_pwCustomers.get() );
}
void CFarmManagerWidget::OnActionStorages()
{
	ui.tabWidget->setCurrentWidget( m_pwStorages.get() );
}
void CFarmManagerWidget::OnActionFinances()
{
	ui.tabWidget->setCurrentWidget( m_pwFinances.get() );
}
void CFarmManagerWidget::OnActionSettings()
{
	ui.tabWidget->setCurrentWidget( m_pwSettings.get() );
}
void CFarmManagerWidget::OnActionNewDeal()
{
	m_pwTrnsactions->GetAddTransactionDialog()->show();
}
void CFarmManagerWidget::OnActionAddCustomer()
{
	m_pwCustomers->GetAddCustomerDialog()->show();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
