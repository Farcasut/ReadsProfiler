//
// Created by alexandru on 12/29/22.
//
#include "string"
#include "vector"
#ifndef BLIPBUG_BOOKLISTSTRUCTURE_H
#define BLIPBUG_BOOKLISTSTRUCTURE_H
struct Book
{
    std::string isbn;
    std::string author;
    std::string title;
    std::string yearOfPublication;
    std::string rating;
    std::vector<std::string> genreList;
    std::vector<std::string> subGenreList;

};
#endif //BLIPBUG_BOOKLISTSTRUCTURE_H
