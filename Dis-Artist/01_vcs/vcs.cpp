#include "vcs.h"
#include "cli.h"

void commit(std::string message) {
    create_revisions_directory(".revisions/commits/");
    status();

    if (!fs::exists(fs::path(".revisions/files/"))) {
        cout<<"Nothing to commit!\n";
        return;
    }

    sqlite3 *db;
    char *errmessage = 0;
    int connection;
    connection = sqlite3_open("vcs.db", &db);
    std::vector<string> added_files;
    get_file_paths(added_files, "added");

    if (!connection) {

        for(int i = 0; i < added_files.size(); i++) {
            std::filesystem::path p{added_files[i]};
            string size;
            if (std::filesystem::is_directory(p)) {
                size = "0";
            }
            else {
                size = to_string(std::filesystem::file_size(p));
            }
            string query = "INSERT INTO files(file_path, file_size) VALUES ('" + added_files[i] + "', '" + size + "');";
            connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

            if( connection != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmessage);
                sqlite3_free(errmessage);
            } else {
            }
        }
    }
    else {
        cout<<"could not open database";
    }

    connection = sqlite3_open("vcs.db", &db);
    std::vector<string> modified_files;
    get_file_paths(modified_files, "modified");
    if (!connection) {

        for(int i = 0; i < modified_files.size(); i++) {
            std::filesystem::path p{modified_files[i]};
            string size;
            if (std::filesystem::is_directory(p)) {
                size = "0";
            }
            else {
                size = to_string(std::filesystem::file_size(p));
            }
            string query = "UPDATE files SET file_size = " + size + " WHERE file_path='" + modified_files[i] + "';";
            connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

            if( connection != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmessage);
                sqlite3_free(errmessage);
            } else {
            }
        }
    }
    else {
        cout<<"could not open database";
    }

    std::vector<std::string> removed_files;
    get_file_paths(removed_files, "removed");
    if (!connection) {

        for(int i = 0; i < removed_files.size(); i++) {
            string query = "DELETE FROM files WHERE file_path='" + removed_files[i] + "';";

            connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

            if( connection != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmessage);
                sqlite3_free(errmessage);
            } else {
            }
        }
    }
    else {
        cout<<"could not open database";
    }

    std::ifstream file(".revisions/HEAD");
    string current_branch;
    std::getline(file, current_branch);
    file.close();

    connection = sqlite3_open("vcs.db", &db);
    if (!connection) {
        string query = "INSERT INTO commits(message, num_files_changed, branch) VALUES('" + message + "', " + to_string(modified_files.size() + added_files.size() + removed_files.size()) + ", '" + current_branch + "');";
        connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);
        if( connection != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmessage);
                sqlite3_free(errmessage);
        } else {
        }
    }

    connection = sqlite3_open("vcs.db", &db);
    int commit_id = 0;
    if (!connection) {
        string query = "SELECT id from commits ORDER BY local_created_at DESC LIMIT 1;";
        connection = sqlite3_exec(db, query.c_str(), get_latest_commit_id, &commit_id, &errmessage);     
        cout<<"Latest commit id: "<<commit_id<<endl;
        if( connection != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmessage);
                sqlite3_free(errmessage);
        } else {
        }
    }

    connection = sqlite3_open("vcs.db", &db);
    if (!connection) {
        for(int i = 0; i < added_files.size(); i++) {
            string query = "INSERT INTO file_revisions(file_path, is_added, commit_id) VALUES ('" + added_files[i] + "',true, " + to_string(commit_id) + ")";
            connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

            if( connection != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmessage);
                sqlite3_free(errmessage);
            } else {
            }
        }
    }

    connection = sqlite3_open("vcs.db", &db);
    if (!connection) {
        for(int i = 0; i < modified_files.size(); i++) {
            string query = "INSERT INTO file_revisions(file_path, is_modified, commit_id) VALUES ('" + modified_files[i] + "',true, " + to_string(commit_id) + ")";
            connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

            if( connection != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmessage);
                sqlite3_free(errmessage);
            } else {
            }
        }
    }

    connection = sqlite3_open("vcs.db", &db);
    if (!connection) {
        for(int i = 0; i < removed_files.size(); i++) {
            string query = "INSERT INTO file_revisions(file_path, is_removed, commit_id) VALUES ('" + removed_files[i] + "',true, " + to_string(commit_id) + ")";
            connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

            if( connection != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmessage);
                sqlite3_free(errmessage);
            } else {
            }
        }
    }
     
    connection = sqlite3_open("vcs.db", &db);
    if (!connection) {
        string query = "INSERT INTO commits_to_push(commit_id) VALUES (" + to_string(commit_id) + ");";
        connection = sqlite3_exec(db, query.c_str(), callback, 0, &errmessage);   

        if( connection != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", errmessage);
            sqlite3_free(errmessage);
        } else {
        }
    }

    sqlite3_close(db);
    std::filesystem::remove(".revisions/added.txt");

    if (!fs::exists(".revisions/commits/" + current_branch))
        fs::create_directory(".revisions/commits/" + current_branch);
    string commit_path = ".revisions/commits/" + current_branch + "/" +  to_string(commit_id) + "/";
    create_revisions_directory(commit_path);
    added_files.insert(added_files.end(), modified_files.begin(), modified_files.end());
    std::vector<string> files_in_commit = added_files;
    

    fs::copy_options copyOptions = fs::copy_options::update_existing | fs::copy_options::recursive;
    for(int i = 0; i < files_in_commit.size(); i++) {
        try {
            auto file_path = std::filesystem::path(commit_path + files_in_commit[i]);
            std::filesystem::copy(files_in_commit[i], file_path, copyOptions);
        }
        catch(int i) {
            cout<<"Exception: "<<i<<endl;
        }
    }

    if (removed_files.size() > 0) {
        std::ofstream deleted_files;
        deleted_files.open(commit_path + "/deleted.txt", std::ios_base::app);
        for (int i = 0; i < removed_files.size(); i++) {
            deleted_files << removed_files[i] <<endl;
        }
        deleted_files.close();
    }


    fs::remove_all(fs::path(".revisions/files"));

    if (!fs::exists(".revisions/branches/" + current_branch))
        fs::create_directories(".revisions/branches/" + current_branch);
    std::ofstream head_file(".revisions/branches/" + current_branch + "/HEAD");
    head_file << commit_id;
    head_file.close();
}

void show_log() {
    sqlite3 *db;
    char *errmessage = 0;
    int connection;
    int n = 1;
    cout<<"Enter number of commits to see log of: ";
    cin>>n;
    std::ifstream file(".revisions/HEAD");
    string current_branch;
    std::getline(file, current_branch);
    file.close();

    connection = sqlite3_open("vcs.db", &db);
    vector<vector<string>> commit_info;
    if (!connection) {
        string query = "SELECT id, message, local_created_at from commits WHERE branch='" + current_branch + "' ORDER BY local_created_at DESC LIMIT " + to_string(n) +  ";";
        connection = sqlite3_exec(db, query.c_str(), get_commits_info, &commit_info, &errmessage);     
        if( connection != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmessage);
                sqlite3_free(errmessage);
        } else {}
    }
    for(int i=0; i<commit_info.size(); i++){
        cout<<"--------------------------\n";
        cout<<"Commit ID: "<<commit_info[i][0]<<endl;
        cout<<"Message: "<<commit_info[i][1]<<endl;
        cout<<"Timestamp: "<<commit_info[i][2]<<endl;
    }
    cout<<"Branch: "<<current_branch<<endl;
}
void checkout_commit_id(string commit_id){

    fs::copy_options copyOptions = fs::copy_options::update_existing | fs::copy_options::recursive;
    for(int i = 1; i < stoi(commit_id)+1; i++) {
        try {
            std::filesystem::copy(fs::path(".revisions/commits/" + to_string(i) + "/"), fs::path("./tmp"), copyOptions);
            if (fs::exists("./tmp/deleted.txt")){
                cout<<"exists"<<endl;
                std::ifstream tmp_file("./tmp/deleted.txt");
                string line;
                while(std::getline(tmp_file,line)){
                    fs::remove("./tmp/" + line);
                }
            }
            fs::remove("./tmp/deleted.txt");
        }
        catch(int i) {
            cout<<"Exception: "<<i<<endl;
        }
    }
}
void create_branch_and_checkout(string branch_name) {
    if (fs::exists(fs::path(".revisions/files"))) {
        cout<<"Commit your changes before creating branch!\n";
        return;
    } else {
        if (!fs::exists(".revisions/branches/" + branch_name)){
            fs::create_directory(".revisions/branches/" + branch_name);
            std::ofstream branches;
            branches.open(".revisions/branches/branches.txt", std::ios_base::app);
            branches << branch_name<<endl;
            branches.close();
            std::ofstream head;
            head.open(".revisions/HEAD", std::ios::trunc);
            head << branch_name << endl;
            head.close();
            cout<<"Created branch "<<branch_name<<"\n";        
            return;
        } else {
            std::ofstream head;
            head.open(".revisions/HEAD", std::ios::trunc);
            head << branch_name << endl;
            head.close();
            return;
        }
    }
}


void display_branches() {
    status();
    std::ifstream file(".revisions/HEAD");
    string current_branch;
    std::getline(file, current_branch);
    file.close();

    std::ifstream branches(".revisions/branches/branches.txt");
    string line;
    cout<<"Branches:\n";
    while(std::getline(branches,line)) {
        if(strcmp(line.c_str(), current_branch.c_str()) == 0) {
            cout<<"* "<<line<<endl;
        } else {
            cout<<line<<endl;
        }
    }
    branches.close();
}

void merge(string branch_2, string branch_1) {
    fs::copy_options copyOptions = fs::copy_options::update_existing | fs::copy_options::recursive;
    fs::copy(".revisions/commits/" + branch_2, ".revisions/commits/" + branch_1, copyOptions);
    cout<<"Merged commits!"<<endl;
}

void display_help() {
    cout<<"Usage: ./main.out [--help] [status] [commit <message>] [push] [checkout <commit_id>] [log]\n\n";
    cout<<"\t Status: Prints current status of files in repository\n";
    cout<<"\t Commit: Records the revised files to local storage\n";
    cout<<"\t Push: Pushes the file changes to remote server\n";
    cout<<"\t Log: Displays the log messages for previous commits\n";
    cout<<"\t Checkout: Go back to some particular commit\n";
}