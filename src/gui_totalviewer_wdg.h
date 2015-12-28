#ifndef GUI_TOTAL_VIEWER_WDG_H
#define GUI_TOTAL_VIEWER_WDG_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef FM_CORE_H
#	include "fm_core.h"
#endif

// UI
#include "ui_totalviewer.h"

// Qt Includes
#include <QWidget>
#include <QDoubleSpinBox>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CTransactionDetails
//

struct SProductBuyInfo
{
	SProductBuyInfo( QString sProductName,
					 int nCount,
					 double dPrice )
					 : sProductName( sProductName ),
					   nCount( nCount ),
					   fPrice( dPrice )
	{}

	// Opertor == [String]
	bool operator==(QString const& sName) const
	{ return sProductName == sName; }

public:
	//
	//	Content
	//
	QString sProductName;
	int     nCount;
	double  fPrice;
};

typedef QList<SProductBuyInfo> t_lstProductBuyInfo;

struct STransactionDetails
{
	//
	//	Content
	//
	QString             sCustomerName;
	t_lstProductBuyInfo lstProductInfo;
	double              fTotalPrice;
	double              fPayedMoney;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CTotalViewer
//
class CTotalViewer : public QWidget
{

	Q_OBJECT

	////////////////////////////////////////////////////////////////////////////////
	//
	//	nested class CProductInfoWidgets
	//
	class CProductInfoWidgets
	{
	public:
		// Constructors
		CProductInfoWidgets()
			: m_plblProductName( nullptr ),
			m_pLcdCount( nullptr ),
			m_psbxPrice( nullptr )
		{}
		CProductInfoWidgets( QLabel* plblProductName,
							 QLCDNumber* plcdCount, 
							 QDoubleSpinBox* psbxPrice )
							 : m_plblProductName( plblProductName ),
							   m_pLcdCount( plcdCount ),
							   m_psbxPrice( psbxPrice )
		{}
	public:
		//
		//	Main Interface
		//
		bool isEmpty()                const { return !m_plblProductName 
			                                      || !m_pLcdCount 
												  || !m_psbxPrice; }
		// GET Methods
		QLabel*         getNameLabel()    const { return m_plblProductName; }
		QLCDNumber*     getLcdCount()     const { return m_pLcdCount; }
		QDoubleSpinBox* getPriceSpinBox() const { return m_psbxPrice; }
		// Info
		int     getCount() const { return m_pLcdCount ? m_pLcdCount->intValue() : -1; }
		QString getName () const { return m_plblProductName ? m_plblProductName->text() : QString(); }
		double  getPrice() const { return m_psbxPrice ? m_psbxPrice->value() : -1; }

	private:
		//
		//	Content
		//
		QLabel*         m_plblProductName;
		QLCDNumber*     m_pLcdCount;
		QDoubleSpinBox* m_psbxPrice;
	};
	////////////////////////////////////////////////////////////////////////////////

public:// Constructors
	CTotalViewer( QWidget* pwParent = nullptr );
	~CTotalViewer() = default;

public:
	bool isEmpty() const { return m_hshProductInfo.isEmpty(); }
	double GetTotalPrice() const { return ui.lcdTotalPrice->value(); }

public slots:
	//
	//	Public Slots
	//
    void OnNewSelection( QString const& sStoreName, 
						 QString const& sItemName, 
						 int            nCount );
	
	// 
	void OnUpdateTotalPrice();
	//  
	void OnClear();

signals:
	//
	//	Signals
	//
	void sigTotalPriceChanged( double fNewPrice );

private:
	//
	//	Tool Methdos
	//
	void AddProduct( QString const& sName, 
					 int nCount, 
					 double fDefaultPrice = 0 );
	// 
	void RemoveProduct( CProductInfoWidgets const& );
	// UpdateTotalPrice

private:
	//
	//	Content
	//
	Ui::TotalViewerWdgUI ui;

	QHash<QString, CProductInfoWidgets> m_hshProductInfo;
	t_lstProductBuyInfo  m_lstSelectedProducts;
};
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_TOTAL_VIEWER_WDG_H