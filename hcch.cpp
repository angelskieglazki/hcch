/**
  * Created by max on 12.03.19.
  */
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <iostream>

#include "version.h"
#include "SourcesCreator.h"

#include <boost/program_options.hpp>
using namespace boost::program_options;
using namespace std;

void print_version()
{
  std::cout<<"Utility for creating c/c++ header-source pair v"
    <<PROJECT_VERSION_MAJOR<<"."
    <<PROJECT_VERSION_MINOR<<"."
    <<PROJECT_VERSION_PATCH
    <<std::endl;
}

void conflicting_options(const variables_map& vm,
                         const char* opt1, const char* opt2)
{
  if (vm.count(opt1) && !vm[opt1].defaulted()
      && vm.count(opt2) && !vm[opt2].defaulted())
    throw logic_error(string("Conflicting options '")
                          + opt1 + "' and '" + opt2 + "'.");
}

// \TODO: Need upgrade this function for array of required options
void option_dependency(const variables_map& vm,
                       const char* for_what, const char* required_option)
{
  if (vm.count(for_what) && !vm[for_what].defaulted())
    if (vm.count(required_option) == 0 || vm[required_option].defaulted())
      throw logic_error(string("Option '") + for_what
                            + "' requires option '" + required_option + "'.");
}

int main(int argc, char* argv[])
{

  try {

    options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "print usage message")
        ("interactive,i", "create sources in interactive mode")
        ("name,n", value<string>(), "source name")
        ("c_sources", bool_switch(), "*.h/*.c pair for c")
        ("cpp_sources", bool_switch(), "*.h/*.cpp pair for c++")
        ("version,V", "output the version number")
        ;

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("interactive")) {
      SourceBuildData data;

      std::string input;
      cout << "Choose source name(DefaultName by default) : ";
      getline(cin,input);
      if (input.length() > 0) {
        data.name = input;
      }

      cout << "Choose source type(\"C\" - 0, \"C++\" - 1, C by default) : ";
      int type = 0;
      getline(cin,input);
      if (input.length() > 0) {
        stringstream(input)>>type;
      }
      data.type = static_cast<source_type>(type);

      cout << "Choose license (\"GPL\" - 1, \"BSD\" - 2, \"Apache\" - 3,  Empty by default) : ";
      getline(cin,input);
      if (input.length() > 0) {
        stringstream(input)>>data.license_idx;
      }

      SourcesCreator::Builder b;
      b.setAllData(data);
      auto sc = b.build();
      sc.create_source();

      return 0;
    }

    if (vm.count("version")) {
      print_version();
      return 0;
    }

    if (vm.count("help")) {
      print_version();
      cout << desc << "\n";
      return 0;
    }

    conflicting_options(vm, "c_sources", "cpp_sources");
    conflicting_options(vm, "version", "c_sources");
    conflicting_options(vm, "version", "cpp_sources");
    option_dependency(vm, "c_sources", "name");
    option_dependency(vm, "cpp_sources", "name");

    if (!vm["cpp_sources"].as<bool>() && !vm["c_sources"].as<bool>()) {
      cout << "Source type missed!" << "\n";
      cout << desc << "\n";
      return 0;
    }

    SourcesCreator::Builder b;
    if (vm["cpp_sources"].as<bool>()) {
      b.setName(vm["name"].as<string>()).setType(source_type::cpp);
    } else if (vm["c_sources"].as<bool>()) {
      b.setName(vm["name"].as<string>()).setType(source_type::c);
    }

    auto sc = b.build();
    sc.create_source();
  }
  catch(exception& e) {
    cerr << e.what() << "\n";
  }
}
