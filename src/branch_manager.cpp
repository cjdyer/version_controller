#include "branch_manager.h"
#include <fstream>
#include <iostream>

BranchManager::BranchManager() 
{
    load_branches();

    // Check if the master branch exists, if not, create it
    if (branches.find("master") == branches.end()) {
        // start from snapshot -1 to make the first commit at index 0, this is a bit wacky...
        branches["master"] = {-1};  
    }

    // Try to load current branch, default to "master" if not found
    std::ifstream current_branch_file(".versions/.current_branch");
    if (current_branch_file) {
        std::getline(current_branch_file, current_branch);
        current_branch_file.close();
    } else {
        current_branch = "master";
    }
}

void BranchManager::create_branch(const std::string &branch_name) 
{
    if (branches.find(branch_name) != branches.end()) {
        std::cerr << "Branch " << branch_name << " already exists." << std::endl;
        return;
    }
    branches[branch_name] = {branches[current_branch].latest_snapshot};
    save_branches();
}

void BranchManager::switch_to_branch(const std::string &branch_name) 
{
    if (branches.find(branch_name) == branches.end()) {
        std::cerr << "Branch " << branch_name << " does not exist." << std::endl;
        return;
    }

    current_branch = branch_name;
    std::ofstream current_branch_file(".versions/.current_branch");
    current_branch_file << current_branch;
    current_branch_file.close();
}

void BranchManager::merge_branch(const std::string &source_branch, const std::string &dest_branch) 
{
    // For simplicity, I've just set the destination branch's latest snapshot to the source's snapshot
    // A real implementation might involve three-way merges, conflict resolution, etc.
    if (branches.find(source_branch) == branches.end() || branches.find(dest_branch) == branches.end()) {
        std::cerr << "One or both branches do not exist." << std::endl;
        return;
    }
    branches[dest_branch].latest_snapshot = branches[source_branch].latest_snapshot;
    save_branches();
}

size_t BranchManager::get_current_branch_snapshot_index() 
{
    return branches[current_branch].latest_snapshot;
}

std::string BranchManager::get_current_branch_name() 
{
    return current_branch;
}

void BranchManager::load_branches() 
{
    std::ifstream branches_file(".versions/.branches");
    std::string line;
    while (std::getline(branches_file, line)) {
        size_t delimiter_pos = line.find(":");
        std::string branch_name = line.substr(0, delimiter_pos);
        ssize_t snapshot_index = std::stol(line.substr(delimiter_pos + 1));
        branches[branch_name] = {snapshot_index};
    }
    branches_file.close();
}

void BranchManager::save_branches() 
{
    std::ofstream branches_file(".versions/.branches");
    for (const auto &pair : branches) {
        branches_file << pair.first << ":" << pair.second.latest_snapshot << "\n";
    }
    branches_file.close();
}

void BranchManager::add_snapshot() 
{
    branches[current_branch].latest_snapshot++;
    save_branches();
}