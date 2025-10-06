#include <cstdlib>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>


class CommandLineArgs {
    using VariablesMap = boost::program_options::variables_map;
    using OptionsDescription = boost::program_options::options_description;
public:
    CommandLineArgs(const int argc, const char* argv[]);
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
        ("format,f", value<std::string>()->value_name("FORMAT")->default_value("text"), "choose output format (text, json)")
        ("output,o", value<std::string>()->value_name("PATH"), "path to output file")
        ("input,i", value<std::string>()->value_name("PATH"), "input file or directory")
    ;

    positional_options_description pdesc {};
    pdesc.add("input,i", 1);

    store(command_line_parser(argc, argv).options(desc_).positional(pdesc).run(), vm_);
    notify(vm_);
}

const CommandLineArgs::VariablesMap& CommandLineArgs::GetVariablesMap() const {
    return vm_;
}

const CommandLineArgs::OptionsDescription& CommandLineArgs::GetOptionsDescription() const {
    return desc_;
}

int main(const int argc, const char* argv[]) {
    const CommandLineArgs args {argc, argv};
    const auto& vm = args.GetVariablesMap();
    if (vm.contains("help")) {
        std::cout << args.GetOptionsDescription() << std::endl;
        return EXIT_SUCCESS;
    }
    else if (vm.contains("version")) {
        std::cout << "atomtex_spe_file version "
            << ATOMTEX_SPE_FILE_VERSION << std::endl;
    }
}
