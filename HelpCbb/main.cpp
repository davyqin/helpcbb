
#include <QApplication>

#include "gui/Controller.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

  Controller controller;
  controller.activate();

	return a.exec();
}
