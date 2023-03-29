#ifndef CLI_H
#define CLI_H

#include <vector>
#include <string>
#include <sqlite3.h>
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <cstring>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::fstream;
using std::ios;
using std::filesystem::recursive_directory_iterator;
using std::to_string;
using std::pair;
using std::filesystem::directory_iterator;
namespace fs = std::filesystem;

void status();
void write_file(string text);
int create_revisions_directory(string text);
void insert(string file_path);
int callback(void *param, int argc, char **argv, char **azColName);
int get_db_files_callback(void *param, int argc, char **argv, char **azColName);
int get_latest_commit_id(void *param, int argc, char **argv, char **azColName );
int get_files_in_commit(void *param, int argc, char **argv, char **azColName);
int get_commits_info(void *param, int argc, char **argv, char **azColName);
void listdir(std::vector<string> &files_ls);
void insert(string file_path, sqlite3 *db);
int create_table();
int file_revision(std::vector<string> file_paths, string revision_type);
void get_file_paths(std::vector<string> &_files, string revision_type);
int push_to_server();



#endif