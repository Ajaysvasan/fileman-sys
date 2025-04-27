#pragma once
#include<string>
#include<unordered_map>
#include<vector>
#include<filesystem>

struct TABLE{
    std::string filename;
    int blockNumber;
    std::vector<std::vector<char>> blocksList;  
    TABLE *next;
};

class FAT_TABLE{
    private:
    TABLE *head;
    std::unordered_map<std::string , TABLE> blockList; 
    public:
    FAT_TABLE();
    std::vector<std::vector<char>> divideIntoBlocks(const std::string& filename);
    std::vector<std::string> getFileNames(const std::string& directory_path);
    bool InsertIntoTable(const std::string& fileName);
    bool createTable(const std::string& curr_dir);
    bool readBlocks();
    ~FAT_TABLE();
};