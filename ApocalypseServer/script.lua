ToolTypes = {
	 Pickaxe = "pickaxe",
	 Shovel = "shovel",
	 Axe = "axe",
	 None = "none",
 }
Blocks = {
	Air = {Id = "air", Render = false},
	Grass = {Id = "grass", Render = true, Hardness = 50, Tool = { Id = "shovel", MinimumLevel = 0}, Texture = {1,1,1,1,2,0}, Transparent = false},
	Dirt = {Id = "dirt", Render = true, Hardness = 50, Tool = { Id = "shovel", MinimumLevel = 0}, Texture = {2,2,2,2,2,2}, Transparent = false},
	Stone = {Id = "stone", Render = true, Hardness = 180, Tool = { Id = "pickaxe", MinimumLevel = 0}, Texture = {3,3,3,3,3,3}, Transparent = false},
	Drygrass = {Id = "drygrass", Render = true, Hardness = 50, Tool = { Id = "shovel", MinimumLevel = 0}, Texture = {12,12,12,12,2,10}, Transparent = false},
	Glass = {Id = "glass", Render = true, Hardness = 60, Tool = { Id = "none", MinimumLevel = 0}, Texture = {7,7,7,7,7,7}, Transparent = true},
	Iron = {Id = "iron", Render = true, Hardness = 250, Tool = { Id = "pickaxe", MinimumLevel = 0}, Texture = {6,6,6,6,6,6}, Transparent = false},
	Log = {Id = "log", Render = true, Hardness = 100, Tool = { Id = "axe", MinimumLevel = 0}, Texture = {4,4,4,4,5,5}, Transparent = false},
	Water = {Id = "water", Render = true, Hardness = 0, Tool = { Id = "none", MinimumLevel = 0}, Texture = {14,14,14,14,14,14}, Transparent = true},
	Leaves = {Id = "leaves", Render = true, Hardness = 15, Tool = { Id = "none", MinimumLevel = 0}, Texture = {8,8,8,8,8,8}, Transparent = true},
}
WorldGeneration = {
	Filler = "air",
	Underground = "stone",
	Dirt = "dirt",
	DryTop = "drygrass",
	WetTop = "grass",
	DeadTop = "dirt",
	StoneTop = "stone",
	Ore = "iron"
 }
 Items = {
	 DryGrassBlade = {Id = "drygrassblade", Texture = 9},
	 Stick = {Id = "stick", Texture = 11},
	 StonePickaxe = {Id = "stone_pickaxe", Texture = 13, Tool = "pickaxe"},
 }