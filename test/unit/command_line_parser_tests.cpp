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

#include <sup/cli/argh.h>
#include <sup/cli/command_line_parser.h>
#include <sup/cli/exceptions.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <stdexcept>
#include <type_traits>
#include <utility>

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
    std::array<const char*, argc> argv{"progname"};
    parser.parse(argc, argv.data());

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
    std::array<const char*, argc> argv{"progname", "--font", "24"};
    parser.parse(argc, argv.data());

    // make sure it's not treated as a flag ([] operator)
    EXPECT_FALSE(parser["--font"]);

    // it reports program name as positional argument
    EXPECT_EQ(parser.size(), 1);
  }

  {  // parameter is pre-registered, but the value is missed
    argh::parser parser;
    parser.add_param("--font");

    const int argc = 2;
    std::array<const char*, argc> argv{"progname", "--font"};
    parser.parse(argc, argv.data());

    // strangely, it appeared as a flag
    EXPECT_TRUE(parser["--font"]);

    // it reports program name as positional argument
    EXPECT_EQ(parser.size(), 1);
  }

  {  // no registration
    argh::parser parser;

    const int argc = 2;
    std::array<const char*, argc> argv{"progname", "--font"};
    parser.parse(argc, argv.data());

    // make sure it's treated as a flag ([] operator)
    EXPECT_TRUE(parser["--font"]);

    // it reports program name as positional argument
    EXPECT_EQ(parser.size(), 1);
  }

  {  // no registration, but we pass free standing parameter
    argh::parser parser;

    const int argc = 3;
    std::array<const char*, argc> argv{"progname", "--font", "24"};
    parser.parse(argc, argv.data());

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

  auto& option = parser.AddHelpOption();

  EXPECT_EQ(option.GetOptionNames(), std::vector<std::string>({"-h", "--help"}));
  EXPECT_FALSE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());
  EXPECT_EQ(parser.GetPositionalOptionCount(), -1);  // no parsing yet
}

//! Testing AddOption method.
TEST_F(CommandLineParserTests, AddOption)
{
  CommandLineParser parser;
  const auto& option1 = parser.AddOption({"-f", "--file"});
  EXPECT_EQ(option1.GetOptionNames(), std::vector<std::string>({"-f", "--file"}));

  const auto& option2 = parser.AddOption({"--help"}, "description");
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
//! A configured parser can be moved, so that a function can hand one over. Copying is forbidden.
TEST_F(CommandLineParserTests, MoveParser)
{
  static_assert(!std::is_copy_constructible_v<CommandLineParser>,
                "the parser owns its options and shall not be copied");
  static_assert(!std::is_copy_assignable_v<CommandLineParser>,
                "the parser owns its options and shall not be copied");
  static_assert(std::is_nothrow_move_constructible_v<CommandLineParser>,
                "the parser shall be movable without throwing");
  static_assert(std::is_nothrow_move_assignable_v<CommandLineParser>,
                "the parser shall be movable without throwing");

  // a function registering a shared set of options, and handing the parser over
  auto create_parser = []()
  {
    CommandLineParser result;
    (void)result.AddOption({"-f", "--file"}).SetParameter(true);
    (void)result.AddOption({"-v", "--verbose"});
    return result;
  };

  auto parser = create_parser();

  const int argc = 4;
  std::array<const char*, argc> argv{"progname", "-v", "--file", "a.xml"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  {  // move construction carries the options and the parsing result along
    const CommandLineParser moved{std::move(parser)};
    EXPECT_TRUE(moved.IsSet("-v"));
    EXPECT_EQ(moved.GetValue<std::string>("--file"), std::string("a.xml"));
    EXPECT_NE(moved.GetOption("--verbose"), nullptr);
  }

  {  // and so does move assignment
    CommandLineParser other = create_parser();
    other = create_parser();
    EXPECT_FALSE(other.IsSet("-v"));

    CommandLineParser source;
    (void)source.AddOption({"-v", "--verbose"});
    std::array<const char*, 2> source_argv{"progname", "-v"};
    EXPECT_TRUE(source.Parse(2, source_argv.data()));

    other = std::move(source);
    EXPECT_TRUE(other.IsSet("-v"));
  }
}

//! Accessing options through a const parser: the option is handed out for reading only.
TEST_F(CommandLineParserTests, GetOptionFromConstParser)
{
  CommandLineParser parser;
  const auto& option = parser.AddOption({"-f", "--file"}, "description");

  const auto& const_parser = parser;

  const auto* found = const_parser.GetOption("--file");
  static_assert(std::is_same_v<decltype(found), const CommandLineOption*>,
                "const parser must hand out a const option");

  EXPECT_EQ(found, &option);
  EXPECT_EQ(const_parser.GetOption("-f"), &option);
  EXPECT_EQ(found->GetDescription(), std::string("description"));

  EXPECT_EQ(const_parser.GetOption("-nonexisting"), nullptr);

  // the same lookup on a mutable parser gives an option that can be configured further
  auto* mutable_found = parser.GetOption("--file");
  static_assert(std::is_same_v<decltype(mutable_found), CommandLineOption*>,
                "mutable parser must hand out a mutable option");
  EXPECT_EQ(mutable_found, &option);
}

TEST_F(CommandLineParserTests, FluentInterface)
{
  CommandLineParser parser;
  auto& option =
      parser.AddOption({"-f", "--file"}).SetDescription("description").SetDefaultValue("value");

  EXPECT_EQ(option.GetOptionNames(), std::vector<std::string>({"-f", "--file"}));
  EXPECT_EQ(option.GetDescription(), std::string("description"));
  EXPECT_EQ(option.GetDefaultValue(), std::string("value"));
}

//! Querying a parser through a const reference, as an application that only reads the parsing
//! result does.
TEST_F(CommandLineParserTests, QueryConstParser)
{
  CommandLineParser parser;
  (void)parser.AddOption({"-v", "--verbose"});
  (void)parser.AddOption({"-f", "--file"}).SetParameter(true);

  const int argc = 4;
  std::array<const char*, argc> argv{"progname", "-v", "--file", "a.xml"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  const auto& const_parser = parser;

  EXPECT_TRUE(const_parser.IsSet("-v"));
  EXPECT_TRUE(const_parser.IsSet("--verbose"));
  EXPECT_TRUE(const_parser.IsSet("--file"));
  EXPECT_THROW(const_parser.IsSet("--non-existing"), InvalidOptionException);
  EXPECT_EQ(const_parser.GetValue<std::string>("--file"), std::string("a.xml"));
}

//! Parsing a single option without a parameter (a flag).
TEST_F(CommandLineParserTests, ParseFlag)
{
  CommandLineParser parser;
  auto& option = parser.AddOption({"--verbose"});

  EXPECT_FALSE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  std::array<const char*, argc> argv{"progname", "--verbose"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  EXPECT_TRUE(parser.IsSet("--verbose"));
  // asking about an option that was never added is a mistake, not a "no"
  EXPECT_THROW(parser.IsSet("-f"), InvalidOptionException);
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
  std::array<const char*, argc> argv{"progname", "-v", "-f"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

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
  auto& option = parser.AddOption({"--font"}).SetParameter(true);

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char*, argc> argv{"progname", "--font=10"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

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
  auto& option = parser.AddOption({"--font"}).SetParameter(true);
  // to make sure that default value doesn't influence the parsing of user input.
  option.SetDefaultValue("42");

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 3;
  // command line contains short version of flags
  std::array<const char*, argc> argv{"progname", "--font", "10"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  EXPECT_TRUE(parser.IsSet("--font"));

  EXPECT_EQ(parser.GetValue<int>("--font"), 10);
  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Attempt to parse single parameter option, when parameter wasn't provided.
TEST_F(CommandLineParserTests, AttemptToParseParameterOptionWithoutParameter)
{
  CommandLineParser parser;
  auto& option = parser.AddOption({"--font"}).SetParameter(true);

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  // command line doesn't contain a parameter
  std::array<const char*, argc> argv{"progname", "--font"};

  EXPECT_FALSE(parser.Parse(argc, argv.data()));

  EXPECT_FALSE(parser.IsSet("--font"));

  EXPECT_THROW(parser.GetValue<int>("--font"), std::runtime_error);

  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Parsing a single parameter (option that has a value) with default parameter defined.
TEST_F(CommandLineParserTests, ParseParameterWithDefaultValue)
{
  CommandLineParser parser;
  auto& option = parser.AddOption({"--font"}).SetParameter(true).SetDefaultValue("42");

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char*, argc> argv{"progname", "--font"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  EXPECT_TRUE(parser.IsSet("--font"));

  EXPECT_EQ(parser.GetValue<int>("--font"), 42);

  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Attempt to parse command line when required option is missed.
TEST_F(CommandLineParserTests, CommandLineWithoutRequiredArguments)
{
  CommandLineParser parser;
  auto& option = parser.AddOption({"--font", "-f"}).SetParameter(true).SetRequired(true);

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  // command line doesn't contain a parameter
  std::array<const char*, argc> argv{"progname", "--version"};

  EXPECT_FALSE(parser.Parse(argc, argv.data()));

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
  auto& option = parser.AddOption({"-f", "--file"}).SetParameter(true).SetRequired(true);

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 2;
  // command line doesn't contain a parameter
  std::array<const char*, argc> argv{"progname", "--version"};

  EXPECT_FALSE(parser.Parse(argc, argv.data()));

  EXPECT_THROW(parser.IsSet("--font"), InvalidOptionException);

  EXPECT_THROW(parser.GetValue<int>("--font"), std::runtime_error);

  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Parsing command line string containing a help option.
TEST_F(CommandLineParserTests, ParseHelpOption)
{
  CommandLineParser parser;
  parser.AddHelpOption();

  const int argc = 2;
  std::array<const char*, argc> argv{"progname", "--help"};

  EXPECT_FALSE(parser.Parse(argc, argv.data()));

  EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
}

//! Parsing file option.
//! Real-life bug when the value is accessed using one of the aliases.
TEST_F(CommandLineParserTests, ParseFileOption)
{
  CommandLineParser parser;
  parser.AddHelpOption();
  auto& option = parser.AddOption({"-f", "--file"}).SetParameter(true).SetRequired(true);

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  const int argc = 3;
  std::array<const char*, argc> argv{"progname", "--f", "filename"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

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
  auto& option = parser.AddOption({"-t", "--timeout"})
                     .SetParameter(true)
                     .SetValueName("sec")
                     .SetDefaultValue("5.0");

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  {  // set -t option
    const int argc = 3;
    std::array<const char*, argc> argv{"progname", "-t", "42"};

    EXPECT_TRUE(parser.Parse(argc, argv.data()));

    EXPECT_TRUE(parser.IsSet("-t"));
    EXPECT_TRUE(parser.IsSet("--timeout"));

    EXPECT_DOUBLE_EQ(parser.GetValue<double>("--timeout"), 42.0);
    EXPECT_DOUBLE_EQ(parser.GetValue<double>("-t"), 42.0);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }

  {  // set --timeout option
    const int argc = 3;
    std::array<const char*, argc> argv{"progname", "--timeout", "42"};

    EXPECT_TRUE(parser.Parse(argc, argv.data()));

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
  auto& option = parser.AddOption({"-t", "--timeout"}).SetParameter(true).SetValueName("sec");

  EXPECT_TRUE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  {  // set -t option with value
    const int argc = 3;
    std::array<const char*, argc> argv{"progname", "-t", "42"};

    EXPECT_TRUE(parser.Parse(argc, argv.data()));

    EXPECT_TRUE(parser.IsSet("-t"));
    EXPECT_TRUE(parser.IsSet("--timeout"));

    EXPECT_DOUBLE_EQ(parser.GetValue<double>("--timeout"), 42.0);
    EXPECT_DOUBLE_EQ(parser.GetValue<double>("-t"), 42.0);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }

  {  // set -t option without value
    const int argc = 2;
    std::array<const char*, argc> argv{"progname", "-t"};

    EXPECT_FALSE(parser.Parse(argc, argv.data()));

    EXPECT_FALSE(parser.IsSet("-t"));
    EXPECT_FALSE(parser.IsSet("--timeout"));

    EXPECT_THROW(parser.GetValue<double>("--timeout"), std::runtime_error);
    EXPECT_THROW(parser.GetValue<double>("-t"), std::runtime_error);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }

  {  // set --timeout option with value
    const int argc = 3;
    std::array<const char*, argc> argv{"progname", "--timeout", "42"};

    EXPECT_TRUE(parser.Parse(argc, argv.data()));

    EXPECT_TRUE(parser.IsSet("-t"));
    EXPECT_TRUE(parser.IsSet("--timeout"));

    EXPECT_DOUBLE_EQ(parser.GetValue<double>("--timeout"), 42.0);
    EXPECT_DOUBLE_EQ(parser.GetValue<double>("-t"), 42.0);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }

  {  // set --timeout option without value
    const int argc = 2;
    std::array<const char*, argc> argv{"progname", "--timeout"};

    EXPECT_FALSE(parser.Parse(argc, argv.data()));

    EXPECT_FALSE(parser.IsSet("-t"));
    EXPECT_FALSE(parser.IsSet("--timeout"));

    EXPECT_THROW(parser.GetValue<double>("--timeout"), std::runtime_error);
    EXPECT_THROW(parser.GetValue<double>("-t"), std::runtime_error);

    EXPECT_EQ(parser.GetPositionalOptionCount(), 0);
  }
}

//! A parameter value containing spaces is returned as it was given, not truncated at the first
//! whitespace.
TEST_F(CommandLineParserTests, ParseValueWithSpaces)
{
  const std::string path_with_spaces{"/path/with space/a.xml"};

  CommandLineParser parser;
  (void)parser.AddOption({"-f", "--file"}).SetParameter(true);
  (void)parser.AddOption({"--msg"}).SetParameter(true);
  (void)parser.AddOption({"--def"}).SetParameter(true).SetDefaultValue("a default with spaces");

  const int argc = 5;
  std::array<const char*, argc> argv{"progname", "--file", path_with_spaces.c_str(), "--msg",
                                     "hello world"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  EXPECT_EQ(parser.GetValue<std::string>("--file"), path_with_spaces);
  EXPECT_EQ(parser.GetValue<std::string>("-f"), path_with_spaces);
  EXPECT_EQ(parser.GetValue<std::string>("--msg"), std::string("hello world"));

  // an option falling back to its default value gets the whole default
  EXPECT_EQ(parser.GetValue<std::string>("--def"), std::string("a default with spaces"));
}

//! A positional value containing spaces is returned as it was given.
TEST_F(CommandLineParserTests, PositionalValueWithSpaces)
{
  const std::string path_with_spaces{"/path/with space/a.xml"};

  CommandLineParser parser;
  parser.AddPositionalOption("<file>", "input file");

  const int argc = 2;
  std::array<const char*, argc> argv{"progname", path_with_spaces.c_str()};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  EXPECT_EQ(parser.GetPositionalOptionCount(), 1);
  EXPECT_EQ(parser.GetPositionalValue<std::string>(0), path_with_spaces);
  EXPECT_EQ(parser.GetPositionalValues(), std::vector<std::string>({path_with_spaces}));
}

//! The three ways of failing to get a value are reported as distinct exceptions, each naming what
//! it choked on.
TEST_F(CommandLineParserTests, ExceptionsWhenValueIsNotAvailable)
{
  CommandLineParser parser;
  (void)parser.AddOption({"-t", "--timeout"}).SetParameter(true);
  (void)parser.AddOption({"--with-default"}).SetParameter(true).SetDefaultValue("42");
  parser.AddPositionalOption("<target>", "target");

  const int argc = 2;
  std::array<const char*, argc> argv{"progname", "target.xml"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  // an option name that was never added to the parser: a mistake in the application, and the
  // message names the option that was asked about
  EXPECT_THAT([&] { (void)parser.GetValue<std::string>("--typo"); },
              ::testing::ThrowsMessage<InvalidOptionException>(::testing::HasSubstr("--typo")));

  // a registered option that was not given and has no default value
  EXPECT_THAT([&] { (void)parser.GetValue<double>("--timeout"); },
              ::testing::ThrowsMessage<MissingValueException>(::testing::AllOf(
                  ::testing::HasSubstr("--timeout"), ::testing::HasSubstr("no default value"))));

  // a positional value beyond the ones found: the message reports both the index asked for and
  // how many values there are
  EXPECT_EQ(parser.GetPositionalValue<std::string>(0), std::string("target.xml"));
  EXPECT_THAT([&] { (void)parser.GetPositionalValue<std::string>(1); },
              ::testing::ThrowsMessage<MissingValueException>(::testing::AllOf(
                  ::testing::HasSubstr("index 1"), ::testing::HasSubstr("found is 1"))));

  // an option with a default value never fails
  EXPECT_EQ(parser.GetValue<int>("--with-default"), 42);
}

//! A value the requested type cannot hold is reported, and not silently turned into a number
//! nobody asked for.
TEST_F(CommandLineParserTests, GetValueIsStrict)
{
  CommandLineParser parser;
  (void)parser.AddOption({"-s", "--size"}).SetParameter(true);
  (void)parser.AddOption({"-c", "--count"}).SetParameter(true);
  (void)parser.AddOption({"--rate"}).SetParameter(true);
  parser.AddPositionalOption("<target>", "target");

  const int argc = 8;
  std::array<const char*, argc> argv{"progname", "--size", "64k", "--count",
                                     "-1",       "--rate", "1e4", "abc"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  // the text of an option value is always available
  EXPECT_EQ(parser.GetValue<std::string>("--size"), std::string("64k"));

  // but it does not convert to a number
  EXPECT_THAT([&] { (void)parser.GetValue<int>("--size"); },
              ::testing::ThrowsMessage<InvalidValueException>(
                  ::testing::AllOf(::testing::HasSubstr("64k"), ::testing::HasSubstr("--size"))));

  // a negative number asked for as unsigned does not wrap around
  EXPECT_EQ(parser.GetValue<int>("--count"), -1);
  EXPECT_THROW(parser.GetValue<unsigned int>("--count"), InvalidValueException);

  // what did convert before still converts
  EXPECT_DOUBLE_EQ(parser.GetValue<double>("--rate"), 10000.0);

  // and so does a positional value, whose message names the index
  EXPECT_EQ(parser.GetPositionalValue<std::string>(0), std::string("abc"));
  EXPECT_THAT([&] { (void)parser.GetPositionalValue<int>(0); },
              ::testing::ThrowsMessage<InvalidValueException>(::testing::AllOf(
                  ::testing::HasSubstr("abc"), ::testing::HasSubstr("positional value 0"))));
}

//! Checking a value against bounds, with the message stating them.
TEST_F(CommandLineParserTests, GetValueInRange)
{
  CommandLineParser parser;
  (void)parser.AddOption({"--port"}).SetParameter(true);
  (void)parser.AddOption({"--rate"}).SetParameter(true);

  const int argc = 5;
  std::array<const char*, argc> argv{"progname", "--port", "70000", "--rate", "nan"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  EXPECT_THAT([&] { (void)parser.GetValueInRange<int>("--port", 1024, 65535); },
              ::testing::ThrowsMessage<InvalidValueException>(
                  ::testing::AllOf(::testing::HasSubstr("70000"), ::testing::HasSubstr("1024"),
                                   ::testing::HasSubstr("65535"))));

  // a not-a-number fails both comparisons, and is reported rather than let through
  EXPECT_THROW(parser.GetValueInRange<double>("--rate", 0.001, 1.0e9), InvalidValueException);

  // a value within the bounds is returned
  CommandLineParser good;
  (void)good.AddOption({"--port"}).SetParameter(true);
  std::array<const char*, 3> good_argv{"progname", "--port", "8080"};
  EXPECT_TRUE(good.Parse(3, good_argv.data()));
  EXPECT_EQ(good.GetValueInRange<int>("--port", 1024, 65535), 8080);
}

//! An empty string is a default value like any other, and is told apart from the absence of one.
TEST_F(CommandLineParserTests, EmptyDefaultValue)
{
  CommandLineParser parser;
  auto& with_default = parser.AddOption({"--prefix"}).SetParameter(true).SetDefaultValue("");
  auto& without_default = parser.AddOption({"--nodefault"}).SetParameter(true);

  EXPECT_TRUE(with_default.HasDefaultValue());
  EXPECT_EQ(with_default.GetDefaultValue(), std::string(""));
  EXPECT_FALSE(without_default.HasDefaultValue());

  const int argc = 1;
  std::array<const char*, argc> argv{"progname"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  // the empty default is returned, rather than reported as no value at all
  EXPECT_EQ(parser.GetValue<std::string>("--prefix"), std::string(""));
  EXPECT_TRUE(parser.FindValue("--prefix").has_value());

  // while an option with no default still has nothing to give
  EXPECT_FALSE(parser.FindValue("--nodefault").has_value());
  EXPECT_THROW(parser.GetValue<std::string>("--nodefault"), MissingValueException);
}

//! A value that was given counts as given even when it is empty. An empty command line argument
//! also has to survive the parsing at all, which it did not before.
TEST_F(CommandLineParserTests, EmptyValueGivenInCommandLine)
{
  CommandLineParser parser;
  (void)parser.AddOption({"-m", "--msg"}).SetParameter(true);
  (void)parser.AddOption({"--other"}).SetParameter(true);

  const int argc = 5;
  std::array<const char*, argc> argv{"progname", "--msg", "", "--other", "x"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  EXPECT_TRUE(parser.IsSet("--msg"));
  EXPECT_TRUE(parser.IsSet("-m"));
  EXPECT_EQ(parser.GetValue<std::string>("--msg"), std::string(""));
  EXPECT_TRUE(parser.FindValue("--msg").has_value());
  EXPECT_EQ(parser.FindValue("--msg").value(), std::string(""));

  // the option after the empty value is unaffected
  EXPECT_EQ(parser.GetValue<std::string>("--other"), std::string("x"));
}

//! Asking for a value without having to know first whether it is there.
TEST_F(CommandLineParserTests, FindValueAndFallback)
{
  CommandLineParser parser;
  (void)parser.AddOption({"--run-id"}).SetParameter(true);
  (void)parser.AddOption({"--count"}).SetParameter(true);
  (void)parser.AddOption({"--warmup"}).SetParameter(true).SetDefaultValue("100");

  const int argc = 3;
  std::array<const char*, argc> argv{"progname", "--count", "7"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  {  // an option that was given
    EXPECT_EQ(parser.FindValue("--count").value(), std::string("7"));
    EXPECT_EQ(parser.GetValue<int>("--count", 1), 7);
  }

  {  // an option that was not given and has no default value
    EXPECT_FALSE(parser.FindValue("--run-id").has_value());
    EXPECT_EQ(parser.GetValue<std::string>("--run-id", ""), std::string(""));
    EXPECT_EQ(parser.GetValue<std::string>("--run-id", "fallback"), std::string("fallback"));
    EXPECT_EQ(parser.GetValue<int>("--run-id", 42), 42);
  }

  {  // an option that was not given, but has a default value: the default wins over the fallback
    EXPECT_EQ(parser.FindValue("--warmup").value(), std::string("100"));
    EXPECT_EQ(parser.GetValue<int>("--warmup", 7), 100);
  }

  // the fallback stands in for an absent value, not for an unusable one
  CommandLineParser bad;
  (void)bad.AddOption({"--size"}).SetParameter(true);
  std::array<const char*, 3> bad_argv{"progname", "--size", "64k"};
  EXPECT_TRUE(bad.Parse(3, bad_argv.data()));
  EXPECT_THROW(bad.GetValue<int>("--size", 1), InvalidValueException);

  // an option name that was never added is a mistake, whichever accessor is used
  EXPECT_THROW(parser.FindValue("--typo"), InvalidOptionException);
  EXPECT_THROW(parser.GetValue<int>("--typo", 1), InvalidOptionException);
}

//! A name that was never added to the parser is a mistake in the application, and is reported as
//! such rather than answered with "not set".
TEST_F(CommandLineParserTests, IsSetReportsUnknownOptionName)
{
  CommandLineParser parser;
  (void)parser.AddOption({"-v", "--verbose"});

  const int argc = 2;
  std::array<const char*, argc> argv{"progname", "-v"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  EXPECT_TRUE(parser.IsSet("-v"));
  EXPECT_TRUE(parser.IsSet("--verbose"));

  // a misspelled name no longer looks like an option the user didn't pass
  EXPECT_THAT(
      [&] { (void)parser.IsSet("--verbsoe"); },
      ::testing::ThrowsMessage<InvalidOptionException>(::testing::AllOf(
          ::testing::HasSubstr("--verbsoe"), ::testing::HasSubstr("not among the options"))));

  // GetOption keeps answering with a null pointer, so it can still be used as a probe
  EXPECT_EQ(parser.GetOption("--verbsoe"), nullptr);
}

//! Claiming a name that is already taken is reported, instead of registering an option that could
//! never be found.
TEST_F(CommandLineParserTests, AddOptionRejectsDuplicateName)
{
  CommandLineParser parser;
  (void)parser.AddOption({"-f", "--file"}, "first");

  // the same name again, alone or among others
  EXPECT_THAT([&] { (void)parser.AddOption({"--file"}, "second"); },
              ::testing::ThrowsMessage<InvalidOptionException>(::testing::AllOf(
                  ::testing::HasSubstr("--file"), ::testing::HasSubstr("already been added"))));
  EXPECT_THROW(parser.AddOption({"-x", "-f"}), InvalidOptionException);

  // a name repeated within one option
  EXPECT_THROW(parser.AddOption({"-y", "-y"}), InvalidOptionException);

  // a help option cannot be added twice either
  (void)parser.AddHelpOption();
  EXPECT_THROW(parser.AddHelpOption(), InvalidOptionException);

  // and a positional option goes through the same check
  parser.AddPositionalOption("<target>", "target");
  EXPECT_THROW(parser.AddPositionalOption("<target>", "again"), InvalidOptionException);

  // the first registration is the one that stands, and is still usable
  EXPECT_EQ(parser.GetOption("--file")->GetDescription(), std::string("first"));

  // free names are still accepted
  EXPECT_NO_THROW((void)parser.AddOption({"-z", "--zap"}));
}

//! Parsing works for an application that offers no help option: Parse must not ask about a name
//! that was never added.
TEST_F(CommandLineParserTests, ParseWithoutHelpOption)
{
  CommandLineParser parser;
  (void)parser.AddOption({"-v", "--verbose"});

  const int argc = 2;
  std::array<const char*, argc> argv{"progname", "-v"};
  EXPECT_NO_THROW((void)parser.Parse(argc, argv.data()));
  EXPECT_TRUE(parser.Parse(argc, argv.data()));
  EXPECT_TRUE(parser.IsSet("-v"));

  // while an application that does offer one still reports help as a failed parsing
  CommandLineParser with_help;
  (void)with_help.AddHelpOption();
  std::array<const char*, 2> help_argv{"progname", "--help"};
  EXPECT_FALSE(with_help.Parse(2, help_argv.data()));
  EXPECT_TRUE(with_help.IsSet("--help"));
}

//! Validate multiline string representing `usage` help for the single option setup.
TEST_F(CommandLineParserTests, GetUsageString)
{
  CommandLineParser parser;
  auto& option = parser.AddOption({"-v", "--verbose"});
  option.SetDescription("description");

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char*, argc> argv{"progname", "--verbose"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  const std::string expected(R"RAW(Usage: progname [options]

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
  std::array<const char*, argc> argv{"progname", "var0"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  const std::string expected(R"RAW(Usage: progname [options] <target>

Options:

Positional Options:
<target>                      positional option

)RAW");
  EXPECT_EQ(parser.GetUsageString(), expected);
}

//! Validate `usage` help when an option name is too long to leave room for the description.
TEST_F(CommandLineParserTests, GetUsageStringWithLongOptionName)
{
  CommandLineParser parser;
  (void)parser.AddOption({"-r", "--receive-buffer-size"}, "Receive socket buffer size")
      .SetParameter(true)
      .SetValueName("bytes");
  (void)parser.AddOption({"-v", "--verbose"}, "Report progress");

  const int argc = 2;
  std::array<const char*, argc> argv{"progname", "--verbose"};
  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  // the long name keeps all of its characters, and every description stays in the same column
  const std::string expected(R"RAW(Usage: progname [options]

Options:
-r, --receive-buffer-size <bytes>
                              Receive socket buffer size
-v, --verbose                 Report progress

)RAW");
  EXPECT_EQ(parser.GetUsageString(), expected);
}

//! Validate multiline :usage" string when header and foor are provided.
TEST_F(CommandLineParserTests, GetUsageStringWithHeaderAndFooter)
{
  CommandLineParser parser;
  parser.SetDescription("header", "footer");
  auto& option = parser.AddOption({"-v", "--verbose"});
  option.SetDescription("description");

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char*, argc> argv{"progname", "--verbose"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  const std::string expected(R"RAW(Usage: progname [options]

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
  std::array<const char*, argc> argv{"progname", "abc"};

  // before parsing
  EXPECT_EQ(parser.GetPositionalOptionCount(), -1);
  EXPECT_THROW(parser.GetPositionalValue<std::string>(0), std::runtime_error);
  EXPECT_TRUE(parser.GetPositionalValues().empty());

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  // after parsing
  EXPECT_EQ(parser.GetPositionalOptionCount(), 1);
  EXPECT_EQ(parser.GetPositionalValue<std::string>(0), std::string("abc"));
  EXPECT_EQ(parser.GetPositionalValues(), std::vector<std::string>({"abc"}));
}

//! Two positional arguments in the command line. No pre-registered options.
TEST_F(CommandLineParserTests, TwoPositionalArguments)
{
  CommandLineParser parser;

  const int argc = 3;
  std::array<const char*, argc> argv{"progname", "abc", "42"};

  // before parsing
  EXPECT_EQ(parser.GetPositionalOptionCount(), -1);
  EXPECT_THROW(parser.GetPositionalValue<std::string>(0), std::runtime_error);
  EXPECT_TRUE(parser.GetPositionalValues().empty());

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  // after parsing
  EXPECT_EQ(parser.GetPositionalOptionCount(), 2);
  EXPECT_EQ(parser.GetPositionalValue<std::string>(0), std::string("abc"));
  EXPECT_EQ(parser.GetPositionalValue<int>(1), 42);
  EXPECT_EQ(parser.GetPositionalValues(), std::vector<std::string>({"abc", "42"}));
}

//! Positional argument, then flag, then positional argument again.
TEST_F(CommandLineParserTests, FlagAndPositionalArgument)
{
  CommandLineParser parser;
  auto& option = parser.AddOption({"--verbose"});
  EXPECT_FALSE(option.IsParameter());
  EXPECT_FALSE(option.IsPositional());

  parser.AddPositionalOption("<varName>");

  const int argc = 4;
  std::array<const char*, argc> argv{"progname", "var0", "--verbose", "var1"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  EXPECT_TRUE(parser.IsSet("--verbose"));
  EXPECT_THROW(parser.IsSet("-f"), InvalidOptionException);
  EXPECT_EQ(parser.GetPositionalOptionCount(), 2);
  EXPECT_EQ(parser.GetPositionalValues(), std::vector<std::string>({"var0", "var1"}));
}

//! Flag with parameter, then positional argument.
TEST_F(CommandLineParserTests, FlagWithParameterAndPositionalArgument)
{
  CommandLineParser parser;
  parser.AddOption({"--font"}).SetParameter(true);

  parser.AddPositionalOption("<varName>");

  const int argc = 4;
  std::array<const char*, argc> argv{"progname", "--font", "42", "128"};

  EXPECT_TRUE(parser.Parse(argc, argv.data()));

  EXPECT_TRUE(parser.IsSet("--font"));
  EXPECT_DOUBLE_EQ(parser.GetValue<int>("--font"), 42);
  EXPECT_EQ(parser.GetPositionalOptionCount(), 1);
  EXPECT_EQ(parser.GetPositionalValue<int>(0), 128);
}
