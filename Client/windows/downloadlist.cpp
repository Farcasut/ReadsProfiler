#include "downloadlist.h"
#include "ui_downloadlist.h"


downloadList::downloadList(std::shared_ptr<Networking> serverObj, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::downloadList)
{
    this->serverObj=serverObj;
    ui->setupUi(this);
    QString path="./DownloadedBooks";
    QDir directory(path);
    directory.setNameFilters({"*.pdf"});
    pdfFiles =  directory.entryInfoList();
    for(const QFileInfo &fileInfo: pdfFiles)
    {
       this->ui->pdfLIst->addItem(fileInfo.fileName());
       this->ui->pdfLIst->setStyleSheet("background-color:#565354; color: #a19798; font: 14pt Arial; border:2px solid #FFFFFF;");
    }
    this->setGeometry(parent->geometry());
    this->back=parent;
}

downloadList::~downloadList()
{
    delete ui;
}

void downloadList::on_backButton_clicked()
{
    this->destroy();
    back->show();
}


void downloadList::on_pdfLIst_itemActivated(QListWidgetItem *item)
{
       std::string open=  "xdg-open /home/alexandru/FinalProject/FinalProject/Blipbug/DownloadedBooks/\""+item->text().toStdString()+"\"";
       std::cout<<open;
       std::cout.flush();
        system(open.c_str());
}

