////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_newdealdlg.h"

// Qt Includes
#include <QMessageBox>
#include <QIntValidator>
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
CNewDealDlg::CNewDealDlg( QWidget *pwParent )
	: QDialog( pwParent )
{
	ui.setupUi( this );
	// Setup Customer Selector
	ui.wOtherCustomer->hide();

	// Setup Store Explorer widget
	m_pwStoreExplorer = new CStoreExplorer();
	QVBoxLayout* pVExpLayout = new QVBoxLayout();
	pVExpLayout->addWidget( m_pwStoreExplorer );
	ui.gbxItemSelection->setLayout( pVExpLayout );

	// Setup Total Viewer widget
	m_pwTotalViewer = new CTotalViewer();
	QVBoxLayout* pVTotalLayout = dynamic_cast<QVBoxLayout*>(layout());
	FM_ASSERT( pVTotalLayout );
	pVTotalLayout->insertWidget( 2, m_pwTotalViewer );
	//adjustSize();
	// Connections
	FM_CONNECT( ui.cbxCustomer, currentIndexChanged( int ), this, OnCustomerChanged( int ) );
	FM_CONNECT( m_pwStoreExplorer, sigNewSelection(QString const&, QString const&, int), 
				m_pwTotalViewer,    OnNewSelection(QString const&, QString const&, int) );
	FM_CONNECT( m_pwTotalViewer, sigTotalPriceChanged(double),
				this,             OnUpedateDebt() );
	FM_CONNECT( ui.sbxPay, valueChanged(double),
				this, OnUpedateDebt() );
	FM_CONNECT( ui.btnCreateDeal, clicked(),
				this, OnCreateDeal() );
	
}

void CNewDealDlg::OnCustomerChanged( int nIndex )
{
	/*int nLastIndex = ui.cbxCustomer->count() - 1;
	if (nIndex == nLastIndex)
		ui.wOtherCustomer->show();
	else
		ui.wOtherCustomer->hide();
	adjustSize();*/
}

// OnUpedateDebt [SLOT]
void CNewDealDlg::OnUpedateDebt()
{
	FM_ASSERT( m_pwTotalViewer );
	double dTotalPrice = m_pwTotalViewer->GetTotalPrice();
	double dPeyedMoney = ui.sbxPay->value();
	double dDebt = dTotalPrice - dPeyedMoney;
	ui.lcdDebt->display( dDebt );
	if (dDebt != 0 || dTotalPrice != 0)
		ui.btnCreateDeal->setEnabled( true );
	else
		ui.btnCreateDeal->setDisabled( true );
}


// OnCreateDeal [SLOT]
void CNewDealDlg::OnCreateDeal()
{

	QDialog::accept();
}

// setLabelColor
void CNewDealDlg::SetLabelColor( QLabel* pLabel, bool bValid )
{
	if (!pLabel)
		return;
	QPalette palette = pLabel->palette();
	palette.setColor( pLabel->foregroundRole(), bValid ? Qt::black : Qt::red );
	pLabel->setPalette( palette );
}



QSize CNewDealDlg::sizeHint() const
{
	return QSize( width(), -1 );
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
