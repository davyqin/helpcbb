#include "ReportFactory.h"

#include "ItemFactory.h"
#include "model/Report.h"

#include <iostream>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


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
  const boost::filesystem::path sourceFolder(cvsFile);
  if (!boost::filesystem::exists(sourceFolder)) {
	  std::cout << "File "<< cvsFile << " does not exist! " << std::endl;
	  return boost::shared_ptr<Report>();
  }

  std::ifstream filein(cvsFile);
  std::string item;
  ItemFactory itemFacory;
  std::vector<boost::shared_ptr<Item> > items;
  while (getline(filein, item)) {
    items.push_back(itemFacory.createStandardItem(item));
  }

  return boost::shared_ptr<Report>(new Report(items));
}
