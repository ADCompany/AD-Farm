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
#ifndef GUI_NOURISH_DLG_H
#	include "gui_nourish_dlg.h"
#endif
#ifndef GUI_ADD_STORE_ITEM_DLG_H
#	include "gui_addstoreitem_dlg.h"
#endif
#ifndef GUI_DECLINE_STORE_ITEM_DLG_H
#	include "gui_declinestoreitemdlg.h"
#endif
#ifndef GUI_ADD_FARM_COSTS_DLG_H
#	include "gui_addfarmcostsdlg.h"
#endif
#ifndef GUI_ADD_STORAGE_COSTS_DLG_H
#	include "gui_addstoragecostsdlg.h"
#endif
#ifndef GUI_MOVE_STORE_ITEM_DLG_H
#	include "gui_movestoreitem_dlg.h"
#endif



// Ui
#include "ui_storages.h"
#include "ui_addstoreitem.h"

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

public slots:// Slots
	void onShowMoveStoreItemDialog();
	void onShowNourishStoreItemDialog();

protected:// Helper Methodes
	void UpdateData(bool bFull = false);

protected slots:// Slots
	inline void onChangeData();

	void onSelectStorage(QModelIndex const& modelIndex);
	void onSelectProduct(QModelIndex const& modelIndex);

	void onMoveStoreItem(SItemMovingInfo const& itemMovingInfo);
	void onNourishStoreItem(SItemNourishInfo const& itemMovingInfo);

	void onAddItem();
	void onAddItemClicked();

	void onDeclineItem();
	void onDeclineItemClicked();

	void onAddStorageCosts();
	void onAddStorageCostsClicked();

	void onAddFarmCosts();
	void onAddFarmCostsClicked();

private:
	Ui::storages m_uiStorages;

	//QDialog* m_pCreateSplDlg;
	std::shared_ptr<CAddStoreItem>			m_pAddItemDlg;
	std::shared_ptr<CDeclineStoreItem>		m_pDeclineItemDlg;
	std::shared_ptr<CAddFarmCostsDlg>		m_pAddFarmCostsDlg;
	std::shared_ptr<CAddStorageCostsDlg>	m_pAddStorageCostsDlg;
	std::shared_ptr<CMoveStoreItemDlg>		m_pMoveStoreItemDlg;
	std::shared_ptr<CNourishStoreItemDlg>	m_pNourishStoreItemDlg;

	std::shared_ptr<db::CStoragesData> m_pStoragesData;
	std::shared_ptr<db::CDBManager> m_pDBManager;

	std::shared_ptr<QStringListModel> m_pStringListModel;

	QModelIndex m_currModelIndex;

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

// Protected Slots
inline void CStoragesWidget::onChangeData()
{
	UpdateData(true);
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_STORAGES_WIDGET_H