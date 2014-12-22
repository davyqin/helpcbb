#pragma once

#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>

class Report;

class ReportFactory
{
public:
  ReportFactory(void);
  ~ReportFactory(void);

  boost::shared_ptr<Report> createReport(const std::string& cvsFile);

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};

