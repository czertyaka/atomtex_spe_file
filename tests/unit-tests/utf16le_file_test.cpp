#include <gtest/gtest.h>

#include <exception>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <ranges>
#include <filesystem>
#include <random>
#include <fstream>

#include "atomtex_spe_file/utf16le_file.hpp"

using namespace atomtex_spe_file;

template<std::size_t N>
std::ostream& write(std::ostream& os, std::array<std::byte, N> arr)
{
    return os.write(reinterpret_cast<const char*>(arr.data()), arr.size());
}

std::ostream& write(std::ostream& os, std::u16string_view str)
{
    return os.write(reinterpret_cast<const char*>(str.data()), str.size() * 2);
}

static std::string describe_iostate_flags(const std::istream& stream)
{
    std::vector<std::string_view> flags{};
    if (stream.rdstate() & std::ios_base::badbit)
    {
        flags.push_back("bad");
    }
    if (stream.rdstate() & std::ios_base::failbit)
    {
        flags.push_back("fail");
    }
    if (stream.rdstate() & std::ios_base::eofbit)
    {
        flags.push_back("eof");
    }
#ifndef __cpp_lib_ranges_join_with
#error "no std::views::join_with!"
#endif
    auto joined = flags | std::views::join_with(' ');

    std::string result{};
    for (auto v : joined)
    {
        result += v;
    }
    return result;
}

TEST(Utf16leFileTest, ThrowsOnEmptyString)
{
    std::stringstream ss{};
    ASSERT_TRUE(ss) << describe_iostate_flags(ss);
    EXPECT_THROW(Utf16leFile(ss, "<anonymous>"), std::exception);
}

TEST(Utf16leFileTest, ThrowsOnInvalidBOM)
{
    std::stringstream ss{};
    write(ss, u"hello");
    ASSERT_TRUE(ss) << describe_iostate_flags(ss);
    EXPECT_THROW(Utf16leFile(ss, "<anonymous>"), std::exception);
}

TEST(Utf16leFileTest, EmptyContent)
{
    std::stringstream ss{};
    write(ss, Utf16leFile::BOM);
    ASSERT_TRUE(ss) << describe_iostate_flags(ss);
    Utf16leFile file(ss, "<anonymous>");
    EXPECT_TRUE(file.Content().empty());
}

TEST(Utf16leFileTest, NonEmptyContent)
{
    std::stringstream ss{};
    write(ss, Utf16leFile::BOM);
    write(ss, u"hello");

    ASSERT_TRUE(ss) << describe_iostate_flags(ss);
    Utf16leFile file(ss, "<anonymous>");
    EXPECT_EQ(file.Content(), u"hello");
}

class TempFile
{
public:
    TempFile()
        : path_(std::filesystem::temp_directory_path() / random_filename())
    {
    }
    ~TempFile()
    {
        std::filesystem::remove(path_);
    }
    std::filesystem::path Path() const
    {
        return path_;
    }

private:
    static std::string random_filename()
    {
        constexpr std::string_view chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuv"
            "wxyz0123456789";

        std::random_device rd{};
        std::mt19937 gen{rd()};

        std::uniform_int_distribution<std::size_t> dist{0, chars.length() - 1};

        std::string result(30, '\0');

        std::generate_n(result.begin(), result.size(),
            [&]() { return chars[dist(gen)]; });
        return result;
    }
    const std::filesystem::path path_;
};

TEST(Utf16FileTest, FromFilesystem)
{
    TempFile tmp{};

    std::ofstream ofs{tmp.Path(), std::ios_base::trunc};
    write(ofs, Utf16leFile::BOM);
    write(ofs, u"hello\n");
    write(ofs, u"world\n");
    ofs.close();

    Utf16leFile file(tmp.Path());
    EXPECT_EQ(file.Content(), u"hello\nworld\n");
}
