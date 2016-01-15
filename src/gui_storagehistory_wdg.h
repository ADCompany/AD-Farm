#ifndef GUI_STORAGE_HISTORY_WDG_H
#define GUI_STORAGE_HISTORY_WDG_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "db_storagesdata.h"

#include "ui_storagehistory.h"

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
// class CStorageHistoryWidget
//
class CStorageHistoryWidget : public QWidget
{
	Q_OBJECT
public:// Constructors
	CStorageHistoryWidget(QWidget* pwParent = nullptr, std::shared_ptr<db::CDBManager> pDBManager = nullptr);
	~CStorageHistoryWidget() = default;

public:// Interface Methodes
	void SetDBManager(std::shared_ptr<db::CDBManager> pDBManager);

	inline std::shared_ptr<QSqlTableModel> GetTableModel();

protected:// Helper Methodes
	void UpdateData(bool bFull = false);

protected slots:// Slots
	void onChangeData()
	{
		UpdateData(true);
	}
	void onChangeDataByStorageData()
	{
		UpdateData();
	}
	void onActivatedStorage(QModelIndex const& modelIndex)
	{
		QString strSelectStoraegName = m_pStringListModel->data(modelIndex, 0).toString();
		if (strSelectStoraegName == m_strCurrentStorageName)
			return;
		
		m_strCurrentStorageName = strSelectStoraegName;
	
		UpdateData();
	}
private:// Members
	std::shared_ptr<db::CStoragesData> m_pStoragesData;
	std::shared_ptr<QStringListModel> m_pStringListModel;
	QString m_strCurrentStorageName;

	Ui::storageHistoryWidget ui;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CStorageHistoryWidget
//

// Interface Methodes
inline std::shared_ptr<QSqlTableModel> CStorageHistoryWidget::GetTableModel()
{
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_STORAGE_HISTORY_WDG_H