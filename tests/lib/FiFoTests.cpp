#include "CppUTest/TestHarness.h"

extern "C" {
#include "FiFo.h"

FiFo myFiFo;
uint8_t buffer[10];
}

TEST_GROUP(FiFoTestGroup) {
	void setup() {
		FiFo_init(&myFiFo, buffer, 1, 10);
	}

	void teardown() {

	}
};

TEST(FiFoTestGroup, CanPushAndPop) {
	uint8_t data = 0x43;
	uint8_t readData = 0;
	FiFo_push(&myFiFo, &data);
	CHECK_TRUE(FiFo_pop(&myFiFo, &readData));
	CHECK_EQUAL(data, readData);
}

TEST(FiFoTestGroup, CannotPopIfNoElem) {
	uint8_t data = 0x43;
	uint8_t readData = 0;
	CHECK_FALSE(FiFo_pop(&myFiFo, &readData));
	FiFo_push(&myFiFo, &data);
	CHECK_TRUE(FiFo_pop(&myFiFo, &readData));
	CHECK_EQUAL(data, readData);
	CHECK_FALSE(FiFo_pop(&myFiFo, &readData));
}

TEST(FiFoTestGroup, CannotPushIfFull) {
	uint8_t data = 0x43;
	for (int i=0; i<10; i++){
		CHECK_TRUE(FiFo_push(&myFiFo, &data));
	}
	CHECK_FALSE(FiFo_push(&myFiFo, &data));
}

TEST(FiFoTestGroup, SizeIsWorkingAsIntended) {
	uint8_t data = 0x43;
	uint16_t size;
	for (int i = 0; i < 10; i++) {
		size = FiFo_size(&myFiFo);
		CHECK_EQUAL(i, size);
		FiFo_push(&myFiFo, &data);
	}
	size = FiFo_size(&myFiFo);
	CHECK_EQUAL(10, size);
	FiFo_push(&myFiFo, &data);
	size = FiFo_size(&myFiFo);
	CHECK_EQUAL(10, size);
	for (int i = 10; i >= 0; i--) {
		size = FiFo_size(&myFiFo);
		CHECK_EQUAL(i, size);
		FiFo_pop(&myFiFo, &data);
	}
	size = FiFo_size(&myFiFo);
	CHECK_EQUAL(0, size);
}
