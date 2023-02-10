#ifndef BOOKLIST_H
#define BOOKLIST_H

#include <QDialog>
#include <QListWidgetItem>
#include "bookitem.h"
#include "../../Networking/Networking.h"
#include "BookListStructure.h"
#include <QCoreApplication>

namespace Ui {
class BookList;
}

class BookList : public QDialog
{
    Q_OBJECT

public:
    explicit BookList(std::shared_ptr<Networking> serverObj,std::vector<Book>,  QWidget *parent = nullptr);
    ~BookList();

private slots:
    void on_backButton_clicked();

    void on_bookList_itemActivated(QListWidgetItem *item);

private:
    Ui::BookList *ui;
    std::vector<Book> bookList;
    QWidget* back;
    std::shared_ptr<Networking> serverObj;

public:
    void reject() {
        serverObj->sendMessage("exit");
        this->destroy();
        QCoreApplication::exit(0);

    }
};

#endif // BOOKLIST_H
