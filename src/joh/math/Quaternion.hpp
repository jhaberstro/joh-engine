//
// RS Game Framework
// Copyright © 2010 Jedd Haberstro
// jhaberstro@gmail.com
// 
// $Id:
//

#ifndef JOH_MATH_QUATERNION_HPP
#define JOH_MATH_QUATERNION_HPP

#include "joh/joh.hpp"
#include "joh/Math/Mathf.hpp"
#include "joh/Math/Vector3.hpp"

namespace joh
{
    namespace math
    {
        // TODO: ADD IN MATRIX INTEROPABILITY
        
        template< typename TReal >
        class Quaternion
        {
        public:
            
            typedef Quaternion< TReal > ThisType;
            
        public:
            
            static Quaternion Slerp(Quaternion const& start, Quaternion const& finish, TReal delta);

            static Quaternion Lerp(Quaternion const& start, Quaternion const& finish, TReal delta);
            
            static TReal DotProduct(Quaternion const& lhs, Quaternion const& rhs);
                        
            Quaternion(TReal x, TReal y, TReal z, TReal w);
            
            Quaternion(Vector3< TReal > const& axis, TReal radianAngle);
            
            Quaternion(TReal eulerAngleX, TReal eulerAngleY, TReal eulerAngleZ);
            
            //Quaternion(Matrix3x3< TReal > matrix);
            
            Quaternion(Quaternion< TReal > const& other);
            
            TReal& operator[](uint32_t index);
            
            TReal operator[](uint32_t index) const;
            
            Quaternion& operator+=(Quaternion const& right);

            Quaternion& operator-=(Quaternion const& right);

            Quaternion& operator*=(Quaternion const& right);
            
            Quaternion& operator*=(TReal right);

            Quaternion& operator/=(TReal right);
            
            Quaternion operator-() const;
            
            Quaternion Inverse() const;
            
            TReal Length() const;
            
            TReal SquaredLength() const;
            
            void Normalize();
            
            Quaternion UnitQuaternion() const;
            
            Vector3< TReal > Transform(Vector3< TReal > const& vector) const;

            Vector3< TReal > InverseTransform(Vector3< TReal > const& vector) const;
            
            void ToAxisAngle(Vector3< TReal >& outVector, TReal& outAngle) const;
            
            void FromAxisAngle(Vector3< TReal > const& vector, TReal angle);
            
            void ToEulerAngles(TReal& x, TReal& y, TReal& z) const;
            
            void FromEulerAngles(TReal x, TReal y, TReal z);
            
            // EXPERIMENTAL // 
            Vector3< TReal > GetForwardDirection() const; // Vector3(0, 0, 1) * (*this)
            
            Vector3< TReal > GetUpDirection() const; // Vector3(0, 1, 0) * (*this)
            
            Vector3< TReal > GetRightDirection() const; // Vector3(1, 0, 0) * (*this)
            
        public:
            
            TReal x, y, z, w;
        };
        
        template< typename TReal >
        Quaternion< TReal > operator+(Quaternion< TReal > const& lhs, Quaternion< TReal > const& rhs);
        
        template< typename TReal >
        Quaternion< TReal > operator-(Quaternion< TReal > const& lhs, Quaternion< TReal > const& rhs);
        
        template< typename TReal >
        Quaternion< TReal > operator*(Quaternion< TReal > const& lhs, Quaternion< TReal > const& rhs);
        
        template< typename TReal >
        Quaternion< TReal > operator*(Quaternion< TReal > const& lhs, TReal rhs);
        
        template< typename TReal >
        Quaternion< TReal > operator*(TReal lhs, Quaternion< TReal > const& rhs);
        
        template< typename TReal >
        Quaternion< TReal > operator/(Quaternion< TReal > const& lhs, TReal const& rhs);
        
        template< typename TReal >
        bool operator==(Quaternion< TReal > const& lhs, Quaternion< TReal > const& rhs);
        
        template< typename TReal >
        bool operator!=(Quaternion< TReal > const& lhs, Quaternion< TReal > const& rhs);
    }
}

#define JOH_MATH_QUATERNION_INCLUDE_GUARD
#include "joh/Math/Quaternion.ipp"
#undef JOH_MATH_QUATERNION_INCLUDE_GUARD

#endif // JOH_MATH_QUATERNION_HPP
