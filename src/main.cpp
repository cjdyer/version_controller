#include "snapshot_tracker.h"
#include <iostream>
#include <string>

void print_usage(const char *name)
{
    std::cout << "Usage: " << std::endl;
    std::cout << name << " create <file_path>" << std::endl;
    std::cout << name << " list" << std::endl;
    std::cout << name << " restore <index> <file_path>" << std::endl;
    std::cout << name << " --help" << std::endl;
}

int main(int argc, char **argv)
{
    SnapshotTracker vcs;

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
    else if (command == "create" && argc == 3)
    {
        std::string file_path = argv[2];
        vcs.create(file_path);
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
    else
    {
        std::cout << "Invalid command or wrong number of arguments." << std::endl;
        print_usage(argv[0]);
    }

    return 0;
}
