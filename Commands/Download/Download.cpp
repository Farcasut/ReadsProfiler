//
// Created by alexandru on 12/16/22.
//

#include "Download.h"

Download::Download(): Command("download") {
    this->username="";
}



bool Download::Execute(std::shared_ptr<Networking> myObj) {
    std::string isbnBook  = myObj->receiveMessage();
    std::string bookName= "/home/alexandru/FinalProject/FinalProject/Blipbug/Books/"+isbnBook+".pdf";
    myObj->sendFile(bookName);
    MySQL mysql(dataBasePath);
    mysql.InsertInto("insert into downloads (username, isbn) values ('"+username+"', '"+isbnBook +"')");

    return true;

}
