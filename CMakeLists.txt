
cmake_minimum_required(VERSION 3.24)
project(Blipbug)
set(CMAKE_CXX_STANDARD 23)
##

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH  "/home/alexandru/Qt/6.4.1/gcc_64/lib/cmake")
find_package(Qt6Widgets REQUIRED)
##
find_package(SQLite3 REQUIRED)

#

#

add_executable(Server Server/server.cpp Networking/Networking.cpp MySQL/MySQL.cpp Commands/AbstractClass/Command.cpp Commands/AbstractClass/Command.h Commands/Login/Login.cpp Commands/Login/Login.h Commands/Register/Register.cpp Commands/Register/Register.h Commands/Search/Search.cpp Commands/Search/Search.h Commands/Download/Download.cpp Commands/Download/Download.h Commands/Rate/Rate.cpp Commands/Rate/Rate.h Commands/Recommand/Recommend.cpp Commands/Recommand/Recommend.h Commands/Description/Description.cpp Commands/Description/Description.h)
add_executable(Client Client/client.cpp Networking/Networking.cpp Networking/Networking.h
        Client/windows/MultiSelectComboBox.cpp Client/windows/MultiSelectComboBox.h
        Client/windows/loginform.h Client/windows/loginform.cpp Client/windows/loginform.ui
        Client/windows/registerform.h Client/windows/registerform.cpp Client/windows/registerform.ui
        Client/windows/mainwindow.h Client/windows/mainwindow.cpp Client/windows/mainwindow.ui
        Client/windows/searchwindow.h Client/windows/searchwindow.cpp Client/windows/searchwindow.ui
        Client/windows/booklist.cpp Client/windows/booklist.h Client/windows/booklist.ui
        Client/windows/bookitem.h Client/windows/bookitem.cpp Client/windows/bookitem.ui
        Client/windows/downloadlist.h Client/windows/downloadlist.cpp Client/windows/downloadlist.ui
        Client/windows/BookListStructure.h)

####UNCOMMENT FOR qt
target_link_libraries(Client Qt6::Widgets)

####SQLITE STUFF
target_link_libraries(Server SQLite::SQLite3)
find_package(Threads REQUIRED)
target_link_libraries(Server Threads::Threads)
target_link_libraries(Server ${CMAKE_DL_LIBS})
