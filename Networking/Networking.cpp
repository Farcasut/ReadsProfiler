//
// Created by alexandru on 12/7/22.
//

#include "Networking.h"
Networking::Networking() {
    this->socketFileDescriptor = -1;
    this->isAlive= false;
}
Networking::Networking(const Networking &toCopy) {
    dup2(toCopy.socketFileDescriptor, this->socketFileDescriptor);
}
void Networking::acceptConection(const Networking &ServerObj) {

    struct sockaddr_in clientAddr{};
    socklen_t length = sizeof(clientAddr);
    if ((this->socketFileDescriptor = accept(ServerObj.socketFileDescriptor, (struct sockaddr *) &clientAddr,
                                             &length)) < 0) {
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("AcceptConnection function:\n" + errorMessage);
    }
    this->isAlive= true;
}

Networking::~Networking(){
    close(this->socketFileDescriptor);
}

void Networking::initializeSocketFileDescriptor() {
    if ((this->socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("InitializeSocketFileDescriptor function:\n" + errorMessage);
    }
}

void Networking::connectSocket(const std::string ip, const uint16_t &port) {
    prepareStructure(ip, port);
    if (connect(this->socketFileDescriptor, (struct sockaddr *) &this->socketData, sizeof(this->socketData)) < 0) {
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("ConnectSocket function:\n" + errorMessage);
    }
}

void Networking::prepareStructure(const std::string &ip, const uint16_t &port) {
    //we prepare the structure for bind
    this->socketData.sin_family = AF_INET;
    this->socketData.sin_port = htons(port);
    this->socketData.sin_addr.s_addr = inet_addr(ip.c_str());
}

void Networking::setReuseAdress() {
    constexpr int on = 1;
    if (setsockopt(this->socketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(1)) < 0) {
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("SetReuseAddress function:\n" + errorMessage);
    }
}

void Networking::bindSocket(const std::string &ip, const uint16_t &port) {
    setReuseAdress();
    this->prepareStructure(ip, port);
    if (bind(this->socketFileDescriptor, (struct sockaddr *) &this->socketData, sizeof(this->socketData)) < 0) {
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("BindSocket function:\n" + errorMessage);
    }
}

void Networking::listenSocket(uint32_t queue) {
    if (listen(this->socketFileDescriptor, queue) < 0) {
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("ListenSocket function:\n" + errorMessage);
    }

}

void Networking::createSocketBindListen(const std::string &ip, const uint16_t &port) {
    this->initializeSocketFileDescriptor();
    this->bindSocket(ip, port);
    this->listenSocket();
}

void Networking::createSocketConnect(const std::string &ip, const uint16_t &port) {
    this->initializeSocketFileDescriptor();
    this->connectSocket(ip, port);
}

void Networking::sendMessage(const std::string &message) {
    uint32_t size = message.size();
    if (send(this->socketFileDescriptor, &size, sizeof(uint32_t), MSG_NOSIGNAL) < 0) {
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("SendMessage function:\n" + errorMessage);
    }
    if(size==0)
        return;
    if (send(this->socketFileDescriptor, message.c_str(), size, MSG_NOSIGNAL) < 0) {
        if(errno==EPIPE)
        {
            this->isAlive= false;
            return ;
        }
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("SendMessage function:\n" + errorMessage);
    }
}

std::string Networking::receiveMessage() {
    uint32_t length = 0;
    int alive=0;
    if ((alive=recv(this->socketFileDescriptor, &length, sizeof(uint32_t), MSG_NOSIGNAL)) < 0) {
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("ReceiveMessage function:\n" + errorMessage);
    }
    if(alive==0)
    {
        isAlive= false;
        return "";
    }


    if(length==0)
        return "";

    char *temp1 = new char[length + 1]{};
    if((recv(this->socketFileDescriptor, temp1, length, MSG_NOSIGNAL)) < 0)
    {
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("ReceiveMessage function:\n" + errorMessage);
    }
    std::string temp2(temp1);
    delete []temp1;
    return temp2;
}

void Networking::sendFile(const std::string &path) {
    char filePage[4096]{};
    int fdFile;
    if ((fdFile = open(path.c_str(), O_RDONLY)) < 0) {
        close(fdFile);
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("SendFile function:\n" + errorMessage);
    }
    int32_t bytesRead = -1;
    while (bytesRead) {
        int32_t byteCount = 0;
        while (byteCount != 4096) {
            if ((bytesRead = read(fdFile, &filePage[byteCount], 4096-byteCount)) < 0) {
                close(fdFile);
                std::string errorMessage=strerror(errno);
                throw std::runtime_error("SendFile function:\n" + errorMessage);
            }
            byteCount += bytesRead;
            if (bytesRead == 0)
                break;
        }

        int32_t currentByteCount = 0;
        while (currentByteCount != 4096) {
            int32_t currentRead = 0;
            if ((currentRead = send(this->socketFileDescriptor, &filePage[currentByteCount], 4096 - currentByteCount,
                                    MSG_NOSIGNAL)) < 0) {
                close(fdFile);
                std::string errorMessage=strerror(errno);
                throw std::runtime_error("SendFile function:\n" + errorMessage);
            }
            currentByteCount += currentRead;
        }
        memset(filePage, 0, 4096);
        if(send(this->socketFileDescriptor, &bytesRead, sizeof(int32_t), MSG_NOSIGNAL)<0)
        {
            std::string errorMessage=strerror(errno);
            throw std::runtime_error("SendFile function:\n" + errorMessage);
        }
    }
    close(fdFile);
}

void Networking::receiveFile(const std::string& fileName) {



    char filePage[4096]{};
    int fdFile;
    if ((fdFile = open(fileName.c_str(), O_WRONLY | O_CREAT, 0666)) < 0) {
        close(fdFile);
        std::string errorMessage=strerror(errno);
        throw std::runtime_error("ReceiveFile function:\n" + errorMessage);
    }
    int32_t bytesRead = -1;
    while (bytesRead) {
        int32_t currentByte = 0;
        while (currentByte != 4096) {
            if ((bytesRead = recv(this->socketFileDescriptor, &filePage[currentByte], 4096 - currentByte, MSG_NOSIGNAL)) < 0) {
                close(fdFile);
                std::string errorMessage=strerror(errno);
                throw std::runtime_error("ReceiveFile function:\n" + errorMessage);
            }
            currentByte += bytesRead;
            if (bytesRead == 0)
                break;
        }
        uint32_t currentByteCount = 0;
        while (currentByteCount != 4096) {
            int32_t currentWrite = 0;
            if ((currentWrite = write(fdFile, &filePage[currentByteCount], 4096 - currentByteCount)) < 0) {
                close(fdFile);
                std::string errorMessage=strerror(errno);
                throw std::runtime_error("ReceiveFile function:\n" + errorMessage);
            }
            currentByteCount += currentWrite;
        }
        memset(filePage, 0, 4096);
        recv(this->socketFileDescriptor, &bytesRead, sizeof (int32_t), MSG_NOSIGNAL);
    }
    close(fdFile);

}

int32_t Networking::getSocketFileDescriptor() const {
    return socketFileDescriptor;
}

bool Networking::GetIsAlive() {
    return this->isAlive;
}


