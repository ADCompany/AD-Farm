////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_addstoreitem_dlg.h"

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
	FM_CONNECT(ui.cbxGroups, currentIndexChanged(QString const&), this, OnGroupSelectionChanged(QString const&));
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
	QString strInfoText =ui.lineEditGroupName->text();
	bool bValidCosts = !strInfoText.isEmpty();

	SetLabelColor(ui.lblGroup, bValidCosts);

	if (bValidCosts)
		QDialog::accept();
}

void CAddStoreItem::SetLabelColor(QLabel* pLabel, bool bValid)
{
	if (!pLabel)
		return;

	QPalette palette = pLabel->palette();
	palette.setColor(pLabel->foregroundRole(), bValid ? Qt::black : Qt::red);
	pLabel->setPalette(palette);
}

void CAddStoreItem::OnItemSelectionChanged( int )
{
	m_nOldCount = 0;
	ui.sbxCount->setValue( 0 );
}

void CAddStoreItem::OnGroupSelectionChanged(QString const& strGroupName)
{
	ui.lineEditGroupName->setText(strGroupName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
