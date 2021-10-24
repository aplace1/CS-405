/* File: NumericOverflows.cpp - Template! - Finished
 Course: CS-405 Secure Coding

 NOTE: Type parameter documentation: 'A type that with basic math functions'
 Left as is to ensure "industry standard code annotations"

 Change the subtract_numbers method to:
  1) Detect when an underflow will happen
  2) Prevent it from happening
  3) Return the correct value when no underflow happened or
  4) Return something to tell underflow the subtraction failed

 Change the underflow method to:
  1) Detect when an addValues failed
  2) Inform the user the underflow happened
  3) A successful result displays the same result as before you changed the method

 NOTE_A: subtractValues method must remain a template in the NumericFunctions header.

 NOTE_B: underflow method must remain a template in the NumericOverflows source file

 NOTE_C: The solution must work for all the data types used in main().

*/

#include <iostream>
#include <limits>

/**
 * @brief  Template function to simplify: A + (B * C)
 *
 * @param  A         First Value
 * @param  B         Second Value
 * @param  C         iterator
 *
 * @tparam T         "A type that with basic math functions"
 *
 * @return           result (A + (B * C))
 */
template <typename T>
T addValues(T const &A, T const &B, int const &C)
{
  T result = A;
  for (int i = 0; i < C; i++)
    result += B;
  return (result);
}

/**
 * @brief Template function to simplify: (A - (B * C))
 *
 * @param  A         First Value
 * @param  B         Second Value
 * @param  C         iterator
 *
 * @tparam  T        "Type that with basic math functions"
 *
 * @return  result (A - (B * C))
 */
template <typename T>
T subtractValues(T const &A, T const &B, size_t const &C)
{
  T result = A;
  for (size_t i = 0; i < C; ++i)
    result -= B;
  return (result);
}

/**
 * @brief Test an overflow for a template.
 *
 * @tparam T  "A type that with basic math functions"
 */
template <typename T>
void tOverflow()
{
  size_t C = 5;
  const T B = std::numeric_limits<T>::max() / C;
  const T A = 0;

  std::cout << " T = " << typeid(T).name();
  std::cout << "\n  Add ("
            << "(" << +B << " * " << +(C) << ")) = ";
  T result = addValues<T>(A, B, C++); // post increment
  std::cout << (+result) << std::endl;
  std::cout << "  Try "
            << "(" << +B << " * " << +(C) << ") = ";
  T tmp = addValues<T>(A, B, C);

  if (tmp > std::numeric_limits<T>::max())
  {
    std::cout << (+tmp) << std::endl;
    return;
  }
  tmp = addValues<T>(A, B, --C); // predecrement back to 5
  std::cout << " [OVERFLOW] " << +(tmp) << std::endl;

} // end of tOverflow

/**
 * @brief Template used for underflow checking
 *
 * @tparam T A type that with basic math functions
 */
template <typename T>
void tUnderflow()
{
  static size_t C = 5;
  const T B = std::numeric_limits<T>::max() / C;
  const T A = std::numeric_limits<T>::max();

  std::cout << " T = " << typeid(T).name();
  std::cout << "\n  Subtract (" << (+A) << " - (" << +B << " * " << C << ")) = ";
  T result = subtractValues<T>(A, B, C++); // post increment Cator

  std::cout << +result;
  std::cout << "\n  Try (" << (+A) << " - (" << +B << " * " << C << ")) = ";
  T tmp = (subtractValues<T>(A, B, C));

  if (tmp <= std::numeric_limits<T>::min())
  {
    std::cout << +tmp << std::endl;
    return;
  }
  tmp = subtractValues<T>(A, B, --C);
  std::cout << " [UNDERFLOW] " << +(tmp) << std::endl;
}

/**
 * @brief Performs overflow tests.
 */
void overflowTests()
{
  std::cout << "Running Overflow Tests" << std::endl;

  // signed integers
  tOverflow<char>();
  tOverflow<wchar_t>();
  tOverflow<short int>();
  tOverflow<int>();
  tOverflow<long>();
  tOverflow<long long>();

  // unsigned integers
  tOverflow<unsigned char>();
  tOverflow<unsigned short int>();
  tOverflow<unsigned int>();
  tOverflow<unsigned long>();
  tOverflow<unsigned long long>();

  // real numbers
  tOverflow<float>();
  tOverflow<double>();
  tOverflow<long double>();

  std::cout << std::endl;
} // end of overflowTests()

void underFlowTests()
{
  std::cout << "Running Underflow Tests" << std::endl;

  // signed integers
  tUnderflow<char>();
  tUnderflow<wchar_t>();
  tUnderflow<short int>();
  tUnderflow<int>();
  tUnderflow<long>();
  tUnderflow<long long>();

  // unsigned integers
  tUnderflow<unsigned char>();
  tUnderflow<unsigned short int>();
  tUnderflow<unsigned int>();
  tUnderflow<unsigned long>();
  tUnderflow<unsigned long long>();

  // real numbers
  tUnderflow<float>();
  tUnderflow<double>();
  tUnderflow<long double>();
}

/**
 * @brief Entry Point for the application
 *
 * @return integer as the return value
 */
int main()
{
  clock_t timer = std::clock(); // I added a clock to make this program feel useful.

  // Run overflow and underflow tests.
  overflowTests();
  std::cout << ((clock() - timer)) << " processor ticks\n";
  underFlowTests();
  std::cout << ((clock() - timer)) << " processor ticks\n";

  // Results
  std::cout << "\n -- Tests Complete!\n";
  std::cout << ((clock() - timer)) << " total processor ticks\n";
  std::cout << std::endl;
  return (0);
}
