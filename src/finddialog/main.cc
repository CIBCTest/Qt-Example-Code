#include <QApplication>
#include "FindDialog.h"

int main_find(int argc, char* argv[])
{
	QApplication app(argc, argv);
	FindDialog* dialog = new FindDialog;
	dialog->show();
	return app.exec();
}