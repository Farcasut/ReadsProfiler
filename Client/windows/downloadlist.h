#ifndef DOWNLOADLIST_H
#define DOWNLOADLIST_H

#include <QDialog>
#include "../../Networking/Networking.h"
#include "QDir"
#include "QListWidgetItem"
#include <QCoreApplication>

namespace Ui {
class downloadList;
}

class downloadList : public QDialog
{
    Q_OBJECT

public:

    explicit downloadList(std::shared_ptr<Networking> serverObj,  QWidget *parent = nullptr);
    ~downloadList();

private slots:
    void on_backButton_clicked();

    void on_pdfLIst_itemActivated(QListWidgetItem *item);

private:
    QFileInfoList pdfFiles;
    Ui::downloadList *ui;
    std::shared_ptr<Networking> serverObj;
    QWidget* back;


public:
    void reject() {
        serverObj->sendMessage("exit");
        this->destroy();
        QCoreApplication::exit(0);

    }
};

#endif // DOWNLOADLIST_H
