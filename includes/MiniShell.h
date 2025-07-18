#pragma once

#include<vector>
#include<string>
std::vector<std::string> parser(std::string command);
std::string getCommand();
	enum commands {
		INVALID = -1,
		MKDIR,
		CREATE,
		WRITE,
		READ,
		DELETE,
		CD,
		LS,
		RMDIR
	};

typedef struct{
	std::string command;
	std::string status;
}Logs;
// The log function should create the log directory and log file within that log directory.
void createLogs();
// This function should give me the last log and it's status.
bool lastLog();
// This function will write the log in the logs file.
bool writeLogs();
class Commands {
private:
	std::vector<std::string> tokens;
	std::string command;	
	bool validCommand();
	void clear();

public:
	Commands(const std::string& command);
	bool MakedirCommand();
	void execute();
	bool CreateCommand();
	bool WriteCommand();
	bool ReadCommand();
	bool DeleteCommand();
	bool CdCommand();
	bool LsCommand();
	bool RmdirCommand();
	~Commands();
};
