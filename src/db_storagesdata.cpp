////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "db_storagesdata.h"

// Qt includes


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CStoragesData
//

//  Static Member Initialization
const QString CStoragesData::table::storage = "storage_name";
const QString CStoragesData::table::storage_info = "storage_info";
const QString CStoragesData::table::storage_history = "storage_history";
const QString CStoragesData::table::storage_history_info = "storage_history_info";
const QString CStoragesData::table::product = "product";

// Interface Methodes
void CStoragesData::Initialize()
{
	QSqlQuery sqlQuery;

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_history_info ("
		"history_id		INTEGER NOT NULL, "
		"product_id		INTEGER	NOT NULL, "
		"count			INTEGER NOT NULL, "
		"cost			REAL	NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_history ("
		"id				INTEGER PRIMARY KEY AUTOINCREMENT, "
		"info_text		TEXT    NOT NULL, "
		"action_id		INTEGER	NOT NULL, "
		"group_info_id	INTEGER NOT NULL, " // ?? group_info_id
		"date_time		TEXT    NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS farm_history ("
		"id				INTEGER PRIMARY KEY AUTOINCREMENT, "
		"storage_id		INTEGER NULL, "
		"info_text		TEXT    NOT NULL, "
		"cost			REAL	NOT NULL, "
		"date_time		TEXT    NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_info (" // don't change places
		"storage_id		INTEGER NOT NULL, "
		"product_id		INTEGER NOT NULL, "
		"count			INTEGER NOT NULL, "
		"prime_cost		REAL	NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS storage_name ("
		"id		INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name	TEXT    NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS group_name ("
		"id				INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name			TEXT    NOT NULL, "
		"income			REAL	NOT NULL, "
		"expenses		REAL	NOT NULL, "
		"date_time		TEXT    NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS group_info ("
		"id				INTEGER PRIMARY KEY AUTOINCREMENT, "
		"group_id		INTEGER NOT NULL, "
		"storage_id		TEXT    NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS product ("
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

	pSqlTableModel->setQuery("SELECT * FROM product");

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

QString CStoragesData::GetLastGroupName()
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT name FROM group_name ORDER BY id DESC LIMIT 1"));

	sqlQuery.next();
	QString strGroupName = sqlQuery.value(0).toString();

	return strGroupName;
}

QString CStoragesData::GetLastGroupNameByStorage(QString const& strStorageName)
{
	QString strStorageId = GetStorageIdByName(strStorageName);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT group_id FROM group_info WHERE storage_id == %1 ORDER BY id DESC LIMIT 1").arg(strStorageId));
	sqlQuery.next();

	QString strGroupId = sqlQuery.value(0).toString();

	return GetGroupNameById(strGroupId);
}

QStringList CStoragesData::GetGroupNamesByStorage(QString const& strStorageName)
{
	QString strStorageId = GetStorageIdByName(strStorageName);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT group_id FROM group_info WHERE storage_id == %1 ORDER BY id DESC").arg(strStorageId));
	sqlQuery.next();

	QStringList strListGroupNames;
	while (sqlQuery.isValid())
	{
		strListGroupNames.append(GetGroupNameById(sqlQuery.value(0).toString()));
		sqlQuery.next();
	}

	return strListGroupNames;
}

QStringList CStoragesData::GetGroupNames()
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT name FROM group_name ORDER BY id DESC"));
	sqlQuery.next();

	QStringList strListGroupNames;
	while (sqlQuery.isValid())
	{
		strListGroupNames.append(sqlQuery.value(0).toString());
		sqlQuery.next();
	}

	return strListGroupNames;
}

double CStoragesData::GetGroupIncome(QString const& strGroup)
{
	QString const& strGroupId = GetGroupIdByName(strGroup);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT income FROM group_name WHERE id == %1").arg(strGroupId));
	sqlQuery.next();

	return sqlQuery.value(0).toDouble();
}

double CStoragesData::GetGroupExpenses(QString const& strGroup)
{
	QString const& strGroupId = GetGroupIdByName(strGroup);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT expenses FROM group_name WHERE id == %1").arg(strGroupId));
	sqlQuery.next();

	return sqlQuery.value(0).toDouble();
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

int CStoragesData::GetStorageScriptCount(QString const& strStorageName, QString const& strGroupName, EAction eAction) const
{
	QString strStorageId = GetStorageIdByName(strStorageName);
	QString strGroupId = GetGroupIdByName(strGroupName);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strGroupId, strStorageId));
	sqlQuery.next();
	QString strGroupInfoId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT SUM(storage_history_info.count) FROM "
		"storage_history_info INNER JOIN storage_history "
		"ON storage_history.id == storage_history_info.history_id AND storage_history.group_info_id == %1 AND storage_history.action_id == %2").arg(
		strGroupInfoId, QString::number((int) (eAction))));
	sqlQuery.next();

	return sqlQuery.value(0).toInt();
}

int CStoragesData::GetStorageScriptCost(QString const& strStorageName, QString const& strGroupName, EAction eAction) const
{
	QString strStorageId = GetStorageIdByName(strStorageName);
	QString strGroupId = GetGroupIdByName(strGroupName);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strGroupId, strStorageId));
	sqlQuery.next();
	QString strGroupInfoId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT SUM(storage_history_info.cost) FROM "
		"storage_history_info INNER JOIN storage_history "
		"ON storage_history.id == storage_history_info.history_id AND storage_history.group_info_id == %1 AND storage_history.action_id == %2").arg(
		strGroupInfoId, QString::number((int) (eAction))));
	sqlQuery.next();

	return sqlQuery.value(0).toInt();
}

double CStoragesData::GetStorageTotalPrice(QString const& strStorageName, QString const& strGroupName, EAction eAction) const
{
	QString strStorageId = GetStorageIdByName(strStorageName);
	QString strGroupId = GetGroupIdByName(strGroupName);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strGroupId, strStorageId));
	sqlQuery.next();
	QString strGroupInfoId = sqlQuery.value(0).toString();

	std::shared_ptr<QSqlQueryModel> pStorageSqlTableModel(new QSqlQueryModel);
	pStorageSqlTableModel->setQuery(QString("SELECT storage_history_info.cost, storage_history_info.count FROM "
		"storage_history_info INNER JOIN storage_history "
		"ON storage_history.id == storage_history_info.history_id AND storage_history.group_info_id == %1 AND storage_history.action_id == %2").arg(
		strGroupInfoId, QString::number((int) (eAction))));

	double dTotalPrice = 0;
	QSqlRecord sqlRecord;
	for (int i = 0; i < pStorageSqlTableModel->rowCount(); ++i)
	{
		sqlRecord = pStorageSqlTableModel->record(i);
		dTotalPrice += sqlRecord.value(0).toDouble() * sqlRecord.value(1).toDouble();
	}

	return dTotalPrice;
}

double CStoragesData::GetStorageTotalPrice(QString const& strStorageName, QString const& strGroupName) const
{
	double dTotalPrice = 0;

	dTotalPrice += GetStorageTotalPrice(strStorageName, strGroupName, EAction::Buy);
	dTotalPrice += GetStorageTotalPrice(strStorageName, strGroupName, EAction::Decline);
	dTotalPrice += GetStorageTotalPrice(strStorageName, strGroupName, EAction::Nourish);
	dTotalPrice += GetStorageTotalPrice(strStorageName, strGroupName, EAction::Consumption);

	return dTotalPrice;
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
	pStorageSqlTableModel->setQuery(QString("SELECT product.name, storage_info.count, storage_info.prime_cost FROM "
		"storage_info INNER JOIN product "
		"ON storage_info.storage_id == %1 AND product.id = storage_info.product_id").arg(strStorageId));

	pStorageSqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant(QString::fromUtf8("\325\217\325\245\325\275\325\241\325\257")));
	pStorageSqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant(QString::fromUtf8("\325\224\325\241\325\266\325\241\325\257")));
	pStorageSqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant(QString::fromUtf8("\324\273\325\266\326\204\325\266\325\241\326\200\325\252\325\245\326\204")));

	m_mapStringToModel.emplace(strStorageName, pStorageSqlTableModel);

	return pStorageSqlTableModel;
}

std::shared_ptr<QSqlQueryModel> CStoragesData::GetHistorySqlTableModelByStorageName(QString const& strStorageName, QString const& strGroupName)
{
	auto itMap = m_mapStorageNameToStorageHistoryModel.find(strStorageName + strGroupName);
	if (itMap != m_mapStorageNameToStorageHistoryModel.end())
		return itMap->second;

	QString strStorageId = GetStorageIdByName(strStorageName);
	QString strGroupId = GetGroupIdByName(strGroupName);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strGroupId, strStorageId));
	sqlQuery.next();
	QString strGroupInfoId = sqlQuery.value(0).toString();

	std::shared_ptr<QSqlQueryModel> pStorageSqlTableModel(new QSqlQueryModel);
	pStorageSqlTableModel->setQuery(QString("SELECT storage_history.date_time, storage_history.info_text, product.name, storage_history_info.count, storage_history_info.cost FROM "
		"storage_history INNER JOIN storage_history_info INNER JOIN product "
		"ON product.id = storage_history_info.product_id AND storage_history_info.history_id == storage_history.id AND storage_history.group_info_id == %1 ORDER BY storage_history.id DESC").arg(strGroupInfoId));

	pStorageSqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant(QString::fromUtf8("\324\261\325\264\325\275\325\241\325\251\325\253\325\276")));
	pStorageSqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant(QString::fromUtf8("\324\263\325\270\326\200\325\256\325\270\325\262\325\270\326\202\325\251\325\265\325\241\325\266 \325\253\325\266\326\206\325\270\326\200\325\264\325\241\326\201\325\253\325\241\325\266")));
	pStorageSqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant(QString::fromUtf8("\324\261\325\272\326\200\325\241\325\266\326\204\325\241\325\277\325\245\325\275\325\241\325\257")));
	pStorageSqlTableModel->setHeaderData(3, Qt::Horizontal, QVariant(QString::fromUtf8("\325\224\325\241\325\266\325\241\325\257")));
	pStorageSqlTableModel->setHeaderData(4, Qt::Horizontal, QVariant(QString::fromUtf8("\324\263\325\253\325\266")));

	m_mapStorageNameToStorageHistoryModel.emplace(strStorageName + strGroupName, pStorageSqlTableModel);

	return pStorageSqlTableModel;
}

std::shared_ptr<QSqlQueryModel> CStoragesData::GetFarmHistorySqlTableModel()
{
	CSqlQueryModel* pCSqlQueryModel = new CSqlQueryModel();
	QList<int> lstDoubleColumnIndices;
	lstDoubleColumnIndices.append(2);
	pCSqlQueryModel->SetDoubleColumnIndices(lstDoubleColumnIndices);

	m_pFarmHistorySqlTableModel = std::shared_ptr<QSqlQueryModel>(pCSqlQueryModel);
	m_pFarmHistorySqlTableModel->setQuery("SELECT farm_history.date_time, farm_history.info_text, farm_history.cost FROM farm_history ORDER BY id DESC");

	m_pFarmHistorySqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant(
		QString::fromUtf8("\324\261\325\264\325\275\325\241\325\251\325\253\325\276")));
	m_pFarmHistorySqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant(
		QString::fromUtf8("\324\263\325\270\326\200\325\256\325\270\325\262\325\270\326\202\325\251\325\265\325\241\325\266 \325\253\325\266\326\206\325\270\326\200\325\264\325\241\326\201\325\253\325\241\325\266")));
	m_pFarmHistorySqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant(
		QString::fromUtf8("\324\263\325\270\326\202\325\264\325\241\326\200")));

	return m_pFarmHistorySqlTableModel;
}

std::shared_ptr<QSqlQueryModel> CStoragesData::GetGroupHistorySqlTableModel(QString const& strGroupName)
{
	QString strGroupId = GetGroupIdByName(strGroupName);
	std::shared_ptr<QSqlQueryModel> pGroupSqlTableModel(new QSqlQueryModel);
	pGroupSqlTableModel->setQuery(QString("SELECT storage_history.date_time, storage_history.info_text, product.name, storage_history_info.count, storage_history_info.cost FROM "
		"storage_history INNER JOIN storage_history_info INNER JOIN product INNER JOIN group_info "
		"ON product.id = storage_history_info.product_id AND storage_history_info.history_id == storage_history.id AND "
		"storage_history.group_info_id == group_info.id AND group_info.group_id == %1 ORDER BY storage_history.id DESC").arg(strGroupId));

	pGroupSqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant(QString::fromUtf8("\324\261\325\264\325\275\325\241\325\251\325\253\325\276")));
	pGroupSqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant(QString::fromUtf8("\324\263\325\270\326\200\325\256\325\270\325\262\325\270\326\202\325\251\325\265\325\241\325\266 \325\253\325\266\326\206\325\270\326\200\325\264\325\241\326\201\325\253\325\241\325\266")));
	pGroupSqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant(QString::fromUtf8("\324\261\325\272\326\200\325\241\325\266\326\204\325\241\325\277\325\245\325\275\325\241\325\257")));
	pGroupSqlTableModel->setHeaderData(3, Qt::Horizontal, QVariant(QString::fromUtf8("\325\224\325\241\325\266\325\241\325\257")));
	pGroupSqlTableModel->setHeaderData(4, Qt::Horizontal, QVariant(QString::fromUtf8("\324\263\325\253\325\266")));

	m_pGroupSqlTableModel = pGroupSqlTableModel;

	return m_pGroupSqlTableModel;
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
	sqlQuery.exec(QString("SELECT name FROM %1").arg(table::product));

	QString strStorageName = "";
	while (sqlQuery.next())
	{
		strStorageName = sqlQuery.value(0).toString();
		lstString.push_back(strStorageName);
	}

	return lstString;
}

void CStoragesData::SubstractProducts(QList<QString> lstProductName, QList<int> lstProductCount) // Not a use
{
	QSqlQuery sqlQuery;
	for (int i = 0; i < lstProductName.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM product WHERE name == \"%1\" ").arg(lstProductName[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();
		sqlQuery.exec(QString("SELECT count FROM product WHERE id == %1 ").arg(strProductId));
		sqlQuery.next();
		int nCurrCount = sqlQuery.value(0).toInt();
		nCurrCount -= lstProductCount[i];

		sqlQuery.exec(QString("UPDATE product SET count = %1 WHERE id == %2").arg(QString::number(nCurrCount), strProductId));
	}
}

void CStoragesData::BuyStorageData(QString const& strStorageName, QString const& strGroupName, QList<QString> lstproductNames,
	QList<int> lstproductsCount, QList<double> lstProductCost, QString const& strInfoText)
{
	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	QString strStorageId = GetStorageIdByName(strStorageName);
	QString strGroupId = GetGroupIdByName(strGroupName);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strGroupId, strStorageId));
	sqlQuery.next();
	QString strGroupInfoId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("INSERT INTO storage_history ( group_info_id, action_id, info_text, date_time ) VALUES ( %1, %2, \"%3\", \"%4\" );").arg(
		strGroupInfoId, QString::number((int) (EAction::Sell)), strInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strStorageHistoryId = sqlQuery.value(0).toString();

	double dIncome = 0;
	for (int i = 0; i < lstproductNames.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM product WHERE name == \"%1\" ").arg(lstproductNames[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();
		sqlQuery.exec(QString("SELECT prime_cost FROM product WHERE id == %1 ").arg(strProductId));
		sqlQuery.next();
		QString strProductPrimeCost = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
			strStorageHistoryId, strProductId, QString::number(lstproductsCount[i]), QString::number(lstProductCost[i], 'f')));

		dIncome += lstproductsCount[i] * lstProductCost[i];
	}
	// update income from group_name
	sqlQuery.exec(QString("SELECT income FROM group_name WHERE id == %1;").arg(strGroupId));
	sqlQuery.next();
	QString strGroupIncome = QString::number(sqlQuery.value(0).toDouble() + dIncome, 'f');
	sqlQuery.exec(QString("UPDATE group_name SET income = %1 WHERE id == %2;").arg(strGroupIncome, strGroupId));

	for (int i = 0; i < lstproductNames.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM product WHERE name == \"%1\" ").arg(lstproductNames[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("SELECT count FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(
			strStorageId, strProductId));
		sqlQuery.next();
		int nProductsCount = sqlQuery.value(0).toInt();
		nProductsCount -= lstproductsCount[i];

		sqlQuery.exec(QString("UPDATE storage_info SET count = %1 WHERE storage_id == %2 AND product_id == %3").arg(
			QString::number(nProductsCount), strStorageId, strProductId));
	}

	UpdateAllSqlTableModel();
}

void CStoragesData::AddNewGroup(QString const& strStorageName, QString const& strGroupName)
{
	QStringList strListGroupNames = GetGroupNames();
	if (strListGroupNames.contains(strGroupName))
		return;

	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("INSERT INTO group_name ( name, income, expenses, date_time ) VALUES ( \"%1\" , %2 , %3 , \"%4\" );").arg(
		strGroupName, QString::number(0), QString::number(0), strDateTime));

	UpdateAllSqlTableModel(); // ?? not all
}

void CStoragesData::AddNewProduct(QString const& strNewProductName, int nCount, double dPrimeCost)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("INSERT INTO product ( name, count, prime_cost ) VALUES ( \"%1\" , %2, %3 );").arg(
		strNewProductName, QString::number(nCount), QString::number(dPrimeCost, 'f')));

	UpdateSqlTableModel(table::product);
}

void CStoragesData::AddNewStore(QString const& strStoreName)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("INSERT INTO storage_name ( name ) VALUES ( \"%1\" );").arg(
		strStoreName));

	UpdateAllSqlTableModel();
}

void CStoragesData::NourishProductFromStorageToStorage(QString const& strInStorageName, QString const& strInStorageGroupName, QString strOutStorage, QString const& strOutStorageGroupName,
	QList<QString> const& lstProductName, QList<int> const& lstProductCount, QString const& strInStorageInfoText, QString const& strOutStorageInfoText)
{
	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	QString strInStorageId = GetStorageIdByName(strInStorageName);
	QString strInStorageGroupId = GetGroupIdByName(strInStorageGroupName);

	QString strOutStorageId = GetStorageIdByName(strOutStorage);
	QString strOutStorageGroupId = GetGroupIdByName(strOutStorageGroupName);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strInStorageGroupId, strInStorageId));
	sqlQuery.next();
	QString strInGroupInfoId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strOutStorageGroupId, strOutStorageId));
	sqlQuery.next();
	QString strOutGroupInfoId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("INSERT INTO storage_history ( group_info_id, action_id, info_text, date_time ) VALUES ( %1, %2, \"%3\", \"%4\" );").arg(
		strInGroupInfoId, QString::number((int) (EAction::Nourish)), strInStorageInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strInStorageHistoryId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("INSERT INTO storage_history ( group_info_id, action_id, info_text, date_time ) VALUES ( %1, %2, \"%3\", \"%4\" );").arg(
		strOutGroupInfoId, QString::number((int) (EAction::Refer)), strOutStorageInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strOutStorageHistoryId = sqlQuery.value(0).toString();

	double dCostsCount = 0;
	for (int i = 0; i < lstProductName.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM product WHERE name == \"%1\"").arg(lstProductName[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("SELECT count FROM product WHERE id == %1").arg(strProductId));
		sqlQuery.next();
		int nProductCount = sqlQuery.value(0).toInt();
		nProductCount -= lstProductCount[i];

		sqlQuery.exec(QString("UPDATE product SET count = %1 WHERE id == %2").arg(
			QString::number(nProductCount), strProductId));

		sqlQuery.exec(QString("SELECT prime_cost FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strOutStorageId, strProductId));
		sqlQuery.next();
		double dProductPrimeCostInOutStorage = sqlQuery.value(0).toDouble();

		sqlQuery.exec(QString("SELECT count FROM storage_info WHERE storage_id == %1 AND product_id == %2").arg(strOutStorageId, strProductId));
		sqlQuery.next();
		int nProductCountInOutStorage = sqlQuery.value(0).toInt();
		nProductCountInOutStorage -= lstProductCount[i];

		dCostsCount += lstProductCount[i] * dProductPrimeCostInOutStorage;

		sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
			strInStorageHistoryId, strProductId, QString::number(lstProductCount[i]), QString::number(dProductPrimeCostInOutStorage, 'f')));

		sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
			strOutStorageHistoryId, strProductId, QString::number(lstProductCount[i]), QString::number(dProductPrimeCostInOutStorage, 'f')));

		sqlQuery.exec(QString("UPDATE storage_info SET count = %1 WHERE storage_id == %2 AND product_id == %3").arg(
			QString::number(nProductCountInOutStorage), strOutStorageId, strProductId));
	}

	AddStoragesCosts(strInStorageName, strInStorageGroupName, dCostsCount, strInStorageName +
		QString::fromUtf8(" \325\272\325\241\325\260\325\270\326\201\325\253 \325\256\325\241\325\255\325\275"));

	UpdateAllSqlTableModel();
}
void CStoragesData::MoveProductFromStorageInStorage(QString const& strInStorageName, QString const& strInStorageGroupName, QString strOutStorage, QString const& strOutStorageGroupName,
	QList<QString> const& lstProductName, QList<int> const& lstProductCount, QString const& strInStorageInfoText, QString const& strOutStorageInfoText)
{
	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	QString strInStorageId = GetStorageIdByName(strInStorageName);
	QString strInStorageGroupId = GetGroupIdByName(strInStorageGroupName);

	QString strOutStorageId = GetStorageIdByName(strOutStorage);
	QString strOutStorageGroupId = GetGroupIdByName(strOutStorageGroupName);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strInStorageGroupId, strInStorageId));
	sqlQuery.next();
	QString strInGroupInfoId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strOutStorageGroupId, strOutStorageId));
	sqlQuery.next();
	QString strOutGroupInfoId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("INSERT INTO storage_history ( group_info_id, action_id, info_text, date_time ) VALUES ( %1, %2, \"%3\", \"%4\" );").arg(
		strInGroupInfoId, QString::number((int) (EAction::Bring)), strInStorageInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strInStorageHistoryId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("INSERT INTO storage_history ( group_info_id, action_id, info_text, date_time ) VALUES ( %1, %2, \"%3\", \"%4\" );").arg(
		strOutGroupInfoId, QString::number((int) (EAction::Refer)), strOutStorageInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strOutStorageHistoryId = sqlQuery.value(0).toString();

	for (int i = 0; i < lstProductName.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM product WHERE name == \"%1\"").arg(lstProductName[i]));
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
			strInStorageHistoryId, strProductId, QString::number(lstProductCount[i]), QString::number(dProductPrimeCostInOutStorage, 'f')));

		sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
			strOutStorageHistoryId, strProductId, QString::number(lstProductCount[i]), QString::number(dProductPrimeCostInOutStorage, 'f')));

		sqlQuery.exec(QString("UPDATE storage_info SET count = %1 WHERE storage_id == %2 AND product_id == %3").arg(
			QString::number(nProductCountInOutStorage), strOutStorageId, strProductId));

		sqlQuery.exec(QString("SELECT * FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strInStorageId, strProductId));
		sqlQuery.next();
		int nRow = sqlQuery.value(0).toInt();
		if (nRow == 0)
		{
			sqlQuery.exec(QString("INSERT INTO storage_info VALUES ( %1, %2, %3, %4 )").arg(
				strInStorageId, strProductId, QString::number(lstProductCount[i]), QString::number(dProductPrimeCostInOutStorage, 'f')));

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
			QString::number(nProductCountInInStorage), QString::number(dProductPrimeCostInInStorage, 'f'), strInStorageId, strProductId));
	}

	UpdateAllSqlTableModel();
}

void CStoragesData::AddProductInStorage(QString const& strStorageName, QString const& strGroupName, QList<QString> const& lstProductName,
	QList<int> const& lstProductCount, QList<double> const& lstProductCost, QString const& strInfoText)
{
	QString strStorageId = GetStorageIdByName(strStorageName);
	QString strStorageGroupId = GetGroupIdByName(strGroupName); // ?? if no group_name fatal error

	QSqlQuery sqlQuery;
	QStringList strListGroupNames = GetGroupNamesByStorage(strStorageName);
	if (!strListGroupNames.contains(strGroupName))
	{
		sqlQuery.exec(QString("INSERT INTO group_info ( group_id, storage_id ) VALUES ( %1, %2 );").arg(strStorageGroupId, strStorageId));
		sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM group_info");
	}
	else
		sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strStorageGroupId, strStorageId));

	sqlQuery.next();
	QString strGroupInfoId = sqlQuery.value(0).toString();

	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	sqlQuery.exec(QString("INSERT INTO storage_history ( group_info_id, action_id, info_text, date_time ) VALUES ( %1, %2, \"%3\", \"%4\" );").arg(
		strGroupInfoId, QString::number((int) (EAction::Buy)), strInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strStorageHistoryId = sqlQuery.value(0).toString();

	double dCosts = 0;
	for (int i = 0; i < lstProductName.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM product WHERE name == \"%1\" ").arg(lstProductName[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();
		sqlQuery.exec(QString("SELECT prime_cost FROM product WHERE id == %1 ").arg(strProductId));
		sqlQuery.next();
		QString strProductPrimeCost = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
			strStorageHistoryId, strProductId, QString::number(lstProductCount[i]), QString::number(lstProductCost[i], 'f')));

		dCosts += lstProductCount[i] * lstProductCost[i];
	}
	// update expenses from group_name
	sqlQuery.exec(QString("SELECT expenses FROM group_name WHERE id == %1;").arg(strStorageGroupId));
	sqlQuery.next();
	QString strGroupExpenses = QString::number(sqlQuery.value(0).toDouble() + dCosts, 'f');
	sqlQuery.exec(QString("UPDATE group_name SET expenses = %1 WHERE id == %2;").arg(strGroupExpenses, strStorageGroupId));

	// Farm history Begin
	sqlQuery.exec(QString("SELECT cost FROM farm_history WHERE storage_id == %1 AND date_time == \"%2\"").arg(strStorageId, strDateTime));
	sqlQuery.next();

	if (sqlQuery.isValid())
	{
		double dSum = sqlQuery.value(0).toDouble() + dCosts;
		sqlQuery.exec(QString("UPDATE farm_history SET cost = %1 WHERE storage_id == %2 AND date_time == \"%3\"").arg(
			QString::number(dSum, 'f'), strStorageId, strDateTime));
	}
	else
		sqlQuery.exec(QString("INSERT INTO farm_history ( storage_id, info_text, cost, date_time ) VALUES ( %1, \"%2\", %3, \"%4\" );").arg(
		strStorageId, strStorageName + QString::fromUtf8(" \325\272\325\241\325\260\325\270\326\201\325\253 \325\256\325\241\325\255\325\275"),
		QString::number(dCosts, 'f'), strDateTime));
	// Farm history End

	for (int i = 0; i < lstProductName.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM product WHERE name == \"%1\" ").arg(lstProductName[i]));
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

		sqlQuery.exec(QString("SELECT count FROM product WHERE id == %1 ").arg(strProductId));
		sqlQuery.next();
		int nProductCountInProduct = sqlQuery.value(0).toInt();

		sqlQuery.exec(QString("SELECT prime_cost FROM product WHERE id == %1 ").arg(strProductId));
		sqlQuery.next();
		double dPrimeCost = sqlQuery.value(0).toDouble();
		dPrimeCost = (lstProductCount[i] * lstProductCost[i] + nProductCountInProduct * dPrimeCost) / (nProductCountInProduct + lstProductCount[i]);
		nProductCountInProduct += lstProductCount[i];

		sqlQuery.exec(QString("UPDATE product SET count = %1, prime_cost = %2 WHERE id == %3").arg(
			QString::number(nProductCountInProduct), QString::number(dPrimeCost, 'f'), strProductId));
		sqlQuery.exec(QString("SELECT * FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strStorageId, strProductId));
		sqlQuery.next();
		int nRow = sqlQuery.value(0).toInt();
		if (nRow == 0)
		{
			sqlQuery.exec(QString("INSERT INTO storage_info VALUES ( %1, %2, %3, %4 )").arg(
				strStorageId, strProductId, QString::number(nProductsCountInStorage), QString::number(dProductPrimeCostInStorage, 'f')));
		}
		else
			sqlQuery.exec(QString("UPDATE storage_info SET count = %1, prime_cost = %2 WHERE storage_id == %3 AND product_id == %4").arg(
			QString::number(nProductsCountInStorage), QString::number(dProductPrimeCostInStorage, 'f'), strStorageId, strProductId));
	}

	UpdateAllSqlTableModel();
}

void CStoragesData::SubstractProductInStorage(QString const& strStorageName, QString const& strProductName, int nCount)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM storage_name WHERE name == \"%1\"").arg(strStorageName));
	sqlQuery.next();
	QString strStorageId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT id FROM product WHERE name == \"%1\" ").arg(strProductName));
	sqlQuery.next();
	QString strProductId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT count FROM product WHERE id == %1 ").arg(strProductId));
	sqlQuery.next();
	int nProductCountInProduct = sqlQuery.value(0).toInt();
	nProductCountInProduct -= nCount;

	sqlQuery.exec(QString("SELECT count FROM storage_info WHERE storage_id == %1 AND product_id == %2 ").arg(strStorageId, strProductId));
	sqlQuery.next();
	int nProductsCountInStorage = sqlQuery.value(0).toInt();
	nProductsCountInStorage -= nCount;

	sqlQuery.exec(QString("UPDATE product SET count = %1 WHERE id == %2").arg(
		QString::number(nProductCountInProduct), strProductId));
	sqlQuery.exec(QString("UPDATE storage_info SET count = %1 WHERE storage_id == %2 AND product_id == %3").arg(
		QString::number(nProductsCountInStorage), strStorageId, strProductId));

	UpdateAllSqlTableModel();
}

void CStoragesData::DeclineProductInStorage(QString const& strStorageName, QString const& strGroupName, QString const& strProductName, int nCount, QString const& strInfoText)
{
	QString strStorageId = GetStorageIdByName(strStorageName);
	QString strGroupId = GetGroupIdByName(strGroupName);

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strGroupId, strStorageId));
	sqlQuery.next();
	QString strGroupInfoId = sqlQuery.value(0).toString();

	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	sqlQuery.exec(QString("INSERT INTO storage_history ( group_info_id, action_id, info_text, date_time ) VALUES ( %1, %2, \"%3\", \"%4\" );").arg(
		strGroupInfoId, QString::number((int) (EAction::Decline)), strInfoText, strDateTime));
	sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
	sqlQuery.next();
	QString strStorageHistoryId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT id FROM product WHERE name == \"%1\" ").arg(strProductName));
	sqlQuery.next();
	QString strProductId = sqlQuery.value(0).toString();

	sqlQuery.exec(QString("SELECT count FROM product WHERE id == %1 ").arg(strProductId));
	sqlQuery.next();
	int nProductCountInProduct = sqlQuery.value(0).toInt();

	sqlQuery.exec(QString("SELECT prime_cost FROM product WHERE id == %1 ").arg(strProductId));
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

	// update group_name expenses
	sqlQuery.exec(QString("SELECT expenses FROM group_name WHERE id == %1;").arg(strGroupId));
	sqlQuery.next();
	QString strGroupExpenses = QString::number(sqlQuery.value(0).toDouble() + dProductPrimeCostInStorage * nCount, 'f');
	sqlQuery.exec(QString("UPDATE group_name SET expenses = %1 WHERE id == %2;").arg(strGroupExpenses, strGroupId));

	// Farm history Begin
	sqlQuery.exec(QString("SELECT cost FROM farm_history WHERE storage_id == %1 AND date_time == \"%2\"").arg(strStorageId, strDateTime));
	sqlQuery.next();

	if (sqlQuery.isValid())
	{
		double dSum = sqlQuery.value(0).toDouble() + nCount * dProductPrimeCostInStorage;
		sqlQuery.exec(QString("UPDATE farm_history SET cost = %1 WHERE storage_id == %2 AND date_time == \"%3\"").arg(
			QString::number(dSum, 'f'), strStorageId, strDateTime));
	}
	else
		sqlQuery.exec(QString("INSERT INTO farm_history ( storage_id, info_text, cost, date_time ) VALUES ( %1, \"%2\", %3, \"%4\" );").arg(
			strStorageId, strStorageName + QString::fromUtf8(" \325\272\325\241\325\260\325\270\326\201\325\253 \325\256\325\241\325\255\325\275"),
			QString::number(nCount * dProductPrimeCostInStorage, 'f'), strDateTime));
	// Farm history End

	sqlQuery.exec(QString("INSERT INTO storage_history_info VALUES ( %1 , %2, %3, %4 );").arg(
		strStorageHistoryId, strProductId, QString::number(nCount), QString::number(dProductPrimeCostInStorage, 'f')));

	if (nProductsCountInStorage - nCount == 0)
		dProductPrimeCostInStorage = 0;
	else
		dProductPrimeCostInStorage = (nProductsCountInStorage * dProductPrimeCostInStorage) / (nProductsCountInStorage - nCount);
	nProductsCountInStorage -= nCount;

	sqlQuery.exec(QString("UPDATE product SET count = %1, prime_cost = %2 WHERE id == %3").arg(
		QString::number(nProductCountInProduct), QString::number(dProductPrimeCost, 'f'), strProductId));
	sqlQuery.exec(QString("UPDATE storage_info SET count = %1, prime_cost = %2 WHERE storage_id == %3 AND product_id == %4").arg(
		QString::number(nProductsCountInStorage), QString::number(dProductPrimeCostInStorage, 'f'), strStorageId, strProductId));

	UpdateAllSqlTableModel();
}

void CStoragesData::AddFarmCosts(double dCosts, QString const& strInfoText)
{
	QSqlQuery sqlQuery;

	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	sqlQuery.exec(QString("INSERT INTO farm_history ( info_text, cost, date_time ) VALUES ( \"%1\", %2, \"%3\" );").arg(
		strInfoText, QString::number(dCosts, 'f'), strDateTime));

	sqlQuery.exec(QString("SELECT * FROM product"));

	double dAllFarmCurrCost = 0;
	while (sqlQuery.next())
		dAllFarmCurrCost += sqlQuery.value(2).toInt() * sqlQuery.value(3).toDouble();

	if (dAllFarmCurrCost == 0)
		return; // ASSERT

	double dProductTypeCost = 0;
	int nProductTypeCount = 0;
	sqlQuery.exec(QString("SELECT * FROM product"));
	while (sqlQuery.next())
	{
		nProductTypeCount = sqlQuery.value(2).toInt();
		dProductTypeCost = sqlQuery.value(3).toDouble() * nProductTypeCount;

		dProductTypeCost = (dProductTypeCost / dAllFarmCurrCost) * (dAllFarmCurrCost + dCosts);
		QSqlQuery sqlTempQuery;
		sqlTempQuery.exec(QString("UPDATE product SET prime_cost = %1 WHERE id == %2").arg(
			QString::number(dProductTypeCost / nProductTypeCount, 'f'), sqlQuery.value(0).toString()));
	}

	sqlQuery.exec(QString("SELECT storage_id FROM storage_info"));
	QMap<QString, double> mapStorageIdToCost;
	while (sqlQuery.next())
		mapStorageIdToCost.insert(sqlQuery.value(0).toString(), 0);

	sqlQuery.exec(QString("SELECT * FROM storage_info"));
	while (sqlQuery.next())
	{
		QString strStorageId = sqlQuery.value(0).toString();

		nProductTypeCount = sqlQuery.value(2).toInt();
		dProductTypeCost = sqlQuery.value(3).toDouble() * nProductTypeCount;

		double dNewProductTypeCost = (dProductTypeCost / dAllFarmCurrCost) * (dAllFarmCurrCost + dCosts);
		auto it = mapStorageIdToCost.find(strStorageId);
		it.value() += dNewProductTypeCost - dProductTypeCost;

		QSqlQuery sqlTempQuery;
		sqlTempQuery.exec(QString("UPDATE storage_info SET prime_cost = %1 WHERE storage_id == %2 AND product_id == %3").arg(
			QString::number(dNewProductTypeCost / nProductTypeCount, 'f'), strStorageId, sqlQuery.value(1).toString()));
	}

	// update groups expenses
	auto it = mapStorageIdToCost.begin();
	while (it != mapStorageIdToCost.end())
	{
		sqlQuery.exec(QString("SELECT group_id FROM group_info WHERE storage_id == %1 ORDER BY id DESC LIMIT 1").arg(it.key()));
		sqlQuery.next();
		QString strGroupId = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("SELECT expenses FROM group_name WHERE id == %1;").arg(strGroupId));
		sqlQuery.next();
		QString strGroupExpenses = QString::number(sqlQuery.value(0).toDouble() + it.value(), 'f');
		sqlQuery.exec(QString("UPDATE group_name SET expenses = %1 WHERE id == %2;").arg(strGroupExpenses, strGroupId));

		++it;
	}

	UpdateAllSqlTableModel();
}

void CStoragesData::AddStoragesCosts(QString const& strStorageName, QString const& strGroupName, double dCosts,
	QString const& strInfoText, QString const& strStorageInfoText)
{
	QString strStorageId = GetStorageIdByName(strStorageName);
	QString strGroupId = GetGroupIdByName(strGroupName);

	// update groups expenses
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT expenses FROM group_name WHERE id == %1;").arg(strGroupId));
	sqlQuery.next();
	QString strGroupExpenses = QString::number(sqlQuery.value(0).toDouble() + dCosts, 'f');
	sqlQuery.exec(QString("UPDATE group_name SET expenses = %1 WHERE id == %2;").arg(strGroupExpenses, strGroupId));

	QString strDateTime = QDate::currentDate().toString("dd") + ' ' +
		GetDBManager()->GetMonthLongNameByMonthNumber(QDate::currentDate().month()) + ' ' + QDate::currentDate().toString("yyyy");

	sqlQuery.exec(QString("SELECT id FROM group_info WHERE group_id == %1 AND storage_id == %2").arg(strGroupId, strStorageId));
	sqlQuery.next();
	QString strInGroupInfoId = sqlQuery.value(0).toString();

	//-- Info text in storage history begin --//-- Work for add store cost
	if (strStorageInfoText != "")
	{
		sqlQuery.exec(QString("INSERT INTO storage_history ( group_info_id, action_id, info_text, date_time ) VALUES ( %1, %2, \"%3\", \"%4\" );").arg(
			strInGroupInfoId, QString::number((int) (EAction::Consumption)), strStorageInfoText, strDateTime));
		sqlQuery.exec("SELECT LAST_INSERT_ROWID() FROM storage_history");
		sqlQuery.next();
		QString strOutStorageHistoryId = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("INSERT INTO storage_history_info ( history_id, product_id, count, cost ) VALUES ( %1, %2, %3, %4 );").arg(
			strOutStorageHistoryId, QString::number(-1), QString::number(0), QString::number(dCosts, 'f')));
	}
	//-- Info text in storage history end --//

	// Farm history Begin
	sqlQuery.exec(QString("SELECT cost FROM farm_history WHERE storage_id == %1 AND date_time == \"%2\"").arg(strStorageId, strDateTime));
	sqlQuery.next();

	if (sqlQuery.isValid())
	{
		double dSum = sqlQuery.value(0).toDouble() + dCosts;
		sqlQuery.exec(QString("UPDATE farm_history SET cost = %1 WHERE storage_id == %2 AND date_time == \"%3\"").arg(
			QString::number(dSum, 'f'), strStorageId, strDateTime));
	}
	else
		sqlQuery.exec(QString("INSERT INTO farm_history ( storage_id, info_text, cost, date_time ) VALUES ( %1, \"%2\", %3, \"%4\" );").arg(
			strStorageId, strInfoText, QString::number(dCosts, 'f'), strDateTime));
	// Farm history End

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
			QString::number(dProductTypeCost / nProductTypeCount, 'f'), sqlQuery.value(0).toString(), sqlQuery.value(1).toString()));

		sqlTempQuery.exec(QString("SELECT count, prime_cost FROM product WHERE id == %1").arg(strProductId));
		sqlTempQuery.next();
 
		nProductTypeCount = sqlTempQuery.value(0).toInt();
		dProductTypeCost = dDeltaCost + sqlTempQuery.value(1).toDouble() * nProductTypeCount;

		sqlTempQuery.exec(QString("UPDATE product SET prime_cost = %1 WHERE id == %2").arg(
			QString::number(dProductTypeCost / nProductTypeCount, 'f'), strProductId));
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////