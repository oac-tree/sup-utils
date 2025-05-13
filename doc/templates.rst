=========
Templates
=========

The ``sup/templates`` package provides template functions and classes.

**Key Features**:
  - Support for decorator patterns with ownership semantics.

**Main Components**:
  1. ``OwningDecorator``: A decorator that owns the decorated object.
  2. ``DecorateWith``: A utility function to create decorated objects.

**Example**:

.. code-block:: c++

  auto decorated = sup::templates::DecorateWith<MyDecorator>(std::make_unique<MyObject>());
