========================
 Command-Line Utilities
========================

The ``sup/cli`` package provides tools for parsing and handling command-line arguments.

**Key Features**

  - Parsing command-line arguments into flags, parameters, and positional arguments.
  - Support for help options and usage strings.
  - Easy-to-use API for defining and retrieving options.

**Example**

.. code-block:: c++

       CommandLineParser parser;
       parser.AddOption({"-v", "--verbose"})->SetDescription("Enable verbose mode");
       parser.AddOption("--file")->SetParameter(true)->SetRequired(true);
       parser.Parse(argc, argv);

