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

// ==================== miniBuffer 测试 ====================

TEST(MiniBufferTest, InitialSizeIsZero)
{
    miniBuffer buf;
    EXPECT_EQ(buf.Size(), 0u);
}

TEST(MiniBufferTest, AppendAndSize)
{
    miniBuffer buf;
    const char* data = "hello";
    buf.Append(data, 5);
    EXPECT_EQ(buf.Size(), 5u);
}

TEST(MiniBufferTest, AppendMultiple)
{
    miniBuffer buf;
    buf.Append("abc", 3);
    buf.Append("def", 3);
    EXPECT_EQ(buf.Size(), 6u);
}

TEST(MiniBufferTest, Clear)
{
    miniBuffer buf;
    buf.Append("test", 4);
    EXPECT_EQ(buf.Size(), 4u);
    buf.Clear();
    EXPECT_EQ(buf.Size(), 0u);
}

TEST(MiniBufferTest, ClearEmpty)
{
    miniBuffer buf;
    buf.Clear();
    EXPECT_EQ(buf.Size(), 0u);
}

TEST(MiniBufferTest, IndexOfFound)
{
    miniBuffer buf;
    buf.Append("hello world", 11);
    int idx = buf.IndexOf("world");
    EXPECT_EQ(idx, 6);
}

TEST(MiniBufferTest, IndexOfNotFound)
{
    miniBuffer buf;
    buf.Append("hello", 5);
    int idx = buf.IndexOf("xyz");
    EXPECT_EQ(idx, -1);
}

TEST(MiniBufferTest, IndexOfWithStartPos)
{
    miniBuffer buf;
    buf.Append("ababab", 6);
    // 从位置 2 开始找 "ab"，应该在索引 2 找到
    int idx = buf.IndexOf(std::string("ab"), 2);
    EXPECT_EQ(idx, 2);
}

TEST(MiniBufferTest, IndexOfCharData)
{
    miniBuffer buf;
    const char data[] = {'x', 'y', 'z'};
    buf.Append("abcxyz123", 9);
    int idx = buf.IndexOf(reinterpret_cast<const char*>(data), 3u);
    EXPECT_EQ(idx, 3);
}

TEST(MiniBufferTest, RemoveOfBasic)
{
    miniBuffer buf;
    buf.Append("abcdef", 6);
    buf.RemoveOf(1, 3);   // 移除 bcd，剩下 aef
    EXPECT_EQ(buf.Size(), 3u);
    EXPECT_EQ(buf.IndexOf(std::string("aef")), 0);
}

TEST(MiniBufferTest, RemoveOfBeyondEnd)
{
    miniBuffer buf;
    buf.Append("abcd", 4);
    buf.RemoveOf(2, 100);   // size 超出范围，应该只移除到末尾
    EXPECT_EQ(buf.Size(), 2u);
    EXPECT_EQ(buf.IndexOf(std::string("ab")), 0);
}

TEST(MiniBufferTest, RemoveOfStartBeyondSize)
{
    miniBuffer buf;
    buf.Append("abcd", 4);
    buf.RemoveOf(10, 1);   // start 超出范围，应该无操作
    EXPECT_EQ(buf.Size(), 4u);
}

TEST(MiniBufferTest, RemoveOfZeroSize)
{
    miniBuffer buf;
    buf.Append("abcd", 4);
    buf.RemoveOf(1, 0);   // 移除 0 个字节
    EXPECT_EQ(buf.Size(), 4u);
}

TEST(MiniBufferTest, GetBuffer)
{
    miniBuffer buf;
    buf.Append("test", 4);
    const std::vector<char>& buffer = buf.GetBuffer();
    EXPECT_EQ(buffer.size(), 4u);
    EXPECT_EQ(buffer[0], 't');
    EXPECT_EQ(buffer[1], 'e');
    EXPECT_EQ(buffer[2], 's');
    EXPECT_EQ(buffer[3], 't');
}

TEST(MiniBufferTest, IndexOfEmpty)
{
    miniBuffer buf;
    // 空缓冲区查找
    int idx = buf.IndexOf(std::string("anything"));
    EXPECT_EQ(idx, -1);
}

TEST(MiniBufferTest, IndexOfBinaryData)
{
    miniBuffer buf;
    uint8_t raw[] = {0x00, 0x01, 0x02, 0xFF, 0xFE, 0xFD};
    buf.Append(reinterpret_cast<const char*>(raw), 6);
    const uint8_t target[] = {0xFF, 0xFE};
    int idx = buf.IndexOf(reinterpret_cast<const char*>(target), 2u);
    EXPECT_EQ(idx, 3);
}

TEST(MiniBufferTest, RemoveOfAll)
{
    miniBuffer buf;
    buf.Append("hello", 5);
    buf.RemoveOf(0, 5);   // 移除全部
    EXPECT_EQ(buf.Size(), 0u);
}

TEST(MiniBufferTest, AppendEmpty)
{
    miniBuffer buf;
    buf.Append("", 0);   // 追加空数据
    EXPECT_EQ(buf.Size(), 0u);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}