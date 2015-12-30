#ifndef GUI_ADD_FARM_COSTS_DLG_H
#define GUI_ADD_FARM_COSTS_DLG_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef FM_CORE_H
#	include "fm_core.h"
#endif

// Ui
#include "ui_addfarmcosts.h"

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
// class CAddFarmCostsDlg
//
class CAddFarmCostsDlg : public QDialog
{
	Q_OBJECT

public:// Constructors
	CAddFarmCostsDlg(QWidget* pwParent = nullptr);
	~CAddFarmCostsDlg() = default;

public:
	//
	//	Main Interface
	//
	// Customer Data Accessors
	inline double	GetCosts() const;
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
	//Ui::addStorageCostsDlg m_uiStorageCostsDlg;
	Ui::addFarmCostsDlg m_uiAddFarmCostsDlg;
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//

inline double CAddFarmCostsDlg::GetCosts() const
{
	QString strCosts = m_uiAddFarmCostsDlg.editCosts->displayText();
	return strCosts.toDouble();
}
// Clear
inline void CAddFarmCostsDlg::Clear()
{
	m_uiAddFarmCostsDlg.editCosts->clear();

	SetLabelColor(m_uiAddFarmCostsDlg.lblCosts, true);
}

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_ADD_FARM_COSTS_DLG_H