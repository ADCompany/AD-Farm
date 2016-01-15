#ifndef GUI_FARM_MANAGER_WIDGET_H
#define GUI_FARM_MANAGER_WIDGET_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "ui_farmmanager.h"
#include "ui_addgorcarq.h"
#include "gui_financeswidget.h"
#include "gui_storageswidget.h"
#include "gui_settingswidget.h"
#include "gui_customerswidget.h"
#include "gui_transactionswidget.h"
#include "gui_storagehistory_wdg.h"

#include "db_manager.h"

// Qt includes
#include <QWidget>
#include <QMainWindow>

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CFarmManagerWidget
//
class CFarmManagerWidget : public QMainWindow
{
	Q_OBJECT

public:
	CFarmManagerWidget(QWidget *parent = 0);
	~CFarmManagerWidget() = default;


	protected slots:
	//
	//	Action Handlers
	//
	void OnActionDeals();
	void OnActionCustomers();
	void OnActionStorages();
	void OnActionFinances();
	void OnActionSettings();
	void OnActionNewDeal();
	void OnActionAddCustomer();

private:
	Ui::FarmManagerClass ui;

	Ui::Dialog dialogUi;

	std::shared_ptr<CStoragesWidget>		m_pwStorages;
	std::shared_ptr<CFinancesWidget>		m_pwFinances;
	std::shared_ptr<CSettingsWidget>		m_pwSettings;
	std::shared_ptr<CCustomersWidget>		m_pwCustomers;
	std::shared_ptr<CTransactionsWidget>	m_pwTrnsactions;
	std::shared_ptr<CStorageHistoryWidget>	m_pwStorageHistory;

	std::shared_ptr<QDialog> m_CreateTrnDlg;

	std::shared_ptr<db::CDBManager> m_pDBManager;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_FARM_MANAGER_WIDGET_H
