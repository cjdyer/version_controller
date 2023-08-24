#ifndef __SNAPSHOT_TRACKER_H__
#define __SNAPSHOT_TRACKER_H__

#include <iostream>
#include <vector>
#include <string>

class SnapshotTracker
{
public:
    void create(const std::string &file_path);
    void list();
    void restore(size_t index, const std::string &file_path);

private:
    std::vector<std::string> versions;
};

#endif // __SNAPSHOT_TRACKER_H__