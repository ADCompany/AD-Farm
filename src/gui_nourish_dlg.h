#ifndef GUI_NOURISH_DLG_H
#define GUI_NOURISH_DLG_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
#include "ui_nourishstoreitem.h"

// Qt Includes
#include <QDialog>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	struct SItemNourishInfo
//
struct SItemNourishInfo
{
	QString sSourceStoreName;
	QString sTargetStoreName;
	QString sProductName;
	int     nProductCount;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	class CNourishStoreItemDlg
//
class CNourishStoreItemDlg : public QDialog
{
	Q_OBJECT

public:// Constructors
	CNourishStoreItemDlg( QStringList const&  lstStorageNames, QWidget* pwParent = nullptr );
	~CNourishStoreItemDlg() = default;

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
	void sigMoveStoreItem( SItemNourishInfo const& oInfo );

private:
	//
	//	Content
	//
	Ui::NourishStoreItemDlg ui;
	CMovingItemSelector* m_pwStoreExplorer;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Clear
inline void CNourishStoreItemDlg::Clear()
{
	// TODO
	m_pwStoreExplorer->OnClear();

	ui.wdgTargetStores->clear();
}
// Update
inline void CNourishStoreItemDlg::Update()
{
	FM_ASSERT( m_pwStoreExplorer );
	m_pwStoreExplorer->Update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // GUI_NOURISH_DLG_H