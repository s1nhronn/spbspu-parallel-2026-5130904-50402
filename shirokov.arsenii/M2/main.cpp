#include <ios>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "commands.hpp"

int main()
{
  shirokov::ellipse_map circles;
  shirokov::set_map sets;

  using cmd_t = void (*)(std::istream&, std::ostream&, shirokov::ellipse_map&, shirokov::set_map&);
  std::unordered_map< std::string, cmd_t > cmds;

  cmds["circle"] = shirokov::circle;
  cmds["ellipse"] = shirokov::ellipse;
  cmds["set"] = shirokov::set;
  cmds["show"] = shirokov::show;
  cmds["showset"] = shirokov::showset;
  // cmds["frame"] = shirokov::frame;
  // cmds["frameset"] = shirokov::frameset;
  // cmds["spawn"] = shirokov::spawn;
  // cmds["area-on"] = shirokov::area_on;
  // cmds["status"] = shirokov::status;
  // cmds["wait"] = shirokov::wait;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, circles, sets);
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<UNKNOWN COMMAND>\n";
      auto to_ignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(to_ignore, '\n');
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto to_ignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(to_ignore, '\n');
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
