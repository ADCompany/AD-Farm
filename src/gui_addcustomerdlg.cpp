////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_addcustomerdlg.h"

// Qt Includes
#include <QMessageBox>
#include <QTextCodec>
#include <QRegExpValidator>
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
CAddCustomerDlg::CAddCustomerDlg( QWidget *pwParent )
	: QDialog( pwParent )
{
	m_uiAddCustomer.setupUi( this );
	setFixedSize( 416, 161 );
	// Set Validators
	QIntValidator* pPhoneNumberValidator = new QIntValidator();
	pPhoneNumberValidator->setBottom( 0 );
	m_uiAddCustomer.editPhone->setValidator( pPhoneNumberValidator );

	// Connections
	FM_CONNECT( m_uiAddCustomer.btnAddCustomer, clicked(), this, OnAdd() );
}

void CAddCustomerDlg::OnAdd()
{
	QString sFirstName = m_uiAddCustomer.editFirstName->text();
	QString sLastName  = m_uiAddCustomer.editLastName->text();
	QString sPhone     = m_uiAddCustomer.editPhone->text();
	bool bValidFirstName = !sFirstName.isEmpty();
	bool bValidLastName  = !sLastName.isEmpty();

	SetLabelColor( m_uiAddCustomer.lblFirstName, bValidFirstName );
	SetLabelColor( m_uiAddCustomer.lblLastName,  bValidLastName );


	/*QMessageBox::critical( this,
	QString::fromUtf8(
	"\325\211\325\254\326\200\325\241\326\201"
	"\325\276\325\241\325\256 \325\244\325\241\325\267\325\277" ),
	QString::fromUtf8(
	"\324\262\325\270\325\254\325\270\326\200"
	"\325\244\325\241\325\267\325\277\325\245\326\200\325\250"
	"\325\272\325\245\325\277\326\204 "
	"\325\247 \325\254\326\200\325\241\326\201"
	"\325\276\325\245\325\266" ) );*/

	if (bValidFirstName && bValidLastName)
		QDialog::accept();
}

// setLabelColor
void CAddCustomerDlg::SetLabelColor( QLabel* pLabel, bool bValid )
{
	if (!pLabel)
		return;
	QPalette palette = pLabel->palette();
	palette.setColor( pLabel->foregroundRole(), bValid? Qt::black : Qt::red );
	pLabel->setPalette( palette );
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
