#include <exception>
//#include <format>  // Одно из направлений доработки данной программы в будущем, для более удобного форматирования.
#include <sstream>
#include "sql.h"

Sql::Sql(std::string const& hostname, std::string const& username, std::string const& password, std::string const& database) {
	int i = 0;

	// Получаем дескриптор соединения
	mysql_init(&_mysql);
	if (&_mysql == nullptr) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	// Подключаемся к серверу
	if (!mysql_real_connect(&_mysql, hostname.c_str(), username.c_str(), password.c_str(), database.c_str(), NULL, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		throw std::exception("Error: can't connect to database");
	}

	mysql_set_character_set(&_mysql, "utf8");

	//Смотрим изменилась ли кодировка на нужную, по умолчанию идёт latin1
	//cout << "connection characterset: " << mysql_character_set_name(&mysql) << endl;
}

Sql::~Sql() {
	// Закрываем соединение с сервером базы данных
	mysql_close(&_mysql);
}

bool Sql::recv_messages(std::vector<Message>& messages) {
	MYSQL_RES* res;
	MYSQL_ROW row;
	auto success = true;

	mysql_query(&_mysql, "SELECT * FROM messages"); //Делаем запрос к таблице

	//Выводим все что есть в таблице через цикл
	if (res = mysql_store_result(&_mysql)) {
		while (row = mysql_fetch_row(res)) {
			if (mysql_num_fields(res) >= 3) {
				Message message(row[0], row[1], row[2]);
				messages.push_back(message);
			}
		}
	}
	else {
		success = false;
	}

	return success;
}

bool Sql::send_messages(std::vector<Message> const& messages) {
	auto success = true;
	for (auto const& message : messages) {
		/*std::string const query = std::format("INSERT INTO messages(login_from, login_to, text) values({}, {}, {})",   // в этом месте планирую использовать <format>
			message.getUserFrom(), message.getUserTo(), message.getUserMessage());*/
		std::stringstream query;
		query << "INSERT INTO messages(login_from, login_to, text) values("
			<< "'" << message.getUserFrom() << "', "
			<< "'" << message.getUserTo() << "', "
			<< "'" << message.getUserMessage() << "')";
		if (mysql_query(&_mysql, query.str().c_str()) != 0)
			success = false;
	}
	return success;
}

bool Sql::recv_users(std::vector<User>& users) {
	MYSQL_RES* res;
	MYSQL_ROW row;
	auto success = true;

	mysql_query(&_mysql, "SELECT * FROM users"); //Делаем запрос к таблице

	//Выводим все что есть в таблице через цикл
	if (res = mysql_store_result(&_mysql)) {
		while (row = mysql_fetch_row(res)) {
			if (mysql_num_fields(res) >= 2) {
				try {
					std::vector<uint> password;
					char* buf = row[1];

					for (auto i = 0; i < 5; ++i) {
						password.push_back(strtoul(buf, &buf, 16));
					}

					User user(row[0], password);
					users.push_back(user);
				}
				catch (std::exception& ex) {
					//std::cout << ex.what() << std::endl;
				}
			}
		}
	}
	else {
		success = false;
	}

	return success;
}

bool Sql::send_users(std::vector<User> const& users) {
	auto success = true;
	for (auto const& user : users) {

		auto const& sha = user.getPassword();
		char buf[56] = { };
		sprintf(buf, "%08x %08x %08x %08x %08x", sha[0], sha[1], sha[2], sha[3], sha[4]);

		std::stringstream query;
		query << "INSERT INTO users(login, password) values("
			<< "'" << user.getUserLogin() << "', "
			<< "'" << buf << "')";
		if (mysql_query(&_mysql, query.str().c_str()) != 0)
			success = false;
	}
	return success;
}
