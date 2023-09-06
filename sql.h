#pragma once
#include <cstdint>  // uint*
#include <cstring>
#include <string>
#include <vector>
#include <mysql.h>
#include "message.h"

class Sql
{
public:
    Sql(std::string const& hostname, std::string const& username, std::string const& password, std::string const& database);
    ~Sql();

    // Функции для работы с таблицей 'messages'
    bool recv_messages(std::vector<Message>& messages);
    bool send_messages(std::vector<Message> const& messages);

    // Функции для работы с таблицей 'users'
    bool recv_users(std::vector<User>& users);
    bool send_users(std::vector<User> const& users);

private:
    MYSQL _mysql;
};
