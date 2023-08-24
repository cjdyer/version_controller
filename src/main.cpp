#include "version_controller.h"
#include <iostream>
#include <string>

void print_usage(const char *name)
{
    std::cout << "Usage: " << std::endl;
    std::cout << name << " create <file_path> <message>" << std::endl;
    std::cout << name << " list" << std::endl;
    std::cout << name << " restore <index> <file_path>" << std::endl;
    std::cout << name << " diff <base_index> <alt_index>" << std::endl;
    std::cout << name << " branch <branch_name>" << std::endl;
    std::cout << name << " switch <branch_name>" << std::endl;
    std::cout << name << " merge <source_branch> <dest_branch>" << std::endl;
    std::cout << name << " --help" << std::endl;
}

int main(int argc, char **argv)
{
    VersionController vcs;

    if (argc < 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    std::string command = argv[1];

    if (command == "--help")
    {
        print_usage(argv[0]);
    }
    else if (command == "create" && argc == 4)
    {
        std::string file_path = argv[2];
        std::string message = argv[3];
        vcs.create(file_path, message);
    }
    else if (command == "list")
    {
        vcs.list();
    }
    else if (command == "restore" && argc == 4)
    {
        size_t index = std::stoi(argv[2]);
        std::string file_path = argv[3];
        vcs.restore(index, file_path);
    }
    else if (command == "diff" && argc == 4)
    {
        size_t base_index = std::stoul(argv[2]);
        size_t alt_index = std::stoul(argv[3]);
        vcs.diff(base_index, alt_index);
    }
    else if (command == "branch" && argc == 3)
    {
        std::string branch_name = argv[2];
        vcs.create_branch(branch_name);
    }
    else if (command == "switch" && argc == 3)
    {
        std::string branch_name = argv[2];
        vcs.switch_to_branch(branch_name);
    }
    else if (command == "merge" && argc == 4)
    {
        std::string source_branch = argv[2];
        std::string dest_branch = argv[3];
        vcs.merge_branch(source_branch, dest_branch);
    }
    else
    {
        std::cout << "Invalid command or wrong number of arguments." << std::endl;
        print_usage(argv[0]);
    }

    return 0;
}
