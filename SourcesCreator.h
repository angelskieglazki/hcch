/**
  * Created by max on 12.03.19.
  */

#pragma once

#include <memory>
#include <iostream>
#include <fstream>

enum class source_type {
  c,
  cpp
};

class SourcesCreator {
 public:
  class Builder;
  ~SourcesCreator() = default;

  void create_source() const {
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
  SourcesCreator(const std::string& n, source_type t) : name(n), type(t) {}

  void create_files(const std::string& p) const {
    std::ofstream source_file (name + p);
    source_file  << "SOURCE" << std::endl;
    source_file.close();

    std::ofstream header_file (name + ".h");
    header_file  << "HEADER" << std::endl;
    header_file.close();

    std::cout<<name<< " created!" << std::endl;
  }
 private:
  std::string name;
  source_type type;
};

class SourcesCreator::Builder {
 private:
  std::string name;
  source_type type;

  static constexpr source_type default_type = source_type::c;

 public:
  Builder() : name("DefaultName"), type(default_type) {}

  Builder& setName(const std::string& n) { this->name = n; return *this; }
  Builder& setType(const source_type t) { this->type = t; return *this; }

  SourcesCreator build() {
    return SourcesCreator(name, type);
  }
};