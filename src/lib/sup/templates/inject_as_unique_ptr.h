/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP templates
 *
 * Author        : Walter Van Herck (IO)
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

#ifndef SUP_TEMPLATES_INJECT_AS_UNIQUE_PTR_H_
#define SUP_TEMPLATES_INJECT_AS_UNIQUE_PTR_H_

#include <sup/templates/decorate_with.h>

namespace sup
{
namespace templates
{

/**
 * @brief Function template that allows to inject a service into a client as a unique_ptr, when
 * the client expects injection by reference. The returned unique_ptr will also manage the
 * lifetime of the injected service.
 *
 * @tparam Client Client type.
 * @tparam Service Service type.
 * @tparam Args Optional extra argument types for the Client ctor.
 * @param dep Unique pointer to a Service object.
 * @param args Optional extra arguments for the Client ctor.
 * @return Unique pointer to a Client that also manages the lifetime of the injected Service.
 */
template <typename Client, typename Service, typename... Args>
std::unique_ptr<Client> InjectAsUniquePtr(std::unique_ptr<Service> dep, Args&&... args)
{
  return std::unique_ptr<Client>{
    new OwningDecorator<Client, Service, Args...>(std::move(dep), std::forward<Args>(args)...)};
}

}  // namespace templates

}  // namespace sup

#endif  // SUP_TEMPLATES_INJECT_AS_UNIQUE_PTR_H_
