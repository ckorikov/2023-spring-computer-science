#include "vcs.h"
#include "cli.h"

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

struct stat info;
std::vector<string> IGNORE {".git", "revisions"};

void write_file(string text) {
    std::ofstream outfile;
    outfile.open("Gfg.txt", std::ios_base::app);
    outfile << text; 
}

int create_revisions_directory(string text) {
    const char* path = text.c_str();

    if( stat( path, &info ) != 0 ) {
        if (mkdir(path, 0777) == -1)
        cerr << "Error :  " << strerror(errno) << endl;
    }
    else if( info.st_mode & S_IFDIR )  
        return 2;    
    else
        cout<<"that is not a directory";
    return 0;
}

void insert(string file_path);

string get_curr_time() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);      
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H-%M-%S");
    string timestamp = oss.str();
    return timestamp;   
}


int callback(void *param, int argc, char **argv, char **azColName) {
    std::string &s = *static_cast<std::string*>(param);
    int i;
    if ((!strcmp(azColName[0], "COUNT(*)")) && (atoi(argv[0]) == 0)) {
        string *flag = &((string*)param)[0];
        *flag = "1";
        return 1; 
        
    }
    printf("\n");
    return 0;
}

int get_db_files_callback(void *param, int argc, char **argv, char **azColName) {
    int i;
    std::vector<pair<string, int>> *s1 = (std::vector<pair<string, int>> *)param;
    string file_path = argv[0];
    int file_size = atoi(argv[1]);
    pair<string, int> tmp(file_path, file_size);
    s1->push_back(tmp);
    return 0;
}



 int get_latest_commit_id(void *param, int argc, char **argv, char **azColName ) {
    int *commit_id = (int*)param;
    *commit_id = atoi(argv[0]);
    return 0;
}

 int get_files_in_commit(void *param, int argc, char **argv, char **azColName) {
    std::vector<string> *s1 = (std::vector<string> *)param;
    for(int i = 0; i<argc; i++) {
        s1->push_back(argv[i]);
    }
    return 0;
}


int get_commits_info(void *param, int argc, char **argv, char **azColName) {
    vector<vector<string>> *s1 = (vector<vector<string>> *)param;
    vector<string> tmp;
    for(int i = 0; i<argc; i++) {
        tmp.push_back(argv[i]);
    }
    s1->push_back(tmp);
    return 0;
}


void listdir(std::vector<string> &files_ls) {
    std::string path = ".";
    for (const auto & file : recursive_directory_iterator(path)) {
        if (file.path().u8string().string::find(".git") == string::npos && file.path().u8string().string::find("revisions") == string::npos) {
            files_ls.push_back(file.path().u8string());
            }
        }
}

void insert(string file_path, sqlite3 *db) {
    char *errmessage = 0;
    int connection;
    connection = sqlite3_open("vcs.db", &db);   

    if (!connection) {
        string timestamp = get_curr_time();
        string query = "INSERT INTO files(file_path, created_at) VALUES ('" + file_path + "', '" + timestamp +"');";
        connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

        if( connection != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", errmessage);
            sqlite3_free(errmessage);
        } else {
            fprintf(stdout, "Records created successfully\n");
        }
    }
}

int create_table() {
    sqlite3 *db;
    char *errmessage = 0;
    int connection;
    connection = sqlite3_open("vcs.db", &db);

    if (!connection) {
        string query = "CREATE TABLE files (id INTEGER PRIMARY KEY DEFAULT 0, file_path VARCHAR(2048), created_at DATETIME DEFAULT(datetime('now', 'localtime')), file_size INTEGER);";
        connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

        if( connection != SQLITE_OK ){
            sqlite3_free(errmessage);
        } else {
            fprintf(stdout, "Files table created successfully\n");
        }
    }
    
    connection = sqlite3_open("vcs.db", &db);
    if (!connection){
        string query = "CREATE TABLE commits (id INTEGER PRIMARY KEY DEFAULT 0, message VARCHAR(256), branch VARCHAR(256),local_created_at DATETIME DEFAULT(datetime('now', 'localtime')), remote_created_at DATETIME NULL, num_files_changed INTEGER);";
        connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

        if( connection != SQLITE_OK ){
            sqlite3_free(errmessage);
        } else {
            fprintf(stdout, "Commit table created successfully\n");
        }
    }

    connection = sqlite3_open("vcs.db", &db);
    if (!connection) {
        string query = "CREATE TABLE file_revisions(id INTEGER PRIMARY KEY DEFAULT 0, file_path VARCHAR(1024),is_added BOOLEAN, is_removed BOOLEAN, is_modified BOOLEAN, commit_id INT, FOREIGN KEY(commit_id) REFERENCES commits(id) ON DELETE CASCADE);";
        connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

        if( connection != SQLITE_OK ){
            sqlite3_free(errmessage);
        } else {
            fprintf(stdout, "Revisions table created successfully\n");
        }
    }

    connection = sqlite3_open("vcs.db", &db);
    if (!connection) {
        string query = "CREATE TABLE commits_to_push (id INTEGER PRIMARY KEY DEFAULT 0, commit_id INT, FOREIGN KEY(commit_id) REFERENCES commits(id) ON DELETE CASCADE);";
        connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

        if( connection != SQLITE_OK ){
            sqlite3_free(errmessage);
        } else {
            fprintf(stdout, "Revisions table created successfully\n");
        }   
    }

    sqlite3_close(db);
    return 0;
}


    int file_revision(std::vector<string> file_paths, string revision_type) {
    if (file_paths.empty())
        return 0;
    create_revisions_directory(".revisions/files/");
    
    string tmp_file = "./.revisions/files/" + revision_type + ".txt";
    
    std::vector<string> _files_in_file;
    get_file_paths(_files_in_file, revision_type);
    
    for (int i=0; i<file_paths.size(); i++) {
        if (std::find(_files_in_file.begin(), _files_in_file.end(), file_paths[i]) == _files_in_file.end() == 0) {
            continue;
        }
        else {
            std::ofstream added;
            added.open(".revisions/files/" + revision_type +".txt", std::ios_base::app);
            added<<file_paths[i]<<endl;
            added.close();
        }
    }
    return 0;
}

void get_file_paths(std::vector<string> &_files, string revision_type){
    if (std::filesystem::exists(".revisions/files/" + revision_type + ".txt")) {
        std::ifstream _type (".revisions/files/" + revision_type + ".txt");
        string line;
        while(std::getline(_type,line)){
            _files.push_back(line);
        }
    }   
}
void status () {
    fs::remove_all(fs::path(".revisions/files/"));
    sqlite3 *db;
    char *errmessage = 0;
    int connection;
    connection = sqlite3_open("vcs.db", &db);

    std::vector<pair<string, int>>db_files; 
    std::vector<string> removed;
    std::vector<string> modified;

    if (!connection) {
        string query = "SELECT file_path, file_size FROM files;";

        connection = sqlite3_exec(db, query.c_str(), get_db_files_callback, &db_files, &errmessage);   
        if( connection != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", errmessage);
            sqlite3_free(errmessage);
        } else {
        
            std::vector<string> files_ls;
            listdir(files_ls);
            for(int i=0; i<db_files.size(); i++) {
                auto idx = std::find(files_ls.begin(), files_ls.end(), db_files[i].first);
                if (idx != files_ls.end()) {
                    std::filesystem::path p {files_ls[idx - files_ls.begin()]};
                    int size;
                    if (std::filesystem::is_directory(p)) {
                        size = 0;
                    } else {
                        size = std::filesystem::file_size(p);
                    }
                    if (db_files[i].second == size){
                        files_ls.erase(std::remove(files_ls.begin(), files_ls.end(), db_files[i].first), files_ls.end());
                    }
                    else {
                        files_ls.erase(std::remove(files_ls.begin(), files_ls.end(), db_files[i].first), files_ls.end());
                        modified.push_back(db_files[i].first);
                    }
                }
                else  
                    removed.push_back(db_files[i].first);
            }

            cout<<"Removed: "<<removed.size()<<endl;
            cout<<"Added: "<<files_ls.size()<<endl;
            cout<<"Modified: "<<modified.size()<<endl;
            file_revision(files_ls, "added");
            for(int i = 0; i < files_ls.size(); i++){
                cout<<"\n\u001b[32m[ADDED] "<<files_ls[i]<<"\u001b[0m";
            }
            file_revision(modified, "modified");
            for (int i = 0; i < modified.size(); i++){
                cout<<"\n\u001b[33m[MODIFIED]"<<modified[i]<<"\u001b[0m";     
            } 
            file_revision(removed, "removed");
            for (int i = 0; i < removed.size(); i++){
                cout<<"\n\u001b[31m[REMOVED]"<<removed[i]<<"\u001b[0m";
            }
        }
    } else {
        cout<<"could not open database";
    }
    cout<<endl;
    sqlite3_close(db);
}

int push_to_server() {
    sqlite3 *db;
    char *errmessage = 0;
    int connection;

    std::vector<string> commit_ids;
    connection = sqlite3_open("vcs.db", &db);
    int commit_id = 0;
    if (!connection) {
        string query = "SELECT commit_id from commits_to_push;";
        connection = sqlite3_exec(db, query.c_str(), get_files_in_commit, &commit_ids, &errmessage);     
        if( connection != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmessage);
                sqlite3_free(errmessage);
        } else {}
    }
    if (commit_ids.size() == 0) {
        cout<<"No commits to push"<<endl;
        return 0;
    }

    connection = sqlite3_open("vcs.db", &db);
    std::vector<string> file_revisions;
    for(int i=0; i<commit_ids.size(); i++){
        string query = "SELECT (file_path) from file_revisions WHERE commit_id = " +  commit_ids[i] + " AND is_removed IS NULL;";
        connection = sqlite3_exec(db, query.c_str(), get_files_in_commit, &file_revisions, &errmessage);
        if (connection != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", errmessage);
            sqlite3_free(errmessage);
        } else {
            for (int i = 0; i < file_revisions.size(); i++) 
                cout<<file_revisions[i]<<endl;
        }
    }
    
    for(int i=0; i<commit_ids.size(); i++){
        connection = sqlite3_open("vcs.db", &db);
        string query = "DELETE FROM commits_to_push WHERE commit_id = " + commit_ids[i] + ";";
        connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);
    }

    cout<<"\u001b[32mPushed successfully to server!\u001b[0m"<<endl;
    return 0;
}




