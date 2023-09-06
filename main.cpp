#include "chat.h"
#include "errno.h"
#include "logger.h"

#include <thread>

Logger logger("chat.log");

int runChat() {
    Chat* chat = nullptr;

    try
    {
        // Создаю объект класса "Чат"
        // Объект выкинет исключение если возникнут ошибки во время создания.
        logger.write(std::stringstream() << time(nullptr) << ": try to create chat");  // Записываю в файл логов время и сообщение о том, что чат открыт
        chat = new Chat();
    }
    catch (...)
    {
        // На виндовс тоже есть errno так что тут всё в порядке
        // errno показывает последнюю системную ошибку
        // std::cout << "Failed to start chat! Error code " << errno << std::endl;
        logger.write(std::stringstream() << time(nullptr) << ": failed to start chat! Error code " << errno << "\n");  // Записываю в файл логов время и сообщение о том, что произошла ошибка при старте чата
        return -1;
    }

    chat->start();

    while (chat->isChatOpen())          // Пока чат работает
    {
        chat->showLoginMenu();          // Вывожу меню входа в систему

        while (chat->getCurrentUser())  // Пока указатель на текущего пользователя не nullptr
        {
            chat->showUserMenu();       // Вывожу пользовательское меню
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
