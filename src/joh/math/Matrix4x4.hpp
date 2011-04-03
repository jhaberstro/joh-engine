//
// RS Game Framework
// Copyright © 2010 Jedd Haberstro
// jhaberstro@gmail.com
// 
// $Id:
//

#ifndef JOH_MATH_GENERICMATRIX4X4_HPP
#define JOH_MATH_GENERICMATRIX4X4_HPP

#include "joh/joh.hpp"
#include "joh/Math/Mathf.hpp"
#include "joh/Math/Vector3.hpp"
#include "joh/Math/Vector4.hpp"

namespace joh
{
    namespace math
    {
        template< typename TReal >
        class Matrix4x4
        {
        public:

            typedef Matrix4x4< TReal > ThisType;

        public:

            static Matrix4x4 OrthographicProjection(TReal width, TReal height, TReal near, TReal far);

            static Matrix4x4 PerspectiveProjection(TReal width, TReal height, TReal near, TReal far);

            static Matrix4x4 PerspectiveFieldOfViewProjection(TReal fieldOfView, TReal aspect, TReal near, TReal far);

            Matrix4x4();

            Matrix4x4(Vector4< TReal > const& x, Vector4< TReal > const& y, Vector4< TReal > const& z, Vector4< TReal > const& w);

            Matrix4x4 Cofactor() const;

            TReal Determinant(Matrix4x4 const& cofactors) const;

            Vector4< TReal > GetColumn(uint32_t index) const;

            TReal const* GetData() const;

            Vector4< TReal > const& GetRow(uint32_t index) const;

            Matrix4x4 Inverse() const;

            Matrix4x4& Invert();

            void SetColumn(uint32_t index, Vector4< TReal > const& column);

            void SetRow(uint32_t index, Vector4< TReal > const& row);

            Matrix4x4 Transpose() const;

            Vector4< TReal > TransformPoint(Vector4< TReal > const& point) const;

            Vector4< TReal > InverseTransformPoint(Vector4< TReal > const& point) const;

            Vector4< TReal > TransformDirection(Vector4< TReal > const& direction) const;

            Vector4< TReal > InverseTransformDirection(Vector4< TReal > const& direction) const;

            Vector3< TReal > TransformPoint(Vector3< TReal > const& point) const;

            Vector3< TReal > InverseTransformPoint(Vector3< TReal > const& point) const;

            Vector3< TReal > TransformDirection(Vector3< TReal > const& direction) const;

            Vector3< TReal > InverseTransformDirection(Vector3< TReal > const& direction) const;

            Vector4< TReal >& operator[](uint32_t index);

            Vector4< TReal > const& operator[](uint32_t index) const;

            Matrix4x4& operator+=(Matrix4x4 const& matrix);

            Matrix4x4& operator-=(Matrix4x4 const& matrix);

            Matrix4x4& operator*=(Matrix4x4 const& matrix);

            Matrix4x4& operator*=(TReal scalar);

            Matrix4x4& operator/=(TReal scalar);

        public:
            
            #if 0
            union
            {
                float m[16];
                struct
                {
                    Vector4< TReal > column0;
                    Vector4< TReal > column1;
                    Vector4< TReal > column2;
                    Vector4< TReal > column3;
                };
            };
            #endif
            
            Vector4< TReal > column0;
            Vector4< TReal > column1;
            Vector4< TReal > column2;
            Vector4< TReal > column3;
        };
        
        typedef Matrix4x4< float > Matrix4x4f;
    }
}

#define JOH_MATH_MATRIX4X4_INCLUDE_GUARD
#include "joh/math/Matrix4x4.ipp"
#undef JOH_MATH_MATRIX4X4_INCLUDE_GUARD

#endif // JOH_MATH_GENERICMATRIX4X4_HPP
