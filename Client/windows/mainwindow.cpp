#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(std::string ip, uint16_t port,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    serverObj=std::make_shared<Networking>();
    serverObj->createSocketConnect(ip, port);

    ui->setupUi(this);
    this->setFixedSize(720, 500);
    this->formLoginExists=false;
    this->formRegisterExists=false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_redirectToLoginFrom_clicked()
{
    if(this->formLoginExists== false)
    {
        this->formLogin= new LoginForm(serverObj, this);
        this->formLoginExists= true;
    }
    this->hide();
    this->formLogin->show();
}


void MainWindow::on_redirectToRegisterForm_clicked()
{
    if(this->formRegisterExists== false)
    {
        this->registerForm=new RegisterForm(serverObj, this);
        this->formRegisterExists=true;
    }
    this->hide();
    this->registerForm->show();
}

