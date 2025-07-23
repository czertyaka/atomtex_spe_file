#include <string>
#include <exception>
#include <stdexcept>

#include <gtest/gtest.h>

#include "lines.hpp"

using namespace atomtex_spe_file;

TEST(LinesTest, ThrowsOnEmptyRange)
{
    const std::string s{"hello"};
    EXPECT_THROW(Lines<decltype(s)::const_iterator>(s.cbegin(), s.cbegin()),
        std::exception);
}

TEST(LinesTest, GetOnlyLine)
{
    const std::string s{"hello"};
    static_assert(StringForwardIterator<decltype(s)::const_iterator>);
    static_assert(!StringReverseIterator<decltype(s)::const_iterator>);
    Lines<decltype(s)::const_iterator> lines{s.cbegin(), s.cend()};
    EXPECT_EQ(lines[0], "hello");
}

TEST(LinesTest, GetNonexistentLine)
{
    const std::string s{"hello"};
    Lines<decltype(s)::const_iterator> lines{s.cbegin(), s.cend()};
    EXPECT_THROW(lines[1], std::out_of_range);
}

TEST(LinesTest, GetFirstLine)
{
    const std::string s{"hello\n"
                        "world"};
    Lines<decltype(s)::const_iterator> lines{s.cbegin(), s.cend()};
    EXPECT_EQ(lines[0], "hello");
}

TEST(LinesTest, GetSecondLine)
{
    const std::string s{"hello\n"
                        "world"};
    Lines<decltype(s)::const_iterator> lines{s.cbegin(), s.cend()};
    const auto line = lines[1];
    EXPECT_EQ(line, "world");
}

TEST(LinesTest, GetFifthLine)
{
    const std::string s{"hello\n"
                        "world\n"
                        "!\n"
                        "I\n"
                        "am\n"
                        "test\n"
                        "LinesTest\n"
                        "GetFifthLine\n"};
    Lines<decltype(s)::const_iterator> lines{s.cbegin(), s.cend()};
    EXPECT_EQ(lines[4], "am");
}

TEST(LinesTest, GetLastLines)
{
    const std::string s{"hello\n"
                        "world"};
    Lines<decltype(s)::const_reverse_iterator> lines{s.rbegin(), s.rend()};
    EXPECT_EQ(lines[0], "world");
    EXPECT_EQ(lines[1], "hello");
}

TEST(LinesTest, ReverseGetTwoLines)
{
    const std::string s{"hello\n"
                        "world\n"
                        "!\n"
                        "I\n"
                        "am\n"
                        "test\n"
                        "LinesTest\n"
                        "GetFifthLine"};
    Lines<decltype(s)::const_reverse_iterator> lines{s.rbegin(), s.rend()};
    EXPECT_EQ(lines[1], "LinesTest");
    EXPECT_EQ(lines[3], "am");
}

TEST(LinesTest, ReverseTralingNewline)
{
    const std::string s{"hello\n"
                        "world\n"};
    Lines<decltype(s)::const_reverse_iterator> lines{s.rbegin(), s.rend()};
    EXPECT_EQ(lines[0], "");
    EXPECT_EQ(lines[1], "world");
}

TEST(LinesTest, WorksWithUtf16le)
{
    const std::u16string s{u"hello\n"
                           u"world"};
    Lines<decltype(s)::const_iterator> lines{s.cbegin(), s.cend()};
    EXPECT_EQ(lines[1], u"world");
}
