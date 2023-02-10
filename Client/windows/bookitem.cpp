#include <QInputDialog>
#include "bookitem.h"
#include "ui_bookitem.h"
#include <filesystem>

BookItem::BookItem(std::shared_ptr<Networking> serverObj,const Book item, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookItem)
{

    this->serverObj=serverObj;
    ui->setupUi(this);
    this->item=item;
    this->setGeometry(parent->geometry());
    this->back=parent;
    this->ui->isbnText          ->setText(item.isbn.c_str());
    this->ui->authorText        ->setText(item.author.c_str());
    this->ui->titleText         ->setText(item.title.c_str());
    this->ui->yearOfPublicationText->setText(item.yearOfPublication.c_str());
    this->ui->ratingText->setText(item.rating.c_str());
    serverObj->sendMessage("description");
    serverObj->sendMessage(item.isbn);
    this->ui->descriptionText->setText(serverObj->receiveMessage().c_str());
    std::string genres="";
    for(const auto& x:item.genreList)
    {
        genres+=x;
        genres+=" ";
    }
    std::string subgenres="";
    for(const auto& x:item.subGenreList)
    {
        subgenres+=x;
        subgenres+=" ";
    }
    this->ui->subgenreText->setText(subgenres.c_str());
    this->ui->genreText->setText(genres.c_str());


    this->ui->isbnText->setStyleSheet("background-color:#565354; color: #a19798; font: 14pt Arial; border:2px solid #FFFFFF;");
    this->ui->authorText->setStyleSheet("background-color:#565354; color: #a19798; font: 14pt Arial; border:2px solid #FFFFFF;");
    this->ui->titleText->setStyleSheet("background-color:#565354; color: #a19798; font: 14pt Arial; border:2px solid #FFFFFF;");
    this->ui->descriptionText->setStyleSheet("background-color:#565354; color: #a19798; font: 14pt Arial; border:2px solid #FFFFFF;");
    this->ui->subgenre_2->setStyleSheet("background-color:#565354; color: #a19798; font: 14pt Arial; border:2px solid #FFFFFF;");
    this->ui->genreText->setStyleSheet("background-color:#565354; color: #a19798; font: 14pt Arial; border:2px solid #FFFFFF;");

}

BookItem::~BookItem()
{
    delete ui;
}

void BookItem::on_backButton_clicked()
{
    this->back->setGeometry(this->geometry());
    this->back->show();
    this->destroy();


}


void BookItem::on_rateButton_clicked()
{
    bool ok;
    this->setStyleSheet("color: #a19798;");
    float rating = QInputDialog::getInt(this, tr("Enter a number"),
                                                   tr("Number:"), 0, 1, 10, 1, &ok);
    sendDataToServer(rating);

}


void BookItem::on_downloadButton_clicked()
{
    serverObj->sendMessage("download");
    serverObj->sendMessage(item.isbn);
    std::filesystem::create_directories("./DownloadedBooks");
    serverObj->receiveFile("./DownloadedBooks/"+item.title+" "+item.author+".pdf");
    sendDataToServer(1);
}

void BookItem::sendDataToServer(int rating) {
        std::cout<<rating<<"\n";
        serverObj->sendMessage("rate");
        serverObj->sendMessage(std::to_string(rating));
        serverObj->sendMessage(std::to_string(item.subGenreList.size()));
        for (unsigned i = 0; i < item.subGenreList.size(); i++) {
            serverObj->sendMessage(item.subGenreList[i]);
        }
}

