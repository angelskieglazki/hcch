/**
  * Created by max on 12.03.19.
  */
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <iostream>
using namespace std;

#include "SourcesCreator.h"

//#include <boost/program_options.hpp>
#include <boost/program_options.hpp>
using namespace boost::program_options;


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
        ("name,n", value<string>(), "source name")
        ("c_sources", bool_switch(), "*.h/*.c pair for c")
        ("cpp_sources", bool_switch(), "*.h/*.cpp pair for c++")
        ("version,V", "output the version number")
        ;

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
      cout << desc << "\n";
      return 0;
    }

    conflicting_options(vm, "c_sources", "cpp_sources");
    option_dependency(vm, "c_sources", "name");
    option_dependency(vm, "cpp_sources", "name");

    if (!vm["cpp_sources"].as<bool>() && !vm["c_sources"].as<bool>()) {
      throw logic_error(string("Source type missed!"));
    }

    SourcesCreator::Builder b;
    if (vm["cpp_sources"].as<bool>()) {
      b.setName(vm["name"].as<string>()).setType(source_type::cpp).build();
    } else if (vm["c_sources"].as<bool>()) {
      b.setName(vm["name"].as<string>()).setType(source_type::c).build();
    }

    auto sc = b.build();
    sc.create_source();
  }
  catch(exception& e) {
    cerr << e.what() << "\n";
  }
}