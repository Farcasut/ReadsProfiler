//
// Created by alexandru on 12/29/22.
//

#include "Rate.h"

Rate::Rate(): Command("rate") {

}



bool Rate::Execute(std::shared_ptr<Networking> myObj) {
    //primesc nota
    float rate = std::stof(myObj->receiveMessage())/10.f;
    //Primesc nr de subgenuri
    uint8_t noOfSubgenres= std::stoi(myObj->receiveMessage());
    //citesc nr de subgenuri si inserez in tabel
    MySQL mysql(dataBasePath);
    for(unsigned i=0; i<noOfSubgenres; i++)
    {
        std::string tempGenre =  myObj->receiveMessage();
        if(mysql.CheckIfUserHasRated(username, tempGenre))
        {
            mysql.UpdateRating(username, tempGenre, rate);
        }
        else
        {
            mysql.InsertRating(username, tempGenre, rate);
        }
    }

    return true;
}
