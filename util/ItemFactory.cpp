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
    std::cout << "Standard record is not in correct format: " << line << std::endl;
    throw std::logic_error("Standard record is not in correct format: " + line);
    return boost::shared_ptr<Item>();
  }

  boost::shared_ptr<Item> item(new Item(contents.at(0), contents.at(1)));
  item->setCenter(boost::trim_copy(contents.at(2)));
  item->setCity(boost::trim_copy(contents.at(3)));
  item->setLocal(boost::trim_copy(contents.at(4)));

  return item;
}

boost::shared_ptr<Item> ItemFactory::createDItem(const std::string& line) {
  if (line.empty()) {
    return boost::shared_ptr<Item>();
  }

  std::vector<std::string> contents;
  boost::algorithm::split(contents, line, boost::algorithm::is_any_of(","));
  if (contents.size() != 3) {
    std::cout << "D report record is not in correct format." << std::endl;
    return boost::shared_ptr<Item>();
  }

  std::string id = contents.at(0).empty() ? "999999999" : contents.at(0);
  const std::string title = boost::trim_copy_if(contents.at(1), boost::is_any_of("　 "));
  if (title == "国有工业企业所得税") {
    id = "1010404";
  }
  boost::shared_ptr<Item> item(new Item(id, title));
  std::string value = boost::trim_copy(contents.at(2));
  if (value.empty() || value == "0.00") {
    value = "0.00"; 
    item->setStatus(Item::SKIP);
  }

  item->setLocal(value);

  //if (id.size() < 5) {
  //  item->setStatus(Item::SKIP);
  //}

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
  std::string value = boost::trim_copy(contents.at(2));
  if (value.empty()) {
    value = "0.00";
    item->setStatus(Item::SKIP);
  }

  item->setLocal(value);

  // The fakeId of local item is id without first 3 numbers
  std::string fakeId = contents.at(0);
  item->setFakeId(fakeId.erase(0, 3));

  // Skip id like "40510301480100"
  if (item->id().size() > 12) item->setStatus(Item::SKIP);

  return item;
}