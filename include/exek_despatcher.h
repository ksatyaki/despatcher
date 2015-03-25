extern "C"
{
#include <peiskernel/peiskernel_mt.h>
}

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>

class ExekDespatcher
{
	int owner_id_;
	std::string component_name_;

public:
	ExekDespatcher(const int& owner_id, const std::string& component_name);

	bool despatch(const std::string& action_name, const std::string& parameters);

	void show();
};
