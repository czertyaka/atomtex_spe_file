#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

#include <boost/tokenizer.hpp>

#include "run.hpp"

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data,
    std::size_t size)
{
    std::vector<const char*> argv {};
    int argc {1};

    std::string_view input{reinterpret_cast<const char*>(data), size};
    while (const std::size_t pos = input.find('\0') != std::string_view::npos) {
        std::string_view word {input.substr(0, pos)};
        argv.push_back(word.data());
        if (pos + 1 >= input.size()) {
            break;
        }
        input = input.substr(pos + 1) ;
        argc++;
    }
    argv.push_back(nullptr);

    atomtex_spe_file::cli::run(argc, argv.data());
    return 0;
}
