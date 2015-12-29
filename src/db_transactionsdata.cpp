////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "db_transactionsdata.h"

// Qt Includes
#include <QSqlRecord>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CTransactionsData
//

//  Static Member Initialization
const QString CTransactionsData::table::transaction_info = "transaction_info";
const QString CTransactionsData::table::deal = "deal";

const QString CTransactionsData::table::transaction_info::transaction_id	= "transaction_id";
const QString CTransactionsData::table::transaction_info::product_id		= "product_id";
const QString CTransactionsData::table::transaction_info::count				= "count";
const QString CTransactionsData::table::transaction_info::cost				= "cost";

const QString CTransactionsData::table::deal::id				= "id";
const QString CTransactionsData::table::deal::customer_id	= "customer_id";
const QString CTransactionsData::table::deal::date_time = "date_time";

// Interface Methodes
void CTransactionsData::Initialize()
{
	QSqlQuery sqlQuery;

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS deal ("
		"id					INTEGER PRIMARY KEY AUTOINCREMENT, "
		"cost				REAL	NOT NULL, "
		"customer_id		INTEGER NOT NULL, "
		"customer_cost		REAL	NOT NULL, "
		"date_time			TEXT	NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS transaction_info ("
		"transaction_id		INTEGER NOT NULL, "
		"product_id			INTEGER NOT NULL, "
		"count				INTEGER NOT NULL, "
		"cost				REAL	NOT NULL);");
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

bool CTransactionsData::SetParent(QObject* pParent)
{
	if (pParent == nullptr)
		return false;

	return true;
}

void CTransactionsData::SetDBManager(std::shared_ptr<CDBManager> pDBManager)
{
	CDBComponent::SetDBManager(pDBManager);
}

void CTransactionsData::AddTransactionData(QString const& strCustomerName, QList<QString> const& lstProductNames, QList<int> lstCount, QList<double> lstCost, double dDealCost, double dCustomerCost)
{
	QString strFirstName = "";
	QString strLastName = "";
	int nIndex = 0;
	for (; nIndex < strCustomerName.size() && strCustomerName[nIndex] != ' '; ++nIndex)
	{
		strFirstName += strCustomerName[nIndex];
	}
	++nIndex;
	for (; nIndex < strCustomerName.size(); ++nIndex)
	{
		strLastName += strCustomerName[nIndex];
	}

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM customer WHERE first_name == \"%1\" AND last_name == \"%2\"").arg(strFirstName, strLastName));
	sqlQuery.next();

	QString strCustomerId = sqlQuery.value(0).toString();
	QDateTime dateTime = QDateTime::currentDateTimeUtc();
	QString strDateTime = dateTime.toString();

	sqlQuery.exec(QString("INSERT INTO deal ( cost, customer_id, customer_cost, date_time ) VALUES ( %1, %2, %3, \"%4\" );").arg(QString::number(dDealCost), strCustomerId, QString::number(dCustomerCost), strDateTime));
	sqlQuery.exec(QString("SELECT id FROM deal WHERE customer_id == %1 AND date_time == \"%2\" ").arg(strCustomerId, strDateTime));
	sqlQuery.next();
	QString strTransactionId = sqlQuery.value(0).toString();

	for (int i = 0; i < lstProductNames.count(); ++i)
	{
		sqlQuery.exec(QString("SELECT id FROM producte WHERE name == \"%1\" ").arg(lstProductNames[i]));
		sqlQuery.next();
		QString strProductId = sqlQuery.value(0).toString();

		sqlQuery.exec(QString("INSERT INTO transaction_info VALUES ( %1 , %2, %3, %4 );").arg(strTransactionId, strProductId, QString::number(lstCount[i]), QString::number(lstCost[i])));
	}

	UpdateSqlTableModel(strCustomerName);
}

void CTransactionsData::UpdateSqlTableModel(QString const& strCustomerName)
{
	RemoveSqlTableModel(strCustomerName);
}

void CTransactionsData::RemoveSqlTableModel(QString const& strCustomerName)
{
	m_mapStringToModel.erase(strCustomerName);

	emit sigChangeData();
}

std::shared_ptr<QSqlQueryModel> CTransactionsData::GetSqlTableModelByCustomerName(QString const& strCustomerName)
{
	auto itMap = m_mapStringToModel.find(strCustomerName);
	if (itMap != m_mapStringToModel.end())
		return itMap->second;

	QString strFirstName = "";
	QString strLastName = "";
	int nIndex = 0;
	for (; nIndex < strCustomerName.size() && strCustomerName[nIndex] != ' '; ++nIndex)
	{
		strFirstName += strCustomerName[nIndex];
	}
	++nIndex;
	for (; nIndex < strCustomerName.size(); ++nIndex)
	{
		strLastName += strCustomerName[nIndex];
	}

	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT id FROM customer WHERE first_name == \"%1\" AND last_name == \"%2\"").arg(
		strFirstName, strLastName));
	sqlQuery.next();

	QString strCustomerId = sqlQuery.value(0).toString();

	std::shared_ptr<QSqlQueryModel> pTransactionSqlTableModel(new QSqlQueryModel);
	pTransactionSqlTableModel->setQuery(QString("SELECT deal.id, producte.name, transaction_info.count, transaction_info.cost, deal.cost, deal.customer_cost, deal.date_time FROM "
		"transaction_info INNER JOIN deal INNER JOIN producte "
		"ON deal.customer_id == %1 AND transaction_info.transaction_id == deal.id AND transaction_info.product_id == producte.id").arg(strCustomerId));

	pTransactionSqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant(QString::fromUtf8("\324\263\325\270\326\200\325\256. \325\260\325\241\325\264\325\241\326\200")));
	pTransactionSqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant(QString::fromUtf8("\325\217\325\245\325\275\325\241\325\257")));
	pTransactionSqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant(QString::fromUtf8("\325\224\325\241\325\266\325\241\325\257")));
	pTransactionSqlTableModel->setHeaderData(3, Qt::Horizontal, QVariant(QString::fromUtf8("\325\204\325\253\325\241\325\276\325\270\326\200\325\253 \325\243\325\253\325\266")));
	pTransactionSqlTableModel->setHeaderData(4, Qt::Horizontal, QVariant(QString::fromUtf8("\324\263\325\270\326\200\325\256. \325\243\325\270\326\202\325\264\325\241\326\200")));
	pTransactionSqlTableModel->setHeaderData(5, Qt::Horizontal, QVariant(QString::fromUtf8("\325\216\325\263\325\241\326\200. \325\243\325\270\326\202\325\264\325\241\326\200")));
	pTransactionSqlTableModel->setHeaderData(6, Qt::Horizontal, QVariant(QString::fromUtf8("\324\261\325\264\325\275\325\241\325\251\325\253\325\276")));

	m_mapStringToModel.emplace(strCustomerName, pTransactionSqlTableModel);

	return pTransactionSqlTableModel;
}

// Helper Functions
void CTransactionsData::UpdateSqlTableModel()
{
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////