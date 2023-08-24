#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include <string>
#include <vector>

// More of a directory manager tbh
class FileManager
{
public:
    FileManager();

    std::string read_file(const std::string &file_path);
    void write_file(const std::string &file_path, const std::string &content);

    std::string get_next_snapshot_name();
    std::vector<std::string> get_snapshots();

private:
    size_t highest_snapshot_index;
    void initialise_snapshot_index();
};

#endif // __FILE_MANAGER_H__
