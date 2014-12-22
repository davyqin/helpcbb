#pragma once

#include <QObject>
#include <memory>

class Controller : public QObject
{
  Q_OBJECT

public:
  explicit Controller(QObject *parent = 0);

  ~Controller();

  void activate();

private slots:
  void onWork();

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};

