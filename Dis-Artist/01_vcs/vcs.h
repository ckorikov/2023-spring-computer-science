#ifndef VCH_H
#define VCH_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

using std::string;
using std::endl;
using std::cout;
using std::vector;

void commit(std::string message);
void show_log();
void checkout_commit_id(string commit_id);
void create_branch_and_checkout(string branch_name);
void display_branches();
void merge(string branch_2, string branch_1);
void display_help();

#endif