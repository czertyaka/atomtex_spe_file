#include <benchmark/benchmark.h>

#include "atomtex_spe_file/utf16le_file.hpp"
#include "atomtex_spe_file/spe_file.hpp"

static void BM_ReadSpeFile(benchmark::State& state)
{
    using namespace atomtex_spe_file;

    const auto path{"example.spe"};

    for (auto _ : state)
    {
        const Utf16leFile file{path};
        SpeFile spe{file.Content(), path};
        auto data = spe.Read();
        benchmark::DoNotOptimize(data);
    }
}

BENCHMARK(BM_ReadSpeFile);
BENCHMARK_MAIN();
