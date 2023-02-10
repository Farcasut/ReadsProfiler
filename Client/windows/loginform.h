#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include "searchwindow.h"
#include "../../Networking/Networking.h"
#include <QCoreApplication>

namespace Ui {
class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(std::shared_ptr<Networking> serverObj, QWidget *parent = nullptr);
    ~LoginForm();

private slots:


    void on_sendLoginData_clicked();

    void on_backToMainMenu_clicked();

private:
    Ui::LoginForm *ui;
    QWidget*back;
    SearchWindow* searchWindow;
    std::shared_ptr<Networking> serverObj;

public:
    void reject() {
        serverObj->sendMessage("exit");
        this->destroy();
        QCoreApplication::exit(0);

    }
};

#endif // LOGINFORM_H
