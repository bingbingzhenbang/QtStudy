#include <QtWidgets/QApplication>
#include "quaternionconvertor.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QuaternionConvertor q1;
	q1.show();
	return a.exec();
}
