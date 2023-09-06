#pragma once
#include <fstream>
#include <string>
#include <exception>
#include <ostream>
#include <chrono>
#include <ctime>
#include <shared_mutex>
#include <sstream>  // stringstream

class Logger {
public:
	Logger(std::string const& filename) {                                    // � ������������
		_file.open(filename, std::ios::out | std::ios::in | std::ios::app);  // ������ (��� ��������, ���� ��� ������) ���� � ������ filename �� ������ � ����� �����
		if (!_file.is_open()) {                                              // ���� ���� �� �������� (�� ������� �������)
			throw std::exception("Failed to open file!");                    // ����� ����������
		}
	}

	~Logger() {         // � �����������
		_file.close();  // �������� ����
	}

	// Logger log;
	// log << ""hello" << 45 << 'a';
	/*Logger& operator<<(std::string const& value);
	Logger& operator<<(char const* value);
	Logger& operator<<(int value);
	Logger& operator<<(char value);*/

	void write(std::stringstream const& value) {
		_mutex.lock();
		_file << value.str() << std::endl;
		_mutex.unlock();
	}

	void write(std::string const& value) {
		_mutex.lock();
		_file << value << std::endl;
		_mutex.unlock();
	}

	std::stringstream read() {
		std::stringstream ss;

		//ifstream file(filename);  // ���� �� �������� ����������� ������ �����
		//if (!file.is_open())
		//	return { };

		_mutex.lock();
		_file.seekg(0, std::ios::beg);

		while (true) {
			string s;                   // ���������� ��� �������� ��������� �� ����� ����� ������


			getline(_file, s);


			if (!_file)
				break;

			ss << s << std::endl;
		}
		_mutex.unlock();

		return ss;

		/*while (getline(file, s)) {  // ���� �� ��������� ����� ����� ��������� ��������� ������ � ���������� (s)
			_mutex.lock();
			cout << s << endl;      // ����� �� �����
			s += "\n";              // ������� �� ����� ������
			cout << s << endl;      // ���������� ����� ���������� �� ����� (��� ������ � ����)
			_mutex.unlock();
		}*/
	}

	/*template<typename ty>
	Logger& operator<<(ty const& value) {
		_mutex.lock();
		_fout << value;
		_mutex.unlock();
		return *this;
	}*/

	/*void printLogFile() {
		string s;                   // ���������� ��� �������� ��������� �� ����� ����� �����
		ifstream file("chat.log");  // ���� �� �������� ����������� ������ �����

		while (getline(file, s)) {  // ���� �� ��������� ����� ����� ��������� ��������� ������ � ���������� (s)
			_mutex.lock();
			cout << s << endl;      // ����� �� �����
			s += "\n";              // ������� �� ����� ������
			cout << s << endl;      // ���������� ����� ���������� �� ����� (��� ������ � ����)
			_mutex.unlock();
		}
	}*/

private:
	std::shared_mutex _mutex;
	std::fstream _file;
};
