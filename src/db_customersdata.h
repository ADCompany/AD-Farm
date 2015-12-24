#ifndef DB_CUSTOMERS_DATA_H
#define DB_CUSTOMERS_DATA_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#ifndef DB_COMPONENT_H
#   include "db_component.h"
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
class CCustomersData : public IDBComponent
{
public:// Constructors
	inline CCustomersData();
	~CCustomersData() = default;

public:// Interface Methodes
	void Initialize();

	int GetColumnCount() const;
	int GetRowCount() const;

	QList<QString> GetCoulmnsName() const;

	int GetId(QString const& strFirstName, QString const& strLastName)
	{
		QSqlQuery sqlQuery;
		sqlQuery.prepare("SELECT id FROM customer WHERE first_name = ?");
		sqlQuery.bindValue(1, strFirstName);
		sqlQuery.exec();

		QString sRowCount = sqlQuery.value(0).toString();
		bool bOk = false;
		uint unId = sRowCount.toInt(&bOk);
		return unId;
	}

	QString GetNameByIndex(int nColumn, int nRow) const;
	void SetNameByIndex(int nColumn, int nRow, QString const& strName);

protected:// Helper Methodes


private:// Members
	int m_nColumnCount;
	int m_nRowCount;
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
// class CCustomersData
//

// Constructors
inline CCustomersData::CCustomersData()
	: m_nColumnCount(0),
	  m_nRowCount(0)
{
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////

#endif // DB_CUSTOMERS_DATA_H