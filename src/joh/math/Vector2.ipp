//
// RS Game Framework
// Copyright © 2010 Jedd Haberstro
// jhaberstro@gmail.com
// 
// $Id:
//

#ifndef JOH_MATH_VECTOR2_IPP
#define JOH_MATH_VECTOR2_IPP

#ifndef JOH_MATH_VECTOR2_INCLUDE_GUARD
#   error "Don't include this file directly. Instead include 'joh/math/Vector2.hpp'"
#endif

#include "joh/Assert.hpp"

namespace joh
{
    namespace math
    {
        template< typename TReal > Vector2< TReal > const Vector2< TReal >::kZero(0.0, 0.0);            
        template< typename TReal > Vector2< TReal > const Vector2< TReal >::kOne(1.0, 1.0);
        template< typename TReal > Vector2< TReal > const Vector2< TReal >::kXAxis(1.0, 0.0);
        template< typename TReal > Vector2< TReal > const Vector2< TReal >::kYAxis(0.0, 1.0);
        
        template< typename TReal >
        inline Vector2< TReal > Vector2< TReal >::DotProduct(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs) {
            return (lhs.x * rhs.x) + (lhs.y * rhs.y);
        }
        
        template< typename TReal >
        inline Vector2< TReal > Vector2< TReal >::Distance(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs) {
            return (rhs - lhs).Length();
        }
        
        template< typename TReal >
        inline Vector2< TReal > Vector2< TReal >::Perpendicular(Vector2< TReal > const& vector) {
            return Vector2< TReal >(-vector.y, vector.x);
        }
        
        template< typename TReal >
        inline Vector2< TReal > Vector2< TReal >::SquaredDistance(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs) {
            return (rhs - lhs).SquaredLength();
        }
        
        template< typename TReal >
        inline Vector2< TReal >::Vector2()
        : x(TReal(0.0)),
          y(TReal(0.0)) {
        }
        
        template< typename TReal >
        inline Vector2< TReal >::Vector2(TReal x, TReal y)
        : x(x),
          y(y) {
        }
        
        template< typename TReal >
        inline Vector2< TReal >::Vector2(Vector2< TReal > const& other)
        : x(other.x),
          y(other.y) {
        }
        
        template< typename TReal >
        inline TReal& Vector2< TReal >::operator[](uint32_t index) {
            JOH_ASSERT(index < 2, "index is out of range");
            return *((&x) + index);
        }
        
        template< typename TReal >
        inline TReal Vector2< TReal >::operator[](uint32_t index) const {
            JOH_ASSERT(index < 2, "index is out of range");
            return *((&x) + index);
        }
        
        template< typename TReal >
        inline Vector2< TReal >& Vector2< TReal >::operator+=(Vector2< TReal > const& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        template< typename TReal >
        inline Vector2< TReal >& Vector2< TReal >::operator-=(Vector2< TReal > const& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }
        
        template< typename TReal >
        inline Vector2< TReal >& Vector2< TReal >::operator*=(TReal scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        
        template< typename TReal >
        Vector2< TReal >& Vector2< TReal >::operator/=(TReal scalar) {
            x /= scalar;
            y /= scalar;
            return *this;
        }
        
        template< typename TReal >
        inline Vector2< TReal > Vector2< TReal >::operator-() const {
            return Vector2< TReal >(-x, -y);
        }
        
        template< typename TReal >
        inline TReal Vector2< TReal >::Length() const {
            return Mathf::Sqrt((x*x) + (y*y));
        }
        
        template< typename TReal >
        inline TReal Vector2< TReal >::SquaredLength() const {
            return (x*x) + (y*y);
        }
        
        template< typename TReal >
        inline void Vector2< TReal >::Normalize() {
            TReal invLength = TReal(1.0) / Length();
            x *= invLength;
            y *= invLength;
        }
        
        template< typename TReal >
        inline Vector2< TReal > Vector2< TReal >::UnitVector() const {
            TReal invLength = TReal(1.0) / Length();
            return ThisType(
                x * invLength,
                y * invLength
            );
        }
        
        template< typename TReal >
        inline Vector2< TReal > operator+(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs) {
            return Vector2< TReal >(lhs.x + rhs.x, lhs.y + rhs.y);
        }
        
        template< typename TReal >
        inline Vector2< TReal > operator-(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs) {
            return Vector2< TReal >(lhs.x - rhs.x, lhs.y - rhs.y);
        }
        
        template< typename TReal >
        inline Vector2< TReal > operator*(Vector2< TReal > const& lhs, TReal rhs) {
            return Vector2< TReal >(lhs.x * rhs, lhs.y * rhs);
        }
        
        template< typename TReal >
        inline Vector2< TReal > operator/(Vector2< TReal > const& lhs, TReal rhs) {
            TReal invRhs = TReal(1.0) / rhs;
            return Vector2< TReal >(lhs.x * invRhs, lhs.y * invRhs);
        }
        
        template< typename TReal >
        inline Vector2< TReal > operator*(TReal lhs, Vector2< TReal > const& rhs) {
            return Vector2< TReal >(lhs.x * rhs.x, lhs.y * rhs.y);
        }
        
        template< typename TReal >
        inline Vector2< TReal > operator/(TReal lhs, Vector2< TReal > const& rhs) {
            return Vector2< TReal >(lhs.x / rhs.x, lhs.y / rhs.y);
        }
        
        template< typename TReal >
        inline bool operator==(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs) {
            return Mathf::ApproximatelyEqual(lhs.x, rhs.x, Mathf::kEpsilon)
                && Mathf::ApproximatelyEqual(lhs.y, rhs.y, Mathf::kEpsilon);
        }
        
        template< typename TReal >
        inline bool operator!=(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs) {
            return !(lhs == rhs);
        }
    }
}

#endif // JOH_MATH_VECTOR2_IPP
