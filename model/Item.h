#pragma once

#include <memory>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

class Item
{
public:
  enum ItemStatus {
    UNKNOWN,
    SKIP,
    MATCHING,
    MISMATCHING,
    COMBINE_MATCHING,
    COMBINE_MISMATCHING,
    LOST
  };

  Item(const std::string& id, const std::string& title);
  ~Item(void);

  void setFakeId(const std::string& value);
  void setCenter(const std::string value);
  void setCity(const std::string value);
  void setLocal(const std::string value);
  void setStatus(ItemStatus status);
  void combineItem(boost::shared_ptr<Item> item);

  std::string id() const;
  std::string fakeId() const;
  std::string title() const;
  std::string center() const;
  std::string city() const;
  std::string local() const;
  Item::ItemStatus status() const;
  std::vector<boost::shared_ptr<Item> > combinedItems() const;

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};

