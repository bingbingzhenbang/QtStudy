#include "controlstudy.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ControlStudy w;
	w.show();
	return a.exec();
}
