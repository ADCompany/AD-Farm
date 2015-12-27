#ifndef DB_STORAGES_DATA_H
#define DB_STORAGES_DATA_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef DB_COMPONENT_H
#   include "db_manager.h"
#endif

// Qt includes

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesData
//
class CStoragesData : public CDBComponent
{
public:// Data Property
	struct table
	{
		// tables name
		static const QString storage;
		static const QString storage_info;
		static const QString storage_history;
		static const QString storage_history_info;
		static const QString producte;

		// table customer
		//struct storage
		//{
		//	static const QString id;
		//	static const QString name;
		//};
		//struct storage_info
		//{
		//	static const QString storage_id;
		//	static const QString product_id;
		//	static const QString count;
		//	static const QString prime_cost;
		//};
		//struct storage_history
		//{
		//	static const QString id;
		//	static const QString storage_id;
		//	static const QString date_time;
		//};
		//struct storage_history_info
		//{
		//	static const QString history_id;
		//	static const QString product_id;
		//	static const QString count;
		//	static const QString cost;
		//};
		//struct product
		//{
		//	static const QString id;
		//	static const QString name;
		//	static const QString prime_cost;
		//};
	};
public:// Constructors
	inline CStoragesData(QObject* pParent = nullptr, std::shared_ptr<CDBManager> pDBManager = nullptr);
	~CStoragesData() = default;

public:// Interface Methodes
	void Initialize();

	bool SetParent(QObject* pParent);
	void SetDBManager(std::shared_ptr<CDBManager> pDBManager);

	inline int GetColumnCount() const;

	std::shared_ptr<QSqlQueryModel> GetSqlTableModelByStorageName(QString const& strCustomerName);

	QList<QString> GetStorageNames();

protected:// Helper Methodes
	void UpdateSqlTableModel();

private:// Members
	std::map< QString, std::shared_ptr<QSqlQueryModel> > m_mapStringToModel;
	QObject* m_pParentObject;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesData
//

// Constructors
inline CStoragesData::CStoragesData(QObject* pParent, std::shared_ptr<CDBManager> pDBManager)
	: CDBComponent(pDBManager),
	m_pParentObject(pParent)
{}

// Interface Methodes
inline int CStoragesData::GetColumnCount() const
{

	return 0;
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // DB_STORAGES_DATA_H