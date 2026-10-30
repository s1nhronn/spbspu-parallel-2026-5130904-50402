#include "commands.hpp"
#include <stdexcept>
#include "ellipse.hpp"

void shirokov::circle(std::istream& in, std::ostream&, ellipse_map& ellipses)
{
  std::string name;
  double r, x, y;
  in >> name >> r >> x >> y;
  if (ellipses.find(name) != ellipses.end())
  {
    throw std::invalid_argument("Circle with this name already exist");
  }
  if (r < 0)
  {
    throw std::invalid_argument("Radius must not be negative");
  }

  ellipses.insert({name, {x, y, r, r}});
}

void shirokov::show(std::istream& in, std::ostream& out, ellipse_map& ellipses)
{
  std::string name;
  in >> name;

  if (ellipses.find(name) == ellipses.end())
  {
    throw std::invalid_argument("Figure with this name does't exist");
  }
  const Ellipse& e = ellipses[name];
  out << e.hr << ' ';

  if (e.hr != e.vr)
  {
    out << e.vr << ' ';
  }

  out << '(' << e.x << ' ' << e.y << ')' << '\n';
}
