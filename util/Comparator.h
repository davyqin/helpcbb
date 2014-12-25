#pragma once

#include <memory>
#include <vector>
#include <boost/shared_ptr.hpp>

class Report;
class Item;

class Comparator
{
public:
  Comparator(boost::shared_ptr<Report> standardReport);

  ~Comparator(void);

  std::vector<boost::shared_ptr<Item> > compareDReport(boost::shared_ptr<Report> dReport) const;

  std::vector<boost::shared_ptr<Item> > compareLocal(boost::shared_ptr<Report> localReport) const;

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};

