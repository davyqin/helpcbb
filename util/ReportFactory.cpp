#include "ReportFactory.h"

#include "model/Report.h"

class ReportFactory::Pimpl
{
public:

  Pimpl() {}

  /* data */
};


ReportFactory::ReportFactory(void)
:_pimpl(new Pimpl()) {}

ReportFactory::~ReportFactory(void) {}

boost::shared_ptr<Report> ReportFactory::createReport(const std::string& cvsFile) {
  return boost::shared_ptr<Report>();
}
