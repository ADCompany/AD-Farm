////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "db_customersdata.h"

// Qt Includes
#include <QSqlRecord>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////


//  Static Member Initialization

const QString CCustomersData::table::customer = "customer";

const QString CCustomersData::table::customer::id = "id";
const QString CCustomersData::table::customer::first_name = "first_name";
const QString CCustomersData::table::customer::last_name = "last_name";
const QString CCustomersData::table::customer::debt = "debt";
const QString CCustomersData::table::customer::phone_number = "phone_number";


////////////////////////////////////////////////////////////////////////////////
//
// class CCustomersData
//

// Interface Methodes
void CCustomersData::Initialize()
{
	QSqlQuery sqlQuery;

	EXECUTE_QUERY(sqlQuery, QString(
		"CREATE TABLE IF NOT EXISTS customer ("
		"id	INTEGER PRIMARY KEY AUTOINCREMENT,"
		"first_name	TEXT    NOT NULL, "
		"last_name	TEXT    NOT NULL, "
		"debt INTEGER NOT NULL, "
		"phone_number	TEXT NOT NULL );"));
	//.arg(table::customer,
	//								table::customer::id,
	//								table::customer::first_name,
	//								table::customer::last_name,
	//								table::customer::debt,
	//								table::customer::phone_number));
	
	std::shared_ptr<CDBManager> pDBManager = GetDBManager();
	if (pDBManager != nullptr)
		SetDBManager(pDBManager);
}

bool CCustomersData::SetParent(QObject* pParent)
{
	if (pParent == nullptr)
		return false;

	m_pParentObject = pParent;
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::customer);
	if (pSqlTableModel == nullptr)
		return false;

	pSqlTableModel->setParent(m_pParentObject);

	return true;
}

void CCustomersData::SetDBManager(std::shared_ptr<CDBManager> pDBManager)
{
	CDBComponent::SetDBManager(pDBManager);

	std::shared_ptr<CDBManager> pMyDBManager = GetDBManager();
	if (pMyDBManager == nullptr)
		return;

	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::customer);
	if (pSqlTableModel == nullptr)
	{
		pSqlTableModel = std::shared_ptr<QSqlTableModel>(new QSqlTableModel(m_pParentObject, pDBManager->GetDataBase()));
		SetSqlTableModel(table::customer, pSqlTableModel);
	}

	pSqlTableModel->setTable(table::customer);
	pSqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant("First name"));
	pSqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant("Last name"));
	pSqlTableModel->setHeaderData(3, Qt::Horizontal, QVariant("Dept"));
	pSqlTableModel->setHeaderData(4, Qt::Horizontal, QVariant("Phone number"));
	pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	pSqlTableModel->select();
	pSqlTableModel->submitAll();

	emit sigChangeData();
}

QList<QString> CCustomersData::GetCoulmnsName() const
{
	QList<QString> lstString;

	lstString.push_back(table::customer::id);
	lstString.push_back(table::customer::first_name);
	lstString.push_back(table::customer::last_name);
	lstString.push_back(table::customer::debt);
	lstString.push_back(table::customer::phone_number);

	return lstString;
}

QList<QString> CCustomersData::GetCustomersName() const
{
	QList<QString> lstString;

	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::customer);
	if (pSqlTableModel == nullptr)
		return lstString;

	QSqlQuery sqlQuery = pSqlTableModel->query();
	sqlQuery.exec(QString("SELECT first_name, last_name FROM %1").arg(table::customer));

	QString strFirstName = "";
	while (sqlQuery.next())
	{
		strFirstName = sqlQuery.value(0).toString() + ' ' + sqlQuery.value(1).toString();
		lstString.push_back(strFirstName);
	}

	return lstString;
}

void CCustomersData::AddCustomer(QString const& strFirstName, QString const& strLastName, int nDept, QString strPhoneNumber)
{
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::customer);
	if (pSqlTableModel == nullptr)
		return;

	int nRowCount = pSqlTableModel->rowCount();
	pSqlTableModel->insertRows(nRowCount, 1);

	pSqlTableModel->setData(pSqlTableModel->index(nRowCount, 1), strFirstName);
	pSqlTableModel->setData(pSqlTableModel->index(nRowCount, 2),  strLastName);
	pSqlTableModel->setData(pSqlTableModel->index(nRowCount, 3),		nDept);
	pSqlTableModel->setData(pSqlTableModel->index(nRowCount, 4), strPhoneNumber);

	pSqlTableModel->submitAll();

	emit sigChangeData();
}

void CCustomersData::RemoveCustomer(QString const& strFirstName, QString const& strLastName)
{
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::customer);
	if (pSqlTableModel == nullptr)
		return;

	QSqlQuery sqlQuery = pSqlTableModel->query();
	sqlQuery.exec(QString("DELETE FROM %1 WHERE first_name = %2 AND last_name = %3").arg(table::customer, strFirstName, strLastName));

	UpdateSqlTableModel();
}

void CCustomersData::RemoveCustomer(int nRow)
{
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::customer);
	if (pSqlTableModel == nullptr)
		return;

	pSqlTableModel->removeRow(nRow);
	pSqlTableModel->submitAll();

	emit sigChangeData();
}

void CCustomersData::UpdateCustomerDebt(QString const& strFirstName, QString const& strLastName, double dDebt)
{
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::customer);
	if (pSqlTableModel == nullptr)
		return;

	int nCustomerId = GetCustomerId(strFirstName, strLastName);
	QSqlQuery sqlQuery = pSqlTableModel->query();
	sqlQuery.exec(QString("SELECT debt FROM %1 WHERE id == %2").arg(table::customer, QString::number(nCustomerId)));
	sqlQuery.next();
	double dCurrDebt = sqlQuery.value(0).toDouble();

	dCurrDebt += dDebt;
	sqlQuery.exec(QString("UPDATE %1 SET debt = %2 WHERE id == %3").arg(table::customer, QString::number(dCurrDebt), QString::number(nCustomerId)));

	UpdateSqlTableModel();
}

int CCustomersData::GetCustomerId(QString const& strFirstName, QString const& strLastName)
{
	QSqlQuery sqlQuery;
	sqlQuery.exec(QString("SELECT %1 FROM %2 WHERE first_name == \"%3\" AND last_name == \"%4\"").arg(
		table::customer::id, table::customer, strFirstName, strLastName));
	sqlQuery.next();
	int nId = sqlQuery.value(0).toInt();

	return nId;
}

QString CCustomersData::GetNameByIndex(int nColumn, int nRow) const
{
	return QString();
}

void CCustomersData::SetNameByIndex(int nColumn, int nRow, QString const& strName)
{

}

// Helper Functions
void CCustomersData::UpdateSqlTableModel()
{
	CDBComponent::DeleteSqlTableModel(table::customer);

	SetDBManager(GetDBManager());
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////