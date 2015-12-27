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
		"customer_id		INTEGER NOT NULL, "
		"date_time			TEXT	NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS transaction_info ("
		"transaction_id		INTEGER NOT NULL, "
		"product_id			INTEGER NOT NULL, "
		"count				INTEGER NOT NULL, "
		"cost				INTEGER NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS producte ("
		"id				INTEGER PRIMARY KEY NOT NULL, "
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
	sqlQuery.prepare(QString("SELECT id FROM customer WHERE first_name == \"%1\" AND last_name == \"%2\"").arg(
		strFirstName, strLastName));
	sqlQuery.exec();
	sqlQuery.next();

	QString strCustomerId = sqlQuery.value(0).toString();

	std::shared_ptr<QSqlQueryModel> pTransactionSqlTableModel(new QSqlQueryModel);
	pTransactionSqlTableModel->setQuery(QString("SELECT producte.name, transaction_info.count, transaction_info.cost, deal.date_time FROM "
		"transaction_info INNER JOIN deal INNER JOIN producte "
		"ON deal.customer_id == %1 AND transaction_info.transaction_id == deal.id AND transaction_info.product_id == producte.id").arg(strCustomerId));

	pTransactionSqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant("Type"));
	pTransactionSqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant("Count"));
	pTransactionSqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant("Cost"));
	pTransactionSqlTableModel->setHeaderData(3, Qt::Horizontal, QVariant("Date"));

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