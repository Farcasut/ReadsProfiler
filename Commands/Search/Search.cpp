//
// Created by alexandru on 12/14/22.
//

#include "Search.h"
Search::Search(): Command("search") {

}



bool Search::Execute(std::shared_ptr<Networking> myObj) {
    MySQL mysql(dataBasePath);
    const std::string isbn=myObj->receiveMessage();
    const std::string author=myObj->receiveMessage();
    const std::string title=myObj->receiveMessage();
    const std::string releaseDate=myObj->receiveMessage();
    const std::string rating=myObj->receiveMessage();
    const std::string genres=myObj->receiveMessage();
    const std::string subGenres=myObj->receiveMessage();
    const bool conjunctionGenre  =  std::stoi(myObj->receiveMessage());
    std::cerr<<conjunctionGenre<<"\n";
    const bool conjunctionSubGenre  =  std::stoi(myObj->receiveMessage());
    std::cerr<<conjunctionSubGenre<<"\n";

    std::vector<std::vector<std::string>> bookData;
    if(isbn.size()!=0)
    {
        mysql.OneFieldSearch("isbn",isbn, "books",bookData, 0);
        sendBooks(bookData, myObj);
        return true;
    }
    if(author.size()!=0)
    {
        mysql.AuthorOrTitleSearch("author", author,bookData, 0);
        this->Filter(bookData, title,  releaseDate, rating, genres, subGenres, conjunctionGenre,conjunctionSubGenre);
        sendBooks(bookData, myObj);
        return true;
    }
    if(title.size()!=0)
    {
        mysql.AuthorOrTitleSearch("title",title, bookData, 0);
        this->Filter(bookData, "",  releaseDate, rating, genres, subGenres, conjunctionGenre, conjunctionSubGenre);
        sendBooks(bookData, myObj);
        return true;
    }
    if(rating.size()!=0)
    {
        mysql.OneFieldSearch("rating", rating, "books", bookData, 0);
        this->Filter(bookData, title,  releaseDate, "", genres, subGenres, conjunctionGenre, conjunctionSubGenre);
        sendBooks(bookData, myObj);
        return true;
    }
    if(releaseDate.size()!=0)
    {
        mysql.OneFieldSearch("year", releaseDate, "books", bookData, 0);
        this->Filter(bookData, title,  "", rating, genres, subGenres, conjunctionGenre,conjunctionSubGenre);
        sendBooks(bookData, myObj);
        return true;
    }
    //todo Implement search for genres and subgenres DONE
    if(genres.size()!=0)
    {
        mysql.GenreSearch("genre",genres, "bookgenre",bookData, 0);
        this->Filter(bookData, title,  releaseDate, rating, genres, subGenres, conjunctionGenre, conjunctionSubGenre);
        sendBooks(bookData, myObj);
        return true;
    }
    if(subGenres.size()!=0)
    {
        mysql.GenreSearch("SUBGENRE",subGenres, "BOOKSUBGENRE",bookData, 0);
        this->Filter(bookData, title,  releaseDate, rating, genres, subGenres,conjunctionGenre, conjunctionSubGenre);
        sendBooks(bookData, myObj);
        return true;
    }
    sendBooks(bookData, myObj);
    return true;
}

void deleteBookConjunction(std::vector<std::vector<std::string>> &books, std::string match, std::string start, std::string end)
{

    uint32_t size=  books.size();
    for(unsigned i=0; i<size; i++)
    {
        uint8_t count= std::count(match.begin(), match.end(),',')+1;

        uint32_t booksize=books[i].size();
        for(int j=5; j<booksize; j++)
        {
            if(books[i][j]==start)
            {
                j++;
                while(j<booksize && books[i][j]!=end)
                {
                    if(match.find(books[i][j])!=std::string::npos)
                    {
                        count--;

                    }
                    j++;
                }
            }
        }
        if(count!=0)
        {
            books[i]=books.back();
            books.pop_back();
            size--;
            i--;
        }
    }

}


void Search::Filter(std::vector<std::vector<std::string>> &books, const std::string &title
                    , const std::string &year, const std::string &rating, const std::string &genre,const std::string& subgenre, bool conjunctionGenre, bool conjunctionSubGenre)
{
    ///filter by title
    if(title.size()!=0)
    {
        this->deleteBook(books, title);
    }
    if(year.size()!=0)
    {
        this->deleteBook(books, year);
    }
    if(rating.size()!=0)
    {
        this->deleteBook(books,rating);
    }
    if(genre.size()!=0)
    {

        if(conjunctionGenre)
        {
            deleteBookConjunction(books, genre, "bookgenre", "booksubgenre");
        }
        else
        {
            deleteBook(books, genre);
        }
    }
    if(subgenre.size()!=0)
    {

        if(conjunctionSubGenre)
        {
            deleteBookConjunction(books, subgenre, "booksubgenre", "");
        }
        else
        {
            deleteBook(books, subgenre);
        }
    }
}

void Search::deleteBook(std::vector<std::vector<std::string>> &books, std::string match) {
    uint32_t size= books.size();
    for(uint32_t i=0; i<size; i++)
    {
        bool del=true;
        for(const auto& info:books[i])
        {
            if(match.find(info)!=std::string::npos)
            {
                del= false;
                break;
            }
        }
        if(del)
        {
            books[i]=books.back();
            books.pop_back();
            size--;
            i--;
        }
    }
}


void Search::sendBooks(const std::vector<std::vector<std::string>> &books, std::shared_ptr<Networking> myObj) {
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
