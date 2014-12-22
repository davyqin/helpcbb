#include "Item.h"

class Item::Pimpl
{
public:

  Pimpl() {}

  /* data */
  std::string id;
};

Item::Item(void)
:_pimpl(new Pimpl())
{
}

Item::~Item(void)
{
}

std::string Item::id() const {
  return _pimpl->id;
}
