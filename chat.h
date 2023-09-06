#pragma once
#include <vector>
#include <exception>
#include <memory>
#include "message.h"
#include "sha1.h"
#include "sql.h"
#include <cstring>

struct UserLoginExceptions : public std::exception  //
{                                                   //
	const char* what() const noexcept override      //
	{                                               //  обработка исключений
		return "Error! Login failure!";             //
	}                                               //
};                                                  //

class Chat
{
private:
	bool _isChatOpen = false;                       // переменна€-флаг, показывающа€, открыт чат или нет
	std::vector<User> _users;                       // контейнер пользователей vector
	//std::vector<Message> _messages;                 // контейнер сообщений vector
	User const* _currentUser = nullptr;             // указатель на текущего пользовател€
	Sql _sql;

	void login();                                   // объ€вление функции входа в систему
	void signUp();                                  // объ€вление функции регистрации пользовател€
	void showChat();                                // объ€вление функции отображени€ меню чата
	void showAllUsers();                            // объ€вление функции отображени€ всех пользователей системы
	void addMessage();                              // объ€вление функции добавлени€ сообщений

	std::vector<User>& getAllUsers()
	{
		return _users;
	}

	std::vector<Message> getAllMessages()
	{
		return { };
	}

	User const* getUserByLogin(const std::string& login) const;  // объ€вление функции возвращени€ указател€ на пользовател€ по логину

public:
	Chat() :
		_sql("localhost", "root", "root", "users")
	{

	}

	void start();  // объ€вление функции, показывающей работает чат или нет

	bool isChatOpen() const
	{
		return _isChatOpen;
	}

	User const* getCurrentUser() const  // метод, возвращающий указатель на текущего пользовател€
	{
		return _currentUser;
	}

	void showLoginMenu();                // метод, отображающий меню входа в систему

	void showUserMenu();                 // метод, отображающий пользовательское меню

	bool isEqualPassword(std::string password, uint const* hash)  // метод, сравнивающий хэши паролей дл€ валидации пользовател€
	{
		uint* passwordHash = sha1((char*)password.data(), password.size());  // переменна€ со значением хэша введенного парол€
		int res = memcmp(passwordHash, hash, SHA1HASHLENGTHUINTS);           // переменна€ со значением результата сравнени€ вычисленного хэша с хран€щимс€
		delete[] passwordHash;                                               // удаление хэша введенного парол€
		return res == 0;                                                     // если результат равен нулю, то сравниваемые значени€ совпали
	};
};
