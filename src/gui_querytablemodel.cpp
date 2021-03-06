////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_querytablemodel.h"

// Qt Includes
#include <QMessageBox>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace fm {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CStoreContentModel
//

// Constructors
CStoreContentModel::CStoreContentModel( QObject *pParent )
	: Base( pParent ),
	  m_pDBManager(nullptr)
{
	// TODO ...

	// Configure Model
	
	// TODO
}

// setActiveStore
void CStoreContentModel::setActiveStore( QString const& sStoreName )
{
	setQuery( QString( 
		"SELECT product.name, storage_info.count FROM "
		"storage_name INNER JOIN storage_info INNER JOIN product "
		"ON "
		"storage_name.id == storage_info.storage_id "
		"AND storage_name.name == \"%1\" "
		"AND product.id == storage_info.product_id" ).arg( sStoreName ) );
	
	setHeaderData( 0, Qt::Horizontal, QString::fromUtf8( "\325\217\325\245\325\275\325\241\325\257\325\250" ) ); // Type
	setHeaderData( 1, Qt::Horizontal, QString::fromUtf8( "\325\224\325\241\325\266\325\241\325\257\325\250" ) ); // Count
	// Check Result TODO
	QSqlError oErr = lastError();
	if (oErr.isValid())
	{
		QString sErrMsg = oErr.driverText();
		int i = 8;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace fm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
