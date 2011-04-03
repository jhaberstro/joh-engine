//
// RS Game Framework
// Copyright © 2010 Jedd Haberstro
// jhaberstro@gmail.com
// 
// $Id:
//

#ifndef JOH_MATH_GENERICVECTOR3_HPP
#define JOH_MATH_GENERICVECTOR3_HPP

#include "joh/joh.hpp"
#include "joh/Math/Mathf.hpp"

namespace joh
{
    namespace math
    {
        template< typename TReal >
        class Vector3
        {
        public:            
            typedef Vector3< TReal > ThisType;
            
        public:
            
            static Vector3 const kZero;            
            static Vector3 const kOne;
            static Vector3 const kXAxis;
            static Vector3 const kYAxis;
            static Vector3 const kZAxis;
            
            static Vector3 CrossProduct(Vector3 const& lhs, Vector3 const& rhs);
            
            static TReal DotProduct(Vector3 const& lhs, Vector3 const& rhs);
                        
            static TReal Distance(Vector3 const& lhs, Vector3 const& rhs);

            static Vector3 Lerp(Vector3 const& lhs, Vector3 const& rhs, TReal t);
            
            static Vector3 Project(Vector3 const& vector, Vector3 const& onVector);
            
            static Vector3 Reflect(Vector3 const& incomingDirection, Vector3 const& surfaceNormal);
            
            static TReal SquaredDistance(Vector3 const& lhs, Vector3 const& rhs);
            
            Vector3();
            
            Vector3(TReal x, TReal y, TReal z);
            
            Vector3(Vector3 const& other);
            
            TReal& operator[](uint32_t index);
            
            TReal operator[](uint32_t index) const;
                        
            Vector3& operator+=(Vector3 const& other);

            Vector3& operator-=(Vector3 const& other);
            
            Vector3& operator*=(TReal scalar);
            
            Vector3& operator/=(TReal scalar);
            
            Vector3 operator-() const;
            
            TReal Length() const;
            
            TReal SquaredLength() const;
            
            void Normalize();
            
            Vector3 UnitVector() const;
            
        public:
            
            TReal x, y, z;
        };
        
        template< typename TReal >
        Vector3< TReal > operator+(Vector3< TReal > const& lhs, Vector3< TReal > const& rhs);
        
        template< typename TReal >
        Vector3< TReal > operator-(Vector3< TReal > const& lhs, Vector3< TReal > const& rhs);
        
        template< typename TReal >
        Vector3< TReal > operator*(Vector3< TReal > const& lhs, TReal rhs);
        
        template< typename TReal >
        Vector3< TReal > operator/(Vector3< TReal > const& lhs, TReal rhs);
        
        template< typename TReal >
        Vector3< TReal > operator*(TReal lhs, Vector3< TReal > const& rhs);
        
        template< typename TReal >
        Vector3< TReal > operator/(TReal lhs, Vector3< TReal > const& rhs);
        
        template< typename TReal >
        bool operator==(Vector3< TReal > const& lhs, Vector3< TReal > const& rhs);
        
        template< typename TReal >
        bool operator!=(Vector3< TReal > const& lhs, Vector3< TReal > const& rhs);
        
        typedef Vector3< float > Vector3f;
    }
}

#define JOH_MATH_VECTOR3_INCLUDE_GUARD
#include "joh/math/Vector3.ipp"
#undef JOH_MATH_VECTOR3_INCLUDE_GUARD

#if 0
#ifdef JOH_SIMD == JOH_SIMD_SSE
#   define JOH_MATH_SSEVECTOR3_INCLUDE_GUARD
#   include "joh/math/SseVector3.ipp"
#   undef JOH_MATH_SSEVECTOR3_INCLUDE_GUARD
#endif
#endif

#endif // JOH_MATH_GENERICVECTOR3_HPP
