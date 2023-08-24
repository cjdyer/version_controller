#include <fstream>
#include "snapshot_tracker.h"

SnapshotTracker::SnapshotTracker() : compressor(std::make_unique<LZ77Compression>()) {}

void SnapshotTracker::create(const std::string &file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (file.is_open())
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        std::vector<uint8_t> compressed_data = compressor->compress(content);
        versions.push_back(compressed_data);

        std::cout << "Snapshot created successfully." << std::endl;
    }
    else
    {
        std::cout << "Error opening file." << std::endl;
    }
}

void SnapshotTracker::list()
{
    std::cout << "There are " << versions.size() << " Snapshots" << std::endl;
}

void SnapshotTracker::restore(size_t index, const std::string &file_path)
{
    if (index >= 0 && index < versions.size())
    {
        // Decompress the data
        std::string decompressed_data = compressor->decompress(versions[index]);

        std::ofstream file(file_path, std::ios::binary);
        if (file.is_open())
        {
            file.write(decompressed_data.data(), decompressed_data.size());
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