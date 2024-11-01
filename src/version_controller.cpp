#include "version_controller.h"

#include <iostream>
#include <sstream>

VersionController::VersionController() : compressor(get_compression_algorithm(m_compression_type)) {}

void VersionController::create(const std::string &file_path, const std::string &message)
{
    // Read the content from the file
    std::string content = file_manager.read_file(file_path);

    // Compress the content
    std::vector<uint8_t> compressed_content = compressor->compress(content);

    // Save the compressed content along with the message
    std::string version_file = file_manager.get_next_snapshot_name();
    file_manager.write_file(
        version_file,
        message + "\n" + std::string(compressed_content.begin(), compressed_content.end()));

    // Update the branch manager
    branch_manager.add_snapshot();

    std::cout << "Snapshot created and saved to: " << version_file << std::endl;
}

void VersionController::restore(size_t index, const std::string &file_path)
{
    // Decompress the content
    std::string decompressed_content = load_snapshot(index);

    // Write the decompressed content back to the original file
    file_manager.write_file(file_path, decompressed_content);

    std::cout << "Restored snapshot " << index << " to " << file_path << std::endl;
}

void VersionController::diff(size_t base_index, size_t alt_index)
{
    std::string base_content = load_snapshot(base_index);
    std::string alt_content = load_snapshot(alt_index);

    std::istringstream base_stream(base_content);
    std::istringstream alt_stream(alt_content);

    std::string base_line, alt_line;
    size_t line_number = 0;

    const std::string RED_TEXT = "\033[31m";
    const std::string GREEN_TEXT = "\033[32m";
    const std::string RESET_TEXT = "\033[0m";

    while (true) {
        std::getline(base_stream, base_line);
        std::getline(alt_stream, alt_line);

        bool base_has_no_line = base_line.empty();
        bool alt_has_no_line = alt_line.empty();

        if (base_has_no_line && alt_has_no_line) {
            break;
        }

        line_number++;

        if (base_has_no_line) {
            std::cout << "Line " << line_number << ":" << std::endl;
            std::cout << GREEN_TEXT << "+ " << alt_line << RESET_TEXT << std::endl;
        } else if (alt_has_no_line) {
            std::cout << "Line " << line_number << ":" << std::endl;
            std::cout << RED_TEXT << "- " << base_line << RESET_TEXT << std::endl;
        } else if (base_line != alt_line) {
            std::cout << "Line " << line_number << ":" << std::endl;
            std::cout << RED_TEXT << "- " << base_line << RESET_TEXT << std::endl;
            std::cout << GREEN_TEXT << "+ " << alt_line << RESET_TEXT << std::endl;
        }

        base_line.clear();
        alt_line.clear();
    }
}

void VersionController::list()
{
    std::cout << "Current branch: " << branch_manager.get_current_branch_name()
              << ", Index: " << branch_manager.get_current_branch_snapshot_index() << std::endl;
}

std::string VersionController::load_snapshot(size_t index)
{
    std::string version_file = ".versions/snapshot_" + std::to_string(index);

    std::string message_and_content = file_manager.read_file(version_file);
    std::string compressed_content = message_and_content.substr(message_and_content.find('\n') + 1);

    return compressor->decompress(
        std::vector<uint8_t>(compressed_content.begin(), compressed_content.end()));
}

void VersionController::create_branch(const std::string &branch_name)
{
    branch_manager.create_branch(branch_name);
}

void VersionController::switch_to_branch(const std::string &branch_name)
{
    branch_manager.switch_to_branch(branch_name);
    size_t switch_index = branch_manager.get_current_branch_snapshot_index();

    restore(switch_index, "test.txt");
}

void VersionController::merge_branch(const std::string &source_branch,
                                     const std::string &dest_branch)
{
    branch_manager.merge_branch(source_branch, dest_branch);
    size_t switch_index = branch_manager.get_current_branch_snapshot_index();

    restore(switch_index, "test.txt");
}