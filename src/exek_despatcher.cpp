#include <exek_despatcher.h>

ExekDespatcher::ExekDespatcher(const int& owner_id, const std::string& component_name) :
	component_name_(component_name),
	owner_id_(owner_id)
{
		
}

void ExekDespatcher::show()
{
	std::cout << "Component Name: " << component_name_ << ", ";
	std::cout << "Owner ID: " << owner_id_;
	std::cout << std::endl;
}

bool ExekDespatcher::despatch(const std::string& action_name, const std::string& parameters)
{
	std::string in_prefix_key = "in." + component_name_ + "." + action_name + ".";

	std::string in_command_key = in_prefix_key + "command";
	std::string in_param_key = in_prefix_key + "parameters";

	std::string out_prefix_key = "out." + component_name_ + "." + action_name + ".";

	std::string out_command_key = out_prefix_key + "command";
	std::string out_param_key = out_prefix_key + "parameters";
	std::string out_state_key = out_prefix_key + "state";

	PeisSubscriberHandle stateSubsciber = peiskmt_subscribe(owner_id_, out_state_key.c_str());

	peiskmt_setStringTuple (out_command_key.c_str(), "ON");
	peiskmt_setStringTuple (out_param_key.c_str(), parameters.c_str());

	peiskmt_setMetaTuple(owner_id_, in_command_key.c_str(), peiskmt_peisid(), out_command_key.c_str());
	peiskmt_setMetaTuple(owner_id_, in_param_key.c_str(), peiskmt_peisid(), out_param_key.c_str());
	peiskmt_setRemoteStringTuple(owner_id_, out_state_key.c_str(), "IDLE");

	std::cout << "DESPATCHING: " << action_name << " to Robot: " << component_name_ << std::endl;
		
	PeisTuple* stateTuple = NULL;

	while(!stateTuple)
	{
		stateTuple = peiskmt_getTuple(owner_id_, out_state_key.c_str(), PEISK_KEEP_OLD);
		sleep(1);
	}
		
	while(strcmp(stateTuple->data, "COMPLETED") != 0 && strcmp(stateTuple->data, "FAILED") != 0)
	{
		stateTuple = peiskmt_getTuple(owner_id_, out_state_key.c_str(), PEISK_KEEP_OLD);
		sleep(1);
	}

	if(strcmp(stateTuple->data, "COMPLETED") == 0)
	{
		std::cout << "!!!SUCCESS!!!" << std::endl;
		peiskmt_setStringTuple(out_command_key.c_str(), "OFF");
		peiskmt_setRemoteStringTuple(owner_id_, out_state_key.c_str(), "IDLE");
		return true;
	}

	std::cout << "$$$FAILED$$$" << std::endl;
	peiskmt_setStringTuple(out_command_key.c_str(), "OFF");
	peiskmt_setRemoteStringTuple(owner_id_, out_state_key.c_str(), "IDLE");
	return false;
		
}
