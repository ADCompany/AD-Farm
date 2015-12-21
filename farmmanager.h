#ifndef FARMMANAGER_H
#define FARMMANAGER_H

#include <QMainWindow>
#include "ui_farmmanager.h"
#include "ui_addgorcarq.h"
#include "ui_addaraqichdialog.h"

class FarmManager : public QMainWindow
{
	Q_OBJECT

public:
	FarmManager(QWidget *parent = 0);
	~FarmManager();

private:
	Ui::FarmManagerClass ui;
	Ui::Dialog           dialogUi;
	Ui::AddSupplierDlg   addSupplierDlgUi;

	QDialog*             m_CreateTrnDlg;
	QDialog*             m_CreateSplDlg;
};

#endif // FARMMANAGER_H
