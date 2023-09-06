#include "chat.h"
#include "errno.h"
#include "logger.h"

#include <thread>

Logger logger("chat.log");

int runChat() {
    Chat* chat = nullptr;

    try
    {
        // ������ ������ ������ "���"
        // ������ ������� ���������� ���� ��������� ������ �� ����� ��������.
        logger.write(std::stringstream() << time(nullptr) << ": try to create chat");  // ��������� � ���� ����� ����� � ��������� � ���, ��� ��� ������
        chat = new Chat();
    }
    catch (...)
    {
        // �� ������� ���� ���� errno ��� ��� ��� �� � �������
        // errno ���������� ��������� ��������� ������
        // std::cout << "Failed to start chat! Error code " << errno << std::endl;
        logger.write(std::stringstream() << time(nullptr) << ": failed to start chat! Error code " << errno << "\n");  // ��������� � ���� ����� ����� � ��������� � ���, ��� ��������� ������ ��� ������ ����
        return -1;
    }

    chat->start();

    while (chat->isChatOpen())          // ���� ��� ��������
    {
        chat->showLoginMenu();          // ������ ���� ����� � �������

        while (chat->getCurrentUser())  // ���� ��������� �� �������� ������������ �� nullptr
        {
            chat->showUserMenu();       // ������ ���������������� ����
        }
    }

    logger.write(std::stringstream() << time(nullptr) << ": Chat closed!" << "\n");

}

void printLog() {
    auto const ss = logger.read();
    cout << ss.str() << std::endl;
}

int main(int argc, char** argv)
{
    std::thread th1(runChat);
    th1.join();

    std::thread th2(printLog);
    th2.join();

    return 0;
}
