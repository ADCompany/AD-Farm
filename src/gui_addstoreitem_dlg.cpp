////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_addstoreitem_dlg.h"

// Qt Includes
#include <QMessageBox>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CAddCustomerDlg
//

// Constructors
CAddStoreItem::CAddStoreItem( QStringList const& lstItemNames, QWidget* pwParent )
	: QDialog( pwParent ),
	  m_nOldCount(0)
{
	ui.setupUi( this );
	// Setup Customer Selector
	
	ui.cbxItemType->addItems( lstItemNames );

	// Setup Total Viewer widget
	m_pwTotalViewer = new CTotalViewer();
	QVBoxLayout* pVTotalLayout = dynamic_cast<QVBoxLayout*>(layout());
	FM_ASSERT( pVTotalLayout );
	pVTotalLayout->insertWidget( 1, m_pwTotalViewer );
	
	// Connections
	FM_CONNECT( ui.cbxItemType, currentIndexChanged(int), this, OnItemSelectionChanged(int) );
	FM_CONNECT( ui.sbxCount, valueChanged(int), this, OnCountChanged(int) );
	FM_CONNECT( ui.btnAdd, clicked(), this, OnAddClicked() );
}

void CAddStoreItem::OnCountChanged( int nNewCount )
{
	QString sItem = ui.cbxItemType->currentText();
	if (sItem.isEmpty())
		// Nothig to do
		return;
	m_pwTotalViewer->OnNewSelection( QString(), sItem, nNewCount );
	if (!m_pwTotalViewer->isEmpty())
		ui.btnAdd->setEnabled( true );
	else 
		ui.btnAdd->setDisabled( true );
}

void CAddStoreItem::OnAddClicked()
{
	QDialog::accept();
}

void CAddStoreItem::OnItemSelectionChanged( int )
{
	m_nOldCount = 0;
	ui.sbxCount->setValue( 0 );
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
