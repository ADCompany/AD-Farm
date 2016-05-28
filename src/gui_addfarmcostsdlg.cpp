////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_addfarmcostsdlg.h"

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
// class CAddFarmCostsDlg
//

// Constructors
CAddFarmCostsDlg::CAddFarmCostsDlg(QWidget *pwParent)
	: QDialog(pwParent)
{
	m_uiAddFarmCostsDlg.setupUi(this);
	setFixedSize(300, 104);

	// Set Validators
	QIntValidator* pIntValidator = new QIntValidator();
	m_uiAddFarmCostsDlg.editCosts->setValidator(pIntValidator);

	// Connections
	FM_CONNECT(m_uiAddFarmCostsDlg.btnAddCosts, clicked(), this, OnAdd());
}

void CAddFarmCostsDlg::OnAdd()
{
	// Costs
	QString strCosts = m_uiAddFarmCostsDlg.editCosts->text();
	bool bValidCosts = !strCosts.isEmpty();

	SetLabelColor(m_uiAddFarmCostsDlg.lblCosts, bValidCosts);

	// Info text
	QString strInfoText = m_uiAddFarmCostsDlg.editInfoText->text();
	bValidCosts = !strInfoText.isEmpty();

	SetLabelColor(m_uiAddFarmCostsDlg.lblInfoText, bValidCosts);

	if (bValidCosts)
		QDialog::accept();
}

// setLabelColor
void CAddFarmCostsDlg::SetLabelColor(QLabel* pLabel, bool bValid)
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
