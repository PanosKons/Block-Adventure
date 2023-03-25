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
 ItemStackType = 
 {
	 BlockItem = 0,Item = 1
 }
 function OnLeftMouseClickEvent(UUID)

	local block = GetPlayerFacingBlock(UUID)
	if(block:IsValid()) then
		ReplaceBlock(block, WorldGeneration.Filler)
	end

	local pos = block:GetPosition()
	pos = IntVector3.Set(pos:GetX(),pos:GetY() - 1,pos:GetZ())
	local block2 = GetBlock(pos)
	if(block2:IsValid()) then
		ReplaceBlock(block2, WorldGeneration.Filler)
	end
 end

  function OnRightMouseClickEvent(UUID)
	local block = GetPlayerBlockToPlace(UUID)
	if(block:IsValid()) then
		ReplaceBlock(block,WorldGeneration.Ore)
	end
	local item = CreateItemStack(ItemStackType.Item, "stick", 2)
	RemoveFromInventory(UUID,item)
  end
  function OnMiddleMouseClickEvent(UUID)
	local item = CreateItemStack(ItemStackType.Item, "stick", 3)
	AddToInventory(UUID,item)
  end
  timer = 0
  function Update()
  timer = timer + 1
  if(timer % 10 == 0) then
	INFO(timer)
	end
  end