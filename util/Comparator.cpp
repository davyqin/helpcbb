#include "Comparator.h"

#include "model/Item.h"
#include "model/Report.h"


#include <string>

class Comparator::Pimpl
{
public:

  Pimpl(boost::shared_ptr<Report> standardReport,
        boost::shared_ptr<Report> localReport)
  :standardReport(standardReport)
  ,localReport(localReport) {}

  /* data */
  boost::shared_ptr<Report> standardReport;
  boost::shared_ptr<Report> localReport;
};

Comparator::Comparator(boost::shared_ptr<Report> standardReport,
                       boost::shared_ptr<Report> localReport)
:_pimpl(new Pimpl(standardReport, localReport)) {}

Comparator::~Comparator(void) {}

std::vector<boost::shared_ptr<Item> > Comparator::compareLocal() const {
  std::vector<boost::shared_ptr<Item> > standardItems = _pimpl->standardReport->allItems();
  std::vector<boost::shared_ptr<Item> > localItems = _pimpl->localReport->allItems();

  std::vector<boost::shared_ptr<Item> > tempItems;
  for (auto item : localItems) {
    if (item->fakeId().size() < 7) continue;
    auto iter = std::find_if(standardItems.begin(), standardItems.end(), 
                             [&](boost::shared_ptr<Item> standardItem) {
                               return (item->fakeId() == standardItem->id() &&
                                       item->local() == standardItem->local());
                             });

    if (iter == standardItems.end()) (tempItems.push_back(item));
  }

  return tempItems;
}
