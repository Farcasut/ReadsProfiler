#include "loginform.h"
#include "ui_loginform.h"
#include <QMessageBox>
#include "searchwindow.h"
LoginForm::LoginForm(std::shared_ptr<Networking> serverObj, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    this->serverObj=serverObj;
    back=parent;
    ui->setupUi(this);
    this->setGeometry(parent->geometry());
}

LoginForm::~LoginForm()
{
    delete ui;
}



void LoginForm::on_sendLoginData_clicked()
{
    std::string username =  this->ui->username->text().toStdString();
    std::string password =  this->ui->password->text().toStdString();

    if (password.size() < 5 || username.size() < 5) {
        setStyleSheet("color: #a19798;");
        QMessageBox::information(this, "Warning", "The credentials are invalid\n");
        return ;
    }
    //Show a warning if the passowrd or the username lenegth is bigger than 30
    if (password.size() > 30 || username.size() > 30) {
        setStyleSheet("color: #a19798;");
        QMessageBox::information(this, "Warning", "The credentials are invalid\n");
        return;
    }
    //The password and the username are valid so we
    //send the data to the server
    serverObj->sendMessage("login");
    serverObj->sendMessage(username);
    serverObj->sendMessage(password);
    std::string response =serverObj->receiveMessage();
    std::cerr<<response;
    std::cout.flush();
    if(response=="failure")
    {
        this->ui->username->clear();
        this->ui->password->clear();
        setStyleSheet("color: #a19798;");
        QMessageBox::information(this, "Warning", "The credentials are invalid\n");

    }
    else
    {
        std::cerr<<"You are connected congrats\n";
        this->searchWindow=new SearchWindow(serverObj, this);
        hide();
        searchWindow->show();
        return;
    }

}


void LoginForm::on_backToMainMenu_clicked()
{
    std::cout<<"BackFromLogin"<<"\n";
    this->hide();
    back->setGeometry(this->geometry());
    back->show();
}

