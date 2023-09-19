#include "List_objects.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <Windows.h>

void PrintStart() {
	std::cout << "Requests:" << std::endl;
	std::cout << "READ - read from a file" << std::endl;
	std::cout << "ADD - add a line manually" << std::endl;
	std::cout << "SAVE - save to a file" << std::endl;
	std::cout << "END - end of input" << std::endl;
}

void PrintGroupes() {
	std::cout << "Possible groups:" << std::endl;
	std::cout << "RAW - save data without grouping" << std::endl;
	std::cout << "DIST - adding a line manually" << std::endl;
	std::cout << "NAME - saving to a file" << std::endl;
	std::cout << "TIME - end of input" << std::endl;
}

void ProcessRequests() {
	ObjectListHandler list_handler;
	
	std::string request;
	do {
		std::cin >> request;
		if (request == "READ") {
			std::cout << "input the file name" << std::endl;
			std::string f_name;
			std::cin >> f_name;
			list_handler.Read(f_name);
		}
		else if (request == "ADD") {
			std::cout << "enter a line with the characteristics of the object" << std::endl;
			std::string line;
			std::cin.get();
			std::getline(std::cin,line);
			list_handler.Add(line);
		}
		else if (request == "SAVE") {
			std::cout << "enter the file name" << std::endl;
			std::string f_name;
			std::cin >> f_name;
			std::cout << "enter the group name" << std::endl;
			PrintGroupes();
			std::string g_name;
			std::cin >> g_name;
	
			if (g_name == "TYPE") {
				std::cout << "enter the minimum group size" << std::endl;
			}
	
			std::ofstream out;
			out.open(f_name);
			if (out.is_open()) {
				list_handler.CreateGroupeAndSave(out, g_name,0);
			}
			out.close();
		}
		else {
			std::cout << "oops, invalid request, try again" << std::endl;
		}
	} while (request != "END");
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	PrintStart();
	ProcessRequests();
	
	return 0;
}