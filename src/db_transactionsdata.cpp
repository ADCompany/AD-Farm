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
const QString CTransactionsData::table::transaction = "customer";

const QString CTransactionsData::table::customer::id = "id";
const QString CTransactionsData::table::customer::first_name = "first_name";
const QString CTransactionsData::table::customer::last_name = "last_name";
const QString CTransactionsData::table::customer::debt = "debt";
const QString CTransactionsData::table::customer::phone_number = "phone_number";

// Interface Methodes
void CTransactionsData::Initialize()
{
	QSqlQuery sqlQuery;

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS transaction_info ("
		"transaction_id		INTEGER PRIMARY KEY NOT NULL, "
		"product_id			INTEGER NOT NULL, "
		"count				INTEGER NOT NULL, "
		"cost				INTEGER NOT NULL);");

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS transaction ("
		"id					INTEGER PRIMARY KEY NOT NULL, "
		"customer_id		INTEGER NOT NULL, "
		"date_time			TEXT    NOT NULL);");
	//.arg(table::customer,
	//								table::customer::id,
	//								table::customer::first_name,
	//								table::customer::last_name,
	//								table::customer::debt,
	//								table::customer::phone_number));
	
	std::shared_ptr<CDBManager> pDBManager = GetDBManager();
	if (pDBManager != nullptr)
		SetDBManager(pDBManager);

	GetCustomersName();
}

bool CTransactionsData::SetParent(QObject* pParent)
{
	if (pParent == nullptr)
		return false;

	m_pParentObject = pParent;
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::transaction);
	if (pSqlTableModel == nullptr)
		return false;

	pSqlTableModel->setParent(m_pParentObject);

	return true;
}

void CTransactionsData::SetDBManager(std::shared_ptr<CDBManager> pDBManager)
{
	CDBComponent::SetDBManager(pDBManager);

	std::shared_ptr<CDBManager> pMyDBManager = GetDBManager();
	if (pMyDBManager == nullptr)
		return;

	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::transaction);
	if (pSqlTableModel == nullptr)
	{
		pSqlTableModel = std::shared_ptr<QSqlTableModel>(new QSqlTableModel(m_pParentObject, pDBManager->GetDataBase()));
		SetSqlTableModel(table::transaction, pSqlTableModel);
	}

	pSqlTableModel->setTable(table::transaction);
	pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	pSqlTableModel->select();
	pSqlTableModel->submitAll();
}

QList<QString> CTransactionsData::GetCoulmnsName() const
{
	QList<QString> lstString;

	lstString.push_back(table::customer::id);
	lstString.push_back(table::customer::first_name);
	lstString.push_back(table::customer::last_name);
	lstString.push_back(table::customer::debt);
	lstString.push_back(table::customer::phone_number);

	return lstString;
}

QList<QString> CTransactionsData::GetCustomersName() const
{
	QList<QString> lstString;

	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::transaction);
	if (pSqlTableModel == nullptr)
		return lstString;

	for (int i = 0; i < GetCustomersCount(); ++i)
	{
		QModelIndex modelIndex;
		lstString.push_back((pSqlTableModel->data(modelIndex.sibling(i, 1))).toString());
	}

	return lstString;
}

void CTransactionsData::AddCustomer(QString const& strFirstName, QString const& strLastName, int nDept, int nPhoneNumber)
{
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::transaction);
	if (pSqlTableModel == nullptr)
		return;

	int nRowCount = pSqlTableModel->rowCount();
	pSqlTableModel->insertRows(nRowCount, 1);

	pSqlTableModel->setData(pSqlTableModel->index(nRowCount, 1), strFirstName);
	pSqlTableModel->setData(pSqlTableModel->index(nRowCount, 2),  strLastName);
	pSqlTableModel->setData(pSqlTableModel->index(nRowCount, 3),		nDept);
	pSqlTableModel->setData(pSqlTableModel->index(nRowCount, 4), nPhoneNumber);

	pSqlTableModel->submitAll();
}

void CTransactionsData::RemoveCustomer(QString const& strFirstName, QString const& strLastName)
{
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::transaction);
	if (pSqlTableModel == nullptr)
		return;

	QSqlQuery sqlQuery = pSqlTableModel->query();
	sqlQuery.prepare(QString("DELETE FROM %1 WHERE first_name = %2 AND last_name = %3").arg(table::transaction, strFirstName, strLastName));
	sqlQuery.exec();

	UpdateSqlTableModel();
}

void CTransactionsData::RemoveCustomer(int nRow)
{
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::transaction);
	if (pSqlTableModel == nullptr)
		return;

	pSqlTableModel->removeRow(nRow);
	pSqlTableModel->submitAll();
}


int CTransactionsData::GetCustomerId(QString const& strFirstName, QString const& strLastName)
{
	QSqlQuery sqlQuery;
	sqlQuery.prepare(QString("SELECT %1 FROM %2 WHERE first_name = %3 AND last_name = %4").arg(
		table::customer::id, table::transaction, "\"" + strFirstName + "\"", "\"" + strLastName + "\""));

	sqlQuery.exec();

	int nId = sqlQuery.value(0).toInt();

	return nId;
}

QString CTransactionsData::GetNameByIndex(int nColumn, int nRow) const
{
	return QString();
}

void CTransactionsData::SetNameByIndex(int nColumn, int nRow, QString const& strName)
{

}

// Helper Functions
void CTransactionsData::UpdateSqlTableModel()
{
	CDBComponent::DeleteSqlTableModel(table::transaction);

	SetDBManager(GetDBManager());
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////