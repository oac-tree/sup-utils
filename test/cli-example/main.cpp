/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : Command line argument parsing
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

//! @file
//! Example of sup-cli (command line options) usage.
//!
//! Things to try:
//!   cli-example --help
//!   cli-example --file config.xml input result.xml
//!   cli-example --file "/path/with space/config.xml" --font 24 -V input
//!   cli-example --file config.xml --comment "" input   (an empty value is still a value)
//!   cli-example --file config.xml --font 100 input     (value outside the allowed range)
//!   cli-example --file config.xml --font 24k input     (value that is not a number)
//!   cli-example --file config.xml                      (required positional missing)

#include <sup/cli/command_line_parser.h>

#include <cstddef>
#include <iostream>

namespace
{

//! Creates the parser with all options of this application registered.
//!
//! The parser is returned by value: it is movable, so a set of options shared by several
//! applications can be registered in one place instead of being passed a parser to fill in.
sup::cli::CommandLineParser CreateParser()
{
  sup::cli::CommandLineParser parser;

  parser.SetDescription("The program demonstrates the options offered by the sup-cli library.",
                        "Positional values are matched in the order the options were added.");

  // The help option is the one AddHelpOption creates; IsHelpRequested reports it below.
  parser.AddHelpOption();

  // A parameter option: a name followed by a value. Required, so Parse fails without it.
  parser.AddOption({"-f", "--file"}, "Load and parse <filename>")
      .SetParameter(true)
      .SetValueName("filename")
      .SetRequired(true);

  // A parameter option with a default value: it always has a value to report.
  parser.AddOption({"--font"}, "Application font size, from 6 to 72")
      .SetParameter(true)
      .SetValueName("font size")
      .SetDefaultValue("12");

  parser.AddOption({"-v", "--verbose"}, "Log to standard output")
      .SetParameter(true)
      .SetValueName("log_level")
      .SetDefaultValue("WARNING");

  // A parameter option without a default value: the application decides what its absence means.
  parser.AddOption({"--timeout"}, "Time to wait for the result, in seconds")
      .SetParameter(true)
      .SetValueName("seconds");

  // No default value: FindValue below reports whether it was given at all. A value given as an
  // empty string counts as given; SetDefaultValue("") would likewise be a default like any other.
  parser.AddOption({"--comment"}, "Text to store along with the result")
      .SetParameter(true)
      .SetValueName("text");

  // A flag: no value, just present or absent.
  parser.AddOption({"-V", "--validate"}, "Only parse the input without executing it");

  // Positional options never appear by name in the command line; they take the free standing
  // values in the order they were added. A required one needs a value to reach it.
  parser.AddPositionalOption("<origin>", "File to read the data from").SetRequired(true);
  parser.AddPositionalOption("<target>", "File to write the result to");

  return parser;
}

//! Prints what the command line carried.
//!
//! Every accessor used here throws on a value it cannot deliver, which main reports in one place.
//! The values are read first and printed afterwards, so a bad one is reported instead of appearing
//! halfway through the output.
void PrintOptions(const sup::cli::CommandLineParser& parser)
{
  // A flag is queried with IsSet. Asking about a name that was never added is a programming error
  // and throws, so a misspelled name doesn't quietly read as "not given".
  const auto validate = parser.IsSet("--validate");

  // The whole text of a value is converted, or the call throws: "24k" is not a number here.
  const auto filename = parser.GetValue<std::string>("--file");
  const auto font_size = parser.GetValueInRange<int>("--font", 6, 72);
  const auto severity = parser.GetValue<std::string>("--verbose");

  // An option with no default and no value given: the fallback stands in for the absent value.
  const auto timeout = parser.GetValue<double>("--timeout", 30.0);

  // FindValue tells "not given" from "given this value" without an exception, which matters for
  // an option whose value may legitimately be empty.
  const auto comment = parser.FindValue("--comment");

  std::cout << "Command line options were\n";
  std::cout << "validate  : " << std::boolalpha << validate << "\n";
  std::cout << "filename  : " << filename << "\n";
  std::cout << "font size : " << font_size << "\n";
  std::cout << "severity  : " << severity << "\n";
  std::cout << "timeout   : " << timeout << "\n";
  std::cout << "comment   : " << (comment.has_value() ? "'" + *comment + "'" : "<none>") << "\n";

  const auto count = parser.GetPositionalOptionCount();
  std::cout << "Number of positional values found: " << count << "\n";
  for (std::size_t i = 0; i < count; ++i)
  {
    std::cout << "positional value #" << i << " " << parser.GetPositionalValue<std::string>(i)
              << "\n";
  }
}

}  // namespace

int main(int argc, char* argv[])
{
  auto parser = CreateParser();

  const auto exit_code = sup::cli::ParseOrReport(parser, argc, argv);
  if (exit_code.has_value())
  {
    return *exit_code;
  }

  try
  {
    PrintOptions(parser);
  }
  catch (const sup::cli::CommandLineException& ex)
  {
    // Covers a value that doesn't convert, one outside the allowed range, and a value that isn't
    // there at all. All of them name the option and the text at fault.
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }

  return 0;
}
