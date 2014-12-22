#include "ItemFactory.h"

#include "model/Item.h"

class ItemFactory::Pimpl
{
public:

  Pimpl() {}

  /* data */
  //std::string id;
};

ItemFactory::ItemFactory(void)
:_pimpl(new Pimpl()) {}

ItemFactory::~ItemFactory(void) {}

boost::shared_ptr<Item> ItemFactory::createItem(const std::string& line) {
  return boost::shared_ptr<Item>();
}