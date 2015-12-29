#ifndef GUI_ADD_PRODUCT_DLG_H
#define GUI_ADD_PRODUCT_DLG_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef FM_CORE_H
#	include "fm_core.h"
#endif

// Ui
#include "ui_addproduct.h"

// Qt Includes
#include <QDialog>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CAddProductDlg
//
class CAddProductDlg : public QDialog
{
	Q_OBJECT

public:// Constructors
	CAddProductDlg(QWidget* pwParent = nullptr);
	~CAddProductDlg() = default;

public:
	//
	//	Main Interface
	//
	// Customer Data Accessors
	inline QString	GetName() const;
	inline int		GetCount() const;
	inline double	GetPrimeCost() const;
	// Clear Dialog
	inline void Clear();

	public slots:
	void OnAdd();

protected:// Helper Methodes
	void SetLabelColor(QLabel* pLabel, bool bValid);

private:
	//
	//	Content
	//
	Ui::addProductDlg m_uiAddProduct;
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
// GetFirstName
inline QString CAddProductDlg::GetName() const
{
	return m_uiAddProduct.editName->text();
}
// GetLastName
inline int CAddProductDlg::GetCount() const
{
	QString strCount = m_uiAddProduct.editCount->text();
	return strCount.toInt();
}
// GetPhoneNumber
inline double CAddProductDlg::GetPrimeCost() const
{
	QString strPrimeCost = m_uiAddProduct.editPrimeCost->displayText();
	return strPrimeCost.toDouble();
}
// Clear
inline void CAddProductDlg::Clear()
{
	m_uiAddProduct.editName->clear();
	m_uiAddProduct.editCount->clear();
	m_uiAddProduct.editPrimeCost->clear();

	SetLabelColor(m_uiAddProduct.lblName, true);
	SetLabelColor(m_uiAddProduct.lblCount, true);
	SetLabelColor(m_uiAddProduct.lblPrimeCost, true);
}

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_ADD_PRODUCT_DLG_H