using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using static System.Formats.Asn1.AsnWriter;

namespace Scripting
{
    public enum BlockType
    {
        Invalid = -1, Air, Grass, Dirt, Stone, Glass, Leaves, Log, Water, DryGrass, Iron, BlockTypeSize
    }
    public enum ItemType
    {
        Invalid = -1, Drygrassblade = BlockType.BlockTypeSize, Stick, StonePickaxe, ItemTypeSize
    }
    public struct WorldGenerationData
    {
        public WorldGenerationData(int Octaves, double Frequency, int YLevelStretch, int BiomeStretch)
        {
            this.Octaves = Octaves;
            this.Frequency = Frequency;
            this.YLevelStretch = YLevelStretch;
            this.BiomeStretch = BiomeStretch;
        }
        public int Octaves = 8;
        public double Frequency = 256.0;
        public int YLevelStretch = 96;
        public int BiomeStretch = 4;
    }
    public static class Data
    {
        public static List<BlockProperties> Blocks = new List<BlockProperties>();
        public static List<ItemProperties> Items = new List<ItemProperties>();

        public static BlockType Filler = BlockType.Air;
        public static BlockType UnderGround = BlockType.Stone;
        public static BlockType Dirt = BlockType.Dirt;
        public static BlockType DryTop = BlockType.Dirt;
        public static BlockType WetTop = BlockType.Grass;
        public static BlockType DeadTop = BlockType.Dirt;
        public static BlockType StoneTop = BlockType.Stone;
        public static BlockType Ore = BlockType.Water;

        public static WorldGenerationData GetWorldGenerationData()
        {
            return new WorldGenerationData(8,256.0,96,4);
        }

        static int j = -1;
        public static ItemProperties GetItem()
        {
            j++;
            return Items[j];
        }
        public static int GetItemCount()
        {
            return Items.Count;
        }
        public static int GetBlockCount()
        {
            return Blocks.Count;
        }

        static int i = -1;
        public static BlockProperties GetBlock()
        {
            i++;
            return Blocks[i];
        }
    }
    public struct Vector3<T>
    {
        public T x;
        public T y;
        public T z;
        public Vector3(T x, T y, T z)
        {
            this.x = x; this.y = y; this.z = z;
        }
    }
    public struct Block
    {
        public Vector3<int> Position;
        public BlockType Type;

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetBlock(Vector3<int> Position, out Block block);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetPlayerFacingBlock(ulong UUID, out Block block);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void ReplaceBlock(Block block, BlockType NewBlockType);
    }
    public struct ItemStack
    {
        public ItemStack(ItemType Type,int Count)
        {
            this.Type = Type;
            this.Count = Count;
        }
        public ItemType Type;
        public int Count;
    }
    public struct Player
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void AddItemToInventory(ulong UUID, ItemStack Item);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SetPosition(ulong UUID, Vector3<double> Position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void RemoveItemFromInventory(ulong UUID, ItemStack Item);
    }
    public struct Texture
    {
        public Texture(byte a,byte b, byte c, byte d, byte e, byte f)
        {
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }

        public static Texture SimpleBlock(byte TextureID)
        {
            return new Texture ( TextureID, TextureID, TextureID, TextureID, TextureID, TextureID );
        }

        byte a;
        byte b;
        byte c;
        byte d;
        byte e;
        byte f;
    }
    public struct BlockProperties
    {
        public BlockProperties( bool Render, bool Transparent, float Translucency, Texture BlockTexture)
        {
            this.Render = Render;
            this.Transparent = Transparent;
            this.Translucency = Translucency;
            this.BlockTexture = BlockTexture;
        }

        public Texture BlockTexture;
        public float Translucency = 1.0f;
        public bool Render = false;
        public bool Transparent = false;
    }
    public struct ItemProperties
    {
        public ItemProperties(byte ItemTexture)
        {
            this.ItemTexture = ItemTexture;
        }
        public byte ItemTexture;
    }
    public static class Event
    {
        public static void Initialize()
        {
            Console.WriteLine("Initialized C# !");

            for(int i = 0; i < (int)BlockType.BlockTypeSize; i++)
                Data.Blocks.Add(new BlockProperties());

            Data.Blocks[(int)BlockType.Air] = new BlockProperties(false, false, 1.0f, new Texture( 0, 0, 0, 0, 0, 0 ));
            Data.Blocks[(int)BlockType.Grass] = new BlockProperties(true, false, 1.0f, new Texture( 1, 1, 1, 1, 2, 0 ));
            Data.Blocks[(int)BlockType.Log] = new BlockProperties(true, false, 1.0f, new Texture(4, 4, 4, 4, 5, 5));
            Data.Blocks[(int)BlockType.Dirt] = new BlockProperties(true, false, 1.0f, Texture.SimpleBlock(2));
            Data.Blocks[(int)BlockType.Stone] = new BlockProperties(true, false, 1.0f, Texture.SimpleBlock(3));
            Data.Blocks[(int)BlockType.Glass] = new BlockProperties(true, false, 1.0f, Texture.SimpleBlock(7));
            Data.Blocks[(int)BlockType.DryGrass] = new BlockProperties(true, false, 1.0f, new Texture(12, 12, 12, 12, 2, 10));
            Data.Blocks[(int)BlockType.Iron] =  new BlockProperties(true, false, 1.0f, Texture.SimpleBlock(6));
            Data.Blocks[(int)BlockType.Leaves] =  new BlockProperties(true, false, 1.0f, Texture.SimpleBlock(8));
            Data.Blocks[(int)BlockType.Water] = new BlockProperties(true, false, 1.0f, Texture.SimpleBlock(14));

            for (int i = 0; i < (int)ItemType.ItemTypeSize; i++)
                Data.Items.Add(new ItemProperties());

            Data.Items[(int)BlockType.Air] = new ItemProperties(0);
            Data.Items[(int)BlockType.Grass] = new ItemProperties(1);
            Data.Items[(int)BlockType.Log] = new ItemProperties(4);
            Data.Items[(int)BlockType.Dirt] = new ItemProperties(2);
            Data.Items[(int)BlockType.Stone] = new ItemProperties(3);
            Data.Items[(int)BlockType.Glass] = new ItemProperties(7);
            Data.Items[(int)BlockType.DryGrass] = new ItemProperties(12);
            Data.Items[(int)BlockType.Iron] = new ItemProperties(6);
            Data.Items[(int)BlockType.Leaves] = new ItemProperties(8);
            Data.Items[(int)BlockType.Water] = new ItemProperties(14);

            Data.Items[(int)ItemType.Drygrassblade] = new ItemProperties(9);
            Data.Items[(int)ItemType.Stick] = new ItemProperties(11);
            Data.Items[(int)ItemType.StonePickaxe] = new ItemProperties(13);
        }
        public static void Update(double TimeStep,int two)
        {
            Console.WriteLine("Update C# with:" + TimeStep + two);
        }
        public static void Print(string msg)
        {
            Console.WriteLine(msg);
        }
        public static void OnRightClick(ulong UUID)
        {
        }
        public static void OnLeftClick(ulong UUID)
        {
            Block.GetPlayerFacingBlock(UUID, out Block block);
            if (block.Type != BlockType.Invalid)
            {
                Block.ReplaceBlock(block, BlockType.Air);

                ItemStack itemStack = new ItemStack((ItemType)block.Type, 1);
                Player.AddItemToInventory(UUID, itemStack);
            }
        }
        public static void OnMiddleClick(ulong UUID)
        {
        }
        public static void OnCommand(ulong UUID, string Command)
        {
            string[] Tokens = Command.Split(' ');
            if (Tokens[0].Equals("/give"))
            {
                Player.AddItemToInventory(UUID, new ItemStack(ItemType.Stick, 3));
            }
            else if (Tokens[0].Equals("/clear"))
            {
                Player.RemoveItemFromInventory(UUID, new ItemStack(ItemType.Stick, 1));
            }
            else if (Tokens[0].Equals("/tp"))
            {
                if(Tokens.Length == 4)
                {
                    Player.SetPosition(UUID, new Vector3<double>(double.Parse(Tokens[1]), double.Parse(Tokens[2]), double.Parse(Tokens[3])));
                }
            }
        }
    }
}
