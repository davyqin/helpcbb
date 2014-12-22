
#include <QApplication>
#include <QTextCodec>

#include "gui/Controller.h"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  //QTextCodec *codec = QTextCodec::codecForName("GB2312");
  //QTextCodec::setCodecForLocale(codec);
  
  Controller controller;
  controller.activate();

  return a.exec();
}
