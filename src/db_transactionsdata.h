#ifndef DB_TRANSACTIONS_DATA_H
#define DB_TRANSACTIONS_DATA_H

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
// class CTransactionsData
//
class CTransactionsData : public CDBComponent
{
	Q_OBJECT
public:// Data Property
	struct table
	{
		// tables name
		static const QString deal;
		static const QString transaction_info;

		// table customer
		struct transaction_info
		{
			static const QString transaction_id;
			static const QString product_id;
			static const QString count;
			static const QString cost;
		};
		struct deal
		{
			static const QString id;
			static const QString cost;
			static const QString customer_id;
			static const QString customer_cost;
			static const QString date_time;
		};
	};

public:// Constructors
	inline CTransactionsData(QObject* pParent = nullptr, std::shared_ptr<CDBManager> pDBManager = nullptr);
	~CTransactionsData() = default;

public:// Interface Methodes
	void Initialize();

	bool SetParent(QObject* pParent);
	void SetDBManager(std::shared_ptr<CDBManager> pDBManager);

	inline int GetColumnCount() const;

	void AddTransactionData(QString const& strCustomerName, QList<QString> const& strProductName, QList<int> nCount, QList<double> dCost, double dDealCost, double dCustomerCost);

	std::shared_ptr<QSqlQueryModel> GetSqlTableModelByCustomerName(QString const& strCustomerName);

protected:// Helper Methodes
	void UpdateSqlTableModel();
	void UpdateSqlTableModel(QString const& strCustomerName);
	void RemoveSqlTableModel(QString const& strCustomerName);

signals:
	void sigChangeData();

private:// Members
	std::map< QString, std::shared_ptr<QSqlQueryModel> > m_mapStringToModel;
	QObject* m_pParentObject;
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////// Implementing inline methods //////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// class CTransactionsData
//

// Constructors
inline CTransactionsData::CTransactionsData(QObject* pParent, std::shared_ptr<CDBManager> pDBManager)
	: CDBComponent(pDBManager),
	  m_pParentObject(pParent)
{
}

// Interface Methodes
inline int CTransactionsData::GetColumnCount() const
{

	return 0;
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // DB_TRANSACTIONS_DATA_H