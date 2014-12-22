#pragma once
#include <memory>
#include <string>
#include <vector>

class Item
{
public:
  Item(const std::string& id, const std::string& title);
  ~Item(void);

  void setCenter(const std::string value);
  void setCity(const std::string value);
  void setLocal(const std::string value);

  std::string id() const;
  std::string title() const;
  std::string center() const;
  std::string city() const;
  std::string local() const;

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};

