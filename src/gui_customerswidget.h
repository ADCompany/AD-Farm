#ifndef GUI_CUSTOMERS_WIDGET_H
#define GUI_CUSTOMERS_WIDGET_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef DB_CUSTOMERS_DATA_H
#	include "db_customersdata.h"
#endif
#ifndef GUI_ADD_CUSTOMER_DLG_H
#	include "gui_addcustomerdlg.h"
#endif

// Ui
#include "ui_customers.h"

// Qt Includes
#include <QMenu>

// STD Includes

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CCustomersWidget
//
class CCustomersWidget : public QWidget
{
	Q_OBJECT

public:// Constructors
	CCustomersWidget(QWidget* pwParent = nullptr, std::shared_ptr<db::CDBManager> pDBManager = nullptr);
	~CCustomersWidget() = default;

public:// Interface Methodes
	void SetDBManager(std::shared_ptr<db::CDBManager> pDBManager);
	inline std::shared_ptr<QSqlTableModel> GetTableModel();

protected:// Helper Methodes
	inline void UpdateData();

	void AddCustomer(QString const& strFirstName, QString const& strLastName, int nDept = 0, int nPhoneNumber = 0);

	void RemoveCustomer(int nRow);
	void RemoveCustomer(QString const& strFirstName, QString const& strLastName);

protected slots:// Slots
	void onAddCustomer();
	void onRemoveCustomer()
	{
		RemoveCustomer(m_iCurrModelIndex.row());
	}
	void onMenuShow(QPoint const& pos)
	{
		QModelIndex idxCurrent = m_uiCustomersWidget.tableView->indexAt(pos);
		m_iCurrModelIndex = idxCurrent;
		if (idxCurrent.row() >= 0)
			m_pMenu->popup(m_uiCustomersWidget.tableView->viewport()->mapToGlobal(pos));
	}

private:// Members
	Ui::customersWidget	m_uiCustomersWidget;
	std::shared_ptr<CAddCustomerDlg> m_pAddCustomerDlg;

	std::shared_ptr<db::CCustomersData> m_pCustomersData;

	std::shared_ptr<QMenu> m_pMenu;
	QModelIndex m_iCurrModelIndex;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CCustomersWidget
//

// Interface Methodes
inline std::shared_ptr<QSqlTableModel> CCustomersWidget::GetTableModel()
{
	return m_pCustomersData->GetSqlTableModel();
}

// Helper Functions
inline void CCustomersWidget::UpdateData()
{
	m_uiCustomersWidget.tableView->setModel(m_pCustomersData->GetSqlTableModel().get());
	m_uiCustomersWidget.tableView->hideColumn(0);
	m_uiCustomersWidget.tableView->hideColumn(3);
	m_uiCustomersWidget.tableView->update();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_CUSTOMERS_WIDGET_H