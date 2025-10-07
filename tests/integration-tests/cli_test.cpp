#include <gtest/gtest.h>

#include <cstdlib>
#include <array>

#include "run.hpp"

using atomtex_spe_file::cli::run;

template<typename... Args>
constexpr auto make_args(Args&&... args) -> std::array<const char*, sizeof...(Args) + 1> {
    return {"test", args...};
}

TEST(CliTest, UnknownOption) {
    auto args = make_args("--unknown", "bad");
    EXPECT_EQ(run(args.size(), args.data()), EXIT_FAILURE);
}

TEST(CliTest, HelpMessage) {
    auto args = make_args("-h");
    EXPECT_EQ(run(args.size(), args.data()), EXIT_SUCCESS);
    args = make_args("--help");
    EXPECT_EQ(run(args.size(), args.data()), EXIT_SUCCESS);
}

TEST(CliTest, Version) {
    auto args = make_args("-v");
    EXPECT_EQ(run(args.size(), args.data()), EXIT_SUCCESS);
    args = make_args("--version");
    EXPECT_EQ(run(args.size(), args.data()), EXIT_SUCCESS);
}

TEST(CliTest, NoInput) {
    auto args = make_args("-f", "csv");
    EXPECT_EQ(run(args.size(), args.data()), EXIT_FAILURE);
    args = make_args("-o", "/tmp/file");
    EXPECT_EQ(run(args.size(), args.data()), EXIT_FAILURE);
}

TEST(CliTest, SingleInput) {
    constexpr auto filename {"example.spe"};
    {
        auto args = make_args(filename);
        EXPECT_EQ(run(args.size(), args.data()), EXIT_SUCCESS);
    }
    {
        auto args = make_args("-i", filename);
        EXPECT_EQ(run(args.size(), args.data()), EXIT_SUCCESS);
    }
    {
        auto args = make_args("-f", "csv", "-o", std::tmpnam(nullptr), "-i", filename);
        EXPECT_EQ(run(args.size(), args.data()), EXIT_SUCCESS);
    }
}

TEST(CliTest, Formats) {
    constexpr auto filename {"example.spe"};
    auto args = make_args("-f", "csv", filename);
    EXPECT_EQ(run(args.size(), args.data()), EXIT_SUCCESS);
    args = make_args("-f", "text", filename);
    EXPECT_EQ(run(args.size(), args.data()), EXIT_SUCCESS);
    args = make_args("-f", "json", filename);
    EXPECT_EQ(run(args.size(), args.data()), EXIT_SUCCESS);
    args = make_args("-f", "bad", filename);
    EXPECT_EQ(run(args.size(), args.data()), EXIT_FAILURE);
}
