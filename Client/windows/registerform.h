#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QDialog>
#include <memory>
#include "../../Networking/Networking.h"
#include <QCoreApplication>

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterForm(std::shared_ptr<Networking> serverObj,QWidget *parent = nullptr);
    ~RegisterForm();

private slots:
    void on_backToMainPage_clicked();

    void on_sendRegisterToServer_clicked();

private:
    Ui::RegisterForm *ui;
    QWidget*back;
    std::shared_ptr<Networking> serverObj;

public:
    void reject() {
        serverObj->sendMessage("exit");
        this->destroy();
        QCoreApplication::exit(0);

    }
};

#endif // REGISTERFORM_H
