#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <dirent.h>
#include <unordered_map>

#include "../includes/FAT_table.h"

const int BLOCK_SIZE = 512;

std::ostream& operator<<(std::ostream& COUT, std::vector<std::vector<char>>& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            COUT << matrix[i][j] << "";
        }
        COUT << "\n";
    }
    return COUT;
}

// Constructor
FAT_TABLE::FAT_TABLE() {
    head = nullptr;
}

// Destructor
FAT_TABLE::~FAT_TABLE() {
    std::cout << "Destructor called, deleting table...\n";
    TABLE* temp = head;
    while (temp != nullptr) {
        TABLE* toDelete = temp;
        temp = temp->next;
        delete toDelete;
    }
}

bool FAT_TABLE::InsertIntoTable(const std::string& fileName) {
    static int count = 0;

    TABLE* newNode = new TABLE();
    newNode->filename = fileName;
    newNode->blockNumber = ++count;
    newNode->blocksList = divideIntoBlocks(fileName);
    newNode->next = nullptr;

    if (!head) {
        head = newNode;
        return true;
    }

    TABLE* temp = head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newNode;
    return true;
}

bool FAT_TABLE::createTable(const std::string& curr_dir) {
    std::vector<std::string> filenames = getFileNames(curr_dir);
    for (const auto& filename : filenames) {
        InsertIntoTable(curr_dir + "\\" + filename); // full path
    }
    return true;
}

std::vector<std::string> FAT_TABLE::getFileNames(const std::string& directory_path) {
    std::vector<std::string> fileNames;
    DIR* dir;
    struct dirent* en;
    if ((dir = opendir(directory_path.c_str())) != NULL) {
        while ((en = readdir(dir)) != NULL) {
            if (std::string(en->d_name) != "." && std::string(en->d_name) != "..") {
                fileNames.push_back(en->d_name);
            }
        }
        closedir(dir);
    }
    else {
        std::cerr << "Error opening directory: " << directory_path << "\n";
    }
    return fileNames;
}

std::vector<std::vector<char>> FAT_TABLE::divideIntoBlocks(const std::string& filename) {
    std::cout << "Reading file: " << filename << "\n";
    std::ifstream file(filename, std::ios::binary);
    std::vector<std::vector<char>> blocks;

    if (!file) {
        std::cerr << "File could not be opened: " << filename << "\n";
        return blocks;
    }

    while (!file.eof()) {
        std::vector<char> block(BLOCK_SIZE);
        file.read(block.data(), BLOCK_SIZE);
        std::streamsize bytesRead = file.gcount();

        if (bytesRead > 0) {
            block.resize(bytesRead);  // Trim extra space if last block
            blocks.push_back(block);
        }
    }

    return blocks;
}

bool FAT_TABLE::readBlocks() {
    if (!head) {
        std::cout << "Table is empty.\n";
        return false;
    }

    TABLE* temp = head;
    while (temp != nullptr) {
        std::cout << "Filename: " << temp->filename << "\n";
        std::cout << "Block Number: " << temp->blockNumber << "\n";
        std::cout << "Number of Blocks: " << temp->blocksList.size() << "\n";
        temp = temp->next;
    }
    return true;
}

int main() {
    FAT_TABLE* obj = new FAT_TABLE();
    std::string directory_path = "D:\\SDE projects\\fileman-sys\\test";
    
    obj->createTable(directory_path);
    obj->readBlocks();

    delete obj;
    return 0;
}
