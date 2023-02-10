#include "registerform.h"
#include "ui_registerform.h"
#include <iostream>
#include <QMessageBox>
RegisterForm::RegisterForm(std::shared_ptr<Networking> serverObj, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterForm)
{

    ui->setupUi(this);
    back=parent;
    this->serverObj=serverObj;
    this->setGeometry(parent->geometry());
}

RegisterForm::~RegisterForm()
{
    delete ui;
}



void RegisterForm::on_backToMainPage_clicked()
{

    std::cout<<"BackFromRegister"<<"\n";
    this->hide();
    back->setGeometry(this->geometry());
    back->show();
}


void RegisterForm::on_sendRegisterToServer_clicked()
{
    std::string password  =  this->ui->password->text().toStdString();
    std::string username  =  this->ui->userName->text().toStdString();
        //show a warning if the password or the username length is smaller than 30;
        if (password.size() < 5 || username.size() < 5) {
            setStyleSheet("color: #a19798;");
            QMessageBox::information(this, "Warning", "The length of the username/password must be greater than 4");
            return ;
        }
        //Show a warning if the passowrd or the username lenegth is bigger than 30
        if (password.size() > 30 || username.size() > 30) {
            setStyleSheet("color: #a19798;");
            QMessageBox::information(this, "Warning", "The length of the username/password must be smaller than 30");
            return;
        }
    //The password and the username are valid so we send the register command to the server with the data
    serverObj->sendMessage("register");
    serverObj->sendMessage(username);
    serverObj->sendMessage(password);
    std::string response =serverObj->receiveMessage();
    std::cerr<<response;
    std::cout.flush();
    if( response=="failure")
    {

        setStyleSheet("color: #a19798;");
        QMessageBox::information(this,"Warning", "The username already exists!");
    }
    else
    {
        this->ui->userName->clear();
        this->ui->password->clear();
        setStyleSheet("color: #a19798;");
        QMessageBox::information(this,"Warning", "You have been registered");
        on_backToMainPage_clicked();
    }
}

