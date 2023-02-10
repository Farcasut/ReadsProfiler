//
// Created by alexandru on 12/10/22.
//

#ifndef SQL_MYSQL_H
#define SQL_MYSQL_H
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include "sqlite3.h"
#include <map>

class MySQL {
private:
    sqlite3* dataBase;
private:
    //TODO if you have time
    // MAKE A UNIQUE FUNCTION FOR EXISTS THAT TAKES THE QUERY AND DO THE SAME FOR THE INSERTION

    bool UsernameExists(const std::string& username);
    void completeBook( std::vector<std::string>& returnResults, const std::string&);

public:
    std::vector<std::vector<std::string>> GetDataFromQuery(const std::string string);
    bool CheckIfUserHasRated(const std::string& username, const std::string& subgenre);
    bool InsertInto(const std::string& query);
    MySQL(const std::string &path);
    bool CheckIfUserExists(const std::string& username, const std::string& password);
    bool InsertUserIntoTable(const std::string& username, const std::string&password);
    bool InsertRating(const std::string&username, const std::string& subgenreName, const float rating);
    bool UpdateRating(const std::string&username, const std::string& subgenreName, const float rating);

    void OneFieldSearch(const std::string&searchField, const std::string& value,const std::string& tableName, std::vector<std::vector<std::string>> &returnResults, uint16_t startingCol);
    void GenreSearch(const std::string& field, const std::string& genres, const std::string tableName, std::vector<std::vector<std::string>>&returnResults, uint16_t startingCol);
    void AuthorOrTitleSearch(const std::string& searchField,const std::string&value,  std::vector<std::vector<std::string>>&returnResults, uint16_t startingCol);
    void create();

    void UserProfile( std::vector<std::pair<std::string, float>>&, const std::string username);
    void GetDownloadedBooks(std::map<std::string,bool>&, const std::string username);
    ~MySQL();


};


#endif //SQL_MYSQL_H
