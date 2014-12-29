#pragma once
#include "model/Report.h"

#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>

class ReportFactory
{
public:
  ReportFactory(void);
  ~ReportFactory(void);

  boost::shared_ptr<Report> createReport(const std::string& csvFile, Report::ReportType reportType);

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};

