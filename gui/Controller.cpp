#include "Controller.h"
#include "WorkDialog.h"
#include "model/Report.h"
#include "util/ReportFactory.h"
#include "util/Comparator.h"

#include <QMessageBox>

#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>

class Controller::Pimpl {
public:
  Pimpl() {}
  
  WorkDialog dialog;
  boost::shared_ptr<Report> standardReport;
  boost::shared_ptr<Report> cReport;
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
  ReportFactory reportFactory;
  try {
    _pimpl->standardReport = reportFactory.createReport(csvFiles.at(0), Report::STANDARD);
    _pimpl->cReport = reportFactory.createReport(csvFiles.at(1), Report::DREPORT);
    _pimpl->dReport = reportFactory.createReport(csvFiles.at(2), Report::DREPORT);
    _pimpl->localReport = reportFactory.createReport(csvFiles.at(3), Report::LOCAL);
  }
  catch (const std::logic_error& e){
    QMessageBox::critical(&_pimpl->dialog, tr("Error"), tr(e.what()), QMessageBox::Ok);
    return;
  }

  Comparator comparator(_pimpl->standardReport);
  _pimpl->dialog.setCReportResult(comparator.compareCReport(_pimpl->cReport));
  _pimpl->dialog.setDReportResult(comparator.compareDReport(_pimpl->dReport));
  _pimpl->dialog.setLocalReportResult(comparator.compareLocal(_pimpl->localReport));
}
