#include "WorkDialog.h"
#include "ui_mainDialog.h"

#include <QTextCodec>
#include <QFileDialog>
#include <iostream>

namespace {
  const QString MESSAGE_TEMPLATE =  QString::fromUtf8("%1 %2-----> ");

  void printResult(QTextEdit* edit, const std::vector<boost::shared_ptr<Item> >& items) {
    edit->clear();
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    for (auto item : items) {
      const QString id = QString::fromUtf8(item->id().c_str());
      const QString title = codec->toUnicode(item->title().c_str());
      const QString local = QString::fromUtf8(item->local().c_str());
      const QString message = MESSAGE_TEMPLATE.arg(id).arg(title);
      QColor currentColor = edit->textColor();
    
#if 0
      if (item->status() == Item::MATCHING) {
        const QString itemStatus = codec->toUnicode("MACHING");
        const QString message = MESSAGE_TEMPLATE.arg(id).arg(title);
        _pimpl->ui.resultEdit->append(message);
        _pimpl->ui.resultEdit->setTextColor(Qt::green);
        _pimpl->ui.resultEdit->insertPlainText(itemStatus);
      }
#endif
    
      if (item->status() == Item::LOST) {
        const QString itemStatus = codec->toUnicode("LOST");
        edit->append(message);
        edit->setTextColor(Qt::red);
        edit->insertPlainText(itemStatus);
      }
    
      if (item->status() == Item::MISMATCHING) {
        const QString itemStatus = codec->toUnicode("MISMATCHING");
        edit->append(message);
        edit->setTextColor(Qt::red);
        edit->insertPlainText(local);
      }
    
#if 0
      if (item->status() == Item::SKIP) {
        const QString itemStatus = codec->toUnicode("SKIP");
        const QString message = QString::fromUtf8("%1 %2-----> ").arg(id).arg(title);
        _pimpl->ui.resultEdit->append(message);
        _pimpl->ui.resultEdit->setTextColor(Qt::green);
        _pimpl->ui.resultEdit->insertPlainText(itemStatus);
      }
#endif
    
#if 1
      if (item->status() == Item::COMBINE_MATCHING) {
        const QString itemStatus = codec->toUnicode("COMBINE MATCHING");
        edit->append(message);
        edit->setTextColor(Qt::blue);
        edit->insertPlainText(itemStatus);
    
        std::vector<boost::shared_ptr<Item> > combinedItems = item->combinedItems();
        for (auto combinedItem : combinedItems) {
          const QString combinedId = QString::fromUtf8(combinedItem->id().c_str());
          const QString combinedTitle = codec->toUnicode(combinedItem->title().c_str());
          const QString combineMessage = QString::fromUtf8("---->%1 %2").arg(combinedId).arg(combinedTitle);
          edit->append(combineMessage);
        }
      }
#endif
    
#if 1
      if (item->status() == Item::COMBINE_MISMATCHING) {
        const QString itemStatus = codec->toUnicode("COMBINE MISMATCHING");
        edit->append(message);
        edit->setTextColor(Qt::red);
        edit->insertPlainText(itemStatus);
    
        std::vector<boost::shared_ptr<Item> > combinedItems = item->combinedItems();
        for (auto combinedItem : combinedItems) {
          const QString combinedId = QString::fromUtf8(combinedItem->id().c_str());
          const QString combinedTitle = codec->toUnicode(combinedItem->title().c_str());
          const QString combineMessage = QString::fromUtf8("---->%1 %2").arg(combinedId).arg(combinedTitle);
          edit->append(combineMessage);
        }
      }
#endif
      edit->setTextColor(currentColor);
    }
  }
}

class WorkDialog::Pimpl
{
public:
  Pimpl() {}

  /* data */
  Ui::mainDialog ui;
  QString standardCSV;
  QString cReportCSV;
  QString dReportCSV;
  QString localCSV;
};


WorkDialog::WorkDialog(QWidget* parent)
:QDialog(parent, Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint), _pimpl(new Pimpl())
{
  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForLocale(codec);
  _pimpl->ui.setupUi(this);

  setWindowTitle(tr("Help CBB"));

  connect(_pimpl->ui.browseButton, SIGNAL(clicked()), SLOT(onBrowseFolder()));
  connect(_pimpl->ui.okButton, SIGNAL(clicked()), SIGNAL(workSignal()));
}

WorkDialog::~WorkDialog(void) {}

std::vector<std::string> WorkDialog::csvFiles() const {
  std::vector<std::string> files;
  files.push_back(_pimpl->standardCSV.toStdString());
  files.push_back(_pimpl->cReportCSV.toStdString());
  files.push_back(_pimpl->dReportCSV.toStdString());
  files.push_back(_pimpl->localCSV.toStdString());
  return files;
}

void WorkDialog::onBrowseFolder() {
  QFileDialog browser(this, tr("Select Image Stack Directory"));
  browser.setFileMode(QFileDialog::ExistingFiles);
  browser.setAcceptMode(QFileDialog::AcceptOpen);
  browser.setNameFilter(tr("CSV (*.csv)"));
  if(browser.exec() == QFileDialog::Accepted) {
    _pimpl->standardCSV = browser.selectedFiles().at(0);
    _pimpl->cReportCSV = browser.selectedFiles().at(1);
    _pimpl->dReportCSV = browser.selectedFiles().at(2);
    _pimpl->localCSV = browser.selectedFiles().at(3);

    _pimpl->ui.standardLineEdit->setText(_pimpl->standardCSV);
    _pimpl->ui.cReportLineEdit->setText(_pimpl->cReportCSV);
    _pimpl->ui.dReportLineEdit->setText(_pimpl->dReportCSV);
    _pimpl->ui.localLineEdit->setText(_pimpl->localCSV);
  }
}

void WorkDialog::setCReportResult(const std::vector<boost::shared_ptr<Item> >& items) {
  printResult(_pimpl->ui.cResultEdit, items);
}

void WorkDialog::setDReportResult(const std::vector<boost::shared_ptr<Item> >& items) {
  printResult(_pimpl->ui.resultEdit, items);
}

void WorkDialog::setLocalReportResult(const std::vector<boost::shared_ptr<Item> >& items) {
  printResult(_pimpl->ui.localResultEdit, items);
}