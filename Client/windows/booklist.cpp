#include "booklist.h"
#include "ui_booklist.h"
#include <QListWidgetItem>
BookList::BookList(std::shared_ptr<Networking> serverObj,std::vector<Book> bookList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookList)
{
    this->serverObj=serverObj;
    ui->setupUi(this);
    this->bookList=bookList;
    this->setGeometry(parent->geometry());
    for(const auto& x: bookList)
    {

        std::string itemTitle = "Title: "+x.title+"\nAuthor: "+x.author;
        QListWidgetItem* item = new QListWidgetItem(QString(itemTitle.c_str()));
        this->ui->bookList->addItem(item);
        this->ui->bookList->setStyleSheet("background-color:#565354; color: #a19798; font: 14pt Arial; border:2px solid #FFFFFF;");
    }
    this->back=parent;
}

BookList::~BookList()
{
    delete ui;
}

void BookList::on_backButton_clicked()
{
    this->back->setGeometry(this->geometry());
    this->back->show();
    this->destroy();
}


void BookList::on_bookList_itemActivated(QListWidgetItem *item)
{
    BookItem *window = new BookItem(serverObj, this->bookList[this->ui->bookList->indexFromItem(item).row()], this);
    this->hide();
    window->show();

}

