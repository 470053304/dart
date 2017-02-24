/*
 * Copyright (c) 2017, Graphics Lab, Georgia Tech Research Corporation
 * Copyright (c) 2017, Personal Robotics Lab, Carnegie Mellon University
 * All rights reserved.
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DART_COMMON_SFINAE_HPP_
#define DART_COMMON_SFINAE_HPP_

#include <type_traits>

namespace dart {
namespace common {

//==============================================================================
// HasCopyConstructor
//==============================================================================

template <typename A, typename B>
class HasCopyConstructor
{
protected:
  template <typename C, typename D>
  static constexpr auto check(C c, D d) -> decltype(C(d), std::true_type());
  static constexpr auto check(...) -> decltype(std::false_type());

public:
  static constexpr bool value
    = decltype( check( std::declval<A>(), std::declval<B>() ) )::value;
};

//==============================================================================
// HasAssignmentOperator
//==============================================================================

template <typename A, typename B>
class HasAssignmentOperator
{
protected:
  template <typename C, typename D>
  static constexpr auto check(C c, D d) -> decltype(c = d, std::true_type());
  static constexpr auto check(...) -> decltype(std::false_type());

public:
  static constexpr bool value
    = decltype( check( std::declval<A>(), std::declval<B>() ) )::value;
};

//==============================================================================
// HasMoveAssignmentOperator
//==============================================================================

template <typename A, typename B>
class HasMoveAssignmentOperator
{
protected:
  template <typename C, typename D>
  static constexpr auto check(C c, D d) -> decltype(c = std::move(d), std::true_type());
  static constexpr auto check(...) -> decltype(std::false_type());

public:
  static constexpr bool value
    = decltype( check( std::declval<A>(), std::declval<B>() ) )::value;
};

//==============================================================================
// HasMuliplicationOperator
//==============================================================================

template <typename A, typename B>
class HasMuliplicationOperator
{
protected:
  template <typename C, typename D>
  static constexpr auto check(C c, D d) -> decltype(c * d, std::true_type());
  static constexpr auto check(...) -> decltype(std::false_type());

public:
  static constexpr bool value
    = decltype( check( std::declval<A>(), std::declval<B>() ) )::value;
};

} // namespace common
} // namespace dart

#endif // DART_COMMON_SFINAE_HPP_
