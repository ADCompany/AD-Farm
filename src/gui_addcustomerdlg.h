#ifndef GUI_ADD_CUSTOMER_DLG_H
#define GUI_ADD_CUSTOMER_DLG_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef FM_CORE_H
#	include "fm_core.h"
#endif

// Ui
#include "ui_addcustomer.h"

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
// class CAddCustomerDlg
//
class CAddCustomerDlg : public QDialog
{
	Q_OBJECT

public:// Constructors
	CAddCustomerDlg( QWidget* pwParent = nullptr );
	~CAddCustomerDlg() = default;

public:
	//
	//	Main Interface
	//
	// Customer Data Accessors
	inline QString GetFirstName() const;
	inline QString GetLastName() const;
	inline QString GetPhoneNumber() const;
	// Clear Dialog
	inline void Clear();

public slots:
	void OnAdd();

protected:// Helper Methodes
	void SetLabelColor( QLabel* pLabel, bool bValid );

private:
	//
	//	Content
	//
	Ui::AddCustomersUI	m_uiAddCustomer;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CPhoneNuberValidator
//
class CPhoneNuberValidator : public QIntValidator
{
	typedef QIntValidator Base;
public:
	//
	State validate( QString& sInput, int& pos ) const override
	{
		State eState = Base::validate( sInput, pos );
		if (eState == Intermediate && sInput[0] == '0')
			 return Acceptable;
		return eState;
	}
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
// GetFirstName
inline QString CAddCustomerDlg::GetFirstName() const
{ return m_uiAddCustomer.editFirstName->text(); }
// GetLastName
inline QString CAddCustomerDlg::GetLastName() const
{ return m_uiAddCustomer.editLastName->text(); }
// GetPhoneNumber
inline QString CAddCustomerDlg::GetPhoneNumber() const
{ 
	QString sPhone = m_uiAddCustomer.editPhone->displayText();
	return sPhone;
}
// Clear
inline void CAddCustomerDlg::Clear()
{
	m_uiAddCustomer.editFirstName->clear();
	m_uiAddCustomer.editLastName->clear();
	m_uiAddCustomer.editPhone->clear();

	SetLabelColor(m_uiAddCustomer.lblFirstName, true);
	SetLabelColor(m_uiAddCustomer.lblLastName, true);
}

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
#endif // GUI_ADD_CUSTOMER_DLG_H