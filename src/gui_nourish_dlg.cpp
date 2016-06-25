////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_nourish_dlg.h"

// Qt Includes
#include <QMessageBox>
#include <QIntValidator>
#include <QSplitter>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SItemNourishInfo; // Declearation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CNourishStoreItemDlg
//
// Constructors
CNourishStoreItemDlg::CNourishStoreItemDlg( QStringList const& lstStorageNames,
									  QWidget *pwParent )
	: QDialog( pwParent )
{
	ui.setupUi( this );

	// Setup Store Explorer widget
	m_pwStoreExplorer = new CMovingItemSelector( lstStorageNames );
	ui.vlayStoreExplorer->addWidget( m_pwStoreExplorer );

	// Setup target store selector
	ui.wdgTargetStores->addItems( lstStorageNames );
	
	m_pwStoreExplorer->SetAddBtnName(QString::fromUtf8("\324\277\325\245\326\200\325\241\325\257\326\200\325\245\325\254"));

	QIcon iconAddBtn;
	iconAddBtn.addFile(QString::fromUtf8(":/FarmManager/Resources/nourish.png"), QSize(), QIcon::Normal, QIcon::Off);
	m_pwStoreExplorer->SetAddBtnIcon(iconAddBtn);

	// Connections
	FM_CONNECT( m_pwStoreExplorer, sigNewSelection(QString const&, QString const&, int, int),
				this,               OnNewSelection(QString const&, QString const&, int) );	
}

// SetStoragesNameList
void CNourishStoreItemDlg::AddStorageNames(QStringList const&  lstStorageNames)
{
	m_pwStoreExplorer->AddStorageNames(lstStorageNames);

	ui.wdgTargetStores->addItems(lstStorageNames);
}

// OnNewSelection [SLOT]
void CNourishStoreItemDlg::OnNewSelection( QString const& sStoreName,
										QString const& sItemName,
										int nCount )
{
	FM_ASSERT( !sStoreName.isEmpty() );
	FM_ASSERT( !sItemName.isEmpty() );
	QListWidgetItem* pTargetStore = ui.wdgTargetStores->currentItem();
	FM_ASSERT( pTargetStore );
	if (!pTargetStore->isSelected())
	{
		QMessageBox::critical( this, qtr( "Target Store Not Selected" ), qtr( "Target store is not selected!" ) );
		return;
	}
	if (nCount <= 0)
		return;
	QString sTargetStoreName = pTargetStore->text();
	FM_ASSERT( !sTargetStoreName.isEmpty() );
	SItemNourishInfo oInfo;
	oInfo.nProductCount = nCount;
	oInfo.sProductName = sItemName;
	oInfo.sSourceStoreName = sStoreName;
	oInfo.sTargetStoreName = sTargetStoreName;

	//  
	QMessageBox::StandardButton eReply;
	eReply = QMessageBox::question( this, qtr( "Confirm Moving" ), qtr( "You are going to move %1 %2 from %3 to %4 " )
								   .arg(nCount).arg(sItemName, sStoreName, sTargetStoreName),
								   QMessageBox::Yes | QMessageBox::No );
	if (eReply == QMessageBox::Yes)
		emit sigMoveStoreItem( oInfo );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
