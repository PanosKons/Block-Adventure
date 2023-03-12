Blocks = {
	air = {Render = false},
	grass = {Render = true, Hardness = 50, Tool = { Id = "shovel", MinimumLevel = 0}, Texture = {1,1,1,1,2,0}, Transparent = false},
	dirt = {Render = true, Hardness = 50, Tool = { Id = "shovel", MinimumLevel = 0}, Texture = {2,2,2,2,2,2}, Transparent = false},
	stone = {Render = true, Hardness = 180, Tool = { Id = "pickaxe", MinimumLevel = 0}, Texture = {3,3,3,3,3,3}, Transparent = false},
	drygrass = {Render = true, Hardness = 50, Tool = { Id = "shovel", MinimumLevel = 0}, Texture = {12,12,12,12,2,10}, Transparent = false},
	glass = {Render = true, Hardness = 60, Tool = { Id = "none", MinimumLevel = 0}, Texture = {7,7,7,7,7,7}, Transparent = true},
	iron = {Render = true, Hardness = 250, Tool = { Id = "pickaxe", MinimumLevel = 0}, Texture = {6,6,6,6,6,6}, Transparent = false},
	log = {Render = true, Hardness = 100, Tool = { Id = "axe", MinimumLevel = 0}, Texture = {4,4,4,4,5,5}, Transparent = false},
	water = {Render = true, Hardness = 0, Tool = { Id = "none", MinimumLevel = 0}, Texture = {14,14,14,14,14,14}, Transparent = true},
	leaves = {Render = true, Hardness = 15, Tool = { Id = "none", MinimumLevel = 0}, Texture = {8,8,8,8,8,8}, Transparent = true},
}

Items = {
	 drygrassblade = { Texture = 9},
	 stick = { Texture = 11},
	 stone_pickaxe = {Texture = 13, Tool = "pickaxe"},
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
 Noise = {
	 Octaves = 8,
	 Frequency = 256.0,
	 YLevelStretch = 96,
	 BiomeStretch = 4
 }
 function OnBlockInteract(block, item)
	time = Blocks[block].Hardness
	 if item ~= nil then
		if Blocks[block]["Tool"]["Id"] == Items[item]["Tool"] then
		time = time / 4
		end
	 end
	return time
 end