#ifndef GUI_SETTINGS_WIDGET_H
#define GUI_SETTINGS_WIDGET_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef DB_STORAGES_DATA_H
#	include "db_storagesdata.h"
#endif
#include "gui_addproduct.h"

// Ui
#include "ui_settings.h"

// Qt Includes

// STD Includes

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CSettingsWidget
//
class CSettingsWidget : public QWidget
{
	Q_OBJECT

public:// Constructors
	CSettingsWidget(QWidget* pwParent = nullptr, std::shared_ptr<db::CDBManager> pDBManager = nullptr);
	~CSettingsWidget() = default;

public:// Interface Methodes
	void SetDBManager(std::shared_ptr<db::CDBManager> pDBManager);

protected:// Helper Methodes
	inline void UpdateData();

protected slots:
	void onChangeData()
	{
		UpdateData();
	}
	void onAddCustomer()
	{

	}
	void onShowAddProductDlg()
	{
		if (m_pAddProductDlg == nullptr)
			return;

		m_pAddProductDlg->Clear();
		m_pAddProductDlg->show();
	}
private:// Members
	Ui::settingsWidget m_uiSettingsWidget;
	
	std::shared_ptr<CAddProductDlg> m_pAddProductDlg;

	std::shared_ptr<db::CStoragesData> m_pStoragesData;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CSettingsWidget
//

// Interface Methodes

// Helper Functions
inline void CSettingsWidget::UpdateData()
{
	m_uiSettingsWidget.tableView->setModel(m_pStoragesData->GetProductSqlQueryModel(db::CStoragesData::table::producte).get());
	m_uiSettingsWidget.tableView->hideColumn(0);
	m_uiSettingsWidget.tableView->update();
	m_uiSettingsWidget.tableView_2->setModel(m_pStoragesData->GetStorageSqlQueryModel(db::CStoragesData::table::storage).get());
	m_uiSettingsWidget.tableView_2->update();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_SETTINGS_WIDGET_H