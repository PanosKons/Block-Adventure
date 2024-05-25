#pragma once
#include "pch.h"
#include "Common/Entities/Player/Player.h"
#include "Common/World/WorldConstants.h"

struct MouseEvent
{
public:
	MouseEvent(uint64_t uuid, MouseState leftMouse, MouseState rightMouse, MouseState middleMouse)
		:UUID(uuid), LeftMouse(leftMouse), RightMouse(rightMouse),MiddleMouse(middleMouse) {}
	MouseEvent()
		:UUID(0), LeftMouse(), RightMouse(), MiddleMouse() {}

	uint64_t UUID;
	MouseState LeftMouse;
	MouseState RightMouse;
	MouseState MiddleMouse;
};
struct CommandEvent
{
public:
	CommandEvent(): UUID(0),command(){}
	CommandEvent(uint64_t uuid, Command& command)
		: UUID(uuid), command(command) {}
	uint64_t UUID;
	Command command;
};
struct KeyEvent
{
	uint64_t UUID;
	bool PKeyPressed;
	bool RKeyPressed;
	bool EKeyPressed;
};
class EventManager
{
public:
	static MouseEvent* GetMouseEvent();
	static void AddMouseEvent(MouseEvent& mouseEvent);

	static KeyEvent* GetKeyEvent();
	static void AddKeyEvent(KeyEvent& keyEvent);

	static void AddCommandEvent(CommandEvent& commandEvent);
	static CommandEvent* GetCommandEvent();

private:
	inline static std::vector<MouseEvent> MouseEvents;
	inline static std::vector<CommandEvent> CommandEvents;
	inline static std::vector<KeyEvent> KeyEvents;
};

