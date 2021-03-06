extern "C"
{
#include <peiskernel/peiskernel_mt.h>
}

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <vector>
#include <exek_despatcher.h>
#include <map>

std::vector <std::string> splitStrings(const std::string& input_str)
{
	std::vector <std::string> strings;

	std::string split_str;
	std::istringstream input_str_stream (input_str);

	while(std::getline(input_str_stream, split_str, ' '))
	{
		strings.push_back(split_str);
	}

	return strings;
}


int main(int argn, char* args[])
{
	if(argn < 2)
	{
		std::cout << "Please provide a script file to load and despatch" << std::endl;
		exit(0);
	}
	
	std::fstream fileStream (args[1]);
	peiskmt_initialize(&argn, args);

	sleep(3);

	if(fileStream.is_open())
	{
		std::cout << "File open...\n";
	}

	std::string line;

	std::map <std::string, ExekDespatcher*> all_despatchers;
	
	while(std::getline(fileStream, line))
	{
		std::vector <std::string> all_parts = splitStrings(line);

		if(line.find("owner") != std::string::npos)
		{
			// This will have to be of the form: owner doro 942
			int owner = atof(all_parts[2].c_str());
			all_despatchers[all_parts[1]] = new ExekDespatcher(owner, all_parts[1]);
			all_despatchers[all_parts[1]]->show();
		}
		else if(line.find("sleep") != std::string::npos)
		{
			printf("\nSleeping now!");
			sleep(atoi(all_parts[1].c_str()));
			printf("\nWaking up...");
		}
		else if(all_parts.size() < 3)
			continue;
		else
		{
			all_despatchers[all_parts[0]]->despatch(all_parts[1], all_parts[2]);
		}
	}

	return 0;
	
}




