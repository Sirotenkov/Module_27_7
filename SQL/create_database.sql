-- drop database users;
create database if not exists users;
use users;
create table if not exists users (login VARCHAR(255) PRIMARY KEY, password VARCHAR(255));
create table if not exists messages (login_from VARCHAR(255), login_to VARCHAR(255), text VARCHAR(8191));