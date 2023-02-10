//
// Created by alexandru on 12/14/22.
//

#ifndef BLIPBUG_SEARCH_H
#define BLIPBUG_SEARCH_H
#include "../AbstractClass/Command.h"

class Search: public Command{
private:
    void Filter(std::vector<std::vector<std::string>> &books, const std::string &title,
                        const std::string &year, const std::string &rating, const std::string &genre, const std::string& subgenre, bool, bool );
    void deleteBook(std::vector<std::vector<std::string>> &books, std::string match);
    void sendBooks(const std::vector<std::vector<std::string>> &books, std::shared_ptr<Networking> myObj);
public:
    Search();
    bool Execute(std::shared_ptr<Networking> myObj) override;


};


#endif //BLIPBUG_SEARCH_H
