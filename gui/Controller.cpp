#include "Controller.h"
#include "WorkDialog.h"
#include "model/Report.h"

#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>

class Controller::Pimpl {
public:
  Pimpl() {}
  
  WorkDialog dialog;
  boost::shared_ptr<Report> gkReport;
};

Controller::Controller(QObject *parent)
:QObject(parent), _pimpl(new Pimpl()) {}

Controller::~Controller(void) {}

void Controller::activate() {
  _pimpl->dialog.exec();
}

void Controller::onWork() {
  const std::vector<std::string> csvFiles = _pimpl->dialog.csvFiles();
  if (csvFiles.empty()) {
    std::cout<<"No csv files are selected."<<std::endl;
  }
}
