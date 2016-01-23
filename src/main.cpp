#include "gui_farmmanagerwidget.h"
#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include <QFile>
#include <QByteArray>
#include <QNetworkInterface>
////////////////////////////////////////////////////////////////////////////////
//
//	MAIN
//
int main(int argc, char *argv[])
{
	//QNetworkInterface netInterface;
	//QString strMacAddress = netInterface.name();
	//strMacAddress = netInterface.humanReadableName();
	//QList<QNetworkAddressEntry> lstNetAddress = netInterface.addressEntries();
	//while(strMacAddress.isEmpty())
	//{
	//	strMacAddress = netInterface.hardwareAddress();
	//}

	//QFile fileLicense("ADCompany.lcs");
	//QByteArray byteArray = fileLicense.readAll();
	try
	{
		QApplication oFarmManager( argc, argv );
		// Setup Application Details
		oFarmManager.setOrganizationName( "ADCompany" );
		oFarmManager.setApplicationName( "Farm Manager" );
		oFarmManager.setApplicationVersion( "1.0.0" );

		// Internationalization
		QTranslator oTranslator;
		oTranslator.load( "farmmanager_hy.qm" /*+ QLocale::system().name()*/ );
		oFarmManager.installTranslator( &oTranslator );


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