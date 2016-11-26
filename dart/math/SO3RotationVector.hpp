/*
 * Copyright (c) 2016, Graphics Lab, Georgia Tech Research Corporation
 * Copyright (c) 2016, Humanoid Lab, Georgia Tech Research Corporation
 * Copyright (c) 2016, Personal Robotics Lab, Carnegie Mellon University
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

#ifndef DART_MATH_SO3TROTATIONVECTOR_HPP_
#define DART_MATH_SO3TROTATIONVECTOR_HPP_

#include <Eigen/Eigen>

#include "dart/math/MathTypes.hpp"
#include "dart/math/Geometry.hpp"
#include "dart/math/SO3Base.hpp"

namespace dart {
namespace math {

template <typename S_>
class SO3RotationVector : public SO3Base<SO3RotationVector<S_>>
{
public:

  using This = SO3RotationVector<S_>;
  using Base = SO3Base<This>;
  using S = typename Base::S;

  using RotationMatrix = typename Base::RotationMatrix;
  using RotationVector = typename Base::RotationVector;

  using RepData = typename Base::RepData;
  using Tangent = typename Base::Tangent;
  using so3 = typename Base::so3;

  using Base::operator =;
  using Base::operator *;
  using Base::operator *=;

//  using Base::getCoordinates;
  using Base::setRepData;
  using Base::getRepData;

  using Base::Exp;
  using Base::setExp;
  using Base::Log;
  using Base::getLog;

  /// \{ \name Constructors

  /// Default constructor. By default, the constructed SO(3) is not identity.
  SO3RotationVector() : Base()
  {
    // Do nothing
  }

  /// Copy constructor.
  SO3RotationVector(const SO3RotationVector& other) : Base(), mRepData(other.mRepData)
  {
    // Do nothing
  }

  /// Move constructor.
  SO3RotationVector(SO3RotationVector&& other) : mRepData(std::move(other.mRepData))
  {
    // Do nothing
  }

  /// Construct from other SO3 with different representation.
  template <typename Derived>
  SO3RotationVector(const SO3Base<Derived>& other)
    : Base(),
      mRepData(detail::so3_operations::so3_convert_impl<S, Derived, This>::run(
              other.getRepData()))
  {
    // Do nothing
  }

  /// Construct from other SO3 with different representation.
  template <typename Derived>
  SO3RotationVector(SO3Base<Derived>&& other)
    : Base(),
      mRepData(detail::so3_operations::so3_convert_impl<S, Derived, This>::run(
              std::move(other.getRepData())))
  {
    // Do nothing
  }

  /// Construct from a raw rotation vector where the dimension is 3x1.
  template <typename Derived>
  SO3RotationVector(const Eigen::MatrixBase<Derived>& matrix) : Base(), mRepData(matrix)
  {
    assert(matrix.rows() == 3);
    assert(matrix.cols() == 1);
  }

  /// Construct from a raw rotation matrix where the dimension is 3x3.
  template <typename Derived>
  SO3RotationVector(Eigen::MatrixBase<Derived>&& matrix) : Base(), mRepData(std::move(matrix))
  {
    assert(matrix.rows() == 3);
    assert(matrix.cols() == 1);
  }

  /// \} // Constructors

  /// \{ \name Operators

  /// Assign a SO3 with the same representation.
  SO3RotationVector& operator=(const SO3RotationVector& other)
  {
    mRepData = other.mRepData;
    return *this;
  }

  /// Move in a SO3 with the same representation.
  SO3RotationVector& operator=(SO3RotationVector&& other)
  {
    mRepData = std::move(other.mRepData);
    return *this;
  }

  SO3RotationVector& operator=(const Eigen::AngleAxis<S>& quat)
  {
    mRepData = detail::so3_operations::so3_convert_impl<S, SO3AngleAxis<S>, SO3RotationVector>::run(quat);
    // TODO(JS): improve; need a way to deduce representation type from Eigen
    // data type
    return *this;
  }

  SO3RotationVector& operator=(Eigen::AngleAxis<S>&& quat)
  {
    mRepData = detail::so3_operations::so3_convert_impl<S, SO3AngleAxis<S>, SO3RotationVector>::run(
          std::move(quat));
    // TODO(JS): improve; need a way to deduce representation type from Eigen
    // data type
    return *this;
  }

  template <typename QuatDerived>
  SO3RotationVector& operator=(const Eigen::QuaternionBase<QuatDerived>& quat)
  {
    mRepData = detail::so3_operations::so3_convert_impl<S, SO3Quaternion<S>, SO3RotationVector>::run(quat);
    // TODO(JS): improve; need a way to deduce representation type from Eigen
    // data type
    return *this;
  }

  template <typename QuatDerived>
  SO3RotationVector& operator=(Eigen::QuaternionBase<QuatDerived>&& quat)
  {
    mRepData = detail::so3_operations::so3_convert_impl<
        S, SO3Quaternion<S>, SO3RotationVector>::run(
          std::move(quat));
    // TODO(JS): improve; need a way to deduce representation type from Eigen
    // data type
    return *this;
  }

  template <typename Derived>
  SO3RotationVector& operator=(const Eigen::MatrixBase<Derived>& matrix)
  {
    mRepData = detail::so3_operations::so3_convert_impl<
        S, SO3RotationMatrix<S>, SO3RotationVector>::run(
          matrix);
    return *this;
  }
  // TODO(JS): take rotation vector as well using a template struct

  template <typename Derived>
  SO3RotationVector& operator=(Eigen::MatrixBase<Derived>&& matrix)
  {
    mRepData = detail::so3_operations::so3_convert_impl<
        S, SO3RotationMatrix<S>, SO3RotationVector>::run(
          std::move(matrix));
    return *this;
  }

  /// Whether \b exactly equal to a SO3.
  bool operator ==(const SO3RotationVector& other)
  {
    return mRepData == other.mRepData;
  }

  /// \} // Operators

  /// \{ \name Representation properties

  template <typename MatrixDerived>
  void fromRotationVector(const Eigen::MatrixBase<MatrixDerived>& vector)
  {
    assert(vector.rows() == 3);
    assert(vector.cols() == 1);

    mRepData = vector;
  }

  template <typename MatrixDerived>
  void fromRotationVector(Eigen::MatrixBase<MatrixDerived>&& vector)
  {
    assert(vector.rows() == 3);
    assert(vector.cols() == 1);

    mRepData = std::move(vector);
  }

  Eigen::Matrix<S, 3, 1> toRotationVector() const
  {
    return mRepData;
  }

  void setRotationVector(const RotationVector& axisAngle)
  {
    mRepData = axisAngle;
  }

  const RotationVector& getRotationVector() const
  {
    return mRepData;
  }

  void setRandom()
  {
    mRepData.setRandom();
  }

  /// \} // Representation properties

  /// \{ \name SO3 group operations

  void setIdentity()
  {
    mRepData.setZero();
  }

  bool isIdentity()
  {
    return mRepData == RepData::Zero();
  }

  void invert()
  {
    mRepData *= static_cast<S>(-1);
  }

  const SO3RotationVector getInverse() const
  {
    return SO3RotationVector(-mRepData);
  }

  /// \} // SO3 group operations

protected:
  template <typename>
  friend class SO3Base;

  RepData mRepData{RepData()};
};

using SO3RotationVectorf = SO3RotationVector<float>;
using SO3RotationVectord = SO3RotationVector<double>;

extern template
class SO3RotationVector<double>;

} // namespace math
} // namespace dart

#endif // DART_MATH_SO3TROTATIONVECTOR_HPP_
