#ifndef DB_MANAGER_H
#define DB_MANAGER_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef FM_CORE_H
#	include "fm_core.h"
#endif
#ifndef DB_CONNECTION_STARTER_H
#	include "db_connectionstarter.h"
#endif

// Qt includes
#include <QList>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QDesktopServices>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DB Macros
//

// If execution fails throws an exception with error message
#define EXECUTE_QUERY(_query_, _cmd_)             \
	if(!_query_.exec(_cmd_))                      \
		throw CException(qtr("DB Query Failed: ") \
                              .append(_query_.lastError().databaseText()));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// interface IDBComponent
//
class CSqlQueryModel : public QSqlQueryModel
{
public:// Constructors, Destructor
	inline CSqlQueryModel() = default;
	inline ~CSqlQueryModel() = default;

public:// Interface Methods
	inline void SetDoubleColumnIndices(QList<int> const& lstDoubleColumnIndices)
	{
		m_lstDoubleColumnIndices = lstDoubleColumnIndices;
	}

protected:
	QVariant data(const QModelIndex & i, int role = Qt::DisplayRole) const
	{
		QVariant d = QSqlQueryModel::data(i, role);
		if (m_lstDoubleColumnIndices.contains(i.column()) && role == Qt::DisplayRole)
		{
			return QVariant(QString::number(d.toDouble(), 'f', 0));
		}

		return d;
	}

private:// Contents
	QList<int> m_lstDoubleColumnIndices;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CDBManager;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// interface IDBComponent
//
class IDBComponent : public QObject
{
public:// Constructors
	inline IDBComponent() = default;
	virtual ~IDBComponent() = default;

public:// Interface Methodes
	virtual void Initialize() = 0;

	virtual std::shared_ptr<CDBManager> GetDBManager() = 0;
	virtual void SetDBManager(std::shared_ptr<CDBManager> pDBManager) = 0;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CDBComponent
//
class CDBComponent : public IDBComponent
{
public:// Constructors
	inline CDBComponent(std::shared_ptr<CDBManager> pDBManager = nullptr);
	virtual ~CDBComponent() = default;

public:// Interface Methodes
	virtual void Initialize()
	{};

	inline virtual void SetDBManager(std::shared_ptr<CDBManager> pDBManager);
	inline virtual std::shared_ptr<CDBManager> GetDBManager();

	virtual std::shared_ptr<QSqlTableModel> GetSqlTableModel(QString const& strTableName);
	virtual std::shared_ptr<QSqlTableModel> GetSqlTableModel(QString const& strTableName) const;

	virtual bool SetSqlTableModel(QString const& strTableName, std::shared_ptr<QSqlTableModel> pSqlTableModel);
	virtual bool SetSqlTableModel(std::shared_ptr<QSqlTableModel> pSqlTableModel);

	virtual QList<QString> GetColumnsName(QString const& strTableName);

protected:// Helper Functions
	inline void DeleteSqlTableModel(QString const& strTableModel);

private:// Members
	std::shared_ptr<CDBManager> m_pDBManager;
	std::map< QString, std::shared_ptr<QSqlTableModel> > m_mapStringToTable;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CDBManager
//
class CDBManager
{
public:// Types
	enum class EMonth
	{
		January = 1,
		February = 2,

		March = 3,
		April = 4,
		May = 5,

		June = 6,
		July = 7,
		August = 8,

		September = 9,
		October = 10,
		November = 11,

		December = 12,

		Min = January,
		Max = December,

		Invalid = -1
	};

	struct SMonthLongName
	{
		static const QString January;
		static const QString February;

		static const QString March;
		static const QString April;
		static const QString May;

		static const QString June;
		static const QString July;
		static const QString August;

		static const QString September;
		static const QString October;
		static const QString November;

		static const QString December;
	};

	struct component
	{
		static const QString transactions;
		static const QString storages;
		static const QString customers;
	};

public:// Constructors
	inline CDBManager();
	virtual ~CDBManager() = default;

public:// Interface Methodes
	void Initialize(QString const& strDBFilePath = "");

	inline QSqlDatabase& GetDataBase();
	inline std::shared_ptr<IDBComponent> GetDBComponent(QString const& strComponentName);

	EMonth GetMonthByLongName(QString const& strMonthLongName);
	EMonth GetMontByMonthNumber(int nMonth);

	QString GetMonthLongNameByMonthNumber(int nMonth);
	QString GetMonthLongNameByMonth(EMonth eMonth);

protected:// Helper Methodes
	bool SetDBComponent(std::shared_ptr<IDBComponent> pDBComponent, QString const& strComponentName, bool bInitialize = true);

private:// Members
	std::map< QString, std::shared_ptr<IDBComponent> > m_mapStringToComponent;
	std::shared_ptr<CDBConnectionStarter> m_pDBConnectionStarter;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CDBComponent
//

// Constructors
inline CDBComponent::CDBComponent(std::shared_ptr<CDBManager> pDBManager)
	: m_pDBManager(pDBManager)
{
}

// Interface Methodes
inline void CDBComponent::SetDBManager(std::shared_ptr<CDBManager> pDBManager)
{
	if (pDBManager == nullptr)
		return;

	m_pDBManager = pDBManager;
}

inline std::shared_ptr<CDBManager> CDBComponent::GetDBManager()
{
	return m_pDBManager;
}

inline void CDBComponent::DeleteSqlTableModel(QString const& strTableModel)
{
	m_mapStringToTable.erase(strTableModel);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CDBManager
//

// Constructor
inline CDBManager::CDBManager()
	: m_pDBConnectionStarter(new CDBConnectionStarter()),
	  m_mapStringToComponent()
{
};

// Interface Methodes
inline QSqlDatabase& CDBManager::GetDataBase()
{
	return m_pDBConnectionStarter->GetDataBase();
}

inline std::shared_ptr<IDBComponent> CDBManager::GetDBComponent(QString const& strComponentName)
{
	auto itMap = m_mapStringToComponent.find(strComponentName);
	if (itMap == m_mapStringToComponent.end())
		return nullptr;

	return itMap->second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // DB_MANAGER_H