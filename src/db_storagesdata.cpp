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
		"cost			REAL	NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_history ("
		"id				INTEGER PRIMARY KEY AUTOINCREMENT, "
		"storage_id		INTEGER NOT NULL, "
		"date_time		TEXT    NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_info ("
		"storage_id		INTEGER NOT NULL, "
		"product_id		INTEGER NOT NULL, "
		"count			INTEGER NOT NULL, "
		"prime_cost		REAL	NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_name ("
		"id		INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name	TEXT    NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS producte ("
		"id				INTEGER PRIMARY KEY NOT NULL, "
		"name			TEXT    NOT NULL, "
		"count			INTEGER NOT NULL, "
		"prime_cost		REAL	NOT NULL);");
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

std::shared_ptr<QSqlQueryModel> CStoragesData::GetProductSqlQueryModel(QString const& strTableName)
{
	auto itMap = m_mapStringToModel.find(strTableName);
	if (itMap != m_mapStringToModel.end())
		return itMap->second;

	std::shared_ptr<QSqlQueryModel> pSqlQueryModel(new QSqlQueryModel);
	pSqlQueryModel->setQuery(QString("SELECT * FROM %1").arg(strTableName));

	pSqlQueryModel->setHeaderData(0, Qt::Horizontal, QVariant(QString::fromUtf8("\325\217\325\245\325\275. \325\260\325\241\325\264\325\241\326\200")));
	pSqlQueryModel->setHeaderData(1, Qt::Horizontal, QVariant(QString::fromUtf8("\324\261\325\266\325\270\326\202\325\266")));
	pSqlQueryModel->setHeaderData(2, Qt::Horizontal, QVariant(QString::fromUtf8("\325\224\325\241\325\266\325\241\325\257")));
	pSqlQueryModel->setHeaderData(3, Qt::Horizontal, QVariant(QString::fromUtf8("\324\273\325\266\326\204\325\266\325\241\326\200\325\252\325\245\326\204")));

	m_mapStringToModel.emplace(strTableName, pSqlQueryModel);
	return pSqlQueryModel;
}

std::shared_ptr<QSqlQueryModel> CStoragesData::GetStorageSqlQueryModel(QString const& strTableName)
{
	auto itMap = m_mapStringToModel.find(strTableName);
	if (itMap != m_mapStringToModel.end())
		return itMap->second;

	std::shared_ptr<QSqlQueryModel> pSqlQueryModel(new QSqlQueryModel);
	pSqlQueryModel->setQuery(QString("SELECT storage_name.name, producte.name, storage_info.count, storage_info.prime_cost FROM "
		"storage_info INNER JOIN producte INNER JOIN storage_name "
		"ON "
		"storage_name.id == storage_info.storage_id AND producte.id = storage_info.product_id"));

	pSqlQueryModel->setHeaderData(0, Qt::Horizontal, QVariant(QString::fromUtf8("\324\261\325\266\325\270\326\202\325\266")));
	pSqlQueryModel->setHeaderData(1, Qt::Horizontal, QVariant(QString::fromUtf8("\325\217\325\245\325\275\325\241\325\257")));
	pSqlQueryModel->setHeaderData(2, Qt::Horizontal, QVariant(QString::fromUtf8("\325\224\325\241\325\266\325\241\325\257")));
	pSqlQueryModel->setHeaderData(3, Qt::Horizontal, QVariant(QString::fromUtf8("\324\273\325\266\326\204\325\266\325\241\326\200\325\252\325\245\326\204")));

	m_mapStringToModel.emplace(strTableName, pSqlQueryModel);
	return pSqlQueryModel;
}

std::shared_ptr<QSqlQueryModel> CStoragesData::GetSqlTableModelByStorageName(QString const& strStorageName)
{
	auto itMap = m_mapStringToModel.find(strStorageName);
	if (itMap != m_mapStringToModel.end())
		return itMap->second;

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strStorageName));
	sqlQuery.next();

	QString strStorageId = sqlQuery.value(0).toString();
	std::shared_ptr<QSqlQueryModel> pStorageSqlTableModel(new QSqlQueryModel);
	pStorageSqlTableModel->setQuery(QString("SELECT producte.name, storage_info.count, storage_info.prime_cost FROM "
		"storage_info INNER JOIN producte "
		"ON storage_info.storage_id == %1 AND producte.id = storage_info.product_id").arg(strStorageId));

	pStorageSqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant(QString::fromUtf8("\325\217\325\245\325\275\325\241\325\257")));
	pStorageSqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant(QString::fromUtf8("\325\224\325\241\325\266\325\241\325\257")));
	pStorageSqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant(QString::fromUtf8("\324\273\325\266\326\204\325\266\325\241\326\200\325\252\325\245\326\204")));

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
	sqlQuery.exec(QString("SELECT name FROM %1").arg(table::storage));

	QString strStorageName = "";
	while (sqlQuery.next())
	{
		strStorageName = sqlQuery.value(0).toString();
		lstString.push_back(strStorageName);
	}

	return lstString;
}

void CStoragesData::SubstractProducts(QList<QString> lstProductName, QList<int> lstProductCount)
{
	QSqlQuery sqlQuery;
	for (int i = 0; i < lstProductName.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM producte WHERE name == \"%1\" ").arg(lstProductName[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();
		sqlQuery.exec(QString("SELECT count FROM producte WHERE id == %1 ").arg(strProductId));
		sqlQuery.next();
		int nCurrCount = sqlQuery.value(0).toInt();
		nCurrCount -= lstProductCount[i];

		sqlQuery.exec(QString("UPDATE producte SET count = %1 WHERE id == %2").arg(QString::number(nCurrCount), strProductId));
	}
}

void CStoragesData::BuyStorageData(QString const& strStorageName, QList<QString> lstProducteNames, QList<int> lstProductesCount)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strStorageName));
	sqlQuery.next();

	QString strStorageId = sqlQuery.value(0).toString();
	QDateTime dateTime = QDateTime::currentDateTimeUtc();
	QString strDateTime = dateTime.toString();

	sqlQuery.exec(QString("INSERT INTO storage_history ( storage_id, date_time ) VALUES ( %1, \"%2\" );").arg(strStorageId, strDateTime));
	sqlQuery.exec(QString("SELECT id FROM storage_history WHERE storage_id == %1 AND date_time == \"%2\" ").arg(strStorageId, strDateTime));
	sqlQuery.next();
	QString strStorageHistoryId = sqlQuery.value(0).toString();

	for (int i = 0; i < lstProducteNames.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM producte WHERE name == \"%1\" ").arg(lstProducteNames[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();
		sqlQuery.exec(QString("SELECT prime_cost FROM producte WHERE id == %1 ").arg(strProductId));
		sqlQuery.next();
		QString strProductPrimeCost = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(strStorageHistoryId, strProductId, QString::number(lstProductesCount[i]), strProductPrimeCost));
	}

	for (int i = 0; i < lstProducteNames.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM producte WHERE name == \"%1\" ").arg(lstProducteNames[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("SELECT count FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strStorageId, strProductId));
		sqlQuery.next();
		int nProductsCount = sqlQuery.value(0).toInt();
		nProductsCount -= lstProductesCount[i];

		sqlQuery.exec(QString("UPDATE storage_info SET count = %1 WHERE storage_id == %2").arg(QString::number(nProductsCount), strStorageId));
	}

	UpdateSqlTableModel(strStorageName);
	UpdateSqlTableModel(table::storage);
	UpdateSqlTableModel(table::producte);
}

void CStoragesData::UpdateSqlTableModel(QString const& strStorageName)
{
	RemoveSqlTableModel(strStorageName);
}

void CStoragesData::RemoveSqlTableModel(QString const& strStorageName)
{
	m_mapStringToModel.erase(strStorageName);

	emit sigChangeData();
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