#include "Comparator.h"

#include "model/Item.h"
#include "model/Report.h"

#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace {
  const double IGNOREVALUE = 0.000001;
}

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

  // 1. Perfectly matching
  for (auto item : localItems) {
    if (item->fakeId().size() < 7) {
      item->setStatus(Item::SKIP);
      continue;
    }

    auto iter = std::find_if(standardItems.begin(), standardItems.end(), 
                             [&](boost::shared_ptr<Item> standardItem) {
                               if (standardItem->status() == Item::MATCHING) return false;
                               return (item->fakeId() == standardItem->id() &&
                                       item->local() == standardItem->local());
                             });

    if (iter != standardItems.end()) {
      item->setStatus(Item::MATCHING);
      (*iter)->setStatus(Item::MATCHING);
    }
  }

  // 2. Partly matching
  for (auto item : localItems) {
    if (item->status() == Item::SKIP || item->status() == Item::MATCHING) continue;

    auto iter = std::find_if(standardItems.begin(), standardItems.end(), 
      [&](boost::shared_ptr<Item> standardItem) {
        if (standardItem->status() == Item::MATCHING) return false;
        return (item->fakeId() == standardItem->id());
    });

    if (iter == standardItems.end()) {
      item->setStatus(Item::LOST);
      continue;
    }
    
    (*iter)->setStatus(Item::MATCHING);
    const long double standardLocal = boost::lexical_cast<long double>((*iter)->local());
    const long double realLocal = boost::lexical_cast<long double>(item->local());
    const long double difference = standardLocal - realLocal;

    if (difference < IGNOREVALUE) {
      item->setStatus(Item::MATCHING);
    }
    else {
      item->setStatus(Item::TITLE_MATCHING);
      boost::format fmt("%.2f"); 
      item->setLocal(boost::lexical_cast<std::string>(fmt % difference));
    }
  }

  // 3. Re-verify LOST items

  // 4. Mark standard items
  for (auto item : standardItems) {
    if (item->status() != Item::MATCHING && item->local() != "0.00") {
      item->setStatus(Item::LOST);
    }
  }

  std::vector<boost::shared_ptr<Item> > result = standardItems;
  result.insert(result.end(), localItems.begin(), localItems.end());
  return result;
}
