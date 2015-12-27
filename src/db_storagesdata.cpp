////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "db_storagesdata.h"

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

//  Static Member Initialization
const QString CStoragesData::table::storage = "storage_name";
const QString CStoragesData::table::storage_info = "storage_info";
const QString CStoragesData::table::storage_history = "storage_history";
const QString CStoragesData::table::storage_history_info = "storage_history_info";
const QString CStoragesData::table::producte = "producte";

// Interface Methodes
void CStoragesData::Initialize()
{
	QSqlQuery sqlQuery;

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_history_info ("
		"history_id		INTEGER NOT NULL, "
		"product_id		INTEGER NOT NULL, " 
		"count			INTEGER NOT NULL, "
		"cost			INTEGER NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_history ("
		"id				INTEGER PRIMARY KEY AUTOINCREMENT, "
		"storage_id		INTEGER NOT NULL, "
		"date_time		TEXT    NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_info ("
		"storage_id		INTEGER NOT NULL, "
		"product_id		INTEGER NOT NULL, "
		"count			INTEGER NOT NULL, "
		"prime_cost		INTEGER NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_name ("
		"id		INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name	TEXT    NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS producte ("
		"id				INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name			TEXT    NOT NULL,"
		"prime_cost		INTEGER NOT NULL);");
	//.arg(table::customer,
	//								table::customer::id,
	//								table::customer::first_name,
	//								table::customer::last_name,
	//								table::customer::debt,
	//								table::customer::phone_number));

	std::shared_ptr<CDBManager> pDBManager = GetDBManager();
	if (pDBManager != nullptr)QString strFirstName = "";
		SetDBManager(pDBManager);
}

bool CStoragesData::SetParent(QObject* pParent)
{
	if (pParent == nullptr)
		return false;

	return true;
}

void CStoragesData::SetDBManager(std::shared_ptr<CDBManager> pDBManager)
{
	CDBComponent::SetDBManager(pDBManager);
}

std::shared_ptr<QSqlQueryModel> CStoragesData::GetSqlTableModelByStorageName(QString const& strStorageName)
{
	auto itMap = m_mapStringToModel.find(strStorageName);
	if (itMap != m_mapStringToModel.end())
		return itMap->second;

	QSqlQuery sqlQuery;
	sqlQuery.prepare(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strStorageName));
	sqlQuery.exec();
	sqlQuery.next();

	QString strStorageId = sqlQuery.value(0).toString();
	std::shared_ptr<QSqlQueryModel> pStorageSqlTableModel(new QSqlQueryModel);
	pStorageSqlTableModel->setQuery(QString("SELECT producte.name, storage_info.count, storage_info.prime_cost FROM "
		"storage_info LEFT JOIN producte "
		"WHERE storage_info.storage_id == %1").arg(strStorageId));

	pStorageSqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant("Type"));
	pStorageSqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant("Count"));
	pStorageSqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant("Prime Cost"));

	m_mapStringToModel.emplace(strStorageName, pStorageSqlTableModel);

	return pStorageSqlTableModel;
}

QList<QString> CStoragesData::GetStorageNames()
{
	QList<QString> lstString;

	//std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::storage);
	//if (pSqlTableModel == nullptr)
	//	return lstString;

	QSqlQuery sqlQuery;
	sqlQuery.prepare(QString("SELECT name FROM %1").arg(table::storage));
	sqlQuery.exec();

	QString strStorageName = "";
	while (sqlQuery.next())
	{
		strStorageName = sqlQuery.value(0).toString();
		lstString.push_back(strStorageName);
	}

	return lstString;
}

// Helper Functions
void CStoragesData::UpdateSqlTableModel()
{}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////