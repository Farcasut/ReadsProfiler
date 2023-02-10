#include <thread>
#include <memory>
#include "../Commands/Login/Login.h"
#include "../Commands/Register/Register.h"
#include "../Commands/Search/Search.h"
#include "../Commands/Download/Download.h"
#include "../Networking/Networking.h"
#include "../MySQL/MySQL.h"
#include "../Commands/Rate/Rate.h"
#include "../Commands/Recommand/Recommend.h"
#include "../Commands/Description/Description.h"
const std::string ip = "127.0.0.1";
const uint16_t port = 4010;
uint64_t idClient = 0;
std::string Command::dataBasePath="/home/alexandru/FinalProject/FinalProject/Blipbug/database.db";

void treatClient(std::shared_ptr<Networking> myObj, uint64_t ID);
void similarUsers();


int main() {
    try{
        int32_t getFileDescriptor();
        Networking serverObj;
        serverObj.createSocketBindListen(ip, port);
        //
        std::thread sim(similarUsers);
        sim.detach();

        //
        while (true){
            std::shared_ptr<Networking> clientObj = std::make_shared<Networking>();
            clientObj->acceptConection(serverObj);
            std::thread th(treatClient, clientObj, idClient++);
            th.detach();
        }
    }
    catch (char const*e)
    {
        std::cerr<<e;
    }
}

void treatClient(std::shared_ptr<Networking> myObj, uint64_t ID){
    std::cout << ID << "\n";
    std::cout.flush();
    std::string command;
    bool login = false;
    Login loginObject;
    Register registerObject;
    std::vector<Command*> commands={new Search, new Download, new Rate,new Recommend, new Description};
    while (command != "exit" && myObj->GetIsAlive()) {


        if(myObj->GetIsAlive())
        {
            command = myObj->receiveMessage();
            std::cout<<command<<"\n";
            std::cout.flush();
            if(login==true && command=="logout")
            {
                login=false;
                for (unsigned i=0; i<commands.size(); i++)
                    commands[i]->SetUsername("");
            }
            if(login== false && command=="login")
            {
                login=loginObject.Execute(myObj);
                if (login)
                {
                   for (unsigned i=0; i<commands.size(); i++)
                       commands[i]->SetUsername(loginObject.GetUsername());
                }
                continue;
            }
            if(login== false && command=="register")
            {
                registerObject.Execute(myObj);
                continue;
            }
            if(login== true)
            {
                for(const auto& element:commands )
                {
                    if(element->GetName()==command)
                        element->Execute(myObj);
                }
            }

            std::cout.flush();
        }
        else
        {
            break;
        }
    }
}

void similarUsers() {
    while (1)
    {

    MySQL mySql(Command::dataBasePath);
    mySql.InsertInto("delete from similarusers;");
    std::map<std::string, std::vector<std::string>> usersAndBooks;
    std::map<std::string, std::vector<std::string>> similarUsers;
    std::cerr<<"SIMILAR USERS\n";
    std::cerr.flush();
    auto lambda = [](const std::vector<std::vector<std::string>>& sqlMatrix, uint8_t colNum=0) {
        std::vector<std::string> list;
        for (int i = 0; i < sqlMatrix.size(); i++) {
            list.push_back(sqlMatrix[i][colNum]);
        }
        return list;
    };
    auto sqlMatrix = mySql.GetDataFromQuery("select username from users ;");
    for(auto row:sqlMatrix)
    {
        std::string sql  = "select distinct isbn from downloads where username='"+row[0]+"';";
        usersAndBooks[row[0]]=lambda(mySql.GetDataFromQuery(sql), 0);
    }
    for(auto& [username, books]:usersAndBooks)
    {
        for(auto& [usernameSimilar, booksSimilar]:usersAndBooks)
        {
            if(username!=usernameSimilar && books[0]!="")
            {
                uint8_t booksToBeSimilar=(books.size()+1)/2;
                for(auto& book:books)
                {
                    if(std::find(booksSimilar.begin(), booksSimilar.end(),book)!=booksSimilar.end())
                    {
                        booksToBeSimilar--;
                    }
                }
                if(booksToBeSimilar==0)
                    similarUsers[username].push_back(usernameSimilar);
            }
        }
    }
    for(auto [key, values]: similarUsers)
    {
        std::cerr<<key<<":";
        for(auto x:values)
        {
            std::cerr<<x<<" ";
        }
        std::cerr<<"\n";
    }

    for(auto& [key, value]: similarUsers)
    {
        for(auto& name : value)
        {
            std::string sql="insert into similarusers values('"+key+"','"+name+"');";
            mySql.InsertInto(sql);
        }
    }
    std::cerr<<"\n";
    sleep(300);
    }
}

