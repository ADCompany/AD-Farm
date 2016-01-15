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
		"info_text		TEXT    NOT NULL, "
		"date_time		TEXT    NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS farm_history ("
		"id				INTEGER PRIMARY KEY AUTOINCREMENT, "
		"info_text		TEXT    NOT NULL, "
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

void CStoragesData::RemoveProduct(int nRow)
{

	std::shared_ptr<QSqlQueryModel> pSqlTableModel(new QSqlQueryModel());
	if (pSqlTableModel == nullptr)
		return;

	pSqlTableModel->setQuery("SELECT * FROM producte");

	pSqlTableModel->removeRows(nRow, 1);

	UpdateAllSqlTableModel();
}

void CStoragesData::RemoveStorage(int nRow)
{
	std::shared_ptr<QSqlTableModel> pSqlTableModel(new QSqlTableModel(this, GetDBManager()->GetDataBase()));
	pSqlTableModel->setTable("storage");
	if (pSqlTableModel == nullptr)
		return;

	pSqlTableModel->removeRow(nRow);
	pSqlTableModel->submitAll();

	UpdateAllSqlTableModel();
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
	pSqlQueryModel->setQuery(QString("SELECT * FROM "
		"%1 ").arg(strTableName));

	pSqlQueryModel->setHeaderData(0, Qt::Horizontal, QVariant(QString::fromUtf8("\325\212\325\241\325\260. \325\260\325\241\325\264\325\241\326\200")));
	pSqlQueryModel->setHeaderData(1, Qt::Horizontal, QVariant(QString::fromUtf8("\324\261\325\266\325\270\326\202\325\266")));

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

std::shared_ptr<QSqlQueryModel> CStoragesData::GetHistorySqlTableModelByStorageName(QString const& strStorageName)
{
	auto itMap = m_mapStorageNameToStorageHistoryModel.find(strStorageName);
	if (itMap != m_mapStorageNameToStorageHistoryModel.end())
		return itMap->second;

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strStorageName));
	sqlQuery.next();

	QString strStorageId = sqlQuery.value(0).toString();
	std::shared_ptr<QSqlQueryModel> pStorageSqlTableModel(new QSqlQueryModel);
	pStorageSqlTableModel->setQuery(QString("SELECT storage_history.date_time, storage_history.info_text, producte.name, storage_history_info.count, storage_history_info.cost FROM "
		"storage_history INNER JOIN storage_history_info INNER JOIN producte "
		"ON storage_history.storage_id == %1 AND producte.id = storage_history_info.product_id AND storage_history_info.history_id == storage_history.id").arg(strStorageId));

	pStorageSqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant(QString::fromUtf8("\324\261\325\264\325\275\325\241\325\251\325\253\325\276")));
	pStorageSqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant(QString::fromUtf8("\324\263\325\270\326\200\325\256\325\270\325\262\325\270\326\202\325\251\325\265\325\241\325\266 \325\253\325\266\326\206\325\270\326\200\325\264\325\241\326\201\325\253\325\241\325\266")));
	pStorageSqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant(QString::fromUtf8("\324\261\325\272\326\200\325\241\325\266\326\204\325\241\325\277\325\245\325\275\325\241\325\257")));
	pStorageSqlTableModel->setHeaderData(3, Qt::Horizontal, QVariant(QString::fromUtf8("\325\224\325\241\325\266\325\241\325\257")));
	pStorageSqlTableModel->setHeaderData(4, Qt::Horizontal, QVariant(QString::fromUtf8("\324\263\325\253\325\266")));

	m_mapStorageNameToStorageHistoryModel.emplace(strStorageName, pStorageSqlTableModel);

	return pStorageSqlTableModel;
}

std::shared_ptr<QSqlQueryModel> CStoragesData::GetFarmHistorySqlTableModel()
{
	m_pFarmHistorySqlTableModel = std::shared_ptr<QSqlQueryModel>(new QSqlQueryModel);
	m_pFarmHistorySqlTableModel->setQuery("SELECT farm_history.date_time, farm_history.info_text FROM farm_history");

	m_pFarmHistorySqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant(
		QString::fromUtf8("\324\261\325\264\325\275\325\241\325\251\325\253\325\276")));
	m_pFarmHistorySqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant(
		QString::fromUtf8("\324\263\325\270\326\200\325\256\325\270\325\262\325\270\326\202\325\251\325\265\325\241\325\266 \325\253\325\266\326\206\325\270\326\200\325\264\325\241\326\201\325\253\325\241\325\266")));

	return m_pFarmHistorySqlTableModel;
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

QList<QString> CStoragesData::GetProductNames()
{
	QList<QString> lstString;

	//std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::storage);
	//if (pSqlTableModel == nullptr)
	//	return lstString;

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT name FROM %1").arg(table::producte));

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

void CStoragesData::BuyStorageData(QString const& strStorageName, QList<QString> lstProducteNames,
	QList<int> lstProductesCount, QList<double> lstProductCost, QString const& strInfoText)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strStorageName));
	sqlQuery.next();

	QString strStorageId = sqlQuery.value(0).toString();
	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	sqlQuery.exec(QString("INSERT INTO storage_history ( storage_id, info_text, date_time ) VALUES ( %1, \"%2\", \"%3\" );").arg(strStorageId, strInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
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

		sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
			strStorageHistoryId, strProductId, QString::number(lstProductesCount[i]), QString::number(lstProductCost[i])));
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

		sqlQuery.exec(QString("UPDATE storage_info SET count = %1 WHERE storage_id == %2 AND product_id == %3").arg(QString::number(nProductsCount), strStorageId, strProductId));
	}

	UpdateAllSqlTableModel();
}

void CStoragesData::AddNewProduct(QString const& strNewProductName, int nCount, double dPrimeCost)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("INSERT INTO producte ( name, count, prime_cost ) VALUES ( \"%1\" , %2, %3 );").arg(
		strNewProductName, QString::number(nCount), QString::number(dPrimeCost)));

	UpdateSqlTableModel(table::producte);
}

void CStoragesData::AddNewStore(QString const& strStoreName)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("INSERT INTO storage_name ( name ) VALUES ( \"%1\" );").arg(
		strStoreName));

	UpdateAllSqlTableModel();
}

void CStoragesData::MoveProductFromStorageInStorage(QString const& strInStorageName, QString strOutStorage,
	QList<QString> const& lstProductName, QList<int> const& lstProductCount, QString const& strInStorageInfoText, QString const& strOutStorageInfoText)
{
	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strInStorageName));
	sqlQuery.next();
	QString strInStorageId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strOutStorage));
	sqlQuery.next();
	QString strOutStorageId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("INSERT INTO storage_history ( storage_id, info_text, date_time ) VALUES ( %1, \"%2\", \"%3\" );").arg(strInStorageId, strInStorageInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strInStorageHistoryId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("INSERT INTO storage_history ( storage_id, info_text, date_time ) VALUES ( %1, \"%2\", \"%3\" );").arg(strOutStorageId, strOutStorageInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strOutStorageHistoryId = sqlQuery.value(0).toString();

	for (int i = 0; i < lstProductName.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM producte WHERE name == \"%1\"").arg(lstProductName[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("SELECT prime_cost FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strOutStorageId, strProductId));
		sqlQuery.next();
		double dProductPrimeCostInOutStorage = sqlQuery.value(0).toDouble();

		sqlQuery.exec(QString("SELECT count FROM storage_info WHERE storage_id == %1 AND product_id == %2").arg(strOutStorageId, strProductId));
		sqlQuery.next();
		int nProductCountInOutStorage = sqlQuery.value(0).toInt();
		nProductCountInOutStorage -= lstProductCount[i];

		sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
			strInStorageHistoryId, strProductId, QString::number(lstProductCount[i]), QString::number(dProductPrimeCostInOutStorage)));

		sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
			strOutStorageHistoryId, strProductId, QString::number(lstProductCount[i]), QString::number(dProductPrimeCostInOutStorage)));

		sqlQuery.exec(QString("UPDATE storage_info SET count = %1 WHERE storage_id == %2 AND product_id == %3").arg(
			QString::number(nProductCountInOutStorage), strOutStorageId, strProductId));

		sqlQuery.exec(QString("SELECT * FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strInStorageId, strProductId));
		sqlQuery.next();
		int nRow = sqlQuery.value(0).toInt();
		if (nRow == 0)
		{
			sqlQuery.exec(QString("INSERT INTO storage_info VALUES ( %1, %2, %3, %4 )").arg(
				strInStorageId, strProductId, QString::number(lstProductCount[i]), QString::number(dProductPrimeCostInOutStorage)));

			continue;
		}

		sqlQuery.exec(QString("SELECT count FROM storage_info WHERE storage_id == %1 AND product_id == %2").arg(strInStorageId, strProductId));
		sqlQuery.next();
		int nProductCountInInStorage = sqlQuery.value(0).toInt();

		sqlQuery.exec(QString("SELECT prime_cost FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strInStorageId, strProductId));
		sqlQuery.next();
		double dProductPrimeCostInInStorage = sqlQuery.value(0).toDouble();

		dProductPrimeCostInInStorage = (dProductPrimeCostInInStorage * nProductCountInInStorage + dProductPrimeCostInOutStorage * lstProductCount[i]) / (lstProductCount[i] + nProductCountInInStorage);
		nProductCountInInStorage += lstProductCount[i];

		sqlQuery.exec(QString("UPDATE storage_info SET count = %1, prime_cost = %2 WHERE storage_id == %3 AND product_id == %4").arg(
			QString::number(nProductCountInInStorage), QString::number(dProductPrimeCostInInStorage), strInStorageId, strProductId));
	}

	UpdateAllSqlTableModel();
}

void CStoragesData::AddProductInStorage(QString const& strStorageName, QList<QString> const& lstProductName,
	QList<int> const& lstProductCount, QList<double> const& lstProductCost, QString const& strInfoText)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strStorageName));
	sqlQuery.next();

	QString strStorageId = sqlQuery.value(0).toString();
	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	sqlQuery.exec(QString("INSERT INTO storage_history ( storage_id, info_text, date_time ) VALUES ( %1, \"%2\", \"%3\" );").arg(strStorageId, strInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strStorageHistoryId = sqlQuery.value(0).toString();

	for (int i = 0; i < lstProductName.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM producte WHERE name == \"%1\" ").arg(lstProductName[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();
		sqlQuery.exec(QString("SELECT prime_cost FROM producte WHERE id == %1 ").arg(strProductId));
		sqlQuery.next();
		QString strProductPrimeCost = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
			strStorageHistoryId, strProductId, QString::number(lstProductCount[i]), QString::number(lstProductCost[i])));
	}

	for (int i = 0; i < lstProductName.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM producte WHERE name == \"%1\" ").arg(lstProductName[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("SELECT count FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strStorageId, strProductId));
		sqlQuery.next();
		int nProductsCountInStorage = sqlQuery.value(0).toInt();

		sqlQuery.exec(QString("SELECT prime_cost FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strStorageId, strProductId));
		sqlQuery.next();
		double dProductPrimeCostInStorage = sqlQuery.value(0).toDouble();
		dProductPrimeCostInStorage = (nProductsCountInStorage * dProductPrimeCostInStorage + lstProductCount[i] * lstProductCost[i]) / (nProductsCountInStorage + lstProductCount[i]);
		nProductsCountInStorage += lstProductCount[i];

		sqlQuery.exec(QString("SELECT count FROM producte WHERE id == %1 ").arg(strProductId));
		sqlQuery.next();
		int nProductCountInProduct = sqlQuery.value(0).toInt();

		sqlQuery.exec(QString("SELECT prime_cost FROM producte WHERE id == %1 ").arg(strProductId));
		sqlQuery.next();
		double dPrimeCost = sqlQuery.value(0).toDouble();
		dPrimeCost = (lstProductCount[i] * lstProductCost[i] + nProductCountInProduct * dPrimeCost) / (nProductCountInProduct + lstProductCount[i]);
		nProductCountInProduct += lstProductCount[i];

		sqlQuery.exec(QString("UPDATE producte SET count = %1, prime_cost = %2 WHERE id == %3").arg(
			QString::number(nProductCountInProduct), QString::number(dPrimeCost), strProductId));
		sqlQuery.exec(QString("SELECT * FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strStorageId, strProductId));
		sqlQuery.next();
		int nRow = sqlQuery.value(0).toInt();
		if (nRow == 0)
		{
			sqlQuery.exec(QString("INSERT INTO storage_info VALUES ( %1, %2, %3, %4 )").arg(
				strStorageId, strProductId, QString::number(nProductsCountInStorage), QString::number(dProductPrimeCostInStorage)));
		}
		else
			sqlQuery.exec(QString("UPDATE storage_info SET count = %1, prime_cost = %2 WHERE storage_id == %3 AND product_id == %4").arg(
			QString::number(nProductsCountInStorage), QString::number(dProductPrimeCostInStorage), strStorageId, strProductId));
	}

	UpdateAllSqlTableModel();
}

void CStoragesData::SubstractProductInStorage(QString const& strStorageName, QString const& strProductName, int nCount)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strStorageName));
	sqlQuery.next();
	QString strStorageId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT id FROM producte WHERE name == \"%1\" ").arg(strProductName));
	sqlQuery.next();
	QString strProductId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT count FROM producte WHERE id == %1 ").arg(strProductId));
	sqlQuery.next();
	int nProductCountInProduct = sqlQuery.value(0).toInt();
	nProductCountInProduct -= nCount;

	sqlQuery.exec(QString("SELECT count FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strStorageId, strProductId));
	sqlQuery.next();
	int nProductsCountInStorage = sqlQuery.value(0).toInt();
	nProductsCountInStorage -= nCount;

	sqlQuery.exec(QString("UPDATE producte SET count = %1 WHERE id == %2").arg(
		QString::number(nProductCountInProduct), strProductId));
	sqlQuery.exec(QString("UPDATE storage_info SET count = %1 WHERE storage_id == %2 AND product_id == %3").arg(
		QString::number(nProductsCountInStorage), strStorageId, strProductId));

	UpdateAllSqlTableModel();
}

void CStoragesData::DeclineProductInStorage(QString const& strStorageName, QString const& strProductName, int nCount, QString const& strInfoText)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strStorageName));
	sqlQuery.next();

	QString strStorageId = sqlQuery.value(0).toString();
	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	sqlQuery.exec(QString("INSERT INTO storage_history ( storage_id, info_text, date_time ) VALUES ( %1, \"%2\", \"%3\" );").arg(strStorageId, strInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strStorageHistoryId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT id FROM producte WHERE name == \"%1\" ").arg(strProductName));
	sqlQuery.next();
	QString strProductId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT count FROM producte WHERE id == %1 ").arg(strProductId));
	sqlQuery.next();
	int nProductCountInProduct = sqlQuery.value(0).toInt();

	sqlQuery.exec(QString("SELECT prime_cost FROM producte WHERE id == %1 ").arg(strProductId));
	sqlQuery.next();
	double dProductPrimeCost = sqlQuery.value(0).toDouble();

	if (nProductCountInProduct - nCount == 0)
		dProductPrimeCost = 0;
	else
		dProductPrimeCost = (nProductCountInProduct * dProductPrimeCost) / (nProductCountInProduct - nCount);
	nProductCountInProduct -= nCount;

	sqlQuery.exec(QString("SELECT count FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strStorageId, strProductId));
	sqlQuery.next();
	int nProductsCountInStorage = sqlQuery.value(0).toInt();

	sqlQuery.exec(QString("SELECT prime_cost FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strStorageId, strProductId));
	sqlQuery.next();
	double dProductPrimeCostInStorage = sqlQuery.value(0).toDouble();

	sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
		strStorageHistoryId, strProductId, QString::number(nProductCountInProduct), QString::number(dProductPrimeCostInStorage)));

	if (nProductsCountInStorage - nCount == 0)
		dProductPrimeCostInStorage = 0;
	else
		dProductPrimeCostInStorage = (nProductsCountInStorage * dProductPrimeCostInStorage) / (nProductsCountInStorage - nCount);
	nProductsCountInStorage -= nCount;

	sqlQuery.exec(QString("UPDATE producte SET count = %1, prime_cost = %2 WHERE id == %3").arg(QString::number(nProductCountInProduct), QString::number(dProductPrimeCost), strProductId));
	sqlQuery.exec(QString("UPDATE storage_info SET count = %1, prime_cost = %2 WHERE storage_id == %3 AND product_id == %4").arg(
		QString::number(nProductsCountInStorage), QString::number(dProductPrimeCostInStorage), strStorageId, strProductId));

	UpdateAllSqlTableModel();
}

void CStoragesData::AddFarmCosts(double dCosts, QString const& strInfoText)
{
	QSqlQuery sqlQuery;

	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	sqlQuery.exec(QString("INSERT INTO farm_history ( info_text, date_time ) VALUES ( \"%1\", \"%2\" );").arg(strInfoText, strDateTime));

	sqlQuery.exec(QString("SELECT * FROM producte"));

	double dAllFarmCurrCost = 0;
	while (sqlQuery.next())
		dAllFarmCurrCost += sqlQuery.value(2).toInt() * sqlQuery.value(3).toDouble();

	if (dAllFarmCurrCost == 0)
		return; // ASSERT

	double dProductTypeCost = 0;
	int nProductTypeCount = 0;
	sqlQuery.exec(QString("SELECT * FROM producte"));
	while (sqlQuery.next())
	{
		nProductTypeCount = sqlQuery.value(2).toInt();
		dProductTypeCost = sqlQuery.value(3).toDouble() * nProductTypeCount;

		dProductTypeCost = (dProductTypeCost / dAllFarmCurrCost) * (dAllFarmCurrCost + dCosts);
		QSqlQuery sqlTempQuery;
		sqlTempQuery.exec(QString("UPDATE producte SET prime_cost = %1 WHERE id == %2").arg(
			QString::number(dProductTypeCost / nProductTypeCount), sqlQuery.value(0).toString()));
	}

	sqlQuery.exec(QString("SELECT * FROM storage_info"));
	while (sqlQuery.next())
	{
		nProductTypeCount = sqlQuery.value(2).toInt();
		dProductTypeCost = sqlQuery.value(3).toDouble() * nProductTypeCount;

		dProductTypeCost = (dProductTypeCost / dAllFarmCurrCost) * (dAllFarmCurrCost + dCosts);
		QSqlQuery sqlTempQuery;
		sqlTempQuery.exec(QString("UPDATE storage_info SET prime_cost = %1 WHERE storage_id == %2 AND product_id == %3").arg(
			QString::number(dProductTypeCost / nProductTypeCount), sqlQuery.value(0).toString(), sqlQuery.value(1).toString()));
	}

	UpdateAllSqlTableModel();
}

void CStoragesData::AddStoragesCosts(QString const& strStorageName, double dCosts, QString const& strInfoText)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strStorageName));
	sqlQuery.next();

	QString strStorageId = sqlQuery.value(0).toString();
	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	sqlQuery.exec(QString("INSERT INTO farm_history ( info_text, date_time ) VALUES ( \"%1\", \"%2\" );").arg(strInfoText, strDateTime));

	sqlQuery.exec(QString("SELECT * FROM storage_info WHERE storage_id == %1").arg(strStorageId));

	double dAllStorageCurrCost = 0;
	while (sqlQuery.next())
		dAllStorageCurrCost += sqlQuery.value(2).toInt() * sqlQuery.value(3).toDouble();

	if (dAllStorageCurrCost == 0)
		return; // ASSERT

	double dProductTypeCost = 0;
	int nProductTypeCount = 0;
	sqlQuery.exec(QString("SELECT * FROM storage_info WHERE storage_id == %1").arg(strStorageId));
	while (sqlQuery.next())
	{
		nProductTypeCount = sqlQuery.value(2).toInt();
		if (nProductTypeCount == 0)
			continue;

		dProductTypeCost = sqlQuery.value(3).toDouble() * nProductTypeCount;
		QString strProductId = sqlQuery.value(1).toString();

		dProductTypeCost = (dProductTypeCost / dAllStorageCurrCost) * (dAllStorageCurrCost + dCosts);
		double dDeltaCost = dProductTypeCost - sqlQuery.value(3).toDouble() * nProductTypeCount;
		QSqlQuery sqlTempQuery;
		sqlTempQuery.exec(QString("UPDATE storage_info SET prime_cost = %1 WHERE storage_id == %2 AND product_id == %3").arg(
			QString::number(dProductTypeCost / nProductTypeCount), sqlQuery.value(0).toString(), sqlQuery.value(1).toString()));

		sqlTempQuery.exec(QString("SELECT count, prime_cost FROM producte WHERE id == %1").arg(strProductId));
		sqlTempQuery.next();
 
		nProductTypeCount = sqlTempQuery.value(0).toInt();
		dProductTypeCost = dDeltaCost + sqlTempQuery.value(1).toDouble() * nProductTypeCount;

		sqlTempQuery.exec(QString("UPDATE producte SET prime_cost = %1 WHERE id == %2").arg(
			QString::number(dProductTypeCost / nProductTypeCount), strProductId));
	}

	UpdateAllSqlTableModel();
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