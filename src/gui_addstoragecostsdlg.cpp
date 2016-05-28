////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_addstoragecostsdlg.h"

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
// class CAddStorageCostsDlg
//

// Constructors
CAddStorageCostsDlg::CAddStorageCostsDlg(QWidget *pwParent)
	: QDialog(pwParent)
{
	m_uiAddStorageCostsDlg.setupUi(this);
	setFixedSize(300, 104);

	// Set Validators
	QIntValidator* pIntValidator = new QIntValidator();
	m_uiAddStorageCostsDlg.editCosts->setValidator(pIntValidator);

	// Connections
	FM_CONNECT(m_uiAddStorageCostsDlg.btnAddCosts, clicked(), this, OnAdd());
}

void CAddStorageCostsDlg::OnAdd()
{
	// Costs
	QString strCosts = m_uiAddStorageCostsDlg.editCosts->text();
	bool bValidCosts = !strCosts.isEmpty();

	SetLabelColor(m_uiAddStorageCostsDlg.lblCosts, bValidCosts);

	// Info text
	//QString strInfoText = m_uiAddStorageCostsDlg.editInfoText->text();
	//bValidCosts = !strInfoText.isEmpty();

	//SetLabelColor(m_uiAddStorageCostsDlg.lblInfoText, bValidCosts);

	if (bValidCosts)
		QDialog::accept();
}

// setLabelColor
void CAddStorageCostsDlg::SetLabelColor(QLabel* pLabel, bool bValid)
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
