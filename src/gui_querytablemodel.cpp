////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_querytablemodel.h"

// Qt Includes
#include <QMessageBox>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// class CStoreContentModel
//

// Constructors
CStoreContentModel::CStoreContentModel( QObject *pParent )
	: Base( pParent )
{
	// TODO ...

	// Configure Model
	setHeaderData( 0, Qt::Horizontal, tr( "Type" ) );
	setHeaderData( 1, Qt::Horizontal, tr( "Count" ) );
	// TODO
}

// setActiveStore
void CStoreContentModel::setActiveStore( QString const& sStoreName )
{
	setQuery( QString( 
		"SELECT producte.name, count FROM "
		"storage_name INNER JOIN storage_info INNER JOIN producte "
		"ON "
		"storage_name.id == storage_info.storage_id "
		"AND storage_name.name == '%1' "
		"AND producte.id == storage_info.product_id" ).arg( sStoreName ) );
	
	setHeaderData( 0, Qt::Horizontal, tr( "Type" ) );
	setHeaderData( 1, Qt::Horizontal, tr( "Count" ) );
	// Check Result TODO
	QSqlError oErr = lastError();
	if (oErr.isValid())
	{
		QString sErrMsg = oErr.driverText();
		int i = 8;
	}
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////
