#include <cstdint>
#include <cstddef>
#include <exception>
#include <print>

#include "lines.hpp"

constexpr std::size_t lines_count{5};

#pragma pack(push, 0)
struct Input
{
    char separator;
    std::size_t numbers[lines_count];
};
#pragma pack(pop)

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data,
    std::size_t size)
{
    if (size <= sizeof(Input))
    {
        return -1;
    }

    const auto separator = reinterpret_cast<const Input*>(data)->separator;

    std::array<std::size_t, lines_count> numbers{};
    for (std::size_t i{}; i < lines_count; ++i)
    {
        numbers[i] = reinterpret_cast<const Input*>(data)->numbers[i];
    }

    std::string text{reinterpret_cast<const char*>(data + sizeof(Input)),
        size - sizeof(Input)};

    // std::print("NEW FUZZING RUN\n");
    // std::print("separator = '{}'\n", separator);
    // std::print("text = '{}'\n", text);

    try
    {
        using namespace atomtex_spe_file;
        Lines<std::string::const_iterator> lines{text.cbegin(), text.cend(),
            separator};

        for (auto&& number : numbers)
        {
            const auto line = lines[number];
            (void)line;
            // std::print("line[{}] = '{}'\n", number, line);
        }
    }
    catch (const std::exception& err)
    {
        // std::print("Error: {}\n", err.what());
    }

    return 0;
}
