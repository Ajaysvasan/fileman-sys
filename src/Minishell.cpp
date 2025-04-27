#include "../includes/MiniShell.h"
#include<vector>
#include<string>
#include<sstream>
#include<io.h>
#include<direct.h>
#include<algorithm>
#include<fstream>
#include<iostream>
#include <sys/stat.h>
#include<filesystem>
#include <cstdio>

static std::string curr_dir;

std::vector<std::string> parser(std::string command) {
	std::vector<std::string> tokens;
	std::stringstream ss(command);
	std::string token;
	bool inQuotes = false;
	std::string quotedToken;
	while (ss >> std::ws, !ss.eof()) {
		char c = ss.peek();
		if (c == '"') {
			ss.get();
			std::getline(ss, quotedToken, '"');
			tokens.push_back(quotedToken);
		}
		else {
			ss >> token;
			tokens.push_back(token);
		}
	}
	return tokens;
}

std::string getCommand() {
	std::string command;
	std::cout << "MiniShell>";
	std::getline(std::cin, command);
	return command;
}

static int CommandType(std::vector<std::string>tokens) {
	if (tokens[0] == "mkdir") return MKDIR;
	else if (tokens[0] == "create") return CREATE;
	else if (tokens[0] == "write") return WRITE;
	else if (tokens[0] == "read") return READ;
	else if (tokens[0] == "delete") return DELETE;
	else if (tokens[0] == "cd") return CD;
	else if (tokens[0] == "ls") return LS;
	else if (tokens[0] == "rmdir") return RMDIR;
	else return INVALID;
}

Commands::Commands(const std::string& input) {
	std::cout << "This is got executed\n";
	command = input;
	tokens = parser(command);
}

void Commands::clear() {
	tokens.clear();
	command.clear();
}

Commands::~Commands() {
	clear();
}

bool Commands::validCommand() {
	// We have only 2 special cases i.e write which expects more than 2 args and ls which doesn't expect any args
	int commandType = CommandType(tokens);
	if (commandType != commands::INVALID) {
		if (commandType == commands::WRITE && tokens.size() > 2) {
			return true;
		}
		else if (commandType == commands::LS && tokens.size() == 1) {
			return true;
		}
		else if (tokens.size() == 2) {
			return true;
		}
	}
	return false;
}

bool Commands::MakedirCommand() {
	if (validCommand()) {
		const std::string& dir_name = tokens[1];
		if (_access(dir_name.c_str(), 0) == 0) {
			std::cout << "Directory already exists: " << dir_name << "\n";
			return false;
		}
		if (_mkdir(dir_name.c_str()) == 0) {
			std::cout << "Directory create successfully\n";
			return true;
		}
		else {
			perror("Error in creating the file\n");
			return false;
		}
	}
	return false;
}

bool Commands::CdCommand() {
	if (validCommand()) {
		const std::string& dir_name = tokens[1];
		if (_access(dir_name.c_str() , 0) == 0) {
			std::cout << "Directory found\n";
			curr_dir = dir_name;
			return true;
		}
		std::cerr << "The directory is not found\n";
		return false;
	}
	return false;
}

bool Commands::CreateCommand() {
	if (validCommand()) {
		const std::string& file_name = tokens[1];
		std::string full_path = curr_dir.empty() ? file_name : curr_dir + "\\" + file_name;
		if (_access(full_path.c_str(), 0) == 0) {
			std::cout << "The file already exsists\n";
			return false;
		}
		std::fstream file;
		file.open(full_path, std::ios::app);
		if (file.is_open()) {
			std::cout << "File created successfully\n";
			file.close();
			return true;
		}
		else {
			std::cerr << "Error in creating the file\n";
			return false;
		}
	}
	return false;
}

bool Commands::LsCommand() {
	if (!validCommand()) {
		return false;
	}
	const std::string& dir_path = curr_dir.empty() ? "." : curr_dir;
	struct stat sb;
	for(const auto&entry: std::filesystem::directory_iterator(dir_path)){
		std::cout<<entry.path()<<"\n";
	}
	return true;
}



bool Commands::DeleteCommand(){
	if (validCommand()) {
		const std::string& file_name = tokens[1];
		const std::string& full_path = curr_dir.empty() ? file_name : curr_dir + "\\" + file_name;
		if (_access(full_path.c_str(), 0) == 0) {
			if(remove(full_path.c_str()) == 0 ){
				std::cout<<"The file has been deleted successfully\n";
				return true;
			}else{
				std::cerr<<"Error in deleting the file\n";
				return false;
			}
		}else{
			std::cerr<<"Unexpected error\n";
		}
	}	
	std::cerr<<"Invalid Command Type\n";
	return false;
}

bool Commands::WriteCommand() {
	// write <file_name> content
	if (!validCommand()) {
		return false;
	}
	const std::string& file_name = tokens[1];
	const std::string& full_path = curr_dir.empty() ? file_name : curr_dir + "\\" + file_name;
	if (_access(full_path.c_str(), 0) == 0) {
		std::fstream file;
		file.open(full_path, std::ios::app);
		if (file.is_open()) {
			file << tokens[2]<<"\n";
			file.close();
			return true;
		}
		else {
			std::cerr << "Error in opening or writing the file\n";
			return false;
		}
	}
		std::cerr << "Error in opening or writing in the file\n";
		return false;
}

bool Commands::RmdirCommand() {
if (validCommand()) {
		const std::string& dir_path = tokens[1];
		if (_access(dir_path.c_str(), 0) == 0) {
			if(std::filesystem::remove_all(dir_path.c_str()) == 0 ){
				std::cout<<"The directory has been deleted successfully\n";
				return true;
			}else{
				std::cerr<<"Error in deleting the directory\n";
				return false;
			}
		}else{
			std::cerr<<"Unexpected error\n";
		}
	}	
	std::cerr<<"Invalid Command Type\n";
	return false;
}

bool Commands::ReadCommand() {
	if (!validCommand()) {
		return false;
	}
	const std::string& file_name = tokens[1];
	const std::string& full_path = curr_dir.empty() ? file_name : curr_dir + "\\" + file_name;
	if (_access(full_path.c_str(), 0) == 0) {
		std::fstream file;
		file.open(full_path, std::ios::in);
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				std::cout << line<<"\n";
			}
			file.close();
			return true;
		}
		else {
			std::cerr << "Error in opening or reading the file\n";
		}
	}
		std::cerr << "File doesn't exsists\n";
		return false;

}

void Commands::execute() {
	int commandType = CommandType(tokens);
	switch (commandType) {
	case 0: {
		MakedirCommand();
		break;
	}
	case 1: {
		CreateCommand();
		break;
	}
	case 2: {
		WriteCommand();
		break;
	}
	case 3: {
		ReadCommand();
		break;
	}
	case 4: {
		DeleteCommand();
		break;
	}
	case 5: {
		CdCommand();
		std::cout <<"Current dir: " << curr_dir<<"\n";
		break;
	}
	case 6: {
		LsCommand();
		break;
	}
	case 7: {
		RmdirCommand();
		break;
	}
	default: {
		std::cerr << "Invalid command\n";
		break;
	}
	}
}