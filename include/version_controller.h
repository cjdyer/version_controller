#ifndef __VERSION_CONTROLLER_H__
#define __VERSION_CONTROLLER_H__

#include <string>
#include <memory>
#include "compression.h"
#include "file_manager.h"

class VersionController
{
public:
    VersionController();

    void create(const std::string &file_path, const std::string &message);
    void restore(size_t index, const std::string &file_path);
    void diff(size_t base_index, size_t alt_index);
    void list();

private:
    std::string load_snapshot(size_t index);

    FileManager file_manager;
    std::unique_ptr<Compression> compressor;
};

#endif // __VERSION_CONTROLLER_H__