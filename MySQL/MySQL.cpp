//
// Created by alexandru on 12/10/22.
//

#include <map>
#include <cstring>
#include "MySQL.h"


MySQL::MySQL(const std::string &path) {
    uint32_t rc;
    //std::cerr<<path.c_str()<<"\n";
    rc= sqlite3_open(path.c_str(), &this->dataBase);
    if(false)
    {
        this->create();
    }
    if(rc!=SQLITE_OK)
    {
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("MySqlConstructor function:\n" + errorMessage);
    }
}

bool MySQL::CheckIfUserExists(const std::string &username, const std::string &password) {
    std::string sql="select * from users where username='"+username+"'and password='"+password+"'";
    struct sqlite3_stmt *selectStatement;
    uint32_t result = sqlite3_prepare_v2(this->dataBase, sql.c_str(), -1, &selectStatement, NULL);
    if(result==SQLITE_OK)
    {
        if(sqlite3_step(selectStatement)==SQLITE_ROW)
        {
            sqlite3_finalize(selectStatement);
            return true;
        }
    }
    return false;
}

bool MySQL::UsernameExists(const std::string &username) {
    const std::string sql="select * from users where username='"+username+"'";
    struct sqlite3_stmt *selectStatement;
    uint32_t result = sqlite3_prepare(this->dataBase, sql.c_str(), -1, &selectStatement, NULL);
    if(result==SQLITE_OK)
    {
        if(sqlite3_step(selectStatement)==SQLITE_ROW)
        {
            sqlite3_finalize(selectStatement);
            return true;
        }
        return false;
    }
    return false;
}

bool MySQL::InsertUserIntoTable(const std::string& username, const std::string&password)
{
    if(this->UsernameExists(username))
    {
        return false;
    }
    const std::string sql="insert into users (username, password) values ('"+username+"' , '"+password+"')";
    std::cerr<<sql<<" ";
    char *ErrMsg;
    int32_t rc = sqlite3_exec(this->dataBase, sql.c_str(), nullptr, 0, &ErrMsg);
    if(rc==SQLITE_OK)
    {
        sqlite3_free(ErrMsg);
        return true;
    }

    //TODO MaybeSend a fail message to the users and make it renter the data
    std::string errorMessage=strerror(errno);
    throw std::runtime_error("InsertUserIntoTable function:\n" + errorMessage);

}

MySQL::~MySQL() {
    sqlite3_close(this->dataBase);
}

void MySQL::OneFieldSearch(const std::string&searchField, const std::string &value, const std::string &tableName, std::vector<std::vector<std::string>> &returnResults,uint16_t startingCol)
{
    std::string sql="SELECT * FROM "+tableName+" where " +searchField + "='"+value+"'";
    sqlite3_stmt * selectStatement;
    int32_t rc= sqlite3_prepare_v2(this->dataBase, sql.c_str(),-1, &selectStatement, nullptr);
    if(rc!=SQLITE_OK)
    {
        printf("%s: %s\n", sqlite3_errstr(sqlite3_extended_errcode(this->dataBase)), sqlite3_errmsg(this->dataBase));
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("OneFieldSearch function:\n" + errorMessage);
    }
    while((rc= sqlite3_step(selectStatement))==SQLITE_ROW)
    {
        std::vector<std::string> tempResult;
        uint16_t numberOfColoms = sqlite3_column_count(selectStatement);
        for(unsigned i=startingCol; i<numberOfColoms;i++)
        {
            tempResult.push_back( reinterpret_cast<const char*>(sqlite3_column_text(selectStatement, i)));
        }
        completeBook(tempResult, "bookgenre");
        completeBook(tempResult, "booksubgenre");
        returnResults.push_back(tempResult);
    }
    sqlite3_finalize(selectStatement);
}

void MySQL::AuthorOrTitleSearch(const std::string &searchField, const std::string&value, std::vector<std::vector<std::string>> &returnResults, uint16_t startingCol)
{
    std::string sql="SELECT * FROM books where " +searchField + " like '%"+value+"%'";
    sqlite3_stmt * selectStatement;
    int32_t rc= sqlite3_prepare_v2(this->dataBase, sql.c_str(),-1, &selectStatement, nullptr);
    if(rc!=SQLITE_OK)
    {
        printf("%s: %s\n", sqlite3_errstr(sqlite3_extended_errcode(this->dataBase)), sqlite3_errmsg(this->dataBase));
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("Author function:\n" + errorMessage);
    }
    while((rc= sqlite3_step(selectStatement))==SQLITE_ROW)
    {
        std::vector<std::string> tempResult;
        uint16_t numberOfColoms = sqlite3_column_count(selectStatement);
        for(unsigned i=startingCol; i<numberOfColoms;i++)
        {
            tempResult.push_back( reinterpret_cast<const char*>(sqlite3_column_text(selectStatement, i)));
        }
        completeBook(tempResult, "bookgenre");
        completeBook(tempResult, "booksubgenre");
        returnResults.push_back(tempResult);
    }
    sqlite3_finalize(selectStatement);
}

void MySQL::completeBook(std::vector<std::string> &returnResults, const std::string&tb) {
    std::string sql="SELECT * FROM "+tb+" where isbn='"+returnResults[0]+"'";
    sqlite3_stmt *selectStatement ;
    int32_t rc= sqlite3_prepare_v2(this->dataBase, sql.c_str(), -1, &selectStatement, nullptr);
    if(rc!=SQLITE_OK)
    {
        printf("%s: %s\n", sqlite3_errstr(sqlite3_extended_errcode(this->dataBase)), sqlite3_errmsg(this->dataBase));
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("CompleteBook function:\n" + errorMessage);
    }
    returnResults.push_back(tb);
    while((rc= sqlite3_step(selectStatement))==SQLITE_ROW)
    {
        uint16_t numberOfColoms = sqlite3_column_count(selectStatement);
        for(unsigned i=1; i<numberOfColoms;i++)
        {
            returnResults.push_back( reinterpret_cast<const char*>(sqlite3_column_text(selectStatement, i)));
        }
    }
    sqlite3_finalize(selectStatement);
}

void MySQL::GenreSearch(const std::string &field,const std::string &genres, const std::string tableName, std::vector<std::vector<std::string>>& returnResults,uint16_t startingCol)
{

    std::string sql ="SELECT ISBN FROM "+tableName+" WHERE "+ field +" IN ("+genres+")";
    std::map<std::string, int> isbns;

    sqlite3_stmt * selectStatement;
    int32_t rc= sqlite3_prepare_v2(this->dataBase, sql.c_str(),-1, &selectStatement, nullptr);
    if(rc!=SQLITE_OK)
    {
        printf("%s: %s\n", sqlite3_errstr(sqlite3_extended_errcode(this->dataBase)), sqlite3_errmsg(this->dataBase));
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("GenreSearch function:\n" + errorMessage);
    }
    while((rc= sqlite3_step(selectStatement))==SQLITE_ROW)
    {
        std::string tempResult;
        uint16_t numberOfColoms = sqlite3_column_count(selectStatement);
        for(unsigned i=startingCol; i<numberOfColoms;i++)
        {
            tempResult=( reinterpret_cast<const char*>(sqlite3_column_text(selectStatement, i)));
        }
        if(isbns[tempResult]++==0)
            this->OneFieldSearch("ISBN", tempResult,"books", returnResults, 0);
    }
    sqlite3_finalize(selectStatement);

}

bool MySQL::InsertRating(const std::string &username, const std::string &subgenreName, const float rating) {


    const std::string sql="insert into rating (username, subgenreName, subgenre) values ('"+username+"' , '"+subgenreName+"' , '"+std::to_string(rating)+"')";
    std::cerr<<sql<<" ";
    char *ErrMsg;
    int32_t rc = sqlite3_exec(this->dataBase, sql.c_str(), nullptr, 0, &ErrMsg);
    if(rc==SQLITE_OK)
    {
        sqlite3_free(ErrMsg);
        return true;
    }
    printf("%s: %s\n", sqlite3_errstr(sqlite3_extended_errcode(this->dataBase)), sqlite3_errmsg(this->dataBase));
    std::string errorMessage=strerror(errno);
    throw std::runtime_error("InsertRating function:\n" + errorMessage);
}

bool MySQL::CheckIfUserHasRated(const std::string &username, const std::string &subgenre) {

    const std::string sql="select * from rating where username='"+username+"'"+"and subgenreName='"+subgenre+"'";
    struct sqlite3_stmt *selectStatement;
    uint32_t result = sqlite3_prepare(this->dataBase, sql.c_str(), -1, &selectStatement, NULL);
    if(result==SQLITE_OK)
    {
        if(sqlite3_step(selectStatement)==SQLITE_ROW)
        {
            sqlite3_finalize(selectStatement);
            return true;
        }
        return false;
    }
    return false;
}

bool MySQL::UpdateRating(const std::string &username, const std::string &subgenreName, const float rating) {
    const std::string sql="update rating set subgenre= subgenre+"+std::to_string(rating)+ " where username='"+username+"' and subgenreName='"+subgenreName+"'";
    std::cerr<<sql<<" ";
    char *ErrMsg;
    int32_t rc = sqlite3_exec(this->dataBase, sql.c_str(), nullptr, 0, &ErrMsg);
    if(rc==SQLITE_OK)
    {
        sqlite3_free(ErrMsg);
        return true;
    }
    printf("%s: %s\n", sqlite3_errstr(sqlite3_extended_errcode(this->dataBase)), sqlite3_errmsg(this->dataBase));
    std::string errorMessage=strerror(errno);
    throw std::runtime_error("UpdateRating function:\n" + errorMessage);
}

bool MySQL::InsertInto(const std::string& query) {
    std::cerr<<query<<" ";
    char *ErrMsg;
    int32_t rc = sqlite3_exec(this->dataBase, query.c_str(), nullptr, 0, &ErrMsg);
    if(rc==SQLITE_OK)
    {
        sqlite3_free(ErrMsg);
        return true;
    }
    printf("%s: %s\n", sqlite3_errstr(sqlite3_extended_errcode(this->dataBase)), sqlite3_errmsg(this->dataBase));
    std::string errorMessage=strerror(errno);
    throw std::runtime_error("InsertInto function:\n" + errorMessage);
}

void MySQL::UserProfile(std::vector<std::pair<std::string, float>>& result, const std::string username)
{

    //select subgenrename, subgenre from rating where username = 'Farcasut'
    std::string sql="select subgenrename, subgenre from rating where username = '"+username+"'";
    std::cerr<<sql<<"\n";

    sqlite3_stmt * selectStatement;
    int32_t rc= sqlite3_prepare_v2(this->dataBase, sql.c_str(),-1, &selectStatement, nullptr);
    if(rc!=SQLITE_OK)
    {
        printf("%s: %s\n", sqlite3_errstr(sqlite3_extended_errcode(this->dataBase)), sqlite3_errmsg(this->dataBase));
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("UserProfiler function:\n" + errorMessage);
    }
    while((rc= sqlite3_step(selectStatement))==SQLITE_ROW)
    {
        std::vector<std::string> tempResult;
        uint16_t numberOfColoms = sqlite3_column_count(selectStatement);

        result.push_back(std::make_pair(reinterpret_cast<const char*>(sqlite3_column_text(selectStatement, 0)),
                                        sqlite3_column_double(selectStatement, 1)));

    }
    sqlite3_finalize(selectStatement);
}

void MySQL::GetDownloadedBooks(std::map<std::string,bool>& result, const std::string username)
{
    std::string sql="select distinct isbn from downloads where username = '"+username+"'";
    std::cerr<<sql<<"\n";
    sqlite3_stmt * selectStatement;
    int32_t rc= sqlite3_prepare_v2(this->dataBase, sql.c_str(),-1, &selectStatement, nullptr);
    if(rc!=SQLITE_OK)
    {
        printf("%s: %s\n", sqlite3_errstr(sqlite3_extended_errcode(this->dataBase)), sqlite3_errmsg(this->dataBase));
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("GetDownloadedBooks function:\n" + errorMessage);
    }
    while((rc= sqlite3_step(selectStatement))==SQLITE_ROW)
    {
        result[(reinterpret_cast<const char*>(sqlite3_column_text(selectStatement, 0)))]= true;
    }
    sqlite3_finalize(selectStatement);
}

std::vector<std::vector<std::string>> MySQL::GetDataFromQuery(const std::string sql) {
    sqlite3_stmt * selectStatement;
    std::vector< std::vector<std::string>>result;
    int32_t rc= sqlite3_prepare_v2(this->dataBase, sql.c_str(),-1, &selectStatement, nullptr);
    if(rc!=SQLITE_OK)
    {
        printf("%s: %s\n", sqlite3_errstr(sqlite3_extended_errcode(this->dataBase)), sqlite3_errmsg(this->dataBase));
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("GetDownloadedBooks function:\n" + errorMessage);
    }
    while((rc= sqlite3_step(selectStatement))==SQLITE_ROW)
    {
        std::vector<std::string> row;
        uint16_t colomns = sqlite3_column_count(selectStatement);
        for(int i=0; i<colomns; i++)
        {
            row.push_back( reinterpret_cast<const char *>(sqlite3_column_text(selectStatement, i)));

        }
        result.push_back(row);
    }
    if(result.size()==0)
        result.push_back({""});
    sqlite3_finalize(selectStatement);
    return result;
}

void MySQL::create() {

    std::string sql  = "CREATE TABLE USERS"
                       "("
                       "	USERNAME VARCHAR(30),"
                       "	PASSWORD VARCHAR(30) "
                       ");"
                       " "
                       "CREATE TABLE USERDOWNLOADS"
                       "("
                       "	USERNAME  VARCHAR(30),"
                       "	ISBN VARCHAR(30)"
                       ");"
                       " "
                       "CREATE TABLE BOOKS"
                       "("
                       "ISBN VARCHAR(30),"
                       "AUTHOR VARCHAR(256),"
                       "TITLE VARCHAR(256),"
                       "YEAR NUMBER,"
                       "RATING NUMBER"
                       ");"
                       " "
                       "CREATE TABLE GENRE"
                       "("
                       "	ID_GENRE NUMBER,"
                       "	NUME_GENRE VARCHAR(30)"
                       ");"
                       " "
                       "CREATE TABLE BOOKGENRE"
                       "("
                       "ISBN VARCHAR(30),"
                       "GENRE VARCHAR(30)"
                       ");"
                       " "
                       "CREATE TABLE SUBGENRENAME"
                       "("
                       "ID_SUBGENRE NUMBER,"
                       "NUME_SUBGENRE VARCHAR(30)"
                       ");"
                       " "
                       "CREATE TABLE BOOKSUBGENRE"
                       "("
                       "ISBN  VARCHAR(30),"
                       "SUBGENRE VARCHAR(30)"
                       ");"
                       " "
                       "CREATE TABLE SUBGENRE"
                       "("
                       "ID_GENRE NUMBER,"
                       "ID_SUBGENRE NUMBER"
                       ");"
                       " "
                       "CREATE TABLE RATING"
                       "("
                       "	USERNAME VARCHAR(30),"
                       "	SUBGENRENAME VARCHAR(30),"
                       "	SUBGENRE NUMBER"
                       ");"
                       " "
                       "CREATE TABLE DOWNLOADS"
                       "("
                       "	USERNAME VARCHAR(30),"
                       "	ISBN VARCHAR(30)"
                       ");"
                       " "
                       "CREATE TABLE SIMILARUSERS"
                       "("
                       "	USERNAME VARCHAR(30),"
                       "	SIMILARTO VARCHAR(30)"
                       ");"
                       " "
                       "CREATE TABLE DESCRIPTIONS("
                       "DESCRIPTION VARCHAR2(4096),"
                       "ISBN VARCHAR2(30)"
                       ");";
    char *ErrMsg;
    int32_t rc = sqlite3_exec(this->dataBase, sql.c_str(), nullptr, 0, &ErrMsg);
    if(rc==SQLITE_OK)
        sqlite3_free(ErrMsg);
}


