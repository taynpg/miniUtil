#include <gtest/gtest.h>
#include <miniUtil.h>

TEST(CToolTest, SplitFunction)
{
    std::string str = "apple,banana,cherry";
    std::string delimiter = ",";

    std::vector<std::string> expected = {"apple", "banana", "cherry"};
    std::vector<std::string> result = miniUtil::split(str, delimiter);

    EXPECT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(result[i], expected[i]);
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}