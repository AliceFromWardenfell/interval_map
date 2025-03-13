#include <gtest/gtest.h>

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	std::cout << "Press Enter to exit...";
	std::cin.get();

	return result;
}

