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

////////////////////////////////////////////////////////////////////////////////
// Static Members Initialization

// SMonthLongName
const QString CDBManager::SMonthLongName::January = QString::fromUtf8("\325\200\325\270\326\202\325\266\325\276\325\241\326\200");
const QString CDBManager::SMonthLongName::February = QString::fromUtf8("\325\223\325\245\325\277\326\200\325\276\325\241\326\200");

const QString CDBManager::SMonthLongName::March = QString::fromUtf8("\325\204\325\241\326\200\325\277");
const QString CDBManager::SMonthLongName::April = QString::fromUtf8("\324\261\325\272\326\200\325\253\325\254");
const QString CDBManager::SMonthLongName::May = QString::fromUtf8("\325\204\325\241\325\265\325\253\325\275");

const QString CDBManager::SMonthLongName::June = QString::fromUtf8("\325\200\325\270\326\202\325\266\325\253\325\275");
const QString CDBManager::SMonthLongName::July = QString::fromUtf8("\325\200\325\270\326\202\325\254\325\253\325\275");
const QString CDBManager::SMonthLongName::August = QString::fromUtf8("\325\225\325\243\325\270\325\275\325\277\325\270\325\275");

const QString CDBManager::SMonthLongName::September = QString::fromUtf8("\325\215\325\245\325\272\325\277\325\245\325\264\325\242\325\245\326\200");
const QString CDBManager::SMonthLongName::October = QString::fromUtf8("\325\200\325\270\325\257\325\277\325\245\325\264\325\242\325\245\326\200");
const QString CDBManager::SMonthLongName::November = QString::fromUtf8("\325\206\325\270\325\265\325\245\325\264\325\242\325\245\326\200");

const QString CDBManager::SMonthLongName::December = QString::fromUtf8("\324\264\325\245\325\257\325\277\325\245\325\264\325\242\325\245\326\200");

// component
const QString CDBManager::component::storages = "storages";
const QString CDBManager::component::customers = "customers";
const QString CDBManager::component::transactions = "transactions";
////////////////////////////////////////////////////////////////////////////////

// Interface Methodes
void CDBManager::Initialize(QString const& strDBFilePath)
{
	m_pDBConnectionStarter->StartConnection(strDBFilePath);

	SetDBComponent(std::shared_ptr<IDBComponent>(new CStoragesData(nullptr, std::shared_ptr<CDBManager>(this))), component::storages, true);
	SetDBComponent(std::shared_ptr<IDBComponent>(new CCustomersData(nullptr, std::shared_ptr<CDBManager>(this))), component::customers, true);
	SetDBComponent(std::shared_ptr<IDBComponent>(new CTransactionsData(nullptr, std::shared_ptr<CDBManager>(this))), component::transactions, true);
}

CDBManager::EMonth CDBManager::GetMonthByLongName(QString const& strMonthLongName)
{
	return EMonth::Invalid;
}

CDBManager::EMonth CDBManager::GetMontByMonthNumber(int nMonth)
{
	EMonth eMonth = EMonth::Invalid;
	switch (nMonth)
	{
	case 1:
		eMonth = EMonth::January;
		break;
	case 2:
		eMonth = EMonth::February;
		break;
	case 3:
		eMonth = EMonth::March;
		break;
	case 4:
		eMonth = EMonth::April;
		break;
	case 5:
		eMonth = EMonth::May;
		break;
	case 6:
		eMonth = EMonth::June;
		break;
	case 7:
		eMonth = EMonth::July;
		break;
	case 8:
		eMonth = EMonth::August;
		break;
	case 9:
		eMonth = EMonth::September;
		break;
	case 10:
		eMonth = EMonth::October;
		break;
	case 11:
		eMonth = EMonth::November;
		break;
	case 12:
		eMonth = EMonth::December;
		break;
	default:
		break;
	}

	return eMonth;
}

QString CDBManager::GetMonthLongNameByMonthNumber(int nMonth)
{
	EMonth eMonth = GetMontByMonthNumber(nMonth);
	return GetMonthLongNameByMonth(eMonth);
}

QString CDBManager::GetMonthLongNameByMonth(EMonth eMonth)
{
	QString strMonthLongName = "";

	switch (eMonth)
	{
	case EMonth::January:
		strMonthLongName = SMonthLongName::January;
		break;
	case EMonth::February:
		strMonthLongName = SMonthLongName::February;
		break;
	case EMonth::March:
		strMonthLongName = SMonthLongName::March;
		break;
	case EMonth::April:
		strMonthLongName = SMonthLongName::April;
		break;
	case EMonth::May:
		strMonthLongName = SMonthLongName::May;
		break;
	case EMonth::June:
		strMonthLongName = SMonthLongName::June;
		break;
	case EMonth::July:
		strMonthLongName = SMonthLongName::July;
		break;
	case EMonth::August:
		strMonthLongName = SMonthLongName::August;
		break;
	case EMonth::September:
		strMonthLongName = SMonthLongName::September;
		break;
	case EMonth::October:
		strMonthLongName = SMonthLongName::October;
		break;
	case EMonth::November:
		strMonthLongName = SMonthLongName::November;
		break;
	case EMonth::December:
		strMonthLongName = SMonthLongName::December;
		break;
	case EMonth::Invalid:
	default:
		break;
	}

	return strMonthLongName;
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