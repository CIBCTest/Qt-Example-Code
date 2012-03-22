#include <QApplication>
#include "DiagramWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	DiagramWindow window;
	window.show();
	return app.exec();
}