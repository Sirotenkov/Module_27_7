#pragma once
#include <string>
#include <iostream>
#include "sha1.h"

class User
{
private:
	const std::string _login;
	std::vector<uint> _password;  // ������ ����������� ��������� vector ��� �������� ������ � ������� �������������

public:
	// ����� � ������������ ����������� ������ � ����� ����
	User(const std::string& login, const std::string password) :
		_login(login)
	{
		setUserPassword(password);
	}

	// ��� ������ ��� � ������������� ����
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

	// ���������� � ������ ����� ������ ��� �������� (�.�. ������������ ������ ��� �����������������)
	// �� ����� ���������� ���� ������� ������ ����� ����������. ������ � ����� ���� ������ ��� ������ ����� �� �����
	void setUserPassword(const std::string& password)
	{
		auto const temp = sha1((char*)password.data(), password.size());
		_password = std::vector<uint>(temp, temp + SHA1HASHLENGTHUINTS);
		delete[] temp;
	}

	// ��������� � ������ �������� ������ �� ���� ������, �.�. ������ � ����� �������� � ������������ ����,
	// ��� ������ ��� ����� �� �����, ����� ������ ��� SHA.
	void setUserPassword(const std::vector<uint>& password) {
		_password = password;
	}

};
