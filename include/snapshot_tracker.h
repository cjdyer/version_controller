#ifndef __SNAPSHOT_TRACKER_H__
#define __SNAPSHOT_TRACKER_H__

#include <vector>
#include <string>
#include <memory>
#include "compression.h"

class SnapshotTracker
{
public:
    SnapshotTracker();

    void create(const std::string &file_path, const std::string &message);
    void restore(size_t index, const std::string &file_path);
    void diff(size_t base_index, size_t alt_index);
    void list();

private:
    void initialise_snapshot_index();
    std::string load_snapshot(size_t index);

private:
    size_t snapshot_index;
    std::unique_ptr<Compression> compressor;
};

#endif // __SNAPSHOT_TRACKER_H__