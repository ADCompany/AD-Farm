////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "fm_datamanager.h"

// Qt includes
#include <QDesktopServices>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CDataManager
//

const QLatin1String CDataManager::m_csDataFilePath = QLatin1String("data/");
const QLatin1String CDataManager::m_csDataFileName = QLatin1String("data.sqlite");

// Helper Methodes
void CDataManager::Initialize()
{
	bool bCreateTables = false;

	QDir dirDataFile;
	QString strFilePath = m_csDataFilePath + m_csDataFileName;
	if (!dirDataFile.exists(strFilePath))
	{
		if (!dirDataFile.mkpath(m_csDataFilePath))
			throw CException(qtr("Unable to create path"));
		bCreateTables = true;
	}

	// Create SQlite connection
	m_sqlDataBase = QSqlDatabase::addDatabase("QSQLITE");
	m_sqlDataBase.setDatabaseName(strFilePath);
	if (!m_sqlDataBase.open())
	{
		QString sErrMsg = m_sqlDataBase.lastError().driverText();
		throw CException(qtr("Unable to establish database connection: ").append(sErrMsg));
	}

	// Create DB Infrastructure if necessary
	if (bCreateTables)
		CreateDBInfrastructure();
}

void CDataManager::CreateDBInfrastructure()
{
	QSqlQuery sqlQuery;
	// Create Table user_info
	EXECUTE_QUERY(sqlQuery, "CREATE TABLE user_info ("
		"user_id    INTEGER PRIMARY KEY NOT NULL, "
		"first_name TEXT    NOT NULL, "
		"last_name  TEXT    NOT NULL, "
		"birthday   DATE    NOT NULL, "
		"gender     INTEGER NOT NULL, "
		"avatar     BLOB);");

	// Create Auth Table
	EXECUTE_QUERY(sqlQuery, "CREATE TABLE auth_info ("
		"user_id  INTEGER PRIMARY KEY AUTOINCREMENT,"
		"username TEXT    NOT NULL UNIQUE, "
		"password INTEGER NOT NULL);"
		);

	// Create Friendship Table
	EXECUTE_QUERY(sqlQuery, "CREATE TABLE friendship ("
		"friendship_id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"user_id       INTEGER NOT NULL, "
		"friend_id     INTEGER NOT NULL);");

	// Create Conversation Table
	EXECUTE_QUERY(sqlQuery, "CREATE TABLE conversation ("
		"conv_id    INTEGER PRIMARY KEY AUTOINCREMENT, "
		"table_name TEXT NOT NULL);");

	// Create Conversation Info Table
	EXECUTE_QUERY(sqlQuery, "CREATE TABLE conversation_info ("
		"user_id INTEGER NOT NULL, "
		"conv_id INTEGER NOT NULL);");

	// Create Messages Table
	EXECUTE_QUERY(sqlQuery, "CREATE TABLE messages ("
		"conv_id   INTEGER  NOT NULL, "
		"user_id   INTEGER  NOT NULL, "
		"message   TEXT     NOT NULL, "
		"send_time DATETIME );");
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////