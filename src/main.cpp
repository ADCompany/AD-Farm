#include "gui_farmmanagerwidget.h"
#include <QApplication>
#include <QMessageBox>
////////////////////////////////////////////////////////////////////////////////
//
//	MAIN
//
int main(int argc, char *argv[])
{
	try
	{
		QApplication oFarmManager( argc, argv );
		// Setup Application Details
		oFarmManager.setOrganizationName( "ADCompany" );
		oFarmManager.setApplicationName( "Farm Manager" );
		oFarmManager.setApplicationVersion( "1.0.0" );
		fm::gui::CFarmManagerWidget w;
		w.show();
		return oFarmManager.exec();
	}
	catch (std::exception& oStdExc)
	{
		QMessageBox::critical( 0, "Fatal Error", oStdExc.what() );
		qApp->quit();
	}
	catch (...)
	{
		QMessageBox::critical( 0, "Fatal Error", "Unhandled Exception!\nApplication will be closed." );
		qApp->quit();
	}
}
////////////////////////////////////////////////////////////////////////////////