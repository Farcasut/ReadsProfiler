#include <QMessageBox>
#include "searchwindow.h"
#include "ui_searchwindow.h"
#include "bookitem.h"
#include "downloadlist.h"
const std::vector<QString> GENRES={"'DRAMA'","'FANTASY'", "'ROMANCE'","'SCI-FI'","'POETRY'","'ARTS'", "'THRILLER'", "'POETRY'", "'SCIENCE'", "'NOVEL'" };
const std::vector<QString> SUBGENRES={"'COMEDY'","'TRAGEDY'","'COMPUTER_SCIENCE'","'BILDUNGSROMAN'","'PSYCHOLOGICAL'","'DETECTIVE'", "'PAINTING'","'FANTASY'" };


SearchWindow::SearchWindow(std::shared_ptr<Networking> serverObj,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    for(const QString & element:GENRES)
        this->ui->genresList->addItem(element);
    for(const QString& element:SUBGENRES)
        this->ui->subgenreList->addItem(element);

    this->serverObj=serverObj;
    this->logout=parent;
}

SearchWindow::~SearchWindow()
{
    delete ui;
}



void SearchWindow::on_pushButton_clicked()
{
    std::string isbn=this->ui->ISBN->text().toStdString();
    std::string author=this->ui->author->text().toStdString();
    std::string title=this->ui->Title->text().toStdString();
    std::string releaseDate=this->ui->releaseDate->text().toStdString();
    std::string rating=this->ui->rating->text().toStdString();
    std::string genreList=PrepareString(this->ui->genresList->currentText());
    std::string subgenreList=PrepareString(this->ui->subgenreList->currentText());
    std::cerr<<genreList<<" "<<subgenreList<<"\n";
    serverObj->sendMessage("search");
    serverObj->sendMessage(isbn);
    serverObj->sendMessage(author);
    serverObj->sendMessage(title);
    serverObj->sendMessage(releaseDate);
    serverObj->sendMessage(rating);
    serverObj->sendMessage(genreList);
    serverObj->sendMessage(subgenreList);
    serverObj->sendMessage(std::to_string(this->ui->genreConjunction->isChecked()));
    serverObj->sendMessage(std::to_string(this->ui->subGenreConjunction->isChecked()));
    std::cout<<std::to_string(this->ui->genreConjunction->isChecked())<<"------\n";
    std::cout<<std::to_string(this->ui->subGenreConjunction->isChecked())<<"------\n";

    std::vector<Book> bookList;
    GetBookData(bookList);

    BookList *test =  new BookList(serverObj, bookList, this);
    test->show();
    this->hide();
}

std::string SearchWindow::PrepareString(QList<QString>list ){
   std::string returnString="";
   uint16_t size=  list.size();
    for(unsigned i=0; i<size; i++)
    {
        returnString+=list[i].toStdString();
        if(i<size-1)
            returnString+=",";
    }
    return returnString;
}


void SearchWindow::on_RecommendButton_clicked()
{
    serverObj->sendMessage("recommend");
    std::vector<Book> bookList;
    GetBookData(bookList);
    if(bookList.size()!=0) {
        BookList *test = new BookList(serverObj, bookList, this);
        test->show();
        this->hide();
    }
    else
    {
        setStyleSheet("color: #a19798;");
        QMessageBox::information(this, "Warning", "There is not enough data to recommend books\n");
    }

}

void SearchWindow::GetBookData(std::vector<Book> &bookList) {
    uint16_t noOfBooks= std::stoi(serverObj->receiveMessage());
    for(int i=0; i<noOfBooks; i++)
    {
        Book tempBook;
        uint16_t fieldsRead=0;
        uint16_t bookFields = std::stoi(serverObj->receiveMessage());
        serverObj->receiveMessage();//book;
        tempBook.isbn=serverObj->receiveMessage();
        tempBook.author=serverObj->receiveMessage();
        tempBook.title=serverObj->receiveMessage();
        tempBook.yearOfPublication=serverObj->receiveMessage();
        tempBook.rating=serverObj->receiveMessage();
        std::string temp = serverObj->receiveMessage();//genre;
        fieldsRead+=7;
        while(1)
        {
            temp=serverObj->receiveMessage();
            fieldsRead++;
            if(temp=="booksubgenre")
                break;
            tempBook.genreList.push_back(temp);
        }
        for(;fieldsRead<bookFields;fieldsRead++)
        {
            temp=serverObj->receiveMessage();
            tempBook.subGenreList.push_back(temp);
        }
        bookList.push_back(tempBook);
    }
}


void SearchWindow::on_logout_clicked()
{
    this->serverObj->sendMessage("logout");
    this->logout->show();
    this->destroy();
}




void SearchWindow::on_download_clicked() {
   downloadList* newList =new downloadList(serverObj, this);
   newList->show();
   this->hide();
}

