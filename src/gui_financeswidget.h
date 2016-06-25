#ifndef GUI_FINANCES_WIDGET_H
#define GUI_FINANCES_WIDGET_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef DB_CUSTOMERS_DATA_H
#	include "db_customersdata.h"
#endif
#ifndef DB_STORAGES_DATA_H
#	include "db_storagesdata.h"
#endif

// Ui
#include "ui_finances.h"

// Qt Includes

// STD Includes

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CFinancesWidget
//
class CFinancesWidget : public QWidget
{
	Q_OBJECT

public:// Constructors
	CFinancesWidget(QWidget* pwParent = nullptr, std::shared_ptr<db::CDBManager> pDBManager = nullptr);
	~CFinancesWidget() = default;

public:// Interface Methodes
	void SetDBManager(std::shared_ptr<db::CDBManager> pDBManager);

protected:// Helper Methodes
	inline void UpdateData();

protected slots:
	void onChangeData()
	{
		UpdateData();
	}

	inline void OnGroupSelectionChanged(QString const& strGroup);

private:// Members
	Ui::financesWidget m_uiFinancesWidget;

	std::shared_ptr<db::CCustomersData> m_pCustomersData;
	std::shared_ptr<db::CStoragesData> m_pStoragesData;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CFinancesWidget
//

// Interface Methodes

// Helper Functions
inline void CFinancesWidget::UpdateData()
{

	m_uiFinancesWidget.tableViewCustomers->setModel(m_pCustomersData->GetSqlTableModel().get());
	m_uiFinancesWidget.tableViewCustomers->hideColumn(0);
	m_uiFinancesWidget.tableViewCustomers->resizeColumnsToContents();
	m_uiFinancesWidget.tableViewCustomers->update();

	QStringList strListGroups;
	strListGroups.append(QString());
	strListGroups.append(m_pStoragesData->GetGroupNames());

	m_uiFinancesWidget.comboBoxGroups->clear();
	m_uiFinancesWidget.comboBoxGroups->addItems(strListGroups);

	double dIncome = 0;
	double dExpenses = 0;
	double dProfit = 0;
	QString strGroupName = m_uiFinancesWidget.comboBoxGroups->currentText();
	if (strGroupName.isEmpty())
		m_uiFinancesWidget.tableViewFerma->setModel(m_pStoragesData->GetFarmHistorySqlTableModel().get());
	else
	{
		m_uiFinancesWidget.tableViewFerma->setModel(m_pStoragesData->GetGroupHistorySqlTableModel(strGroupName).get());
		dIncome = m_pStoragesData->GetGroupIncome(strGroupName);
		dExpenses = m_pStoragesData->GetGroupExpenses(strGroupName);
		dProfit = dIncome - dExpenses;
	}
	m_uiFinancesWidget.lcdExpensesTotalPrice->display(dExpenses);
	m_uiFinancesWidget.lcdIncomeTotalPrice->display(dIncome);
	m_uiFinancesWidget.lcdProfitTotalPrice->display(dProfit);

	m_uiFinancesWidget.tableViewFerma->resizeColumnsToContents();
	m_uiFinancesWidget.tableViewFerma->update();
}

inline void CFinancesWidget::OnGroupSelectionChanged(QString const& strGroup)
{

	double dIncome = 0;
	double dExpenses = 0;
	double dProfit = 0;
	QString strGroupName = m_uiFinancesWidget.comboBoxGroups->currentText();
	if (strGroupName.isEmpty())
		m_uiFinancesWidget.tableViewFerma->setModel(m_pStoragesData->GetFarmHistorySqlTableModel().get());
	else
	{
		m_uiFinancesWidget.tableViewFerma->setModel(m_pStoragesData->GetGroupHistorySqlTableModel(strGroupName).get());
		dIncome = m_pStoragesData->GetGroupIncome(strGroupName);
		dExpenses = m_pStoragesData->GetGroupExpenses(strGroupName);
		dProfit = dIncome - dExpenses;
	}

	m_uiFinancesWidget.lcdExpensesTotalPrice->display(dExpenses);
	m_uiFinancesWidget.lcdIncomeTotalPrice->display(dIncome);
	m_uiFinancesWidget.lcdProfitTotalPrice->display(dProfit);

	m_uiFinancesWidget.tableViewFerma->resizeColumnsToContents();
	m_uiFinancesWidget.tableViewFerma->update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // GUI_FINANCES_WIDGET_H