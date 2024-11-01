#include <iostream>
#include <string>
#include <unordered_map>

#include "version_controller.h"

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

enum class Command {
    UNDEFINED = 0,
    HELP,
    CREATE,
    LIST,
    RESTORE,
    DIFF,
    BRANCH,
    SWITCH,
    MERGE,
};

Command string_to_command(const std::string &str)
{
    static const std::unordered_map<std::string, Command> command_map = {
        {"--help", Command::HELP},     {"create", Command::CREATE}, {"list", Command::LIST},
        {"restore", Command::RESTORE}, {"diff", Command::DIFF},     {"branch", Command::BRANCH},
        {"switch", Command::SWITCH},   {"merge", Command::MERGE},
    };

    auto it = command_map.find(str);
    return (it != command_map.end()) ? it->second : Command::UNDEFINED;
}

int main(int argc, char **argv)
{
    VersionController vcs;

    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    Command command = string_to_command(argv[1]);

    switch (command) {
        case Command::HELP:
            print_usage(argv[0]);
            return 0;
        case Command::CREATE: {
            if (argc != 4) {
                break;
            }

            std::string file_path = argv[2];
            std::string message = argv[3];
            vcs.create(file_path, message);
            return 0;
        }
        case Command::LIST:
            vcs.list();
            return 0;
        case Command::RESTORE: {
            if (argc != 4) {
                break;
            }

            size_t index = std::stoi(argv[2]);
            std::string file_path = argv[3];
            vcs.restore(index, file_path);
            return 0;
        }
        case Command::DIFF: {
            if (argc != 4) {
                break;
            }

            size_t base_index = std::stoul(argv[2]);
            size_t alt_index = std::stoul(argv[3]);
            vcs.diff(base_index, alt_index);
            return 0;
        }
        case Command::BRANCH: {
            if (argc != 3) {
                break;
            }

            std::string branch_name = argv[2];
            vcs.create_branch(branch_name);

            return 0;
        }
        case Command::SWITCH: {
            if (argc != 3) {
                break;
            }

            std::string branch_name = argv[2];
            vcs.switch_to_branch(branch_name);
            return 0;
        }
        case Command::MERGE: {
            if (argc != 4) {
                break;
            }

            std::string source_branch = argv[2];
            std::string dest_branch = argv[3];
            vcs.merge_branch(source_branch, dest_branch);
            return 0;
        }

        default:
            break;
    }

    std::cout << "Invalid command or wrong number of arguments." << std::endl;
    print_usage(argv[0]);
    return -1;
}
