#ifndef GUI_STORE_EXPLORER_WDG_H
#define GUI_STORE_EXPLORER_WDG_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef GUI_QUERY_TABLE_MODEL_H
#	include "gui_querytablemodel.h"
#endif

// UI
#include "ui_storeexplorer.h"

// Qt Includes
#include <QWidget>
#include <QHash>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Storage Cpdateing Configuration
//
struct SProductInfo
{
	SProductInfo( QString sProductName, int nProductCount = 0 )
		: sName( sProductName ),
		  nCount( nProductCount )
	{}

	// 
	//	Content
	//
	QString sName;
	int     nCount;

public:
	// operator == 
	bool operator==(SProductInfo const& rhs)
	{ return sName == rhs.sName; }
};

typedef QList<SProductInfo> t_lstProductInfo;

struct SStoreageInfo
{
	SStoreageInfo( QString sStorageName, QList<SProductInfo> const& lst = QList<SProductInfo>() )
		: sStoreageName( sStorageName ),
		lstProducts( lst )
	{}

	//
	//	Content
	//
	QString             sStoreageName;
	t_lstProductInfo    lstProducts;

public:
	// operator ==
	bool operator==(SStoreageInfo const& rhs)
	{ return sStoreageName == rhs.sStoreageName; }
};

typedef QList<SStoreageInfo> CFarmInfo;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CStoreExplorer
//
class CStoreExplorer : public QWidget
{

	Q_OBJECT

public:// Constructors
	CStoreExplorer( QStringList const& lstStores, QWidget* pwParent = nullptr );
	~CStoreExplorer() = default;

public:
	//
	//	Main Interface
	//
	CFarmInfo GetFarmUpdateInfo() const { return m_lstConfig; };

public slots:
	//
	//	Public Slots
	//
	void OnCurrentStoreChanged( QListWidgetItem * pCurrent, QListWidgetItem * pPrevious );
	void OnCurrentProductChanged( QModelIndex const& current, QModelIndex const& previous );
	// Add Selection
	void OnAdd();
	void OnCountCountChanged( int nCount );
	// 
	void OnClear();

signals:
	//
	//	Signals
	//
	void sigNewSelection( QString const& sStoreName, QString const& sItemName, int nCount );

private:
	//
	//	Tool Methdos
	//
	// makeKey
	QString makeKey( QString const& sProductName ) const;
	QString makeKey( QString const& sStoreName,
					 QString const& sProductName ) const;

private:
	//
	//	Content
	//
	Ui::StoreExplorerUI ui;
	//
	CStoreContentModel* m_pContentModel;

	// Registers
	QString m_sCurrentProductName;
	QHash<QString, int> m_hshCountSelection;

	//
	CFarmInfo m_lstConfig;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_STORE_EXPLORER_WDG_H