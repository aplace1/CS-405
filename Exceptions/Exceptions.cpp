/* File: Exceptions.cpp

Purpose: Create exception handlers that catch exceptions...

do_even_more_custom_application_logic:
 1) Throw any standard exception

do_custom_application_logic:
 1) Wrap the call to do_even_more_custom_application_logic()
    with an exception handler that catches std::exception,
    displays a message and the exception.what(), then continues processing.
 2) Throw a custom exception derived from std::exception and catch it explicitly in main divide:
 3) Throw (divide by zero) exceptions using a standard C++ defined exception

do_division:
 1) create an exception handler to capture ONLY the exception thrown by divide.

main:
 1) your custom exception
 2) std::exception
 3) uncaught exception that wraps the whole main function, and displays a message to the console.
*/

#include <iostream>

bool do_even_more_custom_application_logic()
{
  std::cout << "Running Even More Custom Application Logic." << std::endl;
  std::exception exception("Exception caught in main...");
  throw exception;
  return false; // never returned
}
void do_custom_application_logic()
{
  std::cout << "Running Custom Application Logic." << std::endl;

  try
  {
    if (do_even_more_custom_application_logic())
    {
    }
  }
  catch (int)
  {
  }
  std::cout << "Even More Custom Application Logic Succeeded." << std::endl;
  std::cout << "Leaving Custom Application Logic." << std::endl;
}

float divide(const float num, const float den)
{
  try
  {
    return (num / den);
  }
  catch (std::exception &e)
  {
    throw e;
  }
}

void do_division() noexcept
{
  constexpr float numerator = 10.00f;
  constexpr float denominator = 0;
  const auto result = divide(numerator, denominator);
  std::cout << "divide(" << numerator << ", " << denominator << ") = " << result << std::endl;
}

int main()
{
  std::cout << "Exceptions Tests!" << std::endl;

  // encapsulation of the entire main function within a try catch statement...
  try
  {
    do_division();
    do_custom_application_logic();
  }
  catch (std::exception &exception)
  {
    std::cerr << exception.what() << std::endl;
  }

  return 0;
}
