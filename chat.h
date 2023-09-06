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
	{                                               //  ��������� ����������
		return "Error! Login failure!";             //
	}                                               //
};                                                  //

class Chat
{
private:
	bool _isChatOpen = false;                       // ����������-����, ������������, ������ ��� ��� ���
	std::vector<User> _users;                       // ��������� ������������� vector
	//std::vector<Message> _messages;                 // ��������� ��������� vector
	User const* _currentUser = nullptr;             // ��������� �� �������� ������������
	Sql _sql;

	void login();                                   // ���������� ������� ����� � �������
	void signUp();                                  // ���������� ������� ����������� ������������
	void showChat();                                // ���������� ������� ����������� ���� ����
	void showAllUsers();                            // ���������� ������� ����������� ���� ������������� �������
	void addMessage();                              // ���������� ������� ���������� ���������

	std::vector<User>& getAllUsers()
	{
		return _users;
	}

	std::vector<Message> getAllMessages()
	{
		return { };
	}

	User const* getUserByLogin(const std::string& login) const;  // ���������� ������� ����������� ��������� �� ������������ �� ������

public:
	Chat() :
		_sql("localhost", "root", "root", "users")
	{

	}

	void start();  // ���������� �������, ������������ �������� ��� ��� ���

	bool isChatOpen() const
	{
		return _isChatOpen;
	}

	User const* getCurrentUser() const  // �����, ������������ ��������� �� �������� ������������
	{
		return _currentUser;
	}

	void showLoginMenu();                // �����, ������������ ���� ����� � �������

	void showUserMenu();                 // �����, ������������ ���������������� ����

	bool isEqualPassword(std::string password, uint const* hash)  // �����, ������������ ���� ������� ��� ��������� ������������
	{
		uint* passwordHash = sha1((char*)password.data(), password.size());  // ���������� �� ��������� ���� ���������� ������
		int res = memcmp(passwordHash, hash, SHA1HASHLENGTHUINTS);           // ���������� �� ��������� ���������� ��������� ������������ ���� � ����������
		delete[] passwordHash;                                               // �������� ���� ���������� ������
		return res == 0;                                                     // ���� ��������� ����� ����, �� ������������ �������� �������
	};
};
