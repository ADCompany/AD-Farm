////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_storeexplorer_wdg.h"

// Qt Includes

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesWidget
//

// Constructors
CStoreExplorer::CStoreExplorer( QWidget *pwParent )
	: QWidget( pwParent )
{
	ui.setupUi( this );
	// Model
	m_pContentModel = new CStoreContentModel( this );
	ui.viewContentTable->setSelectionMode( QAbstractItemView::SingleSelection );
	ui.viewContentTable->setModel( m_pContentModel );

	// Storage List Should Be Initiazlized :TODO
	//
	// Connections
	FM_CONNECT( ui.wdgStoreList, currentItemChanged(QListWidgetItem*, QListWidgetItem*),
				this,         OnCurrentStoreChanged(QListWidgetItem*, QListWidgetItem*) );
	FM_CONNECT( ui.viewContentTable->selectionModel(), currentRowChanged(const QModelIndex&, const QModelIndex&) ,
				this,                            OnCurrentProductChanged(const QModelIndex&, const QModelIndex&) );
}

// OnCurrentStoreChanged
void CStoreExplorer::OnCurrentStoreChanged( 
	QListWidgetItem* pCurrent, 
	QListWidgetItem* pPrevious )
{
	FM_ASSERT( m_pContentModel );
	FM_ASSERT( pCurrent );
	if (!pCurrent || pCurrent == pPrevious)
		return;

	QString sCurrentStoreName = pCurrent->text();
	m_pContentModel->setActiveStore( sCurrentStoreName );
}

void CStoreExplorer::OnCurrentProductChanged( QModelIndex const& idxCurrent, 
											  QModelIndex const& idxPrevious )
{
	if (idxCurrent.isValid())
	{

		return;
	}

	idxCurrent.data( QAbstractItemView::Role )
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
