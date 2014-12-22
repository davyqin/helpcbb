#include "Item.h"

class Item::Pimpl
{
public:

  //Pimpl() {}

  Pimpl(const std::string& id, const std::string& title) 
  : id(id), title(title) {}

  /* data */
  std::string id;
  std::string title;
  std::string center;
  std::string city;
  std::string local;
};

//Item::Item(void)
//:_pimpl(new Pimpl())
//{
//}

Item::Item(const std::string& id, const std::string& title)
:_pimpl(new Pimpl(id, title)) {}

Item::~Item(void) {}

void Item::setCenter(const std::string value) {
  _pimpl->center = value;
}

void Item::setCity(const std::string value) {
  _pimpl->city = value;
}

void Item::setLocal(const std::string value) {
  _pimpl->local = value;
}

std::string Item::id() const {
  return _pimpl->id;
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

