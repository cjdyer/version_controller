#include "file_manager.h"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

FileManager::FileManager()
{
    if (!fs::exists(".versions"))
    {
        fs::create_directory(".versions");
    }

    initialise_snapshot_index();
}

std::string FileManager::read_file(const std::string &file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

void FileManager::write_file(const std::string &file_path, const std::string &content)
{
    std::ofstream outfile(file_path, std::ios::binary);
    outfile.write(content.data(), content.size());
    outfile.close();
}

void FileManager::initialise_snapshot_index()
{
    size_t highest_index = 0;
    bool found_snapshots = false;
    for (const auto &entry : fs::directory_iterator(".versions"))
    {
        std::string filename = entry.path().filename().string();
        if (filename.find("snapshot_") == 0)
        {
            found_snapshots = true;
            size_t current_index = std::stoul(filename.substr(9));
            if (current_index > highest_index)
            {
                highest_index = current_index;
            }
        }
    }
    highest_snapshot_index = found_snapshots ? highest_index + 1 : 0;
}

std::string FileManager::get_next_snapshot_name()
{
    return ".versions/snapshot_" + std::to_string(highest_snapshot_index++);
}

std::vector<std::string> FileManager::get_snapshots()
{
    std::vector<std::string> snapshots;
    for (const auto &entry : fs::directory_iterator(".versions"))
    {
        snapshots.push_back(entry.path().filename().string());
    }
    return snapshots;
}
