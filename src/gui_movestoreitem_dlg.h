#ifndef GUI_MOVE_STORE_ITEM_DLG_H
#define GUI_MOVE_STORE_ITEM_DLG_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef FM_CORE_H
#	include "fm_core.h"
#endif

#ifndef GUI_STORE_EXPLORER_WDG_H
#	include "gui_storeexplorer_wdg.h"
#endif

// Ui
#include "ui_movestoreitem.h"

// Qt Includes
#include <QDialog>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	struct SItemMovingInfo
//
struct SItemMovingInfo
{
	QString sSourceStoreName;
	QString sTargetStoreName;
	QString sProductName;
	int     nProductCount;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CMoveStoreItemDlg
//
class CMoveStoreItemDlg : public QDialog
{
	Q_OBJECT

public:// Constructors
	CMoveStoreItemDlg( QStringList const&  lstStorageNames, QWidget* pwParent = nullptr );
	~CMoveStoreItemDlg() = default;

public:
	//
	//	Main Interface
	//
	// Clear Dialog
	inline void Clear();
	inline void Update();
	void AddStorageNames(QStringList const&  lstStorageNames);
	

protected slots:
	//
	//	Slots
	//
    void OnNewSelection( QString const& sStoreName, 
						 QString const& sItemName, 
						 int nCount);

signals:
	void sigMoveStoreItem( SItemMovingInfo const& oInfo );

private:
	//
	//	Content
	//
	Ui::MoveStoreItemDlgUI ui;
	CMovingItemSelector* m_pwStoreExplorer;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////

// Clear
inline void CMoveStoreItemDlg::Clear()
{
	// TODO
	m_pwStoreExplorer->OnClear();

	ui.wdgTargetStores->clear();
}
// Update
inline void CMoveStoreItemDlg::Update()
{
	FM_ASSERT( m_pwStoreExplorer );
	m_pwStoreExplorer->Update();
}

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
#endif // GUI_MOVE_STORE_ITEM_DLG_H