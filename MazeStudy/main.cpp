#include <QtWidgets/QApplication>
#include "mazedialog.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MazeDialog w;
	w.show();
	return a.exec();
}