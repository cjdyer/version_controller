#ifndef __VERSION_CONTROLLER_H__
#define __VERSION_CONTROLLER_H__

#include <string>
#include <memory>
#include "compression.h"
#include "file_manager.h"
#include "branch_manager.h"

class VersionController
{
public:
    VersionController();

    // Commiting
    void create(const std::string &file_path, const std::string &message);
    void restore(size_t index, const std::string &file_path);
    void diff(size_t base_index, size_t alt_index);

    // Branching    
    void create_branch(const std::string &branch_name);
    void switch_to_branch(const std::string &branch_name);
    void merge_branch(const std::string &source_branch, const std::string &dest_branch);

    void list();

private:
    std::string load_snapshot(size_t index);
    void initialise_snapshot_index();

private:
    FileManager file_manager;
    BranchManager branch_manager;
    std::unique_ptr<Compression> compressor;

    uint16_t snapshot_index;
};

#endif // __VERSION_CONTROLLER_H__