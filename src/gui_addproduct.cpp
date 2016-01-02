////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_addproduct.h"

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
// class CAddProductDlg
//

// Constructors
CAddProductDlg::CAddProductDlg(QWidget *pwParent)
	: QDialog(pwParent)
{
	m_uiAddProduct.setupUi(this);
	setFixedSize(300, 104);

	// Set Validators
//	QIntValidator* pIntValidator = new QIntValidator();
//	m_uiAddProduct.editPrimeCost->setValidator(pIntValidator);
//	pIntValidator = new QIntValidator();
//	m_uiAddProduct.editCount->setValidator(pIntValidator);

	// Connections
	FM_CONNECT(m_uiAddProduct.btnAddProduct, clicked(), this, OnAdd());
}

void CAddProductDlg::OnAdd()
{
	QString strFirstName = m_uiAddProduct.editName->text();
//	QString strCount = m_uiAddProduct.editCount->text();
//	QString strPrimeCost = m_uiAddProduct.editPrimeCost->text();
	bool bValidName = !strFirstName.isEmpty();
//	bool bValidCount = !strCount.isEmpty();
//	bool bValidPrimeCost = !strPrimeCost.isEmpty();

	SetLabelColor(m_uiAddProduct.lblName, bValidName);
//	SetLabelColor(m_uiAddProduct.lblCount, bValidCount);
//	SetLabelColor(m_uiAddProduct.lblPrimeCost, bValidPrimeCost);

	if (bValidName)
		QDialog::accept();
}

// setLabelColor
void CAddProductDlg::SetLabelColor(QLabel* pLabel, bool bValid)
{
	if (!pLabel)
		return;

	QPalette palette = pLabel->palette();
	palette.setColor(pLabel->foregroundRole(), bValid ? Qt::black : Qt::red);
	pLabel->setPalette(palette);
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
