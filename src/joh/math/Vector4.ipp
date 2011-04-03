//
// RS Game Framework
// Copyright © 2010 Jedd Haberstro
// jhaberstro@gmail.com
// 
// $Id:
//

#ifndef JOH_MATH_VECTOR4_IPP
#define JOH_MATH_VECTOR4_IPP

#ifndef JOH_MATH_VECTOR4_INCLUDE_GUARD
#   error "Don't include this file directly. Instead include 'joh/math/Vector4.hpp'"
#endif

namespace joh
{
    namespace math
    {
        template< typename TReal > Vector4< TReal > const Vector4< TReal >::kZero (0.0, 0.0, 0.0, 0.0);            
        template< typename TReal > Vector4< TReal > const Vector4< TReal >::kOne  (1.0, 1.0, 1.0, 1.0);
        template< typename TReal > Vector4< TReal > const Vector4< TReal >::kXAxis(1.0, 0.0, 0.0, 0.0);
        template< typename TReal > Vector4< TReal > const Vector4< TReal >::kYAxis(0.0, 1.0, 0.0, 0.0);
        template< typename TReal > Vector4< TReal > const Vector4< TReal >::kZAxis(0.0, 0.0, 1.0, 0.0);
        template< typename TReal > Vector4< TReal > const Vector4< TReal >::kWAxis(0.0, 0.0, 0.0, 1.0);

        template< typename TReal >
        inline TReal Vector4< TReal >::DotProduct(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs) {
            return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w);
        }
                    
        template< typename TReal >
        inline TReal Vector4< TReal >::Distance(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs) {
            return (lhs - rhs).Length();
        }

        template< typename TReal >
        inline Vector4< TReal > Vector4< TReal >::Lerp(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs, TReal t) {
            return lhs + ((rhs - lhs) * Mathf::Clamp(t));
        }
        
        template< typename TReal >
        inline TReal Vector4< TReal >::SquaredDistance(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs) {
            return (lhs - rhs).SquaredLength();
        }
        
        template< typename TReal >
        inline Vector4< TReal >::Vector4()
        : x(0.0),
          y(0.0),
          z(0.0),
          w(0.0) {
        }
        
        template< typename TReal >
        inline Vector4< TReal >::Vector4(TReal x, TReal y, TReal z, TReal w)
        : x(x),
          y(y),
          z(z),
          w(w) {
        }
        
        template< typename TReal >
        inline Vector4< TReal >::Vector4(Vector4< TReal > const& other)
        : x(other.x),
          y(other.y),
          z(other.z),
          w(other.w) {
        }
        
        template< typename TReal >
        inline TReal& Vector4< TReal >::operator[](uint32_t index) {
            JOH_ASSERT(index < 4, "index is out of range");
            return *((&x) + index);
        }
        
        template< typename TReal >
        inline TReal Vector4< TReal >::operator[](uint32_t index) const {
            JOH_ASSERT(index < 4, "index is out of range");
            return *((&x) + index);
        }
        
        template< typename TReal >
        inline Vector4< TReal >& Vector4< TReal >::operator+=(Vector4< TReal > const& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        template< typename TReal >
        inline Vector4< TReal >& Vector4< TReal >::operator-=(Vector4< TReal > const& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }
        
        template< typename TReal >
        inline Vector4< TReal >& Vector4< TReal >::operator*=(TReal scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }
        
        template< typename TReal >
        inline Vector4< TReal >& Vector4< TReal >::operator/=(TReal scalar) {
            TReal invScalar = TReal(1.0) / scalar;
            x *= invScalar;
            y *= invScalar;
            z *= invScalar;
            w *= invScalar;
            return *this;
        }
        
        template< typename TReal >
        inline Vector4< TReal> Vector4< TReal >::operator-() const {
            return Vector4< TReal >(-x, -y, -z, -w);
        }
        
        
        template< typename TReal >
        inline TReal Vector4< TReal >::Length() const {
            return Mathf::Sqrt((x*x) + (y*y) + (z*z) + (w*w));
        }
        
        template< typename TReal >
        inline TReal Vector4< TReal >::SquaredLength() const {
            return (x*x) + (y*y) + (z*z) + (w*w);
        }
        
        template< typename TReal >
        inline void Vector4< TReal >::Normalize() {
            TReal invLength = TReal(1.0) / Length();
            x *= invLength;
            y *= invLength;
            z *= invLength;
            w *= invLength;
        }
        
        template< typename TReal >
        inline Vector4< TReal > Vector4< TReal >::UnitVector() const {
            TReal invLength = TReal(1.0) / Length();
            return ThisType(
                x * invLength,
                y * invLength,
                z * invLength,
                w * invLength
            );
        }
        
        template< typename TReal >
        inline Vector4< TReal > operator+(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs) {
            return Vector4< TReal >(
                lhs.x + rhs.x,
                lhs.y + rhs.y,
                lhs.z + rhs.z,
                lhs.w + rhs.w
            );
        }
        
        template< typename TReal >
        inline Vector4< TReal > operator-(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs) {
            return Vector4< TReal >(
                lhs.x - rhs.x,
                lhs.y - rhs.y,
                lhs.z - rhs.z,
                lhs.w - rhs.w
            );
        }
        
        template< typename TReal >
        inline Vector4< TReal > operator*(Vector4< TReal > const& lhs, TReal rhs) {
            return Vector4< TReal >(
                lhs.x * rhs,
                lhs.y * rhs,
                lhs.z * rhs,
                lhs.w * rhs
            );
        }
        
        template< typename TReal >
        inline Vector4< TReal > operator/(Vector4< TReal > const& lhs, TReal rhs) {
            TReal invRhs = TReal(1.0) / rhs;
            return Vector4< TReal >(
                lhs.x * invRhs,
                lhs.y * invRhs,
                lhs.z * invRhs,
                lhs.w * invRhs
            );
        }
        
        template< typename TReal >
        inline Vector4< TReal > operator*(TReal lhs, Vector4< TReal > const& rhs) {
            return Vector4< TReal >(
                rhs.x * lhs,
                rhs.y * lhs,
                rhs.z * lhs,
                rhs.w * lhs
            );
        }
        
        template< typename TReal >
        inline Vector4< TReal > operator/(TReal lhs, Vector4< TReal > const& rhs) {
            TReal invLhs = TReal(1.0) / lhs;
            return Vector4< TReal >(
                rhs.x * invLhs,
                rhs.y * invLhs,
                rhs.z * invLhs,
                rhs.w * invLhs
            );
        }
        
        template< typename TReal >
        inline bool operator==(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs) {
            return Mathf::ApproximatelyEqual(lhs.x, rhs.x, Mathf::kEpsilon)
                && Mathf::ApproximatelyEqual(lhs.y, rhs.y, Mathf::kEpsilon)
                && Mathf::ApproximatelyEqual(lhs.z, rhs.z, Mathf::kEpsilon)
                && Mathf::ApproximatelyEqual(lhs.w, rhs.w, Mathf::kEpsilon);
        }
        
        template< typename TReal >
        inline bool operator!=(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs) {
            return !(lhs == rhs);
        }
        
    }
}

#endif // JOH_MATH_VECTOR4_IPP
