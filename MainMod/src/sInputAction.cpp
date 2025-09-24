#include "sInputAction.h"
#include "sBlock.h"
namespace Script {
	void sInputAction::Register(const InputAction&& ia)
	{
		inputActions[ia.identifier] = ia;
	}
	void sInputAction::RegisterInputActions()
	{
		Register({ .button = LeftMouse, .identifier = LeftHandInteract, .screen = Game });
		Register({ .button = RightMouse, .identifier = RightHandInteract, .screen = Game });
		Register({ .button = P, .identifier = ReloadAssembly, .screen = Game });
		Register({ .button = E, .identifier = OpenInventory, .screen = Game });
	}
}