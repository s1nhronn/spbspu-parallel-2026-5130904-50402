#include "commands.hpp"
#include <cmath>
#include <istream>
#include <limits>
#include <stdexcept>
#include <ellipse.hpp>

namespace
{
  void outFigure(std::ostream& out, const shirokov::Ellipse& e)
  {
    out << e.hr << ' ';

    if (std::fabs(e.hr - e.vr) > std::numeric_limits< double >::epsilon())
    {
      out << e.vr << ' ';
    }

    out << '(' << e.x << ' ' << e.y << ')' << '\n';
  }
}

void shirokov::circle(std::istream& in, std::ostream&, ellipse_map& ellipses, set_map&)
{
  std::string name;
  double r, x, y;
  in >> name >> r >> x >> y;

  if (ellipses.find(name) != ellipses.end())
  {
    throw std::invalid_argument("circle with this name already exist");
  }
  if (r <= 0)
  {
    throw std::invalid_argument("radius must be positive");
  }

  ellipses.insert({name, {x, y, r, r}});
}

void shirokov::ellipse(std::istream& in, std::ostream&, ellipse_map& ellipses, set_map&)
{
  std::string name;
  double hr, vr, x, y;
  in >> name >> hr >> vr >> x >> y;

  if (ellipses.find(name) != ellipses.end())
  {
    throw std::invalid_argument("circle with this name already exist");
  }
  if (hr <= 0 || vr <= 0)
  {
    throw std::invalid_argument("radiuses must be positive");
  }

  ellipses.insert({name, {x, y, hr, vr}});
}

void shirokov::set(std::istream& in, std::ostream&, ellipse_map& ellipses, set_map& sets)
{
  std::string set_name;
  size_t set_size = 0;
  in >> set_name >> set_size;

  if (sets.find(set_name) != sets.end())
  {
    throw std::invalid_argument("set with this name already exist");
  }
  if (!set_size)
  {
    throw std::invalid_argument("incorrect size of set");
  }

  std::string figure_name;
  std::vector< Ellipse > s;
  s.reserve(set_size);
  for (size_t i = 0; i < set_size; ++i)
  {
    in >> figure_name;
    if (ellipses.find(figure_name) == ellipses.end())
    {
      throw std::invalid_argument("figure with this name does't exist: " + figure_name);
    }
    s.push_back(ellipses[figure_name]);
  }

  sets.insert({set_name, std::move(s)});
}

void shirokov::show(std::istream& in, std::ostream& out, ellipse_map& ellipses, set_map&)
{
  std::string name;
  in >> name;

  if (ellipses.find(name) == ellipses.end())
  {
    throw std::invalid_argument("figure with this name does't exist");
  }

  const Ellipse& e = ellipses[name];
  outFigure(out, e);
}

void shirokov::showset(std::istream& in, std::ostream& out, ellipse_map&, set_map& sets)
{
  std::string name;
  in >> name;

  if (sets.find(name) == sets.end())
  {
    throw std::invalid_argument("set with this name does't exist");
  }

  const std::vector< Ellipse >& s = sets[name];
  for (const Ellipse& e : s)
  {
    outFigure(out, e);
  }
}
