#pragma once

#include <QDialog>

#include "model/Item.h"

#include <memory>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

class WorkDialog : public QDialog
{
  Q_OBJECT

public:
  WorkDialog(QWidget* parent = 0);
  ~WorkDialog(void);

  std::vector<std::string> csvFiles() const;
  void setDReportResult(const std::vector<boost::shared_ptr<Item> >& items);
  void setLocalReportResult(const std::vector<boost::shared_ptr<Item> >& items);

signals:
  void workSignal();

private slots:
  void onBrowseFolder();

private:
  void log(QString message);

  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};
