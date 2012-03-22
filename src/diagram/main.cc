#include <QApplication>
#include "DiagramWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	DiagramWindow* window = new DiagramWindow;
	window->show();
	return app.exec();
}