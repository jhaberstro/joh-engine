//
// RS Game Framework
// Copyright © 2010 Jedd Haberstro
// jhaberstro@gmail.com
// 
// $Id:
//

#ifndef JOH_MATH_GENERICVECTOR4_HPP
#define JOH_MATH_GENERICVECTOR4_HPP

#include "joh/joh.hpp"
#include "joh/Math/Mathf.hpp"

namespace joh
{
    namespace math
    {
        template< typename TReal >
        class Vector4
        {
        public:            
            typedef Vector4< TReal > ThisType;
            
        public:
            
            static Vector4 const kZero;            
            static Vector4 const kOne;
            static Vector4 const kXAxis;
            static Vector4 const kYAxis;
            static Vector4 const kZAxis;
            static Vector4 const kWAxis;
            
        public:
                        
            static TReal DotProduct(Vector4 const& lhs, Vector4 const& rhs);
                        
            static TReal Distance(Vector4 const& lhs, Vector4 const& rhs);

            static Vector4 Lerp(Vector4 const& lhs, Vector4 const& rhs, TReal t);            
            
            static TReal SquaredDistance(Vector4 const& lhs, Vector4 const& rhs);
            
            Vector4();
            
            Vector4(TReal x, TReal y, TReal z, TReal w);
            
            Vector4(Vector4 const& other);
            
            TReal& operator[](uint32_t index);
            
            TReal operator[](uint32_t index) const;
                        
            Vector4& operator+=(Vector4 const& other);

            Vector4& operator-=(Vector4 const& other);
            
            Vector4& operator*=(TReal scalar);
            
            Vector4& operator/=(TReal scalar);
            
            Vector4 operator-() const;
            
            TReal Length() const;
            
            TReal SquaredLength() const;
            
            void Normalize();
            
            Vector4 UnitVector() const;
            
        public:
            
            TReal x, y, z, w;
        };
        
        template< typename TReal >
        Vector4< TReal > operator+(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs);
        
        template< typename TReal >
        Vector4< TReal > operator-(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs);
        
        template< typename TReal >
        Vector4< TReal > operator*(Vector4< TReal > const& lhs, TReal rhs);
        
        template< typename TReal >
        Vector4< TReal > operator/(Vector4< TReal > const& lhs, TReal rhs);
        
        template< typename TReal >
        Vector4< TReal > operator*(TReal lhs, Vector4< TReal > const& rhs);
        
        template< typename TReal >
        Vector4< TReal > operator/(TReal lhs, Vector4< TReal > const& rhs);
        
        template< typename TReal >
        bool operator==(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs);
        
        template< typename TReal >
        bool operator!=(Vector4< TReal > const& lhs, Vector4< TReal > const& rhs);
        
        typedef Vector4< float > Vector4f;
    }
}

#define JOH_MATH_VECTOR4_INCLUDE_GUARD
#include "joh/math/Vector4.ipp"
#undef JOH_MATH_VECTOR4_INCLUDE_GUARD

#if 0
#ifdef JOH_SIMD == JOH_SIMD_SSE
#   define JOH_MATH_SSEVECTOR3_INCLUDE_GUARD
#   include "joh/math/SseVector4.ipp"
#   undef JOH_MATH_SSEVECTOR3_INCLUDE_GUARD
#endif
#endif

#endif // JOH_MATH_GENERICVECTOR4_HPP
