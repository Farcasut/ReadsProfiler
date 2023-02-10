#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QDialog>
#include <memory>
#include "booklist.h"
#include "../../Networking/Networking.h"
#include "BookListStructure.h"
#include <QCoreApplication>

namespace Ui {
class SearchWindow;
}

class SearchWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SearchWindow(std::shared_ptr<Networking> serverObj,  QWidget *parent = nullptr);
    ~SearchWindow();

private slots:
    void on_pushButton_clicked();
    void on_RecommendButton_clicked();

    void on_logout_clicked();


    void on_download_clicked();

private:
    std::string PrepareString(QList<QString>);
    Ui::SearchWindow *ui;
    std::shared_ptr<Networking> serverObj;
public:
    void GetBookData(std::vector<Book>& bookList);
    QWidget* logout;

public:
    void reject() {
        serverObj->sendMessage("exit");
        this->destroy();
        QCoreApplication::exit(0);
    }
};

#endif // SEARCHWINDOW_H
