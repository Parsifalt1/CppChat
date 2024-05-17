#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>

#define MaxMessageLength 256
#define IsDebug false

SOCKET Connection = INVALID_SOCKET;
std::string userName;

enum ConsoleColor {
    Black,
    DarkBlue,
    DarkGreen,
    Gray,
    Green,
    Red,
    White
};


void SetConsoleColor(ConsoleColor color) {
    switch (color) {
    case Black:
        system("color 0");
        break;
    case DarkBlue:
        system("color 1");
        break;
    case DarkGreen:
        system("color 2");
        break;
    case Gray:
        system("color 8");
        break;
    case Green:
        system("color A");
        break;
    case Red:
        system("color C");
        break;
    case White:
        system("color F");
        break;
    default:
        std::cout << "������������ ����!" << std::endl;
        break;
    }
}

std::vector<std::string> Split(std::string StringToSplit, std::string SplitterString)
{
    std::vector<std::string> ReturnVector;
    int i = 0;
    std::string SplittedString = "";
    while (i < StringToSplit.size())
    {
        if (StringToSplit[i] == SplitterString[0])
        {
            bool IsSplitter = true;
            for (int j = 1; j < SplitterString.size(); j++)
            {
                if (StringToSplit[i + j] != SplitterString[j])
                {
                    IsSplitter = false;
                    break;
                }
            }
            if (IsSplitter)
            {
                ReturnVector.push_back(SplittedString);
                SplittedString = "";
                i += SplitterString.size();
                continue;
            }
        }
        SplittedString += StringToSplit[i];
        i++;
    }
    ReturnVector.push_back(SplittedString);
    return ReturnVector;
}

void ClientHandler()
{
    char msg[256];
    int ConnectionStatus;
    while (true)
    {
        ConnectionStatus = recv(Connection, msg, MaxMessageLength, NULL);

        if (ConnectionStatus <= 0)
        {
            system("cls");
            std::cout << "�� �������� ������." << std::endl;
            closesocket(Connection);
            Connection = INVALID_SOCKET;
            break;
            Sleep(1);
            std::cout << "������� /help ��� ��������� ������ ������." << std::endl;
        }

        std::cout << msg << std::endl;
    }
}

bool ConnectToServer(std::string ServerAddress, int Port)
{
    //WSAStartup
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
    {
        std::cerr << "������� Winsock!" << std::endl;
        return false;
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr(ServerAddress.c_str());
    addr.sin_port = htons(Port);
    addr.sin_family = AF_INET;

    Connection = socket(AF_INET, SOCK_STREAM, NULL);

    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
    {
        std::cout << "�������: ������ �� ��������." << std::endl;
        return false;
    }

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
    return true;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::string Message;

    std::cout << "������ ��� ��'�: ";
    std::getline(std::cin, userName);
    system("cls");
    std::cout << "������� /help ��� ��������� ������ ������." << std::endl;

    if (IsDebug)
        ConnectToServer("127.0.0.1", 1111);

    while (true)
    {
        getline(std::cin, Message);

        if (Message.size() >= MaxMessageLength)
        {
            std::cerr << "������� �����������." << std::endl;
            continue;
        }

        std::vector<std::string> MessageVector = Split(Message, " ");

        if (MessageVector[0] == "/help")
        {
            system("cls");
            std::cout << "/connect\t ��� ���������� �� �������" << std::endl;
            std::cout << "/disconnect\t ��� ���������� �� �������" << std::endl;
            std::cout << "/create\t\t ��� ��������� ������" << std::endl;
            std::cout << "/remove\t\t ��� ��������� ������" << std::endl;
            std::cout << "/open\t\t ��� ����� �� ������" << std::endl;
            std::cout << "/exit\t\t ��� ������ � ������" << std::endl;
            std::cout << "/ls\t\t �������� �� ������" << std::endl;
            std::cout << "/clear\t\t �������� ��������� �����" << std::endl;
            std::cout << "/custom\t\t ����������� ����" << std::endl;
            
            continue;
            system("cls");
        }

        if (MessageVector[0] == "/connect" && MessageVector.size() >= 3)
        {
            if (Connection == INVALID_SOCKET)
            {
                // Address: "127.0.0.1 1111"
                ConnectToServer(MessageVector[1], atoi(MessageVector[2].c_str()));
            }
            else
                std::cout << "�� ��� ������� �� �������." << std::endl;
            continue;
        }
        if (MessageVector[0] == "/connect" && MessageVector.size() < 3)
        {
            std::cout << "������ �������. ������� ����������� �����." << std::endl;
            std::cout << "������� ��� �����: connect 'IP PORT'" << std::endl;
            continue;
        }

        if (MessageVector[0] == "/disconnect")
        {
            closesocket(Connection);
            Connection = INVALID_SOCKET;
            continue;
        }
        if (MessageVector[0] == "/clear") {
            system("cls");
            continue;
        }
        if (MessageVector[0] == "/custom") {
            std::string changeColor;
            std::cout << "������� ���� ������: DarkBlue / DarkGreen / Gray / Green / Red / White" << std::endl;
            std::cin >> changeColor;

            
            if (changeColor == "DarkBlue")
                SetConsoleColor(DarkBlue);
            else if (changeColor == "DarkGreen")
                SetConsoleColor(DarkGreen);
            else if (changeColor == "Gray")
                SetConsoleColor(Gray);
            else if (changeColor == "Green")
                SetConsoleColor(Green);
            else if (changeColor == "Red")
                SetConsoleColor(Red);
            else if (changeColor == "White")
                SetConsoleColor(White);
            else
                std::cout << "������������ ����!" << std::endl;
            system("cls");
           
            std::cout << "������� /help ��� ��������� ������ ������." << std::endl;
            continue;
        }
        

        // ������ ��'� ����������� ����� ������������, ���� �� �� �������
        if (Message[0] != '/')
        {
            Message = userName + ": " + Message;
        }

        send(Connection, Message.c_str(), MaxMessageLength, NULL);
        Sleep(10);
    }
}
