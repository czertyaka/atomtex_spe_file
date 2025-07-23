#include <cstdint>
#include <cstddef>
#include <string_view>
#include <exception>
#include <print>

#include "atomtex_spe_file/spe_file.hpp"

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data,
    std::size_t size)
{
    if (size % 2)
    {
        return -1;
    }

    std::u16string_view input{reinterpret_cast<const char16_t*>(data),
        size / 2};

    try
    {
        atomtex_spe_file::SpeFile file{input, "<fuzzing>"};
        file.Read();
    }
    catch (const std::exception& err)
    {
        std::print("Error: {}\n", err.what());
    }

    return 0;
}
