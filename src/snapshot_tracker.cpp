#include <iostream>
#include <fstream>
#include <filesystem>

#include "snapshot_tracker.h"

namespace fs = std::filesystem;

SnapshotTracker::SnapshotTracker() : compressor(std::make_unique<LZ77Compression>())
{
    // Ensure the .versions directory exists
    if (!fs::exists(".versions"))
    {
        fs::create_directory(".versions");
    }

    initialise_snapshot_index();
}

void SnapshotTracker::initialise_snapshot_index()
{
    size_t highest_index = 0;
    bool found_snapshots = false;

    for (const auto &entry : fs::directory_iterator(".versions"))
    {
        std::string filename = entry.path().filename().string();
        if (filename.find("snapshot_") == 0)
        {
            found_snapshots = true;
            // Extract the index from "snapshot_X" format
            size_t current_index = std::stoul(filename.substr(9));
            if (current_index > highest_index)
            {
                highest_index = current_index;
            }
        }
    }

    snapshot_index = found_snapshots ? highest_index + 1 : 0;
}

void SnapshotTracker::create(const std::string &file_path, const std::string &message)
{
    // Read the content from the file
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return;
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Compress the content
    std::vector<uint8_t> compressed_content = compressor->compress(content);

    // Save the compressed content along with the message
    std::string version_file = ".versions/snapshot_" + std::to_string(snapshot_index);
    std::ofstream outfile(version_file, std::ios::binary);

    outfile << message << "\n";
    outfile.write((char *)&compressed_content[0], compressed_content.size());
    outfile.close();

    std::cout << "Snapshot created and saved to: " << version_file << std::endl;
}

void SnapshotTracker::list()
{
    std::cout << "Snapshots:" << std::endl;
    for (const auto &entry : fs::directory_iterator(".versions"))
    {
        std::ifstream infile(entry.path(), std::ios::binary);
        std::string message;

        std::getline(infile, message);
        std::cout << entry.path().filename().string() << " - " << message << std::endl;
        infile.close();
    }
}

void SnapshotTracker::restore(size_t index, const std::string &file_path)
{
    std::string version_file = ".versions/snapshot_" + std::to_string(index);
    if (!fs::exists(version_file))
    {
        std::cerr << "Snapshot " << index << " does not exist." << std::endl;
        return;
    }

    std::ifstream infile(version_file, std::ios::binary);

    // Skip the commit message
    std::string message;
    std::getline(infile, message);

    // Read the compressed content
    std::vector<uint8_t> compressed_content((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());

    infile.close();

    // Decompress the content
    std::string decompressed_content = compressor->decompress(compressed_content);

    // Write the decompressed content back to the original file
    std::ofstream outfile(file_path, std::ios::binary);
    outfile.write(decompressed_content.data(), decompressed_content.size());
    outfile.close();

    std::cout << "Restored snapshot " << index << " to " << file_path << std::endl;
}
