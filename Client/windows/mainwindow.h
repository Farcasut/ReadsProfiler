#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QtEvents>
#include <QMessageBox>
#include "../../Networking/Networking.h"

#include "loginform.h"
#include "registerform.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::string ip, uint16_t port,QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_redirectToLoginFrom_clicked();
    void on_redirectToRegisterForm_clicked();

private:

    //CONNECTION to the server
    std::shared_ptr<Networking> serverObj;
    Ui::MainWindow *ui;
    bool formLoginExists;
    LoginForm* formLogin;
    bool formRegisterExists;
    RegisterForm* registerForm;
    void closeEvent(QCloseEvent *event) {
        serverObj->sendMessage("exit");
        QCoreApplication::exit(0);

    }
};

#endif // MAINWINDOW_H

