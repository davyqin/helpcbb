#include "Report.h"

#include "Item.h"

class Report::Pimpl
{
public:

  Pimpl() {}

  Pimpl(const std::vector<boost::shared_ptr<Item> >& items)
  :items(items) {}

  /* data */
  std::vector<boost::shared_ptr<Item> > items;
};

Report::Report(void)
:_pimpl(new Pimpl()) {}

Report::Report(const std::vector<boost::shared_ptr<Item> >& items)
:_pimpl(new Pimpl(items)) {}

Report::~Report(void) {}

boost::shared_ptr<Item> Report::fetchItem(int index) const {
  return _pimpl->items.at(index);
}

std::vector<boost::shared_ptr<Item> > Report::allItems() const {
  return _pimpl->items;
}
