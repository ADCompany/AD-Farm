////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
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
// class CTransactionsData
//

// Interface Methodes
void CTransactionsData::Initialize()
{
	QSqlQuery sqlQuery;

	EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS transaction_info ("
		"transaction_id		INTEGER PRIMARY KEY NOT NULL, "
		"product_id			INTEGER NOT NULL, "
		"count				INTEGER NOT NULL, "
		"cost				INTEGER NOT NULL);");

	//EXECUTE_QUERY(sqlQuery, "CREATE TABLE IF NOT EXISTS transaction ("
	//	"id					INTEGER PRIMARY KEY NOT NULL, "
	//	"customer_id		INTEGER NOT NULL, "
	//	"date_time			TEXT    NOT NULL);");
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////