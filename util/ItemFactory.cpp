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
    std::cout << "Standard record is not in correct format." << std::endl;
    return boost::shared_ptr<Item>();
  }

  boost::shared_ptr<Item> item(new Item(contents.at(0), contents.at(1)));
  item->setCenter(boost::trim_copy(contents.at(2)));
  item->setCity(boost::trim_copy(contents.at(3)));
  item->setLocal(boost::trim_copy(contents.at(4)));

  return item;
}

boost::shared_ptr<Item> ItemFactory::createLocalItem(const std::string& line) {
  if (line.empty()) {
    return boost::shared_ptr<Item>();
  }

  std::vector<std::string> contents;
  boost::algorithm::split(contents, line, boost::algorithm::is_any_of(","));
  if (contents.size() != 3) {
    std::cout << "Local record is not in correct format." << std::endl;
    return boost::shared_ptr<Item>();
  }

  boost::shared_ptr<Item> item(new Item(contents.at(0), contents.at(1)));
  item->setLocal(boost::trim_copy(contents.at(2)));

  // The fakeId of local item is id without first 3 numbers
  std::string fakeId = contents.at(0);
  item->setFakeId(fakeId.erase(0, 3));

  return item;
}