#ifndef __BRANCH_MANAGER_H__
#define __BRANCH_MANAGER_H__

#include <map>
#include <string>
#include <vector>

class BranchManager
{
   public:
    BranchManager();

    void create_branch(const std::string &branch_name);
    void switch_to_branch(const std::string &branch_name);
    void merge_branch(const std::string &source_branch, const std::string &dest_branch);

    std::string get_current_branch_name();
    size_t get_current_branch_snapshot_index();

    void add_snapshot();

   private:
    struct Branch {
        ssize_t latest_snapshot;
    };

    void load_branches();
    void save_branches();

    std::map<std::string, Branch> branches;
    std::string current_branch;
};

#endif  // __BRANCH_MANAGER_H__
