#pragma once

#include <QDialog>
#include <memory>
#include <string>

class WorkDialog : public QDialog
{
  Q_OBJECT

public:
  WorkDialog(QWidget* parent = 0);
  ~WorkDialog(void);

  std::vector<std::string> csvFiles() const;
  void log(std::string message);

signals:
  void workSignal();

private slots:
  void onBrowseFolder();

private:
  void log(QString message);

  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};
