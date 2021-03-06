////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_storeexplorer_wdg.h"

// Qt Includes
#include <QMessageBox>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesWidget
//

// Constructors
CStoreExplorer::CStoreExplorer( QStringList const& lstStores, QWidget* pwParent )
	: QWidget( pwParent )
{
	ui.setupUi( this );
	// Model
	m_pContentModel = new CStoreContentModel( this );
	ui.viewContentTable->setSelectionMode( QAbstractItemView::SingleSelection );
	ui.viewContentTable->setModel( m_pContentModel );
	ui.viewContentTable->verticalHeader()->hide();
	// Storage List Should Be Initiazlized :TODO
	//
	// Count Spin Box Setup
	ui.wdgCount->setDisabled( true );

	// Setup Storage List
	ui.wdgStoreList->addItems( lstStores );

	// Connections
	FM_CONNECT( ui.wdgStoreList, currentItemChanged(QListWidgetItem*, QListWidgetItem*),
				this,         OnCurrentStoreChanged(QListWidgetItem*, QListWidgetItem*) );
	FM_CONNECT( ui.viewContentTable->selectionModel(), currentRowChanged(const QModelIndex&, const QModelIndex&) ,
				this,                            OnCurrentProductChanged(const QModelIndex&, const QModelIndex&) );
	FM_CONNECT( ui.btnAdd, clicked(), this, OnAdd() );
	FM_CONNECT( ui.sbxCount, valueChanged(int), this, OnCountCountChanged(int) );

}

void CStoreExplorer::AddStorageNames(QStringList const& lstStoreNames)
{
	ui.wdgStoreList->addItems(lstStoreNames);
}

void SetAddBtnName(QString const& strAddBtnName)
{

}

// OnCurrentStoreChanged
void CStoreExplorer::OnCurrentStoreChanged( 
	QListWidgetItem* pCurrent, 
	QListWidgetItem* pPrevious )
{
	if (pCurrent && pCurrent == pPrevious)
		return;
	Update();
}

// Update
void CStoreExplorer::Update()
{
	FM_ASSERT( m_pContentModel );
	QListWidgetItem* pCurrent = ui.wdgStoreList->currentItem();
	if (pCurrent)
	{
		pCurrent->setSelected( true );
		QString sCurrentStoreName = pCurrent->text();
		m_pContentModel->setActiveStore( sCurrentStoreName );
		ui.viewContentTable->resizeRowsToContents();
	}
	else
		m_pContentModel->setActiveStore( QString(" ") );

}

// OnCurrentProductChanged
void CStoreExplorer::OnCurrentProductChanged( QModelIndex const& idxCurrent, 
											  QModelIndex const& idxPrevious )
{
	if (!idxCurrent.isValid())
	{
		m_sCurrentProductName.clear();
		ui.sbxCount->setValue( 0 );
		ui.wdgCount->setDisabled( true );
		return;
	}
	int nRow = idxCurrent.row();
	// Product
	QModelIndex idxProduct = m_pContentModel->index( nRow, 0 );
	m_sCurrentProductName = m_pContentModel->data( idxProduct ).toString();
	FM_ASSERT( !m_sCurrentProductName.isEmpty() );
	if (m_sCurrentProductName.isEmpty())
	{
		QMessageBox::critical( this, "Internal Error", "Invalid internal configuration" );
		return;
	}
	// Count
	bool bOk = false;
	QModelIndex idxCount = m_pContentModel->index( nRow, 1 );
	int nCount = m_pContentModel->data( idxCount ).toInt( &bOk );
	FM_ASSERT( bOk );
	if (!bOk)
	{
		QMessageBox::critical( this, "Internal Error", "Invalid internal configuration" );
		return;
	}
	if (nCount == 0)
		// Empty
		return;

	int nSelectedCount = m_hshCountSelection.value( makeKey( m_sCurrentProductName ) );
	// Setup Spin Box
	ui.wdgCount->setEnabled( true );
	ui.sbxCount->setMaximum( nCount );
	ui.sbxCount->setValue( nSelectedCount );
}

// OnAdd
void CStoreExplorer::OnAdd()
{
	int nCount = ui.sbxCount->value();
	if (nCount < 0)
		return;
	QListWidgetItem* pStore = ui.wdgStoreList->currentItem();
	FM_ASSERT( pStore );
	if (!pStore)
	{
		QMessageBox::critical( this, "Invalid Selection", "Invalid store selection. Please select again" );
		return;
	}
	QString sStoreName = pStore->text();
	// Product
	FM_ASSERT( !m_sCurrentProductName.isEmpty() );
	if (m_sCurrentProductName.isEmpty())
	{
		QMessageBox::critical( this, "Invalid Selection", "Invalid product selection. Please select again" );
		return;
	}

	int nSelectedCount = m_hshCountSelection.value( makeKey( sStoreName, m_sCurrentProductName ) );
	int nDiffCount = nCount - nSelectedCount;

	if (nDiffCount != 0)
	{
		// Register current selection
		m_hshCountSelection.insert( makeKey( sStoreName, m_sCurrentProductName ), nCount );
		// Ready
		//// Register in Coinfiguration
		int nIdx = m_lstConfig.indexOf( sStoreName );
		if (nIdx < 0)
		{
			SProductInfo oProductInfo( m_sCurrentProductName, nCount );
			QList<SProductInfo> lstProducts;
			lstProducts.append( oProductInfo );
			m_lstConfig.append( SStoreageInfo( sStoreName, lstProducts ) );
		}
		else
		{
			SStoreageInfo& cfg = m_lstConfig[ nIdx ];
			int nProductIndex = cfg.lstProducts.indexOf( m_sCurrentProductName );
			if (nProductIndex < 0)
			{
				cfg.lstProducts.append( SProductInfo( m_sCurrentProductName, nCount ) );
			}
			else
			{
				cfg.lstProducts[nProductIndex].nCount = nCount;
			}
		}
	}
	////
	emit sigNewSelection( sStoreName, m_sCurrentProductName, nCount, nSelectedCount );
	emit sigNewSelectionDif( sStoreName, m_sCurrentProductName, nDiffCount );
}

void CStoreExplorer::OnCountCountChanged( int nCount )
{
	/*if (nCount > 0)
		ui.btnAdd->setEnabled( true );
	else if (ui.viewContentTable->selectionModel()->r)*/
}

// OnAdd
void CStoreExplorer::OnClear()
{
	ui.wdgStoreList->clear();
	m_pContentModel->setActiveStore(QString());
	ui.sbxCount->setValue( 0 );
	ui.wdgCount->setDisabled( true );
	m_hshCountSelection.clear();
}


QString CStoreExplorer::makeKey( QString const& sProductName ) const
{
	QListWidgetItem* pStore = ui.wdgStoreList->currentItem();
	FM_ASSERT( pStore );
	if (!pStore)
		return QString();
	QString sStoreName = pStore->text();
	return sStoreName + "::" + sProductName;
}

QString CStoreExplorer::makeKey( QString const& sStoreName, 
								 QString const& sProductName ) const
{
	return sStoreName + "::" + sProductName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	class CMovingItemSelector
//
CMovingItemSelector::CMovingItemSelector( QStringList const& lstStores, QWidget* pwParent )
	: Base( lstStores, pwParent )
{
	QWidget* pCountSelector = ui.wdgCount;
	layout()->removeWidget( pCountSelector );
	ui.vlayItems->addWidget( pCountSelector );
	// Setup Count Selector
	ui.btnAdd->setText( QString::fromUtf8( "\325\217\325\245\325\262\325\241\326\203\325\270\325\255\325\245\325\254" ) );
	QIcon oIcon;
	oIcon.addFile( QString::fromUtf8( ":/FarmManager/Resources/move_right2.png" ), QSize(), QIcon::Normal, QIcon::Off );
	ui.btnAdd->setIcon( oIcon );

	ui.wdgCount->layout()->removeItem( ui.verticalSpacer );
}

// Interface Methodes
void CMovingItemSelector::SetAddBtnName(QString const& strBtnName)
{
	ui.btnAdd->setText(strBtnName);
}

void CMovingItemSelector::SetAddBtnIcon(QIcon const& iconAddBtn)
{
	ui.btnAdd->setIcon(iconAddBtn);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
