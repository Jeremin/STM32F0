#include "CppUTest/TestHarness.h"

extern "C" {
#include <stdint.h>
#include "BinUtils.h"
}

TEST_GROUP(BinUtilsTestGroup) {
	void setup() {

	}

	void teardown() {

	}
};

TEST(BinUtilsTestGroup, CanConvert4ByteTo32Bit) {
	uint8_t a = 0x12, b = 0x34, c = 0x56, d = 0xab;
	uint32_t result = BinUtils_create32BitFrom4Bytes(a, b, c, d);
	CHECK_EQUAL(0x123456ab, result);
}

TEST(BinUtilsTestGroup, CanConvert32BitTo4Bytes) {
	uint8_t a = 0, b = 0, c = 0, d = 0;
	uint32_t doubeword = 0xfecb9865;
	BinUtils_get4BytesFrom32Bit(doubeword, &a, &b, &c, &d);
	CHECK_EQUAL(0xfe, a);
	CHECK_EQUAL(0xcb, b);
	CHECK_EQUAL(0x98, c);
	CHECK_EQUAL(0x65, d);
}
