#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <filesystem>
#include <ostream>
#include <string>
#include <utility>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

#include <atomtex_spe_file/measurement.hpp>
#include <atomtex_spe_file/utf16le_file.hpp>
#include <atomtex_spe_file/spe_file.hpp>

namespace asf = atomtex_spe_file;

using Measurements = std::map<std::filesystem::path, asf::Measurement>;

class CommandLineArgs {
    using VariablesMap = boost::program_options::variables_map;
    using OptionsDescription = boost::program_options::options_description;
public:
    CommandLineArgs(const int argc, const char* argv[]);
    void Notify();
    const VariablesMap& GetVariablesMap() const;
    const OptionsDescription& GetOptionsDescription() const;
private:
    static const char* descString_;
    VariablesMap vm_;
    OptionsDescription desc_;
};

// static
const char* CommandLineArgs::descString_ =
    "Reads and parses ATOMTEX *.spe files.\n"
    "Extracts coordinates (longitude and latitude, decimal degrees) and dose rate (μSv/hr).\n\n"
    "Allowed options"
;

CommandLineArgs::CommandLineArgs(const int argc, const char* argv[]) :
    vm_(),
    desc_(descString_)
{
    using namespace boost::program_options;

    desc_.add_options()
        ("help,h", "produce help message")
        ("version,v", "print version string")
        ("format,f", value<std::string>()->value_name("FORMAT")->default_value("text"), "choose output format (text, json, csv)")
        ("output,o", value<std::string>()->value_name("PATH"), "path to output file")
        ("input,i", value<std::string>()->value_name("PATH")->required(), "input file or directory")
    ;

    positional_options_description pdesc {};
    pdesc.add("input", 1);

    store(command_line_parser(argc, argv).options(desc_).positional(pdesc).run(), vm_);
}

void CommandLineArgs::Notify() {
    notify(vm_);
}

const CommandLineArgs::VariablesMap& CommandLineArgs::GetVariablesMap() const {
    return vm_;
}

const CommandLineArgs::OptionsDescription& CommandLineArgs::GetOptionsDescription() const {
    return desc_;
}

asf::Measurement read_measurement(const std::filesystem::path path) {
    const asf::Utf16leFile file{path};
    const asf::SpeFile spe{file.Content(), path.string()};
    return spe.Read();
}

void print_measurements(std::ostream& os, const Measurements& measurements) {
    for (const auto& [path, measurement]: measurements) {
        os << "File: " << path
            << ", longitude: " << measurement.point.lon.DecimalDegrees() << " (dd)"
            << ", latitude: " << measurement.point.lat.DecimalDegrees() << " (dd)"
            << ", dose rate: " << measurement.doseRate.MicroSvPerHour() << " (μSv/hr)";
    }
}

int main(const int argc, const char* argv[]) {
    CommandLineArgs args {argc, argv};
    const auto& vm = args.GetVariablesMap();
    if (vm.contains("help")) {
        std::cout << args.GetOptionsDescription() << std::endl;
        return EXIT_SUCCESS;
    }
    else if (vm.contains("version")) {
        std::cout << "atomtex_spe_file version "
            << ATOMTEX_SPE_FILE_VERSION << std::endl;
        return EXIT_SUCCESS;
    }

    try {
        args.Notify();
    }
    catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    Measurements measurements {};

    const std::filesystem::path input { vm["input"].as<std::string>() };
    if (std::filesystem::is_regular_file(input)) {
        try {
            measurements.insert(std::make_pair(input, read_measurement(input)));
        }
        catch (const std::exception& err) {
            std::cerr << "Error: input must be regular file or directory\n";
            return EXIT_FAILURE;
        }
    }
    else if (std::filesystem::is_directory(input)) {
        // @TODO: add recursive search and reading
    }
    else {
        std::cerr << "Error: input must be regular file or directory\n";
        return EXIT_FAILURE;
    }

    if (vm.contains("output")) {
        const std::filesystem::path output {vm["output"].as<std::string>()};
        std::ofstream ofs {output, std::ios::trunc};
        if (!ofs || !ofs.good()) {
            std::cerr << "Error: can't open file " << output << " for writing" << std::endl;
            return EXIT_FAILURE;
        }
        print_measurements(ofs, measurements);
    }
    else {
        print_measurements(std::cout, measurements);
    }
    return EXIT_SUCCESS;
}
