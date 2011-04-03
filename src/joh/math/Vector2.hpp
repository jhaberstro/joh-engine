//
// RS Game Framework
// Copyright © 2010 Jedd Haberstro
// jhaberstro@gmail.com
// 
// $Id:
//

#ifndef JOH_MATH_GENERICVECTOR2_HPP
#define JOH_MATH_GENERICVECTOR2_HPP

#include "joh/joh.hpp"
#include "joh/Math/Mathf.hpp"

namespace joh
{
    namespace math
    {
        template< typename TReal >
        class Vector2
        {
        public:
            
            typedef Vector2< TReal > ThisType;
            
        public:
            
            static Vector2 const kZero;
            static Vector2 const kOne;
            static Vector2 const kXAxis;
            static Vector2 const kYAxis;
            
            static Vector2 DotProduct(Vector2 const& lhs, Vector2 const& rhs);
            
            static Vector2 Distance(Vector2 const& lhs, Vector2 const& rhs);
            
            static Vector2 Perpendicular(Vector2 const& vector);
            
            static Vector2 SquaredDistance(Vector2 const& lhs, Vector2 const& rhs);
            
            Vector2();
            
            Vector2(TReal x, TReal y);
            
            Vector2(Vector2 const& other);
            
            TReal& operator[](uint32_t index);
            
            TReal operator[](uint32_t index) const;
            
            Vector2& operator+=(Vector2 const& other);

            Vector2& operator-=(Vector2 const& other);
            
            Vector2& operator*=(TReal scalar);
            
            Vector2& operator/=(TReal scalar);
            
            Vector2 operator-() const;
            
            TReal Length() const;
            
            TReal SquaredLength() const;
            
            void Normalize();
            
            Vector2 UnitVector() const;
            
        public:
            
            TReal x, y;
        };
        
        
        template< typename TReal >
        Vector2< TReal > operator+(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs);
        
        template< typename TReal >
        Vector2< TReal > operator-(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs);
        
        template< typename TReal >
        Vector2< TReal > operator*(Vector2< TReal > const& lhs, TReal rhs);
        
        template< typename TReal >
        Vector2< TReal > operator/(Vector2< TReal > const& lhs, TReal rhs);
        
        template< typename TReal >
        Vector2< TReal > operator*(TReal lhs, Vector2< TReal > const& rhs);
        
        template< typename TReal >
        Vector2< TReal > operator/(TReal lhs, Vector2< TReal > const& rhs);
        
        template< typename TReal >
        bool operator==(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs);
        
        template< typename TReal >
        bool operator!=(Vector2< TReal > const& lhs, Vector2< TReal > const& rhs);
        
        typedef Vector2< float > Vector2f;
    }
}

#define JOH_MATH_VECTOR2_INCLUDE_GUARD
#include "joh/math/Vector2.ipp"
#undef JOH_MATH_VECTOR2_INCLUDE_GUARD

#endif // JOH_MATH_GENERICVECTOR2_HPP
