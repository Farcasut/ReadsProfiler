#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QDialog>
#include "BookListStructure.h"
#include "../../Networking/Networking.h"
#include <QCoreApplication>

namespace Ui {
class BookItem;
}

class BookItem : public QDialog
{
    Q_OBJECT

public:
    explicit BookItem(std::shared_ptr<Networking> serverObj,const Book item,  QWidget *parent = nullptr);
    ~BookItem();

private slots:
    void on_backButton_clicked();

    void on_rateButton_clicked();

    void on_downloadButton_clicked();
    void sendDataToServer(int rating);
private:
    Ui::BookItem *ui;
    Book item;
    std::shared_ptr<Networking> serverObj;
    QWidget * back;
public:
    void reject() {
        serverObj->sendMessage("exit");
        this->destroy();
        QCoreApplication::exit(0);

    }
};

#endif // BOOKITEM_H
