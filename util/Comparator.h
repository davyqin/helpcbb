#pragma once

#include <memory>
#include <vector>
#include <boost/shared_ptr.hpp>

class Report;
class Item;

class Comparator
{
public:
  Comparator(boost::shared_ptr<Report> standardReport,
             boost::shared_ptr<Report> localReport);

  ~Comparator(void);

  std::vector<boost::shared_ptr<Item> > compareLocal() const;

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};

