#include "spdlog/spdlog.h"
#include <armadillo>
#include <sstream>

using namespace arma;

int main(int argc, char *argv[]) {

  mat A(4, 5, fill::randu);
  mat B(4, 5, fill::randu);

  auto C = A * B.t();

  std::stringstream ss;
  ss << C;

  spdlog::info("\n{}", ss.str());

  return 0;
}
