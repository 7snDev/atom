#include "atom/atom.hpp"

std::string helloWorld(std::string key)
{
  return R"({"title":"Hello World From C++"})";
}
std::string log(std::string key)
{
  key = key.substr(2);
  key = key.substr(0, key.length() - 2);
  printf("LOG: %s\n", key.c_str());
  return "";
}

int main()
{
  App myApp(true);
  myApp.setSources("./ui/index.html", "./ui/style.css", "./ui/script.js");
  myApp.setSize(800, 600, false);
  myApp.setTitle("Atomic App");;
  myApp.setBind("cppbinded", helloWorld);
  myApp.setBind("cpplog", log);
  myApp.run();
  return 0;
}