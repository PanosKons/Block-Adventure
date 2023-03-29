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
function SimpleBlock(TextureID)
	return {TextureID,TextureID,TextureID,TextureID,TextureID,TextureID}
end
Blocks = {
	air = {Render = false},
	grass = {Render = true, Texture = {1,1,1,1,2,0}, Transparent = false ,Translucency = 1.0, Material = MaterialDirt},
	dirt = {Render = true, Texture = SimpleBlock(2), Transparent = false,Translucency = 1.0, Material = MaterialDirt},
	stone = {Render = true, Texture = SimpleBlock(3), Transparent = false,Translucency = 1.0, Material = MaterialStone},
	drygrass = {Render = true, Texture = {12,12,12,12,2,10}, Transparent = false,Translucency = 1.0, Material = MaterialDirt},
	glass = {Render = true, Texture = SimpleBlock(7), Transparent = true,Translucency = 1.0, Material = MaterialDefault},
	iron = {Render = true, Texture = SimpleBlock(6), Transparent = false,Translucency = 1.0, Material = MaterialIron},
	log = {Render = true, Texture = {4,4,4,4,5,5}, Transparent = false,Translucency = 1.0, Material = MaterialWood},
	water = {Render = true, Texture = {14,14,14,14,14,14}, Transparent = true, Translucency = 0.4, Material = MaterialDefault},
	leaves = {Render = true, Texture = SimpleBlock(8), Transparent = true,Translucency = 1.0, Material = MaterialDefault},
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
	Ore = "water"
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

	if false then
	local pos = block:GetPosition()
	pos = IntVector3.Set(pos:GetX(),pos:GetY() - 1,pos:GetZ())
	local block2 = GetBlock(pos)
	if(block2:IsValid()) then
		ReplaceBlock(block2, WorldGeneration.Filler)
	end

	end
end
  function OnRightMouseClickEvent(UUID)
  end

  function OnMiddleMouseClickEvent(UUID)
	local block = GetPlayerBlockToPlace(UUID)
	if(block:IsValid()) then
		ReplaceBlock(block,"glass")
	end
  end


  function Update()
  end

  function OnCommand(UUID, Command, Args)

	if Command == "/give" then
	local type = -1
	if Args:Get(0) == "item" then
		type = 1
	elseif Args:Get(0) == "block" then
		type = 0
	end
		local item = CreateItemStack(type,Args:Get(1),tonumber(Args:Get(2)))
		AddToInventory(UUID,item)
	end

	if Command == "/clear" then
	if Args:Get(0) == "item" then
		type = 1
	elseif Args:Get(0) == "block" then
		type = 0
	end
		local item = CreateItemStack(type,Args:Get(1),tonumber(Args:Get(2)))
		RemoveFromInventory(UUID,item)
	end

	if Command == "/tp" then
		pos = DoubleVector3.Set(tonumber(Args:Get(0)),tonumber(Args:Get(1)),tonumber(Args:Get(2)))
		SetPlayerPosition(UUID,pos)
	end
  end