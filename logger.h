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
	}

private:
	std::shared_mutex _mutex;
	std::fstream _file;
};
