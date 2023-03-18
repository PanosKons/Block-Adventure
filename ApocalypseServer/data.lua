MaterialDirt = {
	Hardness = 50, Tool = "shovel"
}
MaterialStone = 
{
	Hardness = 180, Tool = "pickaxe"
}
MaterialIron = 
{
	Hardness = 300, Tool = "pickaxe"
}
MaterialWood = 
{
	Hardness = 100, Tool = "axe"
}
MaterialDefault = 
{
	Hardness = 50, Tool = "none"
}
Blocks = {
	air = {Render = false},
	grass = {Render = true, Texture = {1,1,1,1,2,0}, Transparent = false , Material = MaterialDirt},
	dirt = {Render = true, Texture = {2,2,2,2,2,2}, Transparent = false, Material = MaterialDirt},
	stone = {Render = true, Texture = {3,3,3,3,3,3}, Transparent = false, Material = MaterialStone},
	drygrass = {Render = true, Texture = {12,12,12,12,2,10}, Transparent = false, Material = MaterialDirt},
	glass = {Render = true, Texture = {7,7,7,7,7,7}, Transparent = true, Material = MaterialDefault},
	iron = {Render = true, Texture = {6,6,6,6,6,6}, Transparent = false, Material = MaterialIron},
	log = {Render = true, Texture = {4,4,4,4,5,5}, Transparent = false, Material = MaterialWood},
	water = {Render = true, Texture = {14,14,14,14,14,14}, Transparent = true, Material = MaterialDefault},
	leaves = {Render = true, Texture = {8,8,8,8,8,8}, Transparent = true, Material = MaterialDefault},
}

Items = {
	 drygrassblade = { Texture = 9},
	 stick = {Texture = 11},
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
 function OnLeftMouseClickEvent(UUID)
	local block = GetPlayerFacingBlock(UUID)
	if(block:IsValid()) then
		ReplaceBlock(block, WorldGeneration.Filler)
	end
 end

  function OnRightMouseClickEvent(UUID)
  end

  function OnMiddleMouseClickEvent(UUID)
  end

  function Update()
  end