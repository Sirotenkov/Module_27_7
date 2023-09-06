#pragma once
#include <string>
#include <iostream>
#include "sha1.h"

class User
{
private:
	const std::string _login;
	std::vector<uint> _password;  // Создаю стандартный контейнер vector для хранения данных о паролях пользователей

public:
	// Здесь в конструкторе указывается пароль в явном виде
	User(const std::string& login, const std::string password) :
		_login(login)
	{
		setUserPassword(password);
	}

	// Тут пароль уже в зашифрованном виде
	User(const std::string& login, const std::vector<uint>& sha_password) :
		_login(login)
		, _password(sha_password)
	{

	}

	~User() {

	}

	const std::string& getUserLogin() const
	{
		return _login;
	}

	const std::vector<uint>& getPassword() const
	{
		return _password;
	}

	// Вызывается в момент когда пароль ещё известен (т.е. пользователь только что зарегистрировался)
	// По итогу исполнения этой функции пароль будет зашифрован. Пароль в явном виде дальше нам больше будет не нужен
	void setUserPassword(const std::string& password)
	{
		auto const temp = sha1((char*)password.data(), password.size());
		_password = std::vector<uint>(temp, temp + SHA1HASHLENGTHUINTS);
		delete[] temp;
	}

	// Вызывется в момент загрузки пароля из базы данных, т.к. пароли в базах хранятся в зашифроанном виде,
	// сам пароль нам также не нужен, нужен только его SHA.
	void setUserPassword(const std::vector<uint>& password) {
		_password = password;
	}

};
