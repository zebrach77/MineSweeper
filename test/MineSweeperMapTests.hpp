#include <gtest/gtest.h>

TEST(MapTest, test_generation_max_10x10) {
	size_t mines_count = 100 - 9;
	MineSweeperMap map(10, 10, mines_count);

	map.InitMap(0, 0);
	size_t cnt = 0;
	for (size_t i = 0; i < 10; i++) {
		for (size_t j = 0; j < 10; j++) {
			if (map[i][j] == '*') {
				++cnt;
			}
		}
	}
	ASSERT_EQ(mines_count, cnt);
}

TEST(MapTest, test_generation_max_20x20) {
	size_t mines_count = 400 - 9;
	MineSweeperMap map(20, 20, mines_count);
	map.InitMap(0, 0);
	size_t cnt = 0;
	for (size_t i = 0; i < 20; i++) {
		for (size_t j = 0; j < 20; j++) {
			if (map[i][j] == '*') {
				++cnt;
			}
		}
	}
	ASSERT_EQ(mines_count, cnt);
}

