/**
* Created by max on 12.03.19.
*/

#pragma once

#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

extern "C" {
#include "helpers.h"
}

#define SOURCE "/**\n* \\file %s\n* \\author %s\n*\n*\n* Created on %s*/\n\n#ifndef %s_H\n#define %s_H\n\n#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n\n\n\n#ifdef __cplusplus\n}\n#endif\n\n#endif"

#define END_LINE "\n"
#define END_LINE_STAR "\n* "
#define FILE_S "\\file "
#define AUTHOR_S "\\author "
#define DATE_S "Created on "




class MessageBuilder
{

  static void addToStream(std::ostringstream&)
  {
  }

  template<typename T, typename... Args>
  static void addToStream(std::ostringstream& a_stream, T&& a_value, Args&&... a_args)
  {
    // \TODO: change to stream.write
    a_stream << std::forward<T>(a_value);
    addToStream(a_stream, std::forward<Args>(a_args)...);
  }
 public:
  template<typename... Args>
  static inline std::string createMessage(Args&&... a_args)
  {
    std::ostringstream s;
    addToStream(s, std::forward<Args>(a_args)...);
    return s.str();
  }

//  template <typename... Args>
//  static std::string createMessage(Args&& ...args) {
//    std::ostringstream s;
//    (s << ... << args);
//    return s.str();
//  }
};


enum class source_type {
  c,
  cpp
};

class SourcesCreator {
 public:
  class Builder;
  ~SourcesCreator() = default;

  std::string str_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::toupper(c); }
    );
    return s;
  }

  void create_source() {
    std::string postfix = ".";

    switch (type){
      case source_type::c :{
        postfix = ".c";
      } break;
      case source_type::cpp :{
        postfix = ".cpp";
      } break;
      default: {
      }break;
    }
    create_files(postfix);
  }

 private:
  SourcesCreator(const std::string& n, source_type t, std::string& l)
    : name(n), type(t), license(l) {}

  void create_files(const std::string& p) {
    std::ofstream source_file (name + p);
    auto source_str = MessageBuilder::createMessage("/**",
                                                    END_LINE_STAR, FILE_S, name + p,
                                                    END_LINE_STAR, AUTHOR_S, get_user_name(),
                                                    END_LINE_STAR, DATE_S, get_time(),
                                                    "*/", END_LINE,END_LINE, "#include \"",name,".h\"");
    source_file  << source_str << std::endl;
    source_file.close();

    std::ofstream header_file (name + ".h");
    auto header_str = MessageBuilder::createMessage("/**",
                                                    END_LINE_STAR, FILE_S, name + p,
                                                    END_LINE_STAR, AUTHOR_S, get_user_name(),
                                                    END_LINE_STAR, DATE_S, get_time(),
                                                    END_LINE_STAR, license, 
                                                    END_LINE_STAR, END_LINE,
                                                    "*/", END_LINE,
                                                    END_LINE, "#ifndef ", str_toupper(name), "_H",
                                                    END_LINE, "#define ", str_toupper(name), "_H",
                                                    END_LINE, END_LINE, "#endif");
    header_file  << header_str << std::endl;
    header_file.close();

    std::cout<<name<< " created!" << std::endl;
  }
 private:
  std::string name;
  source_type type;
  std::string license;
};

class SourcesCreator::Builder {
  private:
    enum LicenseType {
      kEmpty_License,
      kGPL_License,
      kBSD_License,
      kApache_Licene,
      kLicenseNum
    };

  
    std::string name;
    source_type type;
    std::string license;

    static constexpr source_type default_type = source_type::c;
    static const std::array<std::string, kLicenseNum> license_arr;
   public:
    Builder() : name("DefaultName"), type(default_type), license(license_arr[0]) {}

    Builder& setName(const std::string& n) { this->name = n; return *this; }
    Builder& setType(const source_type t) { this->type = t; return *this; }
    Builder& setLicense(int index) { this->license = license_arr[index]; return *this; }
    SourcesCreator build() {
      return SourcesCreator(name, type, license);
    }
};
