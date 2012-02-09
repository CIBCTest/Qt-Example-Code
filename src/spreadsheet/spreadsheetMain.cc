#include <QApplication>
#include "mainwindow.h"

#ifdef _WINDOWS
#define main WinMain
#endif

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainWindow* mainWin = new MainWindow;
	mainWin->show();
	return app.exec();
}
