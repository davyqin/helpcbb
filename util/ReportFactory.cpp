#include "ReportFactory.h"

#include "ItemFactory.h"
#include "model/Report.h"
#include "model/Item.h"

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

boost::shared_ptr<Report> ReportFactory::createReport(const std::string& csvFile, Report::ReportType reportType) {
  const boost::filesystem::path sourceFolder(csvFile);
  if (!boost::filesystem::exists(sourceFolder)) {
	  std::cout << "CSV File "<< csvFile << " does not exist! " << std::endl;
    throw std::logic_error("CSV File " + csvFile + " does not exist!");
	  return boost::shared_ptr<Report>();
  }

  std::ifstream filein(csvFile);
  std::string itemLine;
  ItemFactory itemFacory;
  std::vector<boost::shared_ptr<Item> > items;
  boost::shared_ptr<Item> newItem;
  while (getline(filein, itemLine)) {
    newItem.reset();
    if (reportType == Report::STANDARD) {
      newItem = itemFacory.createStandardItem(itemLine);
    }

    if (reportType == Report::DREPORT) {
      newItem = itemFacory.createDItem(itemLine);
    }

    if (reportType == Report::LOCAL) {
      newItem = itemFacory.createLocalItem(itemLine);
    }

    if (newItem->status() != Item::SKIP && newItem->id() != "999999999") {
      auto iter = std::find_if(items.begin(), items.end(),
        [&](boost::shared_ptr<Item> item) {
        return (item->id() == newItem->id() && item->status() != Item::SKIP);
      });

      if (iter != items.end()) {
        throw std::logic_error("Found duplicated id: " + newItem->id());
      }
    }

    items.push_back(newItem);
  }

  return boost::shared_ptr<Report>(new Report(items));
}
