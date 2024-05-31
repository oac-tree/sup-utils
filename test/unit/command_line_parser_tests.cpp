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

#include <sup/cli/argh.h>
#include <sup/cli/command_line_parser.h>

#include <gtest/gtest.h>

#include <array>
#include <stdexcept>

using namespace sup::cli;

class CommandLineParserTests : public ::testing::Test
{
};

//! Clarifying Argh! parser behaviour.
TEST_F(CommandLineParserTests, ArghParser)
{
  {  // no pregistered parameters, empty command line
    argh::parser parser;

    const int argc = 1;
    std::array<const char *, argc> argv{"progname"};
    parser.parse(argc, &argv[0]);

    // it reports program name as positional argument
    EXPECT_EQ(parser.size(), 1);
    std::string pos_value;
    parser(0) >> pos_value;
    EXPECT_EQ(pos_value, std::string("progname"));
  }

  {  // parameter is pre-registered
    argh::parser parser;
    parser.add_param("--font");

    const int argc = 3;
    std::array<const char *, argc> argv{"progname", "--font", "24"};
    parser.parse(argc, &argv[0]);

    // make sure it's not treated as a flag ([] operator)
    EXPECT_FALSE(parser["--font"]);

    // it reports program name as positional argument
    EXPECT_EQ(parser.size(), 1);
  }

  {  // parameter is pre-registered, but the value is missed
    argh::parser parser;
    parser.add_param("--font");

    const int argc = 2;
    std::array<const char *, argc> argv{"progname", "--font"};
    parser.parse(argc, &argv[0]);

    // strangely, it appeared as a flag
    EXPECT_TRUE(parser["--font"]);

    // it reports program name as positional argument
    EXPECT_EQ(parser.size(), 1);
  }

  {  // no registration
    argh::parser parser;

    const int argc = 2;
    std::array<const char *, argc> argv{"progname", "--font"};
    parser.parse(argc, &argv[0]);

    // make sure it's treated as a flag ([] operator)
    EXPECT_TRUE(parser["--font"]);

    // it reports program name as positional argument
    EXPECT_EQ(parser.size(), 1);
  }

  {  // no registration, but we pass free standing parameter
    argh::parser parser;

    const int argc = 3;
    std::array<const char *, argc> argv{"progname", "--font", "24"};
    parser.parse(argc, &argv[0]);

    // it is still a flag, despite of a parameter nearby
    EXPECT_TRUE(parser["--font"]);

    // free standing parameter appears as positional argument
    EXPECT_EQ(parser.size(), 2);
    std::string pos_value;
    parser(1) >> pos_value;
    EXPECT_EQ(pos_value, std::string("24"));
  }
}

//! Testing the method to add automatic help otpion.
TEST_F(CommandLineParserTests, AddHelpOption)
{
  CommandLineParser parser;

  auto &option = parser.AddHelpOption();

  EXPECT_EQ(option.GetOptionNames(), std::vector<std::string>({"-h", "--help"}));
  EXPECT_FALSE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());
  EXPECT_EQ(parser.GetPositionalOptionCount(), -1);  // no parsing yet
}

//! Testing AddOption method.
TEST_F(CommandLineParserTests, AddOption)
{
  CommandLineParser parser;
  const auto &option1 = parser.AddOption({"-f", "--file"});
  EXPECT_EQ(option1.GetOptionNames(), std::vector<std::string>({"-f", "--file"}));

  const auto &option2 = parser.AddOption({"--help"}, "description");
  EXPECT_EQ(option2.GetOptionNames(), std::vector<std::string>({"--help"}));
  EXPECT_EQ(option2.GetDescription(), std::string("description"));

  // access to existing options
  EXPECT_EQ(parser.GetOption("-f"), &option1);
  EXPECT_EQ(parser.GetOption("--file"), &option1);
  EXPECT_EQ(parser.GetOption("--help"), &option2);

  // non-existing options
  EXPECT_EQ(parser.GetOption("-file"), nullptr);
  EXPECT_EQ(parser.GetOption("-help"), nullptr);
  EXPECT_EQ(parser.GetOption("-nonexisting"), nullptr);

  EXPECT_EQ(parser.GetPositionalOptionCount(), -1);  // no parsing yet
}

//! Testing fluent interface of CommandLineOption.
TEST_F(CommandLineParserTests, FluentInterface)
{
  CommandLineParser parser;
  auto &option =
      parser.AddOption({"-f", "--file"}).SetDescription("description").SetDefaultValue("value");

  EXPECT_EQ(option.GetOptionNames(), std::vector<std::string>({"-f", "--file"}));
  EXPECT_EQ(option.GetDescription(), std::string("description"));
  EXPECT_EQ(option.GetDefaultValue(), std::string("value"));
}

//! Parsing a single option without a parameter (a flag).
TEST_F(CommandLineParserTests, ParseFlag)
{
  CommandLineParser parser;
  auto &option = parser.AddOption({"--verbose"});

  EXPECT_FALSE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  std::array<const char *, argc> argv{"progname", "--verbose"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  EXPECT_TRUE(parser.IsSet("--verbose"));
  EXPECT_FALSE(parser.IsSet("-f"));
  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Parsing two options without a parameter (flags).
TEST_F(CommandLineParserTests, ParseTwoFlags)
{
  CommandLineParser parser;
  parser.AddOption({"-v", "--verbose"});
  parser.AddOption({"-f", "--file"});

  const int argc = 3;
  // command line contains short version of flags
  std::array<const char *, argc> argv{"progname", "-v", "-f"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  // parser should report both versions (short and long) as set
  EXPECT_TRUE(parser.IsSet("--verbose"));
  EXPECT_TRUE(parser.IsSet("-v"));
  EXPECT_TRUE(parser.IsSet("--file"));
  EXPECT_TRUE(parser.IsSet("-f"));
  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Parsing single parameter (option that has a value).
TEST_F(CommandLineParserTests, ParseParameter)
{
  CommandLineParser parser;
  auto &option = parser.AddOption({"--font"}).SetParameter(true);

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char *, argc> argv{"progname", "--font=10"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  EXPECT_TRUE(parser.IsSet("--font"));

  EXPECT_EQ(parser.GetValue<int>("--font"), 10);
  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Parsing single parameter (option that has a value).
//! Same as above, except that " " space is used instead of equal sign "=" while providing
//! parameter's argument.
TEST_F(CommandLineParserTests, ParseParameterSpaceDelimeter)
{
  CommandLineParser parser;
  auto &option = parser.AddOption({"--font"}).SetParameter(true);
  // to make sure that default value doesn't influence the parsing of user input.
  option.SetDefaultValue("42");

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 3;
  // command line contains short version of flags
  std::array<const char *, argc> argv{"progname", "--font", "10"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  EXPECT_TRUE(parser.IsSet("--font"));

  EXPECT_EQ(parser.GetValue<int>("--font"), 10);
  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Attempt to parse single parameter option, when parameter wasn't provided.
TEST_F(CommandLineParserTests, AttemptToParseParameterOptionWithoutParameter)
{
  CommandLineParser parser;
  auto &option = parser.AddOption({"--font"}).SetParameter(true);

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  // command line doesn't contain a parameter
  std::array<const char *, argc> argv{"progname", "--font"};

  EXPECT_FALSE(parser.Parse(argc, &argv[0]));

  EXPECT_FALSE(parser.IsSet("--font"));

  EXPECT_THROW(parser.GetValue<int>("--font"), std::runtime_error);

  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Parsing a single parameter (option that has a value) with default parameter defined.
TEST_F(CommandLineParserTests, ParseParameterWithDefaultValue)
{
  CommandLineParser parser;
  auto &option = parser.AddOption({"--font"}).SetParameter(true).SetDefaultValue("42");

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char *, argc> argv{"progname", "--font"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  EXPECT_TRUE(parser.IsSet("--font"));

  EXPECT_EQ(parser.GetValue<int>("--font"), 42);

  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Attempt to parse command line when required option is missed.
TEST_F(CommandLineParserTests, CommandLineWithoutRequiredArguments)
{
  CommandLineParser parser;
  auto &option = parser.AddOption({"--font", "-f"}).SetParameter(true).SetRequired(true);

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  // command line doesn't contain a parameter
  std::array<const char *, argc> argv{"progname", "--version"};

  EXPECT_FALSE(parser.Parse(argc, &argv[0]));

  EXPECT_FALSE(parser.IsSet("--font"));

  EXPECT_THROW(parser.GetValue<int>("--font"), std::runtime_error);

  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Attempt to parse command line when required option is missed.
//! Difference with the previous test is the presence of help options (real-life bug)
TEST_F(CommandLineParserTests, CommandLineWithoutRequiredArgumentsWhenHelpIsPresent)
{
  CommandLineParser parser;
  parser.AddHelpOption();
  auto &option = parser.AddOption({"-f", "--file"}).SetParameter(true).SetRequired(true);

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  // command line doesn't contain a parameter
  std::array<const char *, argc> argv{"progname", "--version"};

  EXPECT_FALSE(parser.Parse(argc, &argv[0]));

  EXPECT_FALSE(parser.IsSet("--font"));

  EXPECT_THROW(parser.GetValue<int>("--font"), std::runtime_error);

  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Parsing command line string containing a help option.
TEST_F(CommandLineParserTests, ParseHelpOption)
{
  CommandLineParser parser;
  parser.AddHelpOption();

  const int argc = 2;
  std::array<const char *, argc> argv{"progname", "--help"};

  EXPECT_FALSE(parser.Parse(argc, &argv[0]));

  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Parsing file option.
//! Real-life bug when the value is accessed using one of the aliases.
TEST_F(CommandLineParserTests, ParseFileOption)
{
  CommandLineParser parser;
  parser.AddHelpOption();
  auto &option = parser.AddOption({"-f", "--file"}).SetParameter(true).SetRequired(true);

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 3;
  std::array<const char *, argc> argv{"progname", "--f", "filename"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  EXPECT_TRUE(parser.IsSet("-f"));
  EXPECT_TRUE(parser.IsSet("--file"));

  EXPECT_EQ(parser.GetValue<std::string>("--file"), "filename");
  EXPECT_EQ(parser.GetValue<std::string>("-f"), "filename");

  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Parsing timeout option.
//! Real-life bug when the value is accessed using one of the aliases, and the default value is set.
TEST_F(CommandLineParserTests, ParseTimeoutOption)
{
  CommandLineParser parser;
  parser.AddHelpOption();
  auto &option = parser.AddOption({"-t", "--timeout"})
                     .SetParameter(true)
                     .SetValueName("sec")
                     .SetDefaultValue("5.0");

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  {  // set -t option
    const int argc = 3;
    std::array<const char *, argc> argv{"progname", "-t", "42"};

    EXPECT_TRUE(parser.Parse(argc, &argv[0]));

    EXPECT_TRUE(parser.IsSet("-t"));
    EXPECT_TRUE(parser.IsSet("--timeout"));

    EXPECT_DOUBLE_EQ(parser.GetValue<double>("--timeout"), 42.0);
    EXPECT_DOUBLE_EQ(parser.GetValue<double>("-t"), 42.0);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }

  {  // set --timeout option
    const int argc = 3;
    std::array<const char *, argc> argv{"progname", "--timeout", "42"};

    EXPECT_TRUE(parser.Parse(argc, &argv[0]));

    EXPECT_TRUE(parser.IsSet("-t"));
    EXPECT_TRUE(parser.IsSet("--timeout"));

    EXPECT_DOUBLE_EQ(parser.GetValue<double>("--timeout"), 42.0);
    EXPECT_DOUBLE_EQ(parser.GetValue<double>("-t"), 42.0);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }
}

//! Parsing timeout option (parameter without default value).
TEST_F(CommandLineParserTests, ParseTimeoutOptionNoDefaultValue)
{
  CommandLineParser parser;
  parser.AddHelpOption();
  auto &option = parser.AddOption({"-t", "--timeout"}).SetParameter(true).SetValueName("sec");

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  {  // set -t option with value
    const int argc = 3;
    std::array<const char *, argc> argv{"progname", "-t", "42"};

    EXPECT_TRUE(parser.Parse(argc, &argv[0]));

    EXPECT_TRUE(parser.IsSet("-t"));
    EXPECT_TRUE(parser.IsSet("--timeout"));

    EXPECT_DOUBLE_EQ(parser.GetValue<double>("--timeout"), 42.0);
    EXPECT_DOUBLE_EQ(parser.GetValue<double>("-t"), 42.0);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }

  {  // set -t option without value
    const int argc = 2;
    std::array<const char *, argc> argv{"progname", "-t"};

    EXPECT_FALSE(parser.Parse(argc, &argv[0]));

    EXPECT_FALSE(parser.IsSet("-t"));
    EXPECT_FALSE(parser.IsSet("--timeout"));

    EXPECT_THROW(parser.GetValue<double>("--timeout"), std::runtime_error);
    EXPECT_THROW(parser.GetValue<double>("-t"), std::runtime_error);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }

  {  // set --timeout option with value
    const int argc = 3;
    std::array<const char *, argc> argv{"progname", "--timeout", "42"};

    EXPECT_TRUE(parser.Parse(argc, &argv[0]));

    EXPECT_TRUE(parser.IsSet("-t"));
    EXPECT_TRUE(parser.IsSet("--timeout"));

    EXPECT_DOUBLE_EQ(parser.GetValue<double>("--timeout"), 42.0);
    EXPECT_DOUBLE_EQ(parser.GetValue<double>("-t"), 42.0);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }

  {  // set --timeout option without value
    const int argc = 2;
    std::array<const char *, argc> argv{"progname", "--timeout"};

    EXPECT_FALSE(parser.Parse(argc, &argv[0]));

    EXPECT_FALSE(parser.IsSet("-t"));
    EXPECT_FALSE(parser.IsSet("--timeout"));

    EXPECT_THROW(parser.GetValue<double>("--timeout"), std::runtime_error);
    EXPECT_THROW(parser.GetValue<double>("-t"), std::runtime_error);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }
}

//! Validate multiline string representing `usage` help for the single option setup.
TEST_F(CommandLineParserTests, GetUsageString)
{
  CommandLineParser parser;
  auto &option = parser.AddOption({"-v", "--verbose"});
  option.SetDescription("description");

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char *, argc> argv{"progname", "--verbose"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  std::string expected(R"RAW(Usage: progname [options]

Options:
-v, --verbose                 description

)RAW");
  EXPECT_EQ(parser.GetUsageString(), expected);
}

//! Validate multiline string representing `usage` help for a single positional option.
TEST_F(CommandLineParserTests, GetUsageStringForPositionalOptions)
{
  CommandLineParser parser;
  parser.AddPositionalOption("<target>", "positional option");

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char *, argc> argv{"progname", "var0"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  std::string expected(R"RAW(Usage: progname [options]

Options:

Positional Options:
<target>                      positional option

)RAW");
  EXPECT_EQ(parser.GetUsageString(), expected);
}

//! Validate multiline :usage" string when header and foor are provided.
TEST_F(CommandLineParserTests, GetUsageStringWithHeaderAndFooter)
{
  CommandLineParser parser;
  parser.SetDescription("header", "footer");
  auto &option = parser.AddOption({"-v", "--verbose"});
  option.SetDescription("description");

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char *, argc> argv{"progname", "--verbose"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  std::string expected(R"RAW(Usage: progname [options]

header
Options:
-v, --verbose                 description

footer
)RAW");
  EXPECT_EQ(parser.GetUsageString(), expected);
}

//! Single positional argument in the command line. No pre-registered options.
TEST_F(CommandLineParserTests, PositionalArgument)
{
  CommandLineParser parser;

  const int argc = 2;
  std::array<const char *, argc> argv{"progname", "abc"};

  // before parsing
  EXPECT_EQ(parser.GetPositionalOptionCount(), -1);
  EXPECT_THROW(parser.GetPositionalValue(0), std::runtime_error);
  EXPECT_TRUE(parser.GetPositionalValues().empty());

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  // after parsing
  EXPECT_EQ(parser.GetPositionalOptionCount(), 1);
  EXPECT_EQ(parser.GetPositionalValue(0), std::string("abc"));
  EXPECT_EQ(parser.GetPositionalValues(), std::vector<std::string>({"abc"}));
}

//! Two positional arguments in the command line. No pre-registered options.
TEST_F(CommandLineParserTests, TwoPositionalArguments)
{
  CommandLineParser parser;

  const int argc = 3;
  std::array<const char *, argc> argv{"progname", "abc", "def"};

  // before parsing
  EXPECT_EQ(parser.GetPositionalOptionCount(), -1);
  EXPECT_THROW(parser.GetPositionalValue(0), std::runtime_error);
  EXPECT_TRUE(parser.GetPositionalValues().empty());

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  // after parsing
  EXPECT_EQ(parser.GetPositionalOptionCount(), 2);
  EXPECT_EQ(parser.GetPositionalValue(0), std::string("abc"));
  EXPECT_EQ(parser.GetPositionalValue(1), std::string("def"));
  EXPECT_EQ(parser.GetPositionalValues(), std::vector<std::string>({"abc", "def"}));
}

//! Positional argument, then flag, then positional argument again
TEST_F(CommandLineParserTests, FlagAndPositionalArgument)
{
  CommandLineParser parser;
  auto &option = parser.AddOption({"--verbose"});
  EXPECT_FALSE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  parser.AddPositionalOption("<varName>");

  const int argc = 4;
  std::array<const char *, argc> argv{"progname", "var0", "--verbose", "var1"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  EXPECT_TRUE(parser.IsSet("--verbose"));
  EXPECT_FALSE(parser.IsSet("-f"));
  EXPECT_EQ(parser.GetPositionalOptionCount(), 2);
  EXPECT_EQ(parser.GetPositionalValues(), std::vector<std::string>({"var0", "var1"}));
}
