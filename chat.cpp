#include <iostream>
#include "chat.h"
#include "logger.h"

extern Logger logger;  // Декларирую переменную, определённую в другом месте программы

void Chat::start()
{
	_sql.recv_users(_users);
	_isChatOpen = true;
}

User const* Chat::getUserByLogin(const std::string& login) const  // Описание метода, возвращающего указатель на текущего пользователя по его уникальному логину
{                                                                 //
	for (auto& user : _users)                                     //
	{                                                             //
		if (login == user.getUserLogin())                         //
		{                                                         //
			return &user;                                         // Возвращаю указатель на текущего пользователя по уникальному логину
		}                                                         //
	}                                                             //
	return nullptr;                                               // Если пользователя с таким логином не нашлось в контейнере для хранения данных о пользователях, то возвращаю указателю значение nullptr
}

void Chat::login()                     // Описание функции входа в систему
{
	std::string login, password;
	char operation;

	do
	{
		std::cout << "Login: ";
		std::cin >> login;
		std::cout << "Password: ";
		std::cin >> password;

		_currentUser = getUserByLogin(login);

		if (_currentUser == nullptr || !isEqualPassword(password, _currentUser->getPassword().data()))   // Если указатель на текущего пользователя - nullptr или введён неверный пароль
		{
			logger.write(std::stringstream() << time(0) << ": user " << login << " failed to login");  // Записываю в файл логов время и сообщение о том, что пользователю не удалось залогиниться

			std::cout << "Login failed!..." << std::endl;                               // Вывожу ошибку
			std::cout << "0 - exit this menu, or (any kay) for repeat: " << std::endl;  // Предлагаю выйти из меню или попробовать снова
			std::cin >> operation;

			if (operation == '0') break;
		}
	} while (!_currentUser);

	if (_currentUser) {
		logger.write(std::stringstream() << time(0) << ": user " << login << " login success");  // Записываю в файл логов время и сообщение о том, что регистрация пользователя в системе успешно осуществлена
	}
}

void Chat::signUp()    // Описание функции регистрации пользователя в системе
{
	std::string login, password;

	std::cout << "Login: ";
	std::cin >> login;
	std::cout << "Password: ";
	std::cin >> password;

	if (getUserByLogin(login) || login == "all")
	{
		throw UserLoginExceptions();
	}

	_users.push_back({ login, password });  // Добавляю в конец контейнера хранения данных о пользователях
	_currentUser = &_users.back();          // Указатель на текущего пользователя указывает на этого добавленного пользователя

	_sql.send_users({ { login, password } });
	logger.write(std::stringstream() << time(0) << ": user " << login << " signup success");  // Записываю в файл логов время и сообщение о том, что вход пользователя в систему успешно осуществлен
}

void Chat::showChat()  // Описание функции отображения меню чата
{
	std::string from, to;
	std::vector<Message> messages;

	_sql.recv_messages(messages);

	for (auto& mess : messages)
	{
		if (_currentUser->getUserLogin() == mess.getUserFrom() || _currentUser->getUserLogin() == mess.getUserTo() || mess.getUserTo() == "all")  // По указателю на объект "пользователь" беру его уникальный логин и сортирую сообщения от него, ему и всем
		{
			if (_currentUser->getUserLogin() == mess.getUserFrom()) {
				from = "me ";
			}
			else {
				auto const user = getUserByLogin(mess.getUserFrom());

				if (user) {
					from = user->getUserLogin();
				}
				else {
					from = "?";
				}
			}

			if (mess.getUserTo() == "all")
			{
				to = "all";
			}
			else
			{
				to = (_currentUser->getUserLogin() == mess.getUserTo()) ? "me " : getUserByLogin(mess.getUserTo())->getUserLogin();
			}

			std::cout << "Message from " << from << " to " << to << std::endl;
			std::cout << "Text message: " << mess.getUserMessage() << std::endl;

			logger.write(std::stringstream() << time(0) << ": message " << mess.getUserMessage() << " received from user "
				<< mess.getUserFrom() << " by user " << mess.getUserTo());
		}
	}

	std::cout << "-------------------------" << std::endl;
}

void Chat::showLoginMenu()     // Описание функции вывода меню чата на экран
{
	_currentUser = nullptr;

	char operation;

	do
	{
		std::cout << "1 - Login" << std::endl;
		std::cout << "2 - SignUp" << std::endl;
		std::cout << "0 - Exit" << std::endl;

		std::cin >> operation;

		switch (operation)
		{
		case '1':
			login();
			break;
		case '2':
			try
			{
				signUp();
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
			break;
		case '0':
			_isChatOpen = false;
			break;
		default:
			std::cout << "Input 1 or 2 " << std::endl;
			break;
		}
	} while (!_currentUser && _isChatOpen);  // Цикл работает пока пользователь корректен и он существует и чат работает
}

void Chat::showUserMenu()      // Описание функции, отображающей пользовательское меню
{
	char operation;

	std::cout << "Hi, " << _currentUser->getUserLogin() << std::endl;  // Вывод приветствия для вошедшего в систему пользователя по его логину

	while (_currentUser)
	{
		std::cout << "User menu: 1 - Show chat, 2 - Add message, 3 - Show All users, 0 - Exit" << std::endl;

		std::cin >> operation;

		switch (operation)
		{
		case '1':
			showChat();
			break;
		case '2':
			addMessage();
			break;
		case '3':
			showAllUsers();
			break;
		case '0':
			_currentUser = nullptr;
			break;
		default:
			std::cout << "Unknown choice! " << std::endl;
		}
	}
}

void Chat::showAllUsers()     // Описание функции отображения всех пользователей системы
{
	std::cout << "----- All Users -----" << std::endl;
	for (auto& user : _users)
	{
		std::cout << user.getUserLogin() << std::endl;

		if (_currentUser->getUserLogin() == user.getUserLogin())  // Проверка для определения пользователя, под логином которого в данный момент осуществлён вход в систему
		{
			std::cout << " (me) " << std::endl;
		}
	}
	std::cout << "-----------------------" << std::endl;
}

void Chat::addMessage()        // Описание функции добавления сообщений
{
	std::string to, text;

	std::cout << "To (name or \"all\")" << std::endl;  // Прошу ввести получателя
	std::cin >> to;
	std::cout << "Text message: ";  // Прошу ввести текст сообщения
	std::cin.ignore();
	getline(std::cin, text);  // Считываю текст сообщения с консоли

	if (!(to == "all" || getUserByLogin(to)))    // Если не удалось найти получателя по логину или в качестве адресата не указаны "все"
	{
		std::cout << "Error send message: can't find " << to << std::endl;  // Вывожу сообщение об ошибке
		return;
	}

	Message message(_currentUser->getUserLogin(), to, text);
	_sql.send_messages({ message });

	logger.write(std::stringstream() << time(0) << ": message \"" << message.getUserMessage() << "\" send to user "  // Записываю в файл логов время и сообщение о том, что сообщение следующего содержания от отправителя получателю
		<< message.getUserTo() << " from user " << message.getUserFrom());
}
