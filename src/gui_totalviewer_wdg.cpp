////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_totalviewer_wdg.h"

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
// class CStoragesWidget
//

// Constructors
CTotalViewer::CTotalViewer( QWidget *pwParent )
	: QWidget( pwParent )
{
	ui.setupUi( this );
	//ui.framePriceSelector->
	ui.framePriceSelector->hide();
	// Connections

}

// OnNewSelection
void CTotalViewer::OnNewSelection( QString const& sStoreName, 
								   QString const& sItemName,
								   int nCount )
{
	FM_ASSERT( !sItemName.isEmpty() );
	if (sItemName.isEmpty())
	{
		QMessageBox::critical( this, "Invalid Product Name", 
							   "Invalid product name. Please select product again again" );
		return;
	}
	

	CProductInfoWidgets oInfoWidgets = m_hshProductInfo.value( sItemName );
	if (oInfoWidgets.isEmpty())
	{
		if (nCount > 0)
			AddProduct( sItemName, nCount );
	}
	else
	{
		//int nCurrentCount = oInfoWidgets.getCount();
		//nCurrentCount += nCount;
		if (nCount <= 0)
			RemoveProduct( oInfoWidgets );
		else
			oInfoWidgets.getLcdCount()->display( nCount );
		OnUpdateTotalPrice();
	}
}


void CTotalViewer::AddProduct( QString const& sName,
							   int nCount,
							   double fDefaultPrice )
{
	// Setup Name Label
	QLabel* plblName = new QLabel();
	plblName->setText( sName );

	// Setup Lcd Count
	QLCDNumber* pLcdCount = new QLCDNumber();
	pLcdCount->setMinimumHeight( 25 );
	pLcdCount->display( nCount );

	// Setup Price spin box
	QDoubleSpinBox* psbxPrice = new QDoubleSpinBox();
	psbxPrice->setMaximum( 99999.99 );
	psbxPrice->setSingleStep( 10.0 );
	psbxPrice->setValue( fDefaultPrice );
	QFont font = psbxPrice->font();
	font.setPointSize( 10 );
	psbxPrice->setFont( font );
	FM_CONNECT( psbxPrice, valueChanged( double ), this, OnUpdateTotalPrice() );
	// Add to layout
	int nRowCount = ui.gridLayout->rowCount();
	ui.gridLayout->addWidget( plblName,  nRowCount, 0 );
	ui.gridLayout->addWidget( pLcdCount, nRowCount, 1 );
	ui.gridLayout->addWidget( psbxPrice, nRowCount, 2 );
	// Add to Hash
	m_hshProductInfo.insert( sName, CProductInfoWidgets( plblName, pLcdCount, psbxPrice ) );

	OnUpdateTotalPrice();
	//
	ui.framePriceSelector->show();
}

// RemoveProduct
void CTotalViewer::RemoveProduct( CProductInfoWidgets const& oInfo )
{
	FM_ASSERT( !oInfo.isEmpty() );
	QString sName = oInfo.getName();
	// Remove
	ui.gridLayout->removeWidget( oInfo.getNameLabel() );
	ui.gridLayout->removeWidget( oInfo.getLcdCount() );
	ui.gridLayout->removeWidget( oInfo.getPriceSpinBox() );
	// Delete
	oInfo.getNameLabel()   ->deleteLater();
	oInfo.getLcdCount()    ->deleteLater();
	oInfo.getPriceSpinBox()->deleteLater();
	// Remove from hash
	m_hshProductInfo.remove( sName );

	if (m_hshProductInfo.isEmpty())
		ui.framePriceSelector->hide();
}

// OnAdd
void CTotalViewer::OnClear()
{
	QList<CProductInfoWidgets> lstInfo = m_hshProductInfo.values();
	for each( CProductInfoWidgets const& oInfo in lstInfo )
		RemoveProduct( oInfo );
	m_hshProductInfo.clear();
}

// OnUpdateTotalPrice
void CTotalViewer::OnUpdateTotalPrice()
{
	double dTotalPrice = 0;
	QList<CProductInfoWidgets> lstInfo = m_hshProductInfo.values();
	for each(CProductInfoWidgets const& oInfo in lstInfo)
		dTotalPrice += oInfo.getCount()*oInfo.getPrice();
	
	double dOldPrice = ui.lcdTotalPrice->value();
	ui.lcdTotalPrice->display( dTotalPrice );
	if (dOldPrice != dTotalPrice)
		emit sigTotalPriceChanged( dTotalPrice );
}

// GetProductPriceInfo
t_lstProductPriceInfo CTotalViewer::GetProductPriceInfo() const
{
	t_lstProductPriceInfo lstResult;
	QList<CProductInfoWidgets> lstInfo = m_hshProductInfo.values();
	for each(CProductInfoWidgets const& oInfo in lstInfo)
	{
		QString sName  = oInfo.getName();
		int     nCount = oInfo.getCount();
		double  dPrice = oInfo.getPrice();
		lstResult.append( SProductPriceInfo( sName, nCount, dPrice ) );
	}
	return lstResult;
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
