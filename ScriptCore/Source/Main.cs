using System;
using System.Numerics;
using System.Runtime.CompilerServices;

namespace Scripting
{
    public enum BlockType
    {
        Invalid = -1, Air, Grass, Dirt, Stone, Glass, Leaves, Log, Water, DryGrass, Iron, DirtSlab, DirtStairs, DirtVerticalSlab, LogCarpet, Haybale, BlockTypeSize
    }
    public enum ItemType
    {
        Invalid = -1, Drygrassblade = BlockType.BlockTypeSize, Stick, StonePickaxe, ItemTypeSize
    }
    public enum BlockModelType
    {
        SolidBlock, Slab, Stairs, VerticalSlab,Carpet,FakeBall,ModelTypeSize
    }
    public enum EntityType
    {
        Invalid, LivingBox
    }
    public enum Button
    {
        LeftMouse = -3, RightMouse =-2, MiddleMouse=-1,
        Space = 32,
        Slash = 47,
        n0 = 48, n1, n2, n3, n4, n5, n6, n7, n8, n9,
        A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Shift = 340, Control, Alt,
        EscapeKey = 256, Enter, Tab, BackSpace,
        RightArrow = 262, LeftArrow, DownArrow, UpArrow,
        F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
    }
    public enum ActionName
    {
        LeftHandInteract , RightHandInteract, ReloadAssembly, OpenInventory, ActionNameSize
    }
    public enum Screen
    {
        Game, Chatbox, Gui
    }
    public struct WorldGenerationData
    {
        public WorldGenerationData()
        {}
        public int Octaves = 8;
        public double Frequency = 256.0;
        public int YLevelStretch = 96;
        public int BiomeNoiseRange = 40;

        public BlockType Filler = BlockType.Air;
        public BlockType UnderGround = BlockType.Stone;
        public BlockType Dirt = BlockType.Dirt;
        public BlockType DryTop = BlockType.DryGrass;
        public BlockType WetTop = BlockType.Grass;
        public BlockType DeadTop = BlockType.Dirt;
        public BlockType StoneTop = BlockType.Stone;
        public BlockType Ore = BlockType.Iron;
        public BlockType Water = BlockType.Water;
    }
    public unsafe static class Data
    {
        public static BlockProperties[] Blocks = new BlockProperties[(int)BlockType.BlockTypeSize];
        public static ItemProperties[] Items = new ItemProperties[(int)ItemType.ItemTypeSize];
        public static Model[] BlockModels = new Model[(int)BlockModelType.ModelTypeSize];
        public static InputAction[] InputActions = new InputAction[(int)ActionName.ActionNameSize];
        public static Gui Inventory;
        public static WorldGenerationData GetWorldGenerationData()
        {
            return new WorldGenerationData();
        }
        public static void* GetItems()
        {
            fixed (void* pointer = Items)
            {
                return pointer;
            }
        }
        public static int GetInputActionCount()
        {
            return InputActions.Length;
        }
        public static void* GetInputActions()
        {
            fixed (void* pointer = InputActions)
            {
                return pointer;
            }
        }
        public static int GetItemCount()
        {
            return Items.Length;
        }
        public static int GetBlockCount()
        {
            return Blocks.Length;
        }
        public static void* GetBlocks()
        {
            fixed (void* pointer = Blocks)
            {
                return pointer;
            }
        }
        public static int GetModelCount()
        {
            return BlockModels.Length;
        }
        public static void* GetModel(int index)
        {
            fixed (void* pointer = BlockModels[index].faces)
            {
                return pointer;
            }
        }
        public static int GetFaceCount(int index)
        {
            return BlockModels[index].faces.Length;
        }
    }
    public struct Vector2<T>
    {
        public T x;
        public T y;
        public Vector2(T x, T y)
        {
            this.x = x; this.y = y;
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
    public struct Vector4<T>
    {
        public T x;
        public T y;
        public T z;
        public T w;
        public Vector4(T x, T y, T z, T w)
        {
            this.x = x; this.y = y; this.z = z; this.w = w;
        }
    }
    public struct Slot
    {
        public Vector2<float> Position;
        public bool Active;
    }
    public struct Model
    {
        public Face[] faces;
    }
    public enum Direction
    {
        Forward, Backward, Right, Left, Up, Down
    }
    public struct Face
    {
        public Face(Vector3<float> Position, Vector2<float> Size, Direction Direction, byte Condition, byte TextureIndex)
        {
            this.Position = Position;
            this.Size = Size;
            this.Direction = Direction;
            this.Condition = Condition;
            this.TextureIndex = TextureIndex;
            this.Active = true;
        }
        public Vector3<float> Position;
        public Vector2<float> Size;
        public Direction Direction;
        public byte Condition;
        public byte TextureIndex;
        public bool Active;
    }
    public struct Gui
    {
        public Vector4<float> Color;
        public Slot[] Slots;
    }
    public struct Block
    {
        public Vector3<int> Position;
        public BlockType Type;

        public static void Register(BlockType blockType, BlockProperties blockProperties)
        {
            Data.Blocks[(int)blockType] = blockProperties;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetBlock(Vector3<int> Position, out Block block);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetPlayerFacingBlock(ulong UUID, out Block block);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetPlayerBlockToPlace(ulong UUID, out Block block);
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
    public struct Entity
    {
        public Entity(ulong UUID,Vector3<double> Position, Vector3<double> Hitbox,EntityType Type)
        {
            this.UUID = UUID;
            this.Position = Position;
            this.Hitbox = Hitbox;
            this.entityType = Type;
        }
        public ulong UUID;
        public Vector3<double> Position;
        public Vector3<double> Hitbox;
        public EntityType entityType;

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Create(Entity entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetPlayerFacingEntity(ulong UUID, out Entity entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Kill(ulong UUID);

        public static ulong EntityId = 0;
    }
    public unsafe struct Player
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void AddItemToInventory(ulong UUID, ItemStack Item);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SetPosition(ulong UUID, Vector3<double> Position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetPosition(ulong UUID, out Vector3<double> Position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void RemoveItemFromInventory(ulong UUID, ItemStack Item);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetHoldingItemStack(ulong UUID, out ItemStack Item);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void IncrementRenderDistance(int value);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void HandleGui(ulong UUID, Vector4<float> Color,void* Slots, int SlotCount, bool Open);

        public static void HandleGui(ulong UUID, Gui gui, bool Open)
        {
            fixed(void* Slots = gui.Slots)
            {
                HandleGui(UUID, gui.Color, Slots,gui.Slots.Length, Open);
            }
        }
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
        public BlockProperties( bool Render, bool Transparent, float Translucency, Texture BlockTexture, BlockModelType Model = BlockModelType.SolidBlock)
        {
            this.Render = Render;
            this.Transparent = Transparent;
            this.Translucency = Translucency;
            this.BlockTexture = BlockTexture;
            this.Model = Model;
        }

        public Texture BlockTexture;
        public BlockModelType Model = BlockModelType.SolidBlock;
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
    public struct InputAction
    {
        public InputAction(Button defaultButton, ActionName action, Screen screen)
        {
            this.DefaultButton = defaultButton;
            this.Action = action;
            this.screen = screen;
        }
        public readonly Button DefaultButton;
        public readonly ActionName Action;
        public readonly Screen screen;

        public static void Register(InputAction inputAction)
        {
            Data.InputActions[(int)(inputAction.Action)] = inputAction;
        }
    }
    public unsafe static class Event
    {
        public static void Initialize()
        {
            Console.WriteLine("Initialized C# !");

            Block.Register(BlockType.Air, new BlockProperties(false, false, 1.0f, new Texture( 0, 0, 0, 0, 0, 0 )));
            Block.Register(BlockType.Grass, new BlockProperties(true, false, 1.0f, new Texture( 1, 1, 1, 1, 0, 2 )));
            Block.Register(BlockType.Log, new BlockProperties(true, false, 1.0f, new Texture(4, 4, 4, 4, 5, 5)));
            Block.Register(BlockType.Dirt, new BlockProperties(true, false, 1.0f, Texture.SimpleBlock(2)));
            Block.Register(BlockType.Stone, new BlockProperties(true, false, 1.0f, Texture.SimpleBlock(3)));
            Block.Register(BlockType.Glass, new BlockProperties(true, true, 1.0f, Texture.SimpleBlock(7)));
            Block.Register(BlockType.DryGrass, new BlockProperties(true, false, 1.0f, new Texture(12, 12, 12, 12, 10, 2)));
            Block.Register(BlockType.Iron,  new BlockProperties(true, false, 1.0f, Texture.SimpleBlock(6)));
            Block.Register(BlockType.Leaves,  new BlockProperties(true, true, 1.0f, Texture.SimpleBlock(8)));
            Block.Register(BlockType.Water, new BlockProperties(true, true, 0.4f, Texture.SimpleBlock(14)));
            Block.Register(BlockType.DirtSlab, new BlockProperties(true, true, 1.0f, Texture.SimpleBlock(2),BlockModelType.Slab));
            Block.Register(BlockType.DirtStairs, new BlockProperties(true, true, 1.0f, Texture.SimpleBlock(2), BlockModelType.Stairs));
            Block.Register(BlockType.DirtVerticalSlab, new BlockProperties(true, true, 1.0f, Texture.SimpleBlock(2), BlockModelType.VerticalSlab));
            Block.Register(BlockType.LogCarpet, new BlockProperties(true, true, 1.0f, new Texture(4, 4, 4, 4, 5, 5),BlockModelType.Carpet));
            Block.Register(BlockType.Haybale, new BlockProperties(true, true, 1.0f, new Texture(4, 4, 4, 4, 5, 5), BlockModelType.FakeBall));

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
            Data.Items[(int)BlockType.DirtSlab] = new ItemProperties(2);
            Data.Items[(int)BlockType.DirtStairs] = new ItemProperties(2);
            Data.Items[(int)BlockType.DirtVerticalSlab] = new ItemProperties(2);
            Data.Items[(int)BlockType.LogCarpet] = new ItemProperties(4);
            Data.Items[(int)BlockType.Haybale] = new ItemProperties(5);

            Data.Items[(int)ItemType.Drygrassblade] = new ItemProperties(9);
            Data.Items[(int)ItemType.Stick] = new ItemProperties(11);
            Data.Items[(int)ItemType.StonePickaxe] = new ItemProperties(13);

            Data.BlockModels[(int)BlockModelType.SolidBlock] = new Model()
            {
                faces = new Face[]
                {
                    new Face(new Vector3<float>(1.0f,0.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Right, 1, 0),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Left, 2, 1),
                    new Face(new Vector3<float>(0.0f,0.0f,1.0f), new Vector2<float>(1.0f,1.0f), Direction.Forward, 4, 2),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Backward, 8, 3),
                    new Face(new Vector3<float>(0.0f,1.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Up, 16, 4),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Down, 32, 5),
                }
            };
            Data.BlockModels[(int)BlockModelType.Slab] = new Model()
            {
                faces = new Face[]
                {
                    new Face(new Vector3<float>(1.0f,0.0f,0.0f), new Vector2<float>(1.0f,0.5f), Direction.Right, 1, 0),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,0.5f), Direction.Left, 2, 1),
                    new Face(new Vector3<float>(0.0f,0.0f,1.0f), new Vector2<float>(1.0f,0.5f), Direction.Forward, 4, 2),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,0.5f), Direction.Backward, 8, 3),
                    new Face(new Vector3<float>(0.0f,0.5f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Up, 16, 4),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Down, 32 ,5),
                }
            };
            Data.BlockModels[(int)BlockModelType.Carpet] = new Model()
            {
                faces = new Face[]
                {
                    new Face(new Vector3<float>(1.0f,0.0f,0.0f), new Vector2<float>(1.0f,0.1f), Direction.Right, 1, 0),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,0.1f), Direction.Left, 2, 1),
                    new Face(new Vector3<float>(0.0f,0.0f,1.0f), new Vector2<float>(1.0f,0.1f), Direction.Forward, 4, 2),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,0.1f), Direction.Backward, 8, 3),
                    new Face(new Vector3<float>(0.0f,0.1f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Up, 16, 4),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Down, 32 ,5),
                }
            };
            Data.BlockModels[(int)BlockModelType.VerticalSlab] = new Model()
            {
                faces = new Face[]
                {
                    new Face(new Vector3<float>(0.5f,0.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Right, 1, 0),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Left, 2, 1),
                    new Face(new Vector3<float>(0.0f,0.0f,1.0f), new Vector2<float>(0.5f,1.0f), Direction.Forward, 4, 2),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(0.5f,1.0f), Direction.Backward, 8, 3),
                    new Face(new Vector3<float>(0.0f,1.0f,0.0f), new Vector2<float>(0.5f,1.0f), Direction.Up, 16, 4),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(0.5f,1.0f), Direction.Down, 32 ,5),
                }
            };
            Data.BlockModels[(int)BlockModelType.Stairs] = new Model()
            {
                faces = new Face[]
                {
                    new Face(new Vector3<float>(1.0f,0.0f,0.0f), new Vector2<float>(1.0f,0.5f), Direction.Right, 1, 0),
                    new Face(new Vector3<float>(0.5f,0.5f,0.0f), new Vector2<float>(1.0f,0.5f), Direction.Right, 1, 0),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Left, 2, 1),
                    new Face(new Vector3<float>(0.0f,0.0f,1.0f), new Vector2<float>(1.0f,0.5f), Direction.Forward, 4, 2),
                    new Face(new Vector3<float>(0.0f,0.5f,1.0f), new Vector2<float>(0.5f,0.5f), Direction.Forward, 4, 2),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,0.5f), Direction.Backward, 8, 3),
                    new Face(new Vector3<float>(0.0f,0.5f,0.0f), new Vector2<float>(0.5f,0.5f), Direction.Backward, 8, 3),
                    new Face(new Vector3<float>(0.0f,1.0f,0.0f), new Vector2<float>(0.5f,1.0f), Direction.Up, 16, 4),
                    new Face(new Vector3<float>(0.5f,0.5f,0.0f), new Vector2<float>(0.5f,1.0f), Direction.Up, 16, 4),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(1.0f,1.0f), Direction.Down, 32 ,5),
                }
            };
            Data.BlockModels[(int)BlockModelType.FakeBall] = new Model()
            {
                faces = new Face[]
                {
                    new Face(new Vector3<float>(0.6f,0.0f,0.0f), new Vector2<float>(0.6f,0.6f), Direction.Right, 1, 0),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(0.6f,0.6f), Direction.Left, 2, 1),
                    new Face(new Vector3<float>(0.0f,0.0f,0.6f), new Vector2<float>(0.6f,0.6f), Direction.Forward, 4, 2),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(0.6f,0.6f), Direction.Backward, 8, 3),
                    new Face(new Vector3<float>(0.0f,0.6f,0.0f), new Vector2<float>(0.6f,0.6f), Direction.Up, 16, 4),
                    new Face(new Vector3<float>(0.0f,0.0f,0.0f), new Vector2<float>(0.6f,0.6f), Direction.Down, 32, 5),
                }
            };

            Data.Inventory = new()
            {
                Color = new(0.6f, 0.6f, 0.6f, 0.7f),
                Slots = new Slot[] {
                    new() { Position = new Vector2<float>(0, 0), Active = true},
                    new() { Position = new Vector2<float>(1, 0), Active = true},
                    new() { Position = new Vector2<float>(2, 0), Active = true},
                    new() { Position = new Vector2<float>(3, 0), Active = true},
                    new() { Position = new Vector2<float>(4, 0), Active = true},
                    new() { Position = new Vector2<float>(5, 0), Active = true},
                    new() { Position = new Vector2<float>(6, 0), Active = true},
                    new() { Position = new Vector2<float>(7, 0), Active = true},
                    new() { Position = new Vector2<float>(8, 0), Active = true},
                }
            };

            InputAction.Register(new InputAction(Button.LeftMouse, ActionName.LeftHandInteract, Screen.Game));
            InputAction.Register(new InputAction(Button.RightArrow, ActionName.RightHandInteract, Screen.Game));
            InputAction.Register(new InputAction(Button.P, ActionName.ReloadAssembly, Screen.Game));
            InputAction.Register(new InputAction(Button.E, ActionName.OpenInventory, Screen.Game));
        }
        public static void GlobalUpdateEvent()
        {
            
        }
        public static void PlayerUpdateEvent(ulong UUID)
        {
        }
        public static void OnRightClick(ulong UUID)
        {
            Block.GetPlayerBlockToPlace(UUID, out Block block);
            Block.GetPlayerFacingBlock(UUID, out Block block2);
            if (block.Type != BlockType.Invalid)
            {
                Player.GetHoldingItemStack(UUID, out ItemStack Item);
                if(Item.Count > 0 && (int)Item.Type < (int)BlockType.BlockTypeSize)
                {
                    Block.ReplaceBlock(block, (BlockType)Item.Type);
                    Item.Count = 1;
                    Player.RemoveItemFromInventory(UUID, Item);
                }
                else if(Item.Count == 0 && block2.Type == BlockType.DryGrass)
                {
                    Block.ReplaceBlock(block2, BlockType.Dirt);
                }
                else if(Item.Type == ItemType.Stick)
                {
                    Entity entity = new Entity(Entity.EntityId++, new Vector3<double>(block.Position.x, block.Position.y, block.Position.z), new Vector3<double>(1, 1, 1), EntityType.LivingBox);
                    Entity.Create(entity);
                }
            }
        }
        public static void OnLeftClick(ulong UUID)
        {
            Entity.GetPlayerFacingEntity(UUID, out Entity entity);
            if(entity.entityType != EntityType.Invalid)
            {
                Entity.Kill(entity.UUID);
            }

            Block.GetPlayerFacingBlock(UUID, out Block block);
            if (block.Type != BlockType.Invalid)
            {
                Block.ReplaceBlock(block, BlockType.Air);

                ItemStack itemStack = new ItemStack((ItemType)block.Type, 1);
                Player.AddItemToInventory(UUID, itemStack);
            }
        }
        public static void OnAction(ulong UUID, int identifier)
        {
            ActionName action = (ActionName)identifier;
            switch (action)
            {
                case ActionName.ReloadAssembly:
                    ///////////
                    break;
                case ActionName.OpenInventory:
                    Player.HandleGui(UUID, Data.Inventory, true);
                    break;
                case ActionName.LeftHandInteract:
                    OnLeftClick(UUID);
                    break;
                case ActionName.RightHandInteract:
                    OnRightClick(UUID);
                    break;
                default: Console.WriteLine("[C#]: Unknown action"); break;
            }
        }
        public static void OnCommand(ulong UUID, string Command)
        {
            string[] Tokens = Command.Split(' ');
            if (Tokens[0].Equals("/give"))
            {
                int type;
                if (Tokens[1] == "block")
                {
                    Enum.TryParse(Tokens[2], out BlockType Type);
                    type = (int)Type;
                }
                else
                {
                    Enum.TryParse(Tokens[2], out ItemType Type);
                    type = (int)Type;
                }
                Player.AddItemToInventory(UUID, new ItemStack((ItemType)type, int.Parse(Tokens[3])));

            }
            else if (Tokens[0].Equals("/clear"))
            {
                int type;
                if (Tokens[1] == "block")
                {
                    Enum.TryParse(Tokens[2], out BlockType Type);
                    type = (int)Type;
                }
                else
                {
                    Enum.TryParse(Tokens[2], out ItemType Type);
                    type = (int)Type;
                }
                Player.RemoveItemFromInventory(UUID, new ItemStack((ItemType)type, int.Parse(Tokens[3])));
            }
            else if (Tokens[0].Equals("/tp"))
            {
                if (Tokens.Length == 4)
                {
                    Player.SetPosition(UUID, new Vector3<double>(double.Parse(Tokens[1]), double.Parse(Tokens[2]), double.Parse(Tokens[3])));
                }
            }
            else if (Tokens[0].Equals("/set") && Tokens.Length == 5)
            {
                Block.GetBlock(new Vector3<int> (int.Parse(Tokens[1]), int.Parse(Tokens[2]), int.Parse(Tokens[3])),out Block block);
                Enum.TryParse(Tokens[4], out BlockType Type);
                Block.ReplaceBlock(block, Type);
            }
            else if (Tokens[0].Equals("/set") && Tokens.Length == 8)
            {
                Vector3<int> StartPosition = new Vector3<int>(int.Parse(Tokens[1]), int.Parse(Tokens[2]), int.Parse(Tokens[3]));
                Vector3<int> EndPosition = new Vector3<int>(int.Parse(Tokens[4]), int.Parse(Tokens[5]), int.Parse(Tokens[6]));
                for (int x = StartPosition.x; x <= EndPosition.x; x++)
                {
                    for (int y = StartPosition.y; y <= EndPosition.y; y++)
                    {
                        for (int z = StartPosition.z; z <= EndPosition.z; z++)
                        {
                            Block.GetBlock(new Vector3<int>(x,y,z), out Block block);
                            Enum.TryParse(Tokens[7], out BlockType Type);
                            Block.ReplaceBlock(block, Type); 
                        }
                    }
                }
            }
            else if(Tokens[0].Equals("/summon") && Tokens.Length == 4)
            {
                Entity entity = new Entity(Entity.EntityId++, new Vector3<double>(double.Parse(Tokens[1]), double.Parse(Tokens[2]), double.Parse(Tokens[3])), new Vector3<double>(1, 1, 1),EntityType.LivingBox);
                Entity.Create(entity);
            }
        }
    }
}