////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_addstore_dlg.h"

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
// class CAddProductDlg
//

// Constructors
CAddStoreDlg::CAddStoreDlg( QWidget *pwParent )
	: QDialog(pwParent)
{
	ui.setupUi(this);
	setFixedSize(285, 104);

	// Connections
	FM_CONNECT(ui.btnAdd, clicked(), this, OnAdd());
}

// OnAdd
void CAddStoreDlg::OnAdd()
{
	QString sStoreName = ui.editStoreName->text();
	bool bValidName = !sStoreName.isEmpty();
	SetLabelColor(ui.label, bValidName);
	if (bValidName)
		QDialog::accept();
}

// setLabelColor
void CAddStoreDlg::SetLabelColor( QLabel* pLabel, bool bValid )
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