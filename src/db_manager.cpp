////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "db_manager.h"
#include "db_storagesdata.h"
#include "db_customersdata.h"
#include "db_transactionsdata.h"

// Qt includes

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CDBComponent
//

// Interface Methodes
std::shared_ptr<QSqlTableModel> CDBComponent::GetSqlTableModel(QString const& strTableName)
{
	auto itMap = m_mapStringToTable.find(strTableName);
	if (itMap == m_mapStringToTable.end())
		return nullptr;

	return itMap->second;
}

std::shared_ptr<QSqlTableModel> CDBComponent::GetSqlTableModel(QString const& strTableName) const
{
	auto itMap = m_mapStringToTable.find(strTableName);
	if (itMap == m_mapStringToTable.end())
		return nullptr;

	return itMap->second;
}

bool CDBComponent::SetSqlTableModel(QString const& strTableName, std::shared_ptr<QSqlTableModel> pSqlTableModel)
{
	if (strTableName == "")
		return SetSqlTableModel(pSqlTableModel);

	m_mapStringToTable.emplace(strTableName, pSqlTableModel);

	return true;
}

bool CDBComponent::SetSqlTableModel(std::shared_ptr<QSqlTableModel> pSqlTableModel)
{
	if (pSqlTableModel == nullptr)
		return false;

	QString strTableName = pSqlTableModel->tableName();
	if (strTableName == "")
		return false;

	m_mapStringToTable.emplace(strTableName, pSqlTableModel);

	return true;
}

QList<QString> CDBComponent::GetColumnsName(QString const& strTableName)
{
	QList<QString> lstString;

	return lstString;
}

// Helper Functions

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CDBManager
//

// Interface Methodes
void CDBManager::Initialize(QString const& strDBFilePath)
{
	m_pDBConnectionStarter->StartConnection(strDBFilePath);

//	SetDBComponent(std::shared_ptr<IDBComponent>(new CStoragesData()), "StoragesData", true);
	SetDBComponent(std::shared_ptr<IDBComponent>(new CCustomersData()), "CustomersData", true);
//	SetDBComponent(std::shared_ptr<IDBComponent>(new CTransactionsData()), "TransactionsData", true);
}

// Helper Functions
bool CDBManager::SetDBComponent(std::shared_ptr<IDBComponent> pDBComponent, QString const& strComponentName, bool bInitialize)
{
	if (pDBComponent == nullptr || strComponentName == "")
		return false;

	if (bInitialize)
		pDBComponent->Initialize();

	m_mapStringToComponent.emplace(strComponentName, pDBComponent);

	return true;
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////