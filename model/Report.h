#pragma once

#include <memory>
#include <vector>
#include <boost/shared_ptr.hpp>

class Item;

class Report
{
public:
  enum ReportType {
    STANDARD,
    CITY,
    LOCAL
  };

  Report(void);

  Report(const std::vector<boost::shared_ptr<Item> >& items);

  ~Report(void);

  boost::shared_ptr<Item> fetchItem(int index) const;

  std::vector<boost::shared_ptr<Item> > allItems() const;

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};

