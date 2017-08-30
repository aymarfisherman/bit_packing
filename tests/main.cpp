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
	bitset.pushInt<0, 100>(84);
	bitset.pushInt<0, 100>(1);
	bitset.pushBool(false);
	bitset.pushBool(false);
	bitset.pushBool(true);
	bitset.pushInt<0, 100>(0);

	EXPECT_EQ(bitset.getSizeInBytes(), 16);
	EXPECT_EQ(bitset.getTotalBitsSet(), 76);

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
}

TEST(BitPackingTests, BitSetTest2) {
	bit_packing::BitSet bitset;
	bitset.pushFloat<-5000, 5000, 2>(5.01021f);
	bitset.pushFloat<-5000, 5000, 2>(684.541f);
	bitset.pushFloat<0, 300, 2>(250.585f);

	EXPECT_EQ(bitset.getSizeInBytes(), 8);
	EXPECT_EQ(bitset.getTotalBitsSet(), 55);

	auto foo = bitset.popFloat<-5000, 5000, 2>();
	EXPECT_NEAR(foo, 5.01f, 0.01f);
	foo = bitset.popFloat<-5000, 5000, 2>();
	EXPECT_NEAR(foo, 684.54f, 0.01f);
	foo = bitset.popFloat<0, 300, 2>();
	EXPECT_NEAR(foo, 250.58f, 0.01f);
}

TEST(BitPackingTests, FloatPackingTest1) {
	float foo = 3.1415f;
	int packBitCount;
	auto bits = bit_packing::packFloat<-5, 5, 1>(foo, packBitCount);
	EXPECT_EQ(packBitCount, 7);

	int unpackBitCount;
	auto bar = bit_packing::unpackFloat<-5, 5, 1>(bits, unpackBitCount);
	EXPECT_NEAR(bar, 3.1f, 0.1f);
	EXPECT_NEAR(bar, foo, 0.1f);
	EXPECT_EQ(unpackBitCount, packBitCount);
}

TEST(BitPackingTests, FloatPackingTest2) {
	float foo = -3141.59265f;
	int packBitCount;
	auto bits = bit_packing::packFloat<-5000, 0, 1>(foo, packBitCount);
	EXPECT_EQ(packBitCount, 16);

	int unpackBitCount;
	auto bar = bit_packing::unpackFloat<-5000, 0, 1>(bits, unpackBitCount);
	EXPECT_NEAR(bar, -3141.6f, 0.1f);
	EXPECT_NEAR(bar, foo, 0.1f);
	EXPECT_EQ(unpackBitCount, packBitCount);
}

TEST(BitPackingTests, FloatPackingTest3) {
	float foo = 3141.59265f;
	int packBitCount;
	auto bits = bit_packing::packFloat<0, 10000, 3>(foo, packBitCount);
	EXPECT_EQ(packBitCount, 24);

	int unpackBitCount;
	auto bar = bit_packing::unpackFloat<0, 10000, 3>(bits, unpackBitCount);
	EXPECT_NEAR(bar, 3141.593f, 0.1f);
	EXPECT_NEAR(bar, foo, 0.1f);
	EXPECT_EQ(unpackBitCount, packBitCount);
}

TEST(BitPackingTests, IntPackingTest1) {
	int foo = -51;
	int packBitCount;
	auto bits = bit_packing::packInt<-100, 0>(foo, packBitCount);
	EXPECT_EQ(packBitCount, 7);

	int unpackBitCount;
	auto bar = bit_packing::unpackInt<-100, 0>(bits, unpackBitCount);
	EXPECT_EQ(bar, -51);
	EXPECT_EQ(bar, foo);
	EXPECT_EQ(unpackBitCount, packBitCount);
}