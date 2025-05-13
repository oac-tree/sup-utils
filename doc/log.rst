=================
Logging Utilities
=================

The ``sup/log`` package provides a flexible and efficient logging framework.

**Key Features**:

  - Multiple log severity levels (e.g., ``DEBUG``, ``INFO``, ``ERROR``).
  - Support for logging to system logs and standard output.
  - Compile-time and runtime log filtering.

**Main Components**:

  1. ``LoggerT``: A templated logger class with compile-time severity filtering.
  2. ``BasicLogger``: Encapsulates basic logging functionality.
  3. ``DefaultLogger``: A logger with default configurations for standard output and system logs.
  4. ``Log Severity Levels``: ``SUP_LOG_EMERG``, ``SUP_LOG_ALERT``, ``SUP_LOG_CRIT``, ``SUP_LOG_ERR``, ``SUP_LOG_WARNING``, ``SUP_LOG_NOTICE``, ``SUP_LOG_INFO``, ``SUP_LOG_DEBUG``, ``SUP_LOG_TRACE``.

**Example**:

.. code-block:: c++

  auto logger = sup::log::CreateDefaultStdoutLogger("MyApp");
  logger.Info("Application started");
  logger.Error("An error occurred");
