#include "WorkDialog.h"
#include "ui_mainDialog.h"

#include <QTextCodec>
#include <QFileDialog>
#include <iostream>

namespace {
  const QString MESSAGE_TEMPLATE =  QString::fromUtf8("%1 %2-----> ");
}

class WorkDialog::Pimpl
{
public:
  Pimpl() {}

  /* data */
  Ui::mainDialog ui;
  QString standardCSV;
  QString localCSV;
};


WorkDialog::WorkDialog(QWidget* parent)
:QDialog(parent, Qt::WindowMinMaxButtonsHint), _pimpl(new Pimpl())
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
  files.push_back(_pimpl->standardCSV.toStdString());
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
    _pimpl->localCSV = browser.selectedFiles().at(1);
    _pimpl->ui.standardLineEdit->setText(_pimpl->standardCSV);
    _pimpl->ui.localLineEdit->setText(_pimpl->localCSV);
  }
}

void WorkDialog::log(QString message) {
  _pimpl->ui.resultEdit->append(message);
  std::cout<<message.toStdString()<<std::endl;
}

void WorkDialog::log(const std::vector<boost::shared_ptr<Item> >& items) {
  _pimpl->ui.resultEdit->clear();
  QTextCodec *codec = QTextCodec::codecForName("GB2312"); 
  for (auto item : items) {

    const QString id = QString::fromUtf8(item->id().c_str());
    const QString title = codec->toUnicode(item->title().c_str());
    const QString local = QString::fromUtf8(item->local().c_str());
    QColor currentColor = _pimpl->ui.resultEdit->textColor();

#if 0
    if (item->status() == Item::MATCHING) {
      const QString itemStatus = codec->toUnicode("MACHING");
      const QString message = MESSAGE_TEMPLATE.arg(id).arg(title);
        //QString::fromUtf8("%1 %2-----> ").arg(id).arg(title);
      _pimpl->ui.resultEdit->append(message);      
      _pimpl->ui.resultEdit->setTextColor(Qt::green);
      _pimpl->ui.resultEdit->insertPlainText(itemStatus);
    }
#endif

    if (item->status() == Item::LOST) {
      const QString itemStatus = codec->toUnicode("LOST");
      const QString message = QString::fromUtf8("%1 %2-----> ").arg(id).arg(title);
      _pimpl->ui.resultEdit->append(message);      
      _pimpl->ui.resultEdit->setTextColor(Qt::red);
      _pimpl->ui.resultEdit->insertPlainText(itemStatus);      
    }

    if (item->status() == Item::TITLE_MATCHING) {
      const QString message = QString::fromUtf8("%1 %2-----> ").arg(id).arg(title);
      _pimpl->ui.resultEdit->append(message);
      _pimpl->ui.resultEdit->setTextColor(Qt::blue);
      _pimpl->ui.resultEdit->insertPlainText(local);
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

#if 0
    if (item->status() == Item::COMBINE) {
      const QString itemStatus = codec->toUnicode("COMBINE");
      const QString message = QString::fromUtf8("%1 %2-----> ").arg(id).arg(title);
      _pimpl->ui.resultEdit->append(message);      
      _pimpl->ui.resultEdit->setTextColor(Qt::blue);
      _pimpl->ui.resultEdit->insertPlainText(itemStatus);

      std::vector<boost::shared_ptr<const Item> > combinedItems = item->combinedItems();
      for (auto combinedItem : combinedItems) {
        const QString combinedId = QString::fromUtf8(combinedItem->id().c_str());
        const QString combinedTitle = codec->toUnicode(combinedItem->title().c_str());
        const QString combineMessage = QString::fromUtf8("---->%1 %2").arg(combinedId).arg(combinedTitle);
        _pimpl->ui.resultEdit->append(combineMessage);
      }
    }
#endif
    _pimpl->ui.resultEdit->setTextColor(currentColor);
  }
}