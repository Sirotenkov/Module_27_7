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
	Logger(std::string const& filename) {                                    // в конструкторе
		_file.open(filename, std::ios::out | std::ios::in | std::ios::app);  // создаю (или открываю, если уже создан) файл с именем filename на запись в конец файла
		if (!_file.is_open()) {                                              // если файл не открылся (не удалось создать)
			throw std::exception("Failed to open file!");                    // кидаю исключение
		}
	}

	~Logger() {         // в деструкторе
		_file.close();  // закрываю файл
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

		//ifstream file(filename);  // файл из которого считываются строки логов
		//if (!file.is_open())
		//	return { };

		_mutex.lock();
		_file.seekg(0, std::ios::beg);

		while (true) {
			string s;                   // переменная для хранения считанной из файла логов строки


			getline(_file, s);


			if (!_file)
				break;

			ss << s << std::endl;
		}
		_mutex.unlock();

		return ss;

		/*while (getline(file, s)) {  // пока не достигнут конец файла добавлять очередную строку в переменную (s)
			_mutex.lock();
			cout << s << endl;      // вывод на экран
			s += "\n";              // перевод на новую строку
			cout << s << endl;      // дальнейший вывод информации на экран (без записи в файл)
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
		string s;                   // переменная для хранения считанных из файла логов строк
		ifstream file("chat.log");  // файл из которого считываются строки логов

		while (getline(file, s)) {  // пока не достигнут конец файла добавлять очередную строку в переменную (s)
			_mutex.lock();
			cout << s << endl;      // вывод на экран
			s += "\n";              // перевод на новую строку
			cout << s << endl;      // дальнейший вывод информации на экран (без записи в файл)
			_mutex.unlock();
		}
	}*/

private:
	std::shared_mutex _mutex;
	std::fstream _file;
};
