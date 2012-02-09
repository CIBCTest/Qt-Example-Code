#include <QApplication>
#include <QDialog>

#include "gotocelldialog.h"

int main_goto(int argc, char *argv[])
{
	QApplication app(argc, argv);
	GoToCellDialog* dialog = new GoToCellDialog;
	dialog->show();
	return app.exec();
}