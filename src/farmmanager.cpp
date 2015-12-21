#include "farmmanager.h"
#include "fm_datamanager.h"
// Qt
#include <QSplitter>
#include <QDialog>

FarmManager::FarmManager(QWidget *parent)
	: QMainWindow(parent)
{
	fm::db::CDataManager dataManager;
	ui.setupUi(this);
	m_CreateTrnDlg = new QDialog( this );
	dialogUi.setupUi( m_CreateTrnDlg );

	m_CreateSplDlg = new QDialog( this );
	addSupplierDlgUi.setupUi( m_CreateSplDlg );

	// Connections
	QObject::connect( ui.btnCreateTransaction, SIGNAL( clicked() ),
					  m_CreateTrnDlg, SLOT( show() ) );

	QObject::connect( ui.btnAddSupplier, SIGNAL( clicked() ),
					  m_CreateSplDlg, SLOT( show() ) );
}

FarmManager::~FarmManager()
{

}
