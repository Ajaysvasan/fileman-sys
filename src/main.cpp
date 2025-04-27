// 🔧 Core Components You’ll Implement:
// 1. Disk Simulation
// Create a file like disk.img to represent a virtual disk.

// Divide it into blocks (e.g., 4KB each).

// You’ll manage reading/writing at the block level using fstream.

// 2. File Allocation Table (FAT) or Inode Table
// FAT: Like an array where FAT[i] = j means block i is followed by j.

// Inodes: Each file has an inode holding metadata + pointers to data blocks.

// You can choose either:

// FAT → simpler, easier to visualize

// Inode → more UNIX-like, deeper understanding


#include <iostream>
#include<string>
#include "../includes/MiniShell.h"

static std::ostream& operator<<(std::ostream& COUT,const std::vector<std::string>&tokens) {
	for (const auto& value : tokens) {
		COUT << value << "\n";
	}
	return COUT;
}		


int main()
{
	try {
		while (true) {
			std::string command = getCommand();
			if (command.empty()) {
				std::cerr << "Invalid Input\n";
			}
			if (command == "exit") {
				std::cout << "Exiting....";
				break;
			}
			Commands* cmd = new Commands(command);
			cmd->execute();
			delete cmd;
		}
	}
	catch (...) {
		std::cerr << "Exception safely captured\n";
		exit(EXIT_FAILURE);
	}
	system("pause>0");
	return 0;
}