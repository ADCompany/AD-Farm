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
#include "gui_addstore_dlg.h"
// Ui
#include "ui_settings.h"

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

	void RemoveProduct(int nRow);
	void RemoveStorage(int nRow);

protected slots:
	void onChangeData()
	{
		UpdateData();
	}
	void onAddNewProduct()
	{
		QString strNewProductName = m_pAddProductDlg->GetName();
		int nCount = m_pAddProductDlg->GetCount();
		double dPrimeCost = m_pAddProductDlg->GetPrimeCost();

		m_pStoragesData->AddNewProduct(strNewProductName, nCount, dPrimeCost);
	}
	void onAddNewStore()
	{
		QString strStoreName = m_pAddStoreDlg->GetName();
		m_pStoragesData->AddNewStore(strStoreName);
	}
	void onShowAddProductDlg()
	{
		if (m_pAddProductDlg == nullptr)
			return;

		m_pAddProductDlg->Clear();
		m_pAddProductDlg->show();
	}
	void onShowAddStoreDlg()
	{
		if (m_pAddStoreDlg == nullptr)
			return;

		m_pAddStoreDlg->Clear();
		m_pAddStoreDlg->show();
	}
	void onProductMenuShow(QPoint const& pos)
	{
		QModelIndex idxCurrent = m_uiSettingsWidget.tableView->indexAt(pos);
		m_iCurrModelIndex = idxCurrent;
		if (idxCurrent.row() >= 0)
			m_pProductMenu->popup(m_uiSettingsWidget.tableView->viewport()->mapToGlobal(pos));
	}
	void onStorageMenuShow(QPoint const& pos)
	{
		QModelIndex idxCurrent = m_uiSettingsWidget.tableView->indexAt(pos);
		m_iCurrModelIndex = idxCurrent;
		if (idxCurrent.row() >= 0)
			m_pStorageMenu->popup(m_uiSettingsWidget.tableView_2->viewport()->mapToGlobal(pos));
	}
	void onRemoveProduct()
	{
		RemoveProduct(m_iCurrModelIndex.row());
	}
	void onRemoveStorage()
	{
		RemoveStorage(m_iCurrModelIndex.row());
	}
private:// Members
	Ui::settingsWidget m_uiSettingsWidget;
	
	std::shared_ptr<CAddProductDlg> m_pAddProductDlg;
	std::shared_ptr<CAddStoreDlg> m_pAddStoreDlg;

	std::shared_ptr<QMenu> m_pProductMenu;
	std::shared_ptr<QMenu> m_pStorageMenu;

	QModelIndex m_iCurrModelIndex;

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