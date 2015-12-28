#ifndef DB_CUSTOMERS_DATA_H
#define DB_CUSTOMERS_DATA_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef DB_MANAGER_H
#   include "db_manager.h"
#endif

// Qt Includes
#include <QList>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CCustomersData
//
class CCustomersData : public CDBComponent
{
	Q_OBJECT
public:// Data Property
	struct table
	{
		// tables name
		static const QString customer;

		// table customer
		struct customer
		{
			static const QString id;
			static const QString first_name;
			static const QString last_name;
			static const QString debt;
			static const QString phone_number;
		};
	};

public:// Constructors
	inline CCustomersData(QObject* pParent = nullptr, std::shared_ptr<CDBManager> pDBManager = nullptr);
	~CCustomersData() = default;

public:// Interface Methodes
	void Initialize();

	bool SetParent(QObject* pParent);
	void SetDBManager(std::shared_ptr<CDBManager> pDBManager);

	inline int GetColumnCount() const;
	inline int GetCustomersCount() const;

	QList<QString> GetCoulmnsName() const;
	QList<QString> GetCustomersName() const;
	inline std::shared_ptr<QSqlTableModel> GetSqlTableModel();
	inline std::shared_ptr<QSqlTableModel> GetSqlTableModel() const;

	void AddCustomer(QString const& strFirstName, QString const& strLastName, int nDept, QString strPhoneNumber);
	void RemoveCustomer(QString const& strFirstName, QString const& strLastName);
	void RemoveCustomer(int nRow);

	void UpdateCustomerDebt(QString const& strFirstName, QString const& strLastName, double dDebt);

	int GetCustomerId(QString const& strFirstName, QString const& strLastName);

	QString GetNameByIndex(int nColumn, int nRow) const;
	void SetNameByIndex(int nColumn, int nRow, QString const& strName);

protected:// Helper Methodes
	void UpdateSqlTableModel();

signals:
	void sigChangeData();

private:// Members
	QObject* m_pParentObject;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CCustomersData
//

// Constructors
inline CCustomersData::CCustomersData(QObject* pParent, std::shared_ptr<CDBManager> pDBManager)
	: CDBComponent(pDBManager),
	  m_pParentObject(pParent)
{
}

// Interface Methodes
inline int CCustomersData::GetColumnCount() const
{
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::customer);
	if (pSqlTableModel == nullptr)
		return -1;

	return pSqlTableModel->columnCount();
}

inline int CCustomersData::GetCustomersCount() const
{
	std::shared_ptr<QSqlTableModel> pSqlTableModel = CDBComponent::GetSqlTableModel(table::customer);
	if (pSqlTableModel == nullptr)
		return -1;

	return pSqlTableModel->rowCount();
}

inline std::shared_ptr<QSqlTableModel> CCustomersData::GetSqlTableModel()
{
	return CDBComponent::GetSqlTableModel(table::customer);
}

inline std::shared_ptr<QSqlTableModel> CCustomersData::GetSqlTableModel() const
{
	return CDBComponent::GetSqlTableModel(table::customer);
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // DB_CUSTOMERS_DATA_H