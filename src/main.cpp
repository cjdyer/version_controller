#include "snapshot_tracker.h"

int main()
{
    SnapshotTracker vcs;
    std::string file_path = "../test.txt";

    while (true)
    {
        std::cout << "1. Create snapshot" << std::endl;
        std::cout << "2. List snapshots" << std::endl;
        std::cout << "3. Restore snapshot" << std::endl;
        std::cout << "4.Exit" << std::endl;

        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            vcs.create(file_path);
            break;
        case 2:
            vcs.list();
            break;
        case 3:
            std::cout << "Enter snapshot index to restore: ";
            size_t index;
            std::cin >> index;

            vcs.restore(index, file_path);
            break;
        case 4:
            return 0;
        default:
            std::cout << "Invalid choice." << std::endl;
        }
    }

    return 0;
}
