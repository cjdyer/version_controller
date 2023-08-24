#include <fstream>
#include "snapshot_tracker.h"

void SnapshotTracker::create(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (file.is_open())
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        versions.push_back(content);
        std::cout << "Snapshot created successfully." << std::endl;
    }
    else
    {
        std::cout << "Error opening file." << std::endl;
    }
}

void SnapshotTracker::list()
{
    std::cout << "Listing all snapshots:" << std::endl;
    for (size_t i = 0; i < versions.size(); ++i)
    {
        std::cout << i << ": " << (versions[i].substr(0, 30) + "...") << std::endl;
    }
}

void SnapshotTracker::restore(size_t index, const std::string &file_path)
{
    if (index >= 0 && index < versions.size())
    {
        std::ofstream file(file_path);
        if (file.is_open())
        {
            file << versions[index];
            std::cout << "Snapshot restored successfully." << std::endl;
        }
        else
        {
            std::cout << "Error writing to file." << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid snapshot index." << std::endl;
    }
}
