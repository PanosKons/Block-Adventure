Blocks = {
	Air = {Id = "air", Render = false},
	Dirt = {Id = "dirt", Render = true, Hardness = 50, Tool = { Id = "shovel", MinimumLevel = 0}, Texture = {2,2,2,2,2,2}, Transparent = false},
	Stone = {Id = "stone", Render = true,Hardness = 180, Tool = { Id = "pickaxe", MinimumLevel = 0}, Texture = {3,3,3,3,3,3}, Transparent = false},
	Drygrass = {Id = "drygrass", Render = true,Hardness = 50, Tool = { Id = "shovel", MinimumLevel = 0}, Texture = {12,12,12,12,2,10}, Transparent = false},
}
WorldGeneration = {
	Fillerblock = "air",
	Stone = "stone",
	Dirt = "dirt",
	BiomeDryTop = "drygrass",
	BiomeWetTop = "grass",
	BiomeDeadTop = "dirt",
	BiomeStoneTop = "stone",
	Ore = "iron"
 }
 Items = {

 }