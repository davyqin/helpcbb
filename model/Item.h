#pragma once
#include <memory>
#include <string>

class Item
{
public:
  Item(void);
  ~Item(void);

  std::string id() const;

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};

