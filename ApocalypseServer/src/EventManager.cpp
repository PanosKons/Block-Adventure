#include "EventManager.h"

static std::mutex command_lock;
static std::mutex action_lock;

void EventManager::AddCommandEvent(CommandEvent& commandEvent)
{
	command_lock.lock();
	CommandEvents.push_back(commandEvent);
	command_lock.unlock();
}

CommandEvent* EventManager::GetCommandEvent()
{
	CommandEvent* ReturnPointer = nullptr;
	command_lock.lock();
	CommandEvent commandEvent;
	if (CommandEvents.size() != 0)
	{
		commandEvent = CommandEvents[CommandEvents.size() - 1];
		CommandEvents.pop_back();
		ReturnPointer = &commandEvent;
	}
	command_lock.unlock();
	return ReturnPointer;
}

void EventManager::AddActionEvent(ActionEvent& actionEvent)
{
	action_lock.lock();
	ActionEvents.push_back(actionEvent);
	action_lock.unlock();
}

ActionEvent* EventManager::GetActionEvent()
{
	ActionEvent* ReturnPointer = nullptr;
	action_lock.lock();
	ActionEvent actionEvent;
	if (ActionEvents.size() != 0)
	{
		actionEvent = ActionEvents[ActionEvents.size() - 1];
		ActionEvents.pop_back();
		ReturnPointer = &actionEvent;
	}
	action_lock.unlock();
	return ReturnPointer;
}
