#include "Comparator.h"

#include "model/Item.h"
#include "model/Report.h"

#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

namespace {
  const long double EPSILON = 0.0000001;
}

class Comparator::Pimpl
{
public:

  Pimpl(boost::shared_ptr<Report> standardReport)
  :standardReport(standardReport) {}

  /* data */
  boost::shared_ptr<Report> standardReport;
  //boost::shared_ptr<Report> localReport;
};

Comparator::Comparator(boost::shared_ptr<Report> standardReport)
:_pimpl(new Pimpl(standardReport)) {}

Comparator::~Comparator(void) {}

std::vector<boost::shared_ptr<Item> > 
Comparator::compareDReport(boost::shared_ptr<Report> dReport) const {
  std::vector<boost::shared_ptr<Item> > standardItems = _pimpl->standardReport->allItems();
  std::vector<boost::shared_ptr<Item> > localItems = dReport->allItems();

  // 1. Perfectly matching
  for (auto item : localItems) {
    if (item->id().size() < 5) {
      item->setStatus(Item::SKIP);
      continue;
    }

    auto iter = std::find_if(standardItems.begin(), standardItems.end(),
      [&](boost::shared_ptr<Item> standardItem) {
      if (standardItem->status() == Item::MATCHING) {
        return false;
      }

      return (item->id() == standardItem->id() &&
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
      return (item->id() == standardItem->id());
    });

    if (iter == standardItems.end()) {
      item->setStatus(Item::LOST);
      continue;
    }

    (*iter)->setStatus(Item::MATCHING);
    const long double standardLocal = boost::lexical_cast<long double>((*iter)->local());
    const long double realLocal = boost::lexical_cast<long double>(item->local());
    const long double difference = standardLocal - realLocal;

    if (abs(difference) < EPSILON) {
      item->setStatus(Item::MATCHING);
    }
    else {
      item->setStatus(Item::MISMATCHING);
      boost::format fmt("%.2f");
      item->setLocal(boost::lexical_cast<std::string>(fmt % difference));
    }
  }

  // 3. Re-verify LOST items
  for (auto localItem : localItems) {
    if (localItem->status() != Item::LOST) {
      continue;
    }
    const std::string id = localItem->id();
    long double localValue = boost::lexical_cast<long double>(localItem->local());
    for (auto standardItem : standardItems) {
      if (boost::algorithm::starts_with(standardItem->id(), id)) {
        localValue -= boost::lexical_cast<long double>(standardItem->local());
        localItem->combineItem(standardItem);
      }
    }

    if (!localItem->combinedItems().empty() && (abs(localValue) < EPSILON)) {
      localItem->setStatus(Item::COMBINE_MATCHING);
      for (auto combineItem : localItem->combinedItems()) {
        combineItem->setStatus(Item::MATCHING);
      }
    }

    if (!localItem->combinedItems().empty() && (abs(localValue) > EPSILON)) {
      localItem->setStatus(Item::COMBINE_MISMATCHING);
      for (auto combineItem : localItem->combinedItems()) {
        combineItem->setStatus(Item::MISMATCHING);
      }
    }
  }

  // 4. Handle "999999999" items
  for (unsigned int i = 0; i < localItems.size(); ++i) {
    boost::shared_ptr<Item> localItem = localItems.at(i);
    if (localItem->status() == Item::SKIP || localItem->id() != "999999999") {
      continue;
    }
    
    const std::string id = localItems.at(i - 1)->id().substr(0, 7);
    long double localValue = boost::lexical_cast<long double>(localItem->local());

    for (auto standardItem : standardItems) {
      if (boost::algorithm::starts_with(standardItem->id(), id) && 
          (standardItem->status() != Item::MATCHING)) {
        localValue -= boost::lexical_cast<long double>(standardItem->local());
        localItem->combineItem(standardItem);
      }
    }

    if (!localItem->combinedItems().empty() && (abs(localValue) < EPSILON)) {
      localItem->setStatus(Item::COMBINE_MATCHING);
      for (auto combineItem : localItem->combinedItems()) {
        combineItem->setStatus(Item::MATCHING);
      }
    }

    if (!localItem->combinedItems().empty() && (abs(localValue) > EPSILON)) {
      localItem->setStatus(Item::COMBINE_MISMATCHING);
      for (auto combineItem : localItem->combinedItems()) {
        combineItem->setStatus(Item::MISMATCHING);
      }
    }
  }

  // 5. Mark standard items
  for (auto item : standardItems) {
    //if (item->status() != Item::MATCHING && item->status() != Item::COMBINE_MISMATCHING && item->local() != "0.00") {
    if (item->status() == Item::UNKNOWN && item->local() != "0.00") {
      item->setStatus(Item::LOST);
    }
  }

  std::vector<boost::shared_ptr<Item> > result = standardItems;
  result.insert(result.end(), localItems.begin(), localItems.end());
  return result;
}

std::vector<boost::shared_ptr<Item> > 
Comparator::compareLocal(boost::shared_ptr<Report> localReport) const {
  std::vector<boost::shared_ptr<Item> > standardItems = _pimpl->standardReport->allItems();
  std::vector<boost::shared_ptr<Item> > localItems = localReport->allItems();

  // 1. Perfectly matching
  for (auto item : localItems) {
    if (item->fakeId().size() < 7) {
      item->setStatus(Item::SKIP);
      continue;
    }

    auto iter = std::find_if(standardItems.begin(), standardItems.end(), 
      [&](boost::shared_ptr<Item> standardItem) {
        if (standardItem->status() == Item::MATCHING) {
          return false;
        }
        
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

    if (abs(difference) < EPSILON) {
      item->setStatus(Item::MATCHING);
    }
    else {
      item->setStatus(Item::MISMATCHING);
      boost::format fmt("%.2f"); 
      item->setLocal(boost::lexical_cast<std::string>(fmt % difference));
    }
  }

  // 3. Re-verify LOST items
  for (auto localItem : localItems) {
    if (localItem->status() != Item::LOST) continue;
    const std::string fakeId = localItem->fakeId();
    long double localValue = boost::lexical_cast<long double>(localItem->local());
    for (auto standardItem : standardItems) {
      if (boost::algorithm::starts_with(standardItem->id(), fakeId)) {
        localValue -= boost::lexical_cast<long double>(standardItem->local());
        localItem->combineItem(standardItem);
      }
    }

    if (!localItem->combinedItems().empty() && (abs(localValue) < EPSILON)) {
      localItem->setStatus(Item::COMBINE_MATCHING);
    }

    if (!localItem->combinedItems().empty() && (abs(localValue) > EPSILON)) {
      localItem->setStatus(Item::COMBINE_MISMATCHING);
    }
  }

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
