cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(chat VERSION 1.0)

# add the executable
add_executable(chat main.cpp)

target_link_directories(chat PRIVATE "C:/Program Files/MySQL/MySQL Server 8.0/lib")
target_sources(chat PRIVATE chat.cpp sha1.cpp sql.cpp)
target_include_directories(chat PRIVATE ./ "C:/Program Files/MySQL/MySQL Server 8.0/include")
target_link_libraries(chat PRIVATE libmysql.lib)