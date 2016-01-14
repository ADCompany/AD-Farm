////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_movestoreitem_dlg.h"

// Qt Includes
#include <QMessageBox>
#include <QIntValidator>
#include <QSplitter>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CMoveStoreItemDlg
//
// Constructors
CMoveStoreItemDlg::CMoveStoreItemDlg( QStringList const& lstStorageNames,
									  QWidget *pwParent )
	: QDialog( pwParent )
{
	ui.setupUi( this );

	// Setup Store Explorer widget
	m_pwStoreExplorer = new CMovingItemSelector( lstStorageNames );
	ui.vlayStoreExplorer->addWidget( m_pwStoreExplorer );

	// Setup target store selector
	ui.wdgTargetStores->addItems( lstStorageNames );

	// Connections
	FM_CONNECT( m_pwStoreExplorer, sigNewSelection(QString const&, QString const&, int),
				this,               OnNewSelection(QString const&, QString const&, int) );	
}

// SetStoragesNameList
void CMoveStoreItemDlg::AddStorageNames(QStringList const&  lstStorageNames)
{
	m_pwStoreExplorer->AddStorageNames(lstStorageNames);

	ui.wdgTargetStores->addItems(lstStorageNames);
}

// OnNewSelection [SLOT]
void CMoveStoreItemDlg::OnNewSelection( QString const& sStoreName,
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
	SItemMovingInfo oInfo;
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


////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
