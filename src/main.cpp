#include "gui_farmmanagerwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	fm::gui::CFarmManagerWidget w;
	w.show();
	return a.exec();
}
