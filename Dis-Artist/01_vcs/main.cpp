#include "vcs.h"
#include "cli.h"


bool check_args(const std::string &value, const std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

int main(int argc, char* argv[])
{
    std::vector<std::string> commands {"status", "commit", "push", "remove", "log", "--help", "checkout", "branch", "merge"};
    create_table();
    if (!fs::exists(".revisions")) {
        create_revisions_directory(".revisions");
        std::ofstream head(".revisions/HEAD");
        head << "main";
        head.close();
        create_revisions_directory(".revisions/branches/");
        std::ofstream branches(".revisions/branches/branches.txt");
        branches << "main\n";
        branches.close();
    }

    if (argc == 1) {
        cout<<"Incorrect usage. Run ./a.out <add/commit/push>. Run ./main.out --help for more information."<<endl;
        return 1;
    }

    if (check_args(argv[1], commands)) {
        if (strcmp(argv[1], "commit") == 0){
            if (argc != 3) {
                cout<<"Invalid format: Enter ./main.out commit <message> "<<endl;
                return -1;
            }
            commit(argv[2]);
        }
        if (strcmp(argv[1], "status") == 0)
            status();
        if (strcmp(argv[1], "log") == 0)
            show_log();
        if (strcmp(argv[1], "push") == 0)
            push_to_server();
        if (strcmp(argv[1], "--help") == 0)
            display_help();
        if (strcmp(argv[1], "checkout") == 0){
            if (!(2<argc<4)) {
                cout<<"Invalid format: Enter ./main.out checkout <commit_id> or ./main.out checkout -b <branch_name>"<<endl;
                return -1;
            }
            if (strcmp(argv[2], "-b")!=0)
                checkout_commit_id(argv[2]);
            else {
                create_branch_and_checkout(argv[3]);
            }
        }
        if ((strcmp(argv[1], "branch") == 0)) {
            display_branches();
        }
        if (strcmp(argv[1], "merge") == 0){
            if (argc != 4)
                cout<<"Invalid format: Enter ./main.out merge <branch2> <branch1>"<<endl;
            else {
                merge(argv[2], argv[3]);
            }
        }
    }
    else {
        cout<<"Unknown command: "<<argv[1]<<endl<<"Run ./a.out <add/commit/push>"<<endl;
    }
    return 0;
}

