#ifndef GUI_ADD_STORE_DLG_H
#define GUI_ADD_STORE_DLG_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef FM_CORE_H
#	include "fm_core.h"
#endif

// Ui
#include "ui_addstore.h"

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
// class CAddStoreDlg
//
class CAddStoreDlg : public QDialog
{
	Q_OBJECT

public:// Constructors
	CAddStoreDlg( QWidget* pwParent = nullptr );
	~CAddStoreDlg() = default;

public:
	//
	//	Main Interface
	//
	// Get Store Name
	inline QString	GetName() const;
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
	Ui::AddStoreDlgUI ui;
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
// GetFirstName
inline QString CAddStoreDlg::GetName() const
{
	return ui.editStoreName->text();
}

// Clear
inline void CAddStoreDlg::Clear()
{
	ui.editStoreName->clear();
	SetLabelColor( ui.label, true );
}

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_ADD_STORE_DLG_H