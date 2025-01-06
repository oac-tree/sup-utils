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
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_TEMPLATES_DECORATE_WITH_H_
#define SUP_TEMPLATES_DECORATE_WITH_H_

#include <memory>

namespace sup
{
namespace templates
{

template <typename Decorator, typename T>
class OwningDecorator : public Decorator
{
public:
  OwningDecorator(std::unique_ptr<T> dep) : Decorator(*dep), m_dep{std::move(dep)}
  {}
private:
  std::unique_ptr<T> m_dep;
};

template <typename Decorator, typename T>
std::unique_ptr<T> DecorateWith(std::unique_ptr<T> dep)
{
  return std::unique_ptr<T>{new OwningDecorator<Decorator, T>{std::move(dep)}};
}

}  // namespace templates

}  // namespace sup

#endif  // SUP_TEMPLATES_DECORATE_WITH_H_
