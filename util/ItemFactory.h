#pragma once

#include <memory>
#include <boost/shared_ptr.hpp>

class Item;

class ItemFactory
{
public:
  ItemFactory(void);
  ~ItemFactory(void);

  boost::shared_ptr<Item> createItem(const std::string& line);

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};

