#include "fm_farmmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	fm::CFarmManager w;
	w.show();
	return a.exec();
}
