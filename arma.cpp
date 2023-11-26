#include "spdlog/spdlog.h"
#include <armadillo>
#include <sstream>

#include "outcome/outcome.hpp"
namespace outcome = OUTCOME_V2_NAMESPACE;

using namespace arma;

// Function that attempts to divide two numbers and returns an outcome
outcome::outcome<double> safe_divide(double numerator, double denominator) {
  if (denominator == 0) {
    // Return an error code for division by zero
    return outcome::failure(std::make_error_code(std::errc::invalid_argument));
  }
  // Assuming no other errors, return the division result
  return numerator / denominator;
}

int main(int argc, char *argv[]) {

  mat A(4, 5, fill::randu);
  mat B(4, 5, fill::randu);

  auto C = A * B.t();

  std::stringstream ss;
  ss << C;

  spdlog::info("\n{}", ss.str());

  auto result = safe_divide(10, 2); // This should return an error

  if (!result) {
    // Handle error
    std::cerr << "Error: " << result.error().message() << "\n";
  } else {
    try {
      // Try to get the value (this might throw)
      std::cout << "Result: " << result.value() << "\n";
    } catch (const std::exception &ex) {
      // Handle the exception
      std::cerr << "Exception: " << ex.what() << "\n";
    }
  }

  return 0;
}
