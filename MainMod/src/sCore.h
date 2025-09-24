#pragma once
#include "Common/Script/WorldGenerationData.h"
#include "Common/World/WorldConstants.h"
namespace Script {
	class Core {
	public:
		static int Initialize();
		static WorldGenerationData GetWorldGenerationData();
		static void OnCommand(uint64_t UUID, const Command& command);
	};
}