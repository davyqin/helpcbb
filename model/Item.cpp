#include "Item.h"

class Item::Pimpl
{
public:

  Pimpl(const std::string& id, const std::string& title) 
  : id(id), fakeId(id), title(title), status(UNKNOWN) {}

  /* data */
  std::string id;
  std::string fakeId;
  std::string title;
  std::string center;
  std::string city;
  std::string local;
  ItemStatus status;
  std::vector<boost::shared_ptr<Item> > combinedItems;
};

Item::Item(const std::string& id, const std::string& title)
:_pimpl(new Pimpl(id, title)) {}

Item::~Item(void) {}

void Item::setFakeId(const std::string& value) {
  _pimpl->fakeId = value;
}

void Item::setCenter(const std::string value) {
  _pimpl->center = value;
}

void Item::setCity(const std::string value) {
  _pimpl->city = value;
}

void Item::setLocal(const std::string value) {
  _pimpl->local = value;
}

void Item::setStatus(ItemStatus status) {
  _pimpl->status = status;
}

void Item::combineItem(boost::shared_ptr<Item> item) {
  _pimpl->combinedItems.push_back(item);
}

std::string Item::id() const {
  return _pimpl->id;
}

std::string Item::fakeId() const {
  return _pimpl->fakeId;
}

std::string Item::title() const {
  return _pimpl->title;
}

std::string Item::center() const {
  return _pimpl->center;
}
std::string Item::city() const {
  return _pimpl->city;
}

std::string Item::local() const {
  return _pimpl->local;
}

Item::ItemStatus Item::status() const {
  return _pimpl->status;
}

std::vector<boost::shared_ptr<Item> > 
Item::combinedItems() const {
  return _pimpl->combinedItems;
}
