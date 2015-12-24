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

#ifndef DB_MANAGER_H
#	include "db_manager.h"
#endif

// Ui
#include "ui_customers.h"

// Qt Includes
#include <QWidget>
#include <QSqlTableModel>

// STD Includes
#include <memory>
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
	CCustomersWidget(QWidget* pwParent = nullptr);
	~CCustomersWidget() = default;

public:// Interface Methodes
	void SetDBManager(std::shared_ptr<db::CDBManager> pDBManager);
	inline std::shared_ptr<QSqlTableModel> GetTableModel();

protected:// Override Methodes
	bool event(QEvent* pEvent) override;

protected:// Helper Methodes
	inline void UpdateData();

	void AddCustomer(QString const& strFirstName, QString const& strLastName, int nPhoneNumber = -1);

	void RemoveCustomer(int nRow);
	void RemoveCustomer(QString const& strFirstName, QString const& strLastName);

protected slots:// Slots
	void onAddCustomer();

private:// Members
	Ui::customersWidget	m_uiCustomersWidget;
	CAddCustomerDlg*	m_pAddCustomerDlg;

	std::shared_ptr<db::CDBManager>		m_pDBManager;
	std::shared_ptr<db::CCustomersData> m_pCustomersData;

	std::shared_ptr<QSqlTableModel>	m_pSqlTableModel;

};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
// class CCustomersWidget
//

// Interface Methodes
inline std::shared_ptr<QSqlTableModel> CCustomersWidget::GetTableModel()
{
	return m_pSqlTableModel;
}

// Helper Functions
inline void CCustomersWidget::UpdateData()
{
	m_uiCustomersWidget.tableView->setModel(m_pSqlTableModel.get());
	m_uiCustomersWidget.tableView->update();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_CUSTOMERS_WIDGET_H