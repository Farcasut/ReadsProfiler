#include "../Networking/Networking.h"
#include <QApplication>
#include "windows/mainwindow.h"

const std::string ip = "127.0.0.1";
const uint16_t port=4010;

int main(int argc, char *argv[]){

    QApplication application(argc, argv);
    MainWindow window(ip, port);
    window.show();
    application.exec();
    return 0;
}