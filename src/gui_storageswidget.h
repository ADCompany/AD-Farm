#ifndef GUI_STORAGES_WIDGET_H
#define GUI_STORAGES_WIDGET_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef DB_MANAGER_H
#	include "db_manager.h"
#endif
#ifndef DB_STORAGES_DATA_H
#	include "db_storagesdata.h"
#endif
#ifndef GUI_ADD_STORE_ITEM_DLG_H
#	include "gui_addstoreitem_dlg.h"
#endif


// Ui
#include "ui_storages.h"
#include "ui_addstoreitem.h"
#include "ui_subtractproduct.h"

// Qt includes
#include <QWidget>
#include <QStringListModel>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesWidget
//
class CStoragesWidget : public QWidget
{
	Q_OBJECT
public:// Constructors
	CStoragesWidget(QWidget* pwParent = nullptr, std::shared_ptr<db::CDBManager> pDBManager = nullptr);
	~CStoragesWidget() = default;

public:// Interface Methodes
	void SetDBManager(std::shared_ptr<db::CDBManager> pDBManager);
	inline std::shared_ptr<QSqlTableModel> GetTableModel();

protected:// Helper Methodes
	inline void UpdateData(bool bFull = false);


protected slots:// Slots
	void onChangeData()
	{
		UpdateData(true);
	}
	void onActivatedStorage(QModelIndex const& modelIndex)
	{
		QString strSelectCustomerName = m_pStringListModel->data(modelIndex, 0).toString();
		if (strSelectCustomerName == m_strCurrentStorageName)
			return;

		m_strCurrentStorageName = strSelectCustomerName;

		UpdateData();
	}
	//
	void onAddItemClicked();

private:
	Ui::storages m_uiStorages;

	//QDialog* m_pCreateSplDlg;
	CAddStoreItem* m_pAddItemDlg;

	std::shared_ptr<db::CStoragesData> m_pStoragesData;
	std::shared_ptr<db::CDBManager> m_pDBManager;

	std::shared_ptr<QStringListModel> m_pStringListModel;
	QString m_strCurrentStorageName;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesWidget
//

// Interface Methodes
inline std::shared_ptr<QSqlTableModel> CStoragesWidget::GetTableModel()
{
	return nullptr;
}

// Helper Functions
inline void CStoragesWidget::UpdateData(bool bFull)
{
	if (bFull)
	{
		m_pStringListModel = std::shared_ptr<QStringListModel>(new QStringListModel(m_pStoragesData->GetStorageNames()));

		m_uiStorages.listView->setModel(m_pStringListModel.get());
		m_uiStorages.listView->update();
	}

	if (m_strCurrentStorageName == "")
		return;

	m_uiStorages.tableView->setModel(m_pStoragesData->GetSqlTableModelByStorageName(m_strCurrentStorageName).get());
	m_uiStorages.tableView->update();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_STORAGES_WIDGET_H