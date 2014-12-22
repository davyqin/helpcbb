#include "ItemFactory.h"

#include "model/Item.h"

#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>

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

boost::shared_ptr<Item> ItemFactory::createStandardItem(const std::string& line) {
  if (line.empty()) {
    return boost::shared_ptr<Item>();
  }

  std::vector<std::string> contents;
  boost::algorithm::split(contents, line, boost::algorithm::is_any_of(","));
  if (contents.size() != 5) {
    std::cout << "GK record is not in correct format." << std::endl;
    return boost::shared_ptr<Item>();
  }

  boost::shared_ptr<Item> item(new Item(contents.at(0), contents.at(1)));
  item->setCenter(contents.at(2));
  item->setCity(contents.at(2));
  item->setLocal(contents.at(3));

  return item;
}