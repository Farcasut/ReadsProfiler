//
// Created by alexandru on 12/7/22.
//

#ifndef NETWORKINGCLASS_NETWORKING_H
#define NETWORKINGCLASS_NETWORKING_H
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
class Networking {
private:
int32_t socketFileDescriptor;
bool isAlive;
struct sockaddr_in socketData;
    void setReuseAdress();
    void prepareStructure(const std::string&, const uint16_t&);
public:

    //Primitives
    Networking();//sets sockFileDescriptor = -1
    Networking(const Networking&);
    void initializeSocketFileDescriptor();                                             //call socket() and throw error in case of failure
    void bindSocket(const std::string& ip,const uint16_t& port);                       //call bind() and has reuse option and throw error in case of failure
    void acceptConection(const Networking & serverObj);                                //call accept() and throw error in case of failure
    void connectSocket(const std::string ip, const uint16_t& port);                    //call connect() and throw error in case of failure
    void listenSocket(uint32_t queue = 10);                                            //call listen() and throw error in case of failure
    int32_t getSocketFileDescriptor() const;
    bool GetIsAlive();
    ~Networking();

    //High level function
    void createSocketBindListen(const std::string&ip, const uint16_t& port);           //Make a socket and bind it to ip&port and listen for connections(server)
    void createSocketConnect(const std::string&ip, const uint16_t& port);              //Make a socket and connect to an ip&port(client)
    void sendMessage(const std::string&);                                              //Send a message through the socket
    std::string receiveMessage();                                                      //Returns a message receive through the socket
    void sendFile(const std::string & path);
    void receiveFile(const std::string& fileNanem);


};


#endif //NETWORKINGCLASS_NETWORKING_H
