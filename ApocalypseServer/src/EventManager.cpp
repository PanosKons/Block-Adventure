#include "EventManager.h"

static std::mutex mouse_lock;
static std::mutex key_lock;
static std::mutex command_lock;

void EventManager::AddMouseEvent(MouseEvent& mouseEvent)
{
	mouse_lock.lock();
	MouseEvents.push_back(mouseEvent);
	mouse_lock.unlock();
}
void EventManager::AddKeyEvent(KeyEvent& keyEvent)
{
	key_lock.lock();
	KeyEvents.push_back(keyEvent);
	key_lock.unlock();
}

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

MouseEvent* EventManager::GetMouseEvent()
{
	MouseEvent* ReturnPointer = nullptr;
	mouse_lock.lock();
	MouseEvent mouseEvent;
	if (MouseEvents.size() != 0)
	{
		mouseEvent = MouseEvents[MouseEvents.size() - 1];
		MouseEvents.pop_back();
		ReturnPointer = &mouseEvent;
	}
	mouse_lock.unlock();
	return ReturnPointer;
}
KeyEvent* EventManager::GetKeyEvent()
{
	KeyEvent* ReturnPointer = nullptr;
	key_lock.lock();
	KeyEvent keyEvent;
	if (KeyEvents.size() != 0)
	{
		keyEvent = KeyEvents[KeyEvents.size() - 1];
		KeyEvents.pop_back();
		ReturnPointer = &keyEvent;
	}
	key_lock.unlock();
	return ReturnPointer;
}
