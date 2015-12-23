#ifndef FM_FARM_MANAGER_H
#define FM_FARM_MANAGER_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "ui_farmmanager.h"
#include "ui_addgorcarq.h"
#include "gui_storageswidget.h"
#include "gui_customerswidget.h"
#include "gui_transactionswidget.h"

#include "db_manager.h"

// Qt includes
#include <QWidget>
#include <QMainWindow>

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CFarmManager
//
class CFarmManager : public QMainWindow
{
	Q_OBJECT

public:
	CFarmManager(QWidget *parent = 0);
	~CFarmManager();

private:
	Ui::FarmManagerClass ui;

	Ui::Dialog           dialogUi;

	gui::CStoragesWidget*		m_pwStorages;
	gui::CCustomersWidget*		m_pwCustomers;
	gui::CTransactionsWidget*	m_pwTrnsactions;

	QDialog* m_CreateTrnDlg;

	std::shared_ptr<db::CDBManager> m_pDBManager;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // FM_FARM_MANAGER_H
