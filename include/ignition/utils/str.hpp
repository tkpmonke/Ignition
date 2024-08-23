#pragma once

#include <string>
#include <vector>
#include <sstream>
namespace STR {
   static std::vector<std::string> split(std::string* s, char c) {
      std::vector<std::string> o;
      std::string l;
      std::istringstream f(*s);
      while (std::getline(f, l, c))
         o.push_back(l);
      return o;
   }
   static std::string tail(std::string* s, char c) { return s->substr(s->find(c),s->size()); }
   static std::string front(std::string* s, char c) { return s->substr(0,s->find(c)); }
}
