#include "Controller.h"
#include "WorkDialog.h"
#include "model/Report.h"
#include "util/ReportFactory.h"
#include "util/Comparator.h"

#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>

class Controller::Pimpl {
public:
  Pimpl() {}
  
  WorkDialog dialog;
  boost::shared_ptr<Report> standardReport;
  boost::shared_ptr<Report> dReport;
  boost::shared_ptr<Report> localReport;
};

Controller::Controller(QObject *parent)
:QObject(parent), _pimpl(new Pimpl())
{
  connect(&_pimpl->dialog, SIGNAL(workSignal()), SLOT(onWork()));
}

Controller::~Controller(void) {}

void Controller::activate() {
  _pimpl->dialog.show();
}

void Controller::onWork() {
  const std::vector<std::string> csvFiles = _pimpl->dialog.csvFiles();
  if (csvFiles.empty()) {
    std::cout<<"No csv files are selected."<<std::endl;
	  return;
  }

  ReportFactory reportFactory;
  _pimpl->standardReport = reportFactory.createReport(csvFiles.at(0), Report::STANDARD);
  _pimpl->dReport = reportFactory.createReport(csvFiles.at(1), Report::DREPORT);
  _pimpl->localReport = reportFactory.createReport(csvFiles.at(2), Report::LOCAL);

  Comparator comparator(_pimpl->standardReport);
  _pimpl->dialog.setDReportResult(comparator.compareDReport(_pimpl->dReport));
  _pimpl->dialog.setLocalReportResult(comparator.compareLocal(_pimpl->localReport));
}
