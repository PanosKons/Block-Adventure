#pragma once
#include "pch.h"
#include "Common/Entities/Player/Player.h"
#include "Common/World/WorldConstants.h"

struct CommandEvent
{
public:
	CommandEvent(): UUID(0),command(){}
	CommandEvent(uint64_t uuid, Command& command)
		: UUID(uuid), command(command) {}
	uint64_t UUID;
	Command command;
};
struct ActionEvent
{
	uint64_t UUID;
	int identifier;
};
class EventManager
{
public:
	static void AddCommandEvent(CommandEvent& commandEvent);
	static CommandEvent* GetCommandEvent();

	static void AddActionEvent(ActionEvent& actionEvent);
	static ActionEvent* GetActionEvent();
private:
	inline static std::vector<ActionEvent> ActionEvents;
	inline static std::vector<CommandEvent> CommandEvents;
};

