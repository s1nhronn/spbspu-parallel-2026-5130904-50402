#include <cctype>
#include <cerrno>
#include <future>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>

namespace shirokov
{
  struct Circle
  {
    size_t r;
    int x, y;
  };

  size_t stringToSizeT(const char* line);
  bool isInside(double x, double y, double r);
  size_t calc(double r, size_t tests, size_t seed);
  double area(double r, size_t threads, size_t tests);
}

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    std::cerr << "Not enough arguments" << '\n';
    return 1;
  }

  size_t threads = 0, tries = 0, seed = 0;
  try
  {
    threads = shirokov::stringToSizeT(argv[1]);
    tries = shirokov::stringToSizeT(argv[2]);
    if (!tries)
    {
      std::cerr << "number of tries is not positive" << '\n';
      return 1;
    }
    if (argc == 4)
    {
      seed = shirokov::stringToSizeT(argv[3]);
    }
  }
  catch (const std::invalid_argument& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  // TODO: убрать заглушки
  (void)threads;
  (void)seed;

  std::vector< shirokov::Circle > circles;
  while (std::cin)
  {
    shirokov::Circle c{};
    int _;
    long long r = 0;
    std::cin >> r >> _ >> c.x >> c.y;
    if (std::cin.fail() && circles.empty())
    {
      std::cerr << "Input error" << '\n';
      return 2;
    }
    if (r < 0)
    {
      std::cerr << "Negative radius" << '\n';
      return 2;
    }
    c.r = static_cast< size_t >(r);
    circles.push_back(std::move(c));
  }

  return 0;
}

size_t shirokov::stringToSizeT(const char* str)
{
  if (!str)
  {
    throw std::invalid_argument("null pointer was passed");
  }

  while (std::isspace(static_cast< unsigned char >(*str)))
  {
    ++str;
  }

  if (*str == '-')
  {
    throw std::invalid_argument("negative number");
  }

  char* end = nullptr;
  errno = 0;
  unsigned long long res = std::strtoull(str, &end, 10);
  if (end == str)
  {
    throw std::invalid_argument("not number");
  }
  if (errno == ERANGE || res > std::numeric_limits< size_t >::max())
  {
    throw std::invalid_argument("overflow");
  }

  return static_cast< size_t >(res);
}

bool shirokov::isInside(double x, double y, double r)
{
  double dx = r - x, dy = r - y;
  return dx * dx + dy * dy <= r * r;
}

size_t shirokov::calc(double r, size_t tests, size_t seed)
{
  std::default_random_engine engine(seed);

  double minVal = 0, maxVal = 2 * r;
  std::uniform_real_distribution< double > dist(minVal, maxVal);

  size_t res = 0;
  for (size_t i = 0; i < tests; ++i)
  {
    double x = dist(engine);
    double y = dist(engine);
    if (isInside(x, y, r))
    {
      ++res;
    }
  }
  return res;
}

double shirokov::area(double r, size_t threads, size_t tests)
{
  std::vector< std::future< size_t > > results;
  results.reserve(threads);

  for (size_t i = 0; i < threads; ++i)
  {
    results.push_back(std::async(std::launch::async, calc, r, tests, i));
  }

  size_t count = 0;
  for (size_t i = 0; i < threads; ++i)
  {
    count += results[i].get();
  }

  return 4 * r * r * static_cast< double >(count) / static_cast< double >(threads * tests);
}

