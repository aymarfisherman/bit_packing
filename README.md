# bit_packing
Fast bit packing library for efficient data packing/unpacking. 
Great to pack/unpack real-time network data for games.

The library relies heavily on template parameters to use the least amount of bits necessary in order to pack your data.

It exposes a class named bit_packing::BitSet, which have a series of push and pop methods.

A basic pipeline when using it for network would be:
- You push your data to a bit_packing::BitSet;
- Extract the packed data via bitSet.getPackedData();
- Send it to other peers via any networking framework (see Boost::Asio);
- Now on the receiving side, you receive the packedData and construct a BitSet;
- Then you pop the data and feed your objects, syncing the data between the server and the client (if that was the case).

Each push must be matched with a similar pop.

The push/pop methods are:
```
//precision defines the resolution of your data, e.g: precision 2 means it guarantees the packed data will be equal to the pushed data up to the second mantissa digit (resolution of 0.01)
void pushFloat<min, max, precision>(float);
float popFloat<min, max, precision>();

void pushInt<min, max>(int);
int popInt<min, max>();

void pushBool(bool);
bool popBool();

void pushString<maxLength>(string);
string popString<maxLength>();

void pushBitSet(otherBitSet);
bit_packing::BitSet popBitSet();
```

Keep in mind that this library will crop up your data in favor of reducing the number of bytes necessary to represent it. It's absolutely not Lossless. But it is very useful neverthless. For instance, let's assume the following data for a multiplayer game:

```
struct PlayerData{
    int life; // from 0 to 100: 4 bytes
    float position[3]; // x and z ranges from -500.0 to 500.0 and y ranges from 0.0 to 100.0, and they need not a resolution finer than 0.01: 3*4 bytes
    float direction[3]; // unitary vector, resolution of 0.001 works great!: 3*4 bytes
    float speed; // from 0 to 15, resolution of 0.1 works fine: 4 bytes
}
```

If we would naively serialize this data and send to other machines at a tick rate of 20 fps, we would send at least 32 bytes per player per tick. If it had 4 players the server would send at least 2560 bytes per second.

Now using the bit_packing library we could define the following methods:
```
std::string packPlayerData(const PlayerData& playerData) {
    bit_packing::BitSet bitSet;
    bitSet.pushInt<0, 100>(playerData.life); // 7 bits
    bitSet.pushFloat<-500, 500, 2>(playerData.position[0]); // 17 bits
    bitSet.pushFloat<0, 100, 2>(playerData.position[1]); // 14 bits
    bitSet.pushFloat<-500, 500, 2>(playerData.position[2]); // 17 bits
    bitSet.pushFloat<-1, 1, 3>(playerData.direction[0]); // 11 bits
    bitSet.pushFloat<-1, 1, 3>(playerData.direction[1]); // 11 bits
    bitSet.pushFloat<-1, 1, 3>(playerData.direction[2]); // 11 bits
    bitSet.pushFloat<0, 15, 1>(playerData.speed); // 8 bits
    return bitSet.getPackedData(); // total 96 bits: 12 bytes
}

void unpackPlayerData(const std::string packed, PlayerData& playerData) {
    bit_packing::BitSet bitSet(packed);
    playerData.life = bitSet.popInt<0, 100>();
    playerData.position[0] = bitSet.popFloat<-500, 500, 2>();
    playerData.position[1] = bitSet.popFloat<0, 100, 2>();
    playerData.position[2] = bitSet.popFloat<-500, 500, 2>();
    playerData.direction[0] = bitSet.popFloat<-1, 1, 3>();
    playerData.direction[1] = bitSet.popFloat<-1, 1, 3>();
    playerData.direction[2] = bitSet.popFloat<-1, 1, 3>();
    playerData.speed = bitSet.popFloat<0, 15, 1>();
}
```

Now each player data takes 12 bytes. Assuming the same case, the server now sends 960 bytes per second, 62.5% less data than the naive approach.

Example:
```
#include <gtest_wrapper.h>
#include <BitPacking.h>

TEST(BitPackingTests, BitSetTest1) {
	bit_packing::BitSet bitset;
	bitset.pushFloat<-5, 5, 1>(3.1415f);
	bitset.pushFloat<-5000, 0, 1>(-3141.59265f);
	bitset.pushInt<-100, 0>(-51);
	bitset.pushFloat<32, 42, 3>(36.5413f);
	bitset.pushInt<0, 100>(99);
	bitset.pushBool(true);
	bitset.pushString<100>("foo bar foo bar foo bar");
	bitset.pushInt<0, 100>(84);
	bitset.pushInt<0, 100>(1);
	bitset.pushBool(false);
	bitset.pushBool(false);
	bitset.pushBool(true);
	bitset.pushInt<0, 100>(0);
	bitset.pushFloat<-600, -500, 3>(-584.514f);

	EXPECT_EQ(bitset.getSizeInBytes(), 37);
	EXPECT_EQ(bitset.getTotalBitsSet(), 289);

	auto foo = bitset.popFloat<-5, 5, 1>();
	EXPECT_NEAR(foo, 3.1f, 0.1f);
	foo = bitset.popFloat<-5000, 0, 1>();
	EXPECT_NEAR(foo, -3141.6f, 0.1f);
	foo = bitset.popInt<-100, 0>();
	EXPECT_EQ(foo, -51);
	foo = bitset.popFloat<32, 42, 3>();
	EXPECT_NEAR(foo, 36.541f, 0.001f);
	foo = bitset.popInt<0, 100>();
	EXPECT_EQ(foo, 99);
	bool fooBool = bitset.popBool();
	EXPECT_TRUE(fooBool);
	EXPECT_EQ(bitset.popString<100>(), "foo bar foo bar foo bar");
	foo = bitset.popInt<0, 100>();
	EXPECT_EQ(foo, 84);
	foo = bitset.popInt<0, 100>();
	EXPECT_EQ(foo, 1);
	fooBool = bitset.popBool();
	EXPECT_FALSE(fooBool);
	fooBool = bitset.popBool();
	EXPECT_FALSE(fooBool);
	fooBool = bitset.popBool();
	EXPECT_TRUE(fooBool);
	foo = bitset.popInt<0, 100>();
	EXPECT_EQ(foo, 0);
	foo = bitset.popFloat<-600, -500, 3>();
	EXPECT_NEAR(foo, -584.514f, 0.001f);

	bit_packing::BitSet bitset2(bitset.getPackedData());

	foo = bitset2.popFloat<-5, 5, 1>();
	EXPECT_NEAR(foo, 3.1f, 0.1f);
	foo = bitset2.popFloat<-5000, 0, 1>();
	EXPECT_NEAR(foo, -3141.6f, 0.1f);
	foo = bitset2.popInt<-100, 0>();
	EXPECT_EQ(foo, -51);
	foo = bitset2.popFloat<32, 42, 3>();
	EXPECT_NEAR(foo, 36.541f, 0.001f);
	foo = bitset2.popInt<0, 100>();
	EXPECT_EQ(foo, 99);
	fooBool = bitset2.popBool();
	EXPECT_TRUE(fooBool);
	EXPECT_EQ(bitset2.popString<100>(), "foo bar foo bar foo bar");
	foo = bitset2.popInt<0, 100>();
	EXPECT_EQ(foo, 84);
	foo = bitset2.popInt<0, 100>();
	EXPECT_EQ(foo, 1);
	fooBool = bitset2.popBool();
	EXPECT_FALSE(fooBool);
	fooBool = bitset2.popBool();
	EXPECT_FALSE(fooBool);
	fooBool = bitset2.popBool();
	EXPECT_TRUE(fooBool);
	foo = bitset2.popInt<0, 100>();
	EXPECT_EQ(foo, 0);
	foo = bitset2.popFloat<-600, -500, 3>();
	EXPECT_NEAR(foo, -584.514f, 0.001f);
}
```