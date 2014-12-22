#include "WorkDialog.h"
#include "ui_mainDialog.h"

#include <QTextCodec>
#include <QFileDialog>
#include <iostream>

class WorkDialog::Pimpl
{
public:
  Pimpl() {}

  /* data */
  Ui::mainDialog ui;
  QString gkCSV;
  QString xianCSV;
};


WorkDialog::WorkDialog(QWidget* parent)
:QDialog(parent), _pimpl(new Pimpl())
{
  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForLocale(codec);
  _pimpl->ui.setupUi(this);

  setWindowTitle(tr("Help CBB"));

  connect(_pimpl->ui.browseButton, SIGNAL(clicked()), SLOT(onBrowseFolder()));
  connect(_pimpl->ui.okButton, SIGNAL(clicked()), SIGNAL(workSignal()));
}

WorkDialog::~WorkDialog(void) {}

void WorkDialog::log(std::string message) {
  _pimpl->ui.resultEdit->append(QString::fromUtf8(message.c_str()));
  std::cout<<message<<std::endl;
}

std::vector<std::string> WorkDialog::csvFiles() const {
  std::vector<std::string> files;
  files.push_back(_pimpl->gkCSV.toStdString());
  files.push_back(_pimpl->xianCSV.toStdString());
  return files;
}

void WorkDialog::onBrowseFolder() {
  QFileDialog browser(this, tr("Select Image Stack Directory"));
  browser.setFileMode(QFileDialog::ExistingFiles);
  browser.setAcceptMode(QFileDialog::AcceptOpen);
  browser.setNameFilter(tr("CSV (*.csv)"));
  if(browser.exec() == QFileDialog::Accepted) {
    _pimpl->gkCSV = browser.selectedFiles().at(0);
    log(QString("Select GK file: %1").arg(_pimpl->gkCSV));
  }
}

void WorkDialog::log(QString message) {
  _pimpl->ui.resultEdit->append(message);
  std::cout<<message.toStdString()<<std::endl;
}

void WorkDialog::log(const std::vector<boost::shared_ptr<Item> >& items) {
  QString msgTemplate = QString::fromUtf8("%1 %2 %3 %4 %5");
  QTextCodec *codec = QTextCodec::codecForName("GB2312"); 
  for (auto item : items) {
    const QString id = QString::fromUtf8(item->id().c_str());
    const QString title = codec->toUnicode(item->title().c_str()); //QString::fromUtf8(item->title().c_str());
    const QString center = QString::fromUtf8(item->center().c_str());
    const QString city = QString::fromUtf8(item->city().c_str());
    const QString local = QString::fromUtf8(item->local().c_str());
    const QString message = msgTemplate.arg(id).arg(title).arg(center).arg(city).arg(local);
    _pimpl->ui.resultEdit->append(message);
  }
}