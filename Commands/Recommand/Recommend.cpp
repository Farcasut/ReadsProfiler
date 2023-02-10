//
// Created by alexandru on 12/29/22.
//

#include "Recommend.h"
#include <map>
#include "algorithm"
#include "../Search/Search.h"
Recommend::Recommend(): Command("recommend") {

}



bool Recommend::Execute(std::shared_ptr<Networking> myObj)
{
    MySQL mysql(dataBasePath);
    std::vector<std::pair<std::string, float>>profile;
    std::map<std::string,bool> downloadedBooks;
    mysql.UserProfile(profile, username);
    mysql.GetDownloadedBooks(downloadedBooks, username);
    std::vector<std::vector<std::string>> BOOKS;
    std::map<std::string, float> bookScore;
    uint32_t index=0;
    std::string sql= "select similarto from similarusers where username='"+username+"'";



    for(auto row: mysql.GetDataFromQuery(sql))
    {
        std::string sqlBOoks= "select distinct isbn from downloads where username='"+row[0]+"'";
        for(auto row1:mysql.GetDataFromQuery(sqlBOoks))
        {
            bookScore[row[0]]+=1;
        }
    }


    for(const auto&x :  profile)
    {
        mysql.GenreSearch("SUBGENRE","'"+x.first+"'", "BOOKSUBGENRE",BOOKS, 0);
        for(;index<BOOKS.size(); index++)
        {
          if(downloadedBooks.find(BOOKS[index][0])!=downloadedBooks.end())
          {
              std::swap(BOOKS[index], BOOKS.back());
              BOOKS.pop_back();
              index--;
          }
          else
          {
              bookScore[BOOKS[index][0]]+=x.second;

          }
        }
    }
    std::vector<std::pair<float, int>> toSort;
    for(int i=0; i<BOOKS.size(); i++)
    {
        toSort.push_back({bookScore[BOOKS[i][0]], i});
    }

    std::sort(toSort.begin(), toSort.end(), [](const auto& a, const auto&b){return a.first>b.first;});
    decltype(BOOKS) sortedBooks;

    for(int i=0; i<toSort.size(); i++)
    {
        sortedBooks.push_back(BOOKS[toSort[i].second]);
    }
    sendBooks(sortedBooks, myObj);
    return true;

}
void Recommend::sendBooks(const std::vector<std::vector<std::string>> &books, std::shared_ptr<Networking> myObj) {
    uint16_t rowNum=books.size();
    myObj->sendMessage(std::to_string(rowNum));
    for(auto& book:books)
    {
        uint16_t colNumb=book.size();
        myObj->sendMessage(std::to_string(colNumb+1));
        myObj->sendMessage("book");
        for(unsigned i=0; i<colNumb; i++)
        {
            myObj->sendMessage(book[i]);
        }
    }
}