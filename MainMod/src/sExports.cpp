#include "sCore.h"
#include "sBlock.h"
#include "sItem.h"
#include "sInputAction.h"
namespace Script {
#define EXPORT0(ret, cls, fn) \
extern "C" __declspec(dllexport) ret cls##_##fn() { \
    return cls::fn(); \
}

#define EXPORT1(ret, cls, fn, t1) \
extern "C" __declspec(dllexport) ret cls##_##fn(t1 arg0) { \
    return cls::fn(arg0); \
}

#define EXPORT2(ret, cls, fn, t1, t2) \
extern "C" __declspec(dllexport) ret cls##_##fn(t1 arg0, t2 arg1) { \
    return cls::fn(arg0, arg1); \
}

#define EXPORT3(ret, cls, fn, t1, t2, t3) \
extern "C" __declspec(dllexport) ret cls##_##fn(t1 arg0, t2 arg1, t3 arg2) { \
    return cls::fn(arg0, arg1, arg2); \
}

EXPORT0(int, Core, Initialize);
EXPORT1(void, Core, OnCommand, uint64_t, const Command&);
EXPORT0(WorldGenerationData, Core, GetWorldGenerationData);
EXPORT0(std::vector<BlockProperties>*, Block, GetRegisteredBlocks);
EXPORT0(std::vector<ItemProperties>*, Item, GetRegisteredItems);
EXPORT0(std::vector<Model>*, Block, GetRegisteredModels);
EXPORT0(std::vector<InputAction>*, sInputAction, GetInputActions);
}