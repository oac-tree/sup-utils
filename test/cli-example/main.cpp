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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

//! @file
//! Example of sup-cli (command line options) usage.

#include <sup/cli/command_line_parser.h>

#include <iostream>

int main(int argc, char* argv[])
{
  sup::cli::CommandLineParser parser;

  const std::string header;

  parser.SetDescription(
      header,
      "The program loads <filename>, parses it into an executable behaviour tree and "
      "executes it.");

  parser.AddHelpOption();

  parser.AddOption({"-f", "--file"}, "Load, parse and execute <filename>")
      .SetParameter(true)
      .SetValueName("filename")
      .SetRequired(true);

  parser.AddOption({"--font"}, "Application font size")
      .SetParameter(true)
      .SetValueName("font size")
      .SetDefaultValue("12");

  parser.AddOption({"-v", "--verbose"}, "Log to standard output (default: WARNING)")
      .SetParameter(true)
      .SetValueName("log_level")
      .SetDefaultValue("WARNING");

  parser.AddOption({"-V", "--validate"}, "Only parse and setup the procedure without running it");

  parser.AddPositionalOption("<origin>", "Positional option #0");
  parser.AddPositionalOption("<target>", "Positional option #1");

  if (!parser.Parse(argc, argv))
  {
    std::cout << parser.GetUsageString();
    return 0;
  }

  // printing some output
  std::cout << "Command line options were"
            << "\n";

  std::cout << "filename  : " << parser.GetValue<std::string>("--file") << "\n";
  std::cout << "font size : " << parser.GetValue<int>("--font") << "\n";
  std::cout << "severity  : " << parser.GetValue<std::string>("--verbose") << "\n";
  std::cout << "validate  : " << parser.IsSet("--validate") << "\n";
  std::cout << "Number of positional options found:" << parser.GetPositionalOptionCount() << "\n";
  for (size_t i = 0; i < parser.GetPositionalOptionCount(); ++i)
  {
    std::cout << "positional option #" << i << " " << parser.GetPositionalValue<std::string>(i)
              << "\n";
  }
}
