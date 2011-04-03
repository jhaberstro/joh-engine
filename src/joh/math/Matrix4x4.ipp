//
// RS Game Framework
// Copyright © 2010 Jedd Haberstro
// jhaberstro@gmail.com
// 
// $Id:
//

#ifndef JOH_MATH_MATRIX4X4_IPP
#define JOH_MATH_MATRIX4X4_IPP

#ifndef JOH_MATH_MATRIX4X4_INCLUDE_GUARD
#   error "Don't include this file directly. Instead include 'joh/math/Matrix4x4.hpp'"
#endif

namespace joh
{
    namespace math
    {
        namespace internal
        {
            template< typename TReal >
            Vector4< float > CofactorColumn(Matrix4x4< TReal > const& mat, uint32_t column) {
                typedef union
                {
                    float f;
                    int32_t i;
                } signadj;
                
                static uint32_t const sel0[] = { 1, 0, 0, 0 };
                static uint32_t const sel1[] = { 2, 2, 1, 1 };
                static uint32_t const sel2[] = { 3, 3, 3, 2 };

                uint32_t const col0 = sel0[column];
                uint32_t const col1 = sel1[column];
                uint32_t const col2 = sel2[column];
                
                static uint32_t const znzn[] = { 0x00000000, 0x80000000, 0x00000000, 0x80000000 };
                static uint32_t const nznz[] = { 0x80000000, 0x00000000, 0x80000000, 0x00000000 };
                
                // Computer the det of the 3x3 matrix:
                // 
                //   [ a b c ]
                //   [ d e f ] = aei - ahf + dhc - dbi + gbf - gec = (aei + dhc + gbf) - (ahf + dbi + gec)
                //   [ g h i ] 
                //

                // pos_part1 = mat.GetColumn(col0)[row0] * mat.GetColumn(col1)[row1] * mat.GetColumn(col2)[row2]; // aei
                TReal const	r0_pos_part1 = mat.GetColumn(col0)[1] * mat.GetColumn(col1)[2] * mat.GetColumn(col2)[3];
                TReal const	r1_pos_part1 = mat.GetColumn(col0)[2] * mat.GetColumn(col1)[3] * mat.GetColumn(col2)[0];
                TReal const	r2_pos_part1 = mat.GetColumn(col0)[3] * mat.GetColumn(col1)[0] * mat.GetColumn(col2)[1];
                TReal const	r3_pos_part1 = mat.GetColumn(col0)[0] * mat.GetColumn(col1)[1] * mat.GetColumn(col2)[2];

                // pos_part2 = mat.GetColumn(col0)[row1] * mat.GetColumn(col1)[row2] * mat.GetColumn(col2)[row0]; // dhc
                TReal const	r0_pos_part2 = mat.GetColumn(col0)[2] * mat.GetColumn(col1)[3] * mat.GetColumn(col2)[1];
                TReal const	r1_pos_part2 = mat.GetColumn(col0)[3] * mat.GetColumn(col1)[0] * mat.GetColumn(col2)[2];
                TReal const	r2_pos_part2 = mat.GetColumn(col0)[0] * mat.GetColumn(col1)[1] * mat.GetColumn(col2)[3];
                TReal const	r3_pos_part2 = mat.GetColumn(col0)[1] * mat.GetColumn(col1)[2] * mat.GetColumn(col2)[0];

                // pos_part3 = mat.GetColumn(col0)[row2] * mat.GetColumn(col1)[row0] * mat.GetColumn(col2)[row1]; // gbf
                TReal const	r0_pos_part3 = mat.GetColumn(col0)[3] * mat.GetColumn(col1)[1] * mat.GetColumn(col2)[2];
                TReal const	r1_pos_part3 = mat.GetColumn(col0)[0] * mat.GetColumn(col1)[2] * mat.GetColumn(col2)[3];
                TReal const	r2_pos_part3 = mat.GetColumn(col0)[1] * mat.GetColumn(col1)[3] * mat.GetColumn(col2)[0];
                TReal const	r3_pos_part3 = mat.GetColumn(col0)[2] * mat.GetColumn(col1)[0] * mat.GetColumn(col2)[1];

                // neg_part1 = mat.GetColumn(col0)[row0] * mat.GetColumn(col1)[row2] * mat.GetColumn(col2)[row1]; // ahf
                TReal const	r0_neg_part1 = mat.GetColumn(col0)[1] * mat.GetColumn(col1)[3] * mat.GetColumn(col2)[2];
                TReal const	r1_neg_part1 = mat.GetColumn(col0)[2] * mat.GetColumn(col1)[0] * mat.GetColumn(col2)[3];
                TReal const	r2_neg_part1 = mat.GetColumn(col0)[3] * mat.GetColumn(col1)[1] * mat.GetColumn(col2)[0];
                TReal const	r3_neg_part1 = mat.GetColumn(col0)[0] * mat.GetColumn(col1)[2] * mat.GetColumn(col2)[1];

                // neg_part2 = mat.GetColumn(col0)[row1] * mat.GetColumn(col1)[row0] * mat.GetColumn(col2)[row2]; // dbi
                TReal const	r0_neg_part2 = mat.GetColumn(col0)[2] * mat.GetColumn(col1)[1] * mat.GetColumn(col2)[3];
                TReal const	r1_neg_part2 = mat.GetColumn(col0)[3] * mat.GetColumn(col1)[2] * mat.GetColumn(col2)[0];
                TReal const	r2_neg_part2 = mat.GetColumn(col0)[0] * mat.GetColumn(col1)[3] * mat.GetColumn(col2)[1];
                TReal const	r3_neg_part2 = mat.GetColumn(col0)[1] * mat.GetColumn(col1)[0] * mat.GetColumn(col2)[2];

                // neg_part3 = mat.GetColumn(col0)[row2] * mat.GetColumn(col1)[row1] * mat.GetColumn(col2)[row0]; // gec
                TReal const	r0_neg_part3 = mat.GetColumn(col0)[3] * mat.GetColumn(col1)[2] * mat.GetColumn(col2)[1];
                TReal const	r1_neg_part3 = mat.GetColumn(col0)[0] * mat.GetColumn(col1)[3] * mat.GetColumn(col2)[2];
                TReal const	r2_neg_part3 = mat.GetColumn(col0)[1] * mat.GetColumn(col1)[0] * mat.GetColumn(col2)[3];
                TReal const	r3_neg_part3 = mat.GetColumn(col0)[2] * mat.GetColumn(col1)[1] * mat.GetColumn(col2)[0];

                // pos_part  = pos_part1 + pos_part2 + pos_part3;
                TReal const	r0_pos_part  = r0_pos_part1 + r0_pos_part2 + r0_pos_part3;
                TReal const	r1_pos_part  = r1_pos_part1 + r1_pos_part2 + r1_pos_part3;
                TReal const	r2_pos_part  = r2_pos_part1 + r2_pos_part2 + r2_pos_part3;
                TReal const	r3_pos_part  = r3_pos_part1 + r3_pos_part2 + r3_pos_part3;	

                // neg_part  = neg_part1 + neg_part2 + neg_part3;
                TReal const	r0_neg_part  = r0_neg_part1 + r0_neg_part2 + r0_neg_part3;
                TReal const	r1_neg_part  = r1_neg_part1 + r1_neg_part2 + r1_neg_part3;
                TReal const	r2_neg_part  = r2_neg_part1 + r2_neg_part2 + r2_neg_part3;
                TReal const	r3_neg_part  = r3_neg_part1 + r3_neg_part2 + r3_neg_part3;

                // det3x3	  = pos_part - neg_part;
                TReal const	r0_det3x3 = r0_pos_part - r0_neg_part;
                TReal const	r1_det3x3 = r1_pos_part - r1_neg_part;
                TReal const	r2_det3x3 = r2_pos_part - r2_neg_part;
                TReal const	r3_det3x3 = r3_pos_part - r3_neg_part;
                
                // Now let's adjust the sign of the cofactor:	
                signadj r0_cofactor;
                signadj r1_cofactor;
                signadj r2_cofactor;
                signadj r3_cofactor;

                // First we choose the mask depending on the column:
                uint32_t const col_mask   = (uint32_t const)(((int32_t const)((column & 1) << 31)) >> 31);
                uint32_t const u_znzn     = (uint32_t const)(&znzn[0]);
                uint32_t const u_nznz     = (uint32_t const)(&nznz[0]);
                union 
                {
                    uint32_t  u;
                    uint32_t* p;
                } mask;
                mask.u = (u_nznz & col_mask) | (u_znzn & ~col_mask);

                r0_cofactor.f  = static_cast< float >(r0_det3x3);
                r1_cofactor.f  = static_cast< float >(r1_det3x3);
                r2_cofactor.f  = static_cast< float >(r2_det3x3);
                r3_cofactor.f  = static_cast< float >(r3_det3x3);

                r0_cofactor.i ^= mask.p[0];
                r1_cofactor.i ^= mask.p[1];
                r2_cofactor.i ^= mask.p[2];
                r3_cofactor.i ^= mask.p[3];
                
                return Vector4< TReal >(r0_cofactor.f, r1_cofactor.f, r2_cofactor.f, r3_cofactor.f);
            }
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal > operator*(Matrix4x4< TReal > const& matrix, TReal scalar) {
            return Matrix4x4< TReal >(
                matrix.column0 * scalar, 
                matrix.column1 * scalar,
                matrix.column2 * scalar,
                matrix.column3 * scalar
            );
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal > operator*(TReal scalar, Matrix4x4< TReal > const& matrix) {
            return Matrix4x4< TReal >(
                matrix.column0 * scalar, 
                matrix.column1 * scalar,
                matrix.column2 * scalar,
                matrix.column3 * scalar
            );
        }
        
        template< typename TReal >
        Matrix4x4< TReal > operator*(Matrix4x4< TReal > const& matrix1, Matrix4x4< TReal > const& matrix2) {
            Matrix4x4< TReal > result;
            TReal const* lhs = matrix1.GetData();
            TReal const* rhs = matrix2.GetData();
            TReal* m = const_cast< TReal* >(result.GetData());
        	m[0]  = rhs[0] * lhs[0]  + rhs[4] * lhs[1]  + rhs[8]  * lhs[2]  + rhs[12] * lhs[3];
        	m[1]  = rhs[1] * lhs[0]  + rhs[5] * lhs[1]  + rhs[9]  * lhs[2]  + rhs[13] * lhs[3];
        	m[2]  = rhs[2] * lhs[0]  + rhs[6] * lhs[1]  + rhs[10] * lhs[2]  + rhs[14] * lhs[3];
        	m[3]  = rhs[3] * lhs[0]  + rhs[7] * lhs[1]  + rhs[11] * lhs[2]  + rhs[15] * lhs[3];
        	m[4]  = rhs[0] * lhs[4]  + rhs[4] * lhs[5]  + rhs[8]  * lhs[6]  + rhs[12] * lhs[7];
        	m[5]  = rhs[1] * lhs[4]  + rhs[5] * lhs[5]  + rhs[9]  * lhs[6]  + rhs[13] * lhs[7];
        	m[6]  = rhs[2] * lhs[4]  + rhs[6] * lhs[5]  + rhs[10] * lhs[6]  + rhs[14] * lhs[7];
        	m[7]  = rhs[3] * lhs[4]  + rhs[7] * lhs[5]  + rhs[11] * lhs[6]  + rhs[15] * lhs[7];
        	m[8]  = rhs[0] * lhs[8]  + rhs[4] * lhs[9]  + rhs[8]  * lhs[10] + rhs[12] * lhs[11];
        	m[9]  = rhs[1] * lhs[8]  + rhs[5] * lhs[9]  + rhs[9]  * lhs[10] + rhs[13] * lhs[11];
        	m[10] = rhs[2] * lhs[8]  + rhs[6] * lhs[9]  + rhs[10] * lhs[10] + rhs[14] * lhs[11];
        	m[11] = rhs[3] * lhs[8]  + rhs[7] * lhs[9]  + rhs[11] * lhs[10] + rhs[15] * lhs[11];
        	m[12] = rhs[0] * lhs[12] + rhs[4] * lhs[13] + rhs[8]  * lhs[14] + rhs[12] * lhs[15];
        	m[13] = rhs[1] * lhs[12] + rhs[5] * lhs[13] + rhs[9]  * lhs[14] + rhs[13] * lhs[15];
        	m[14] = rhs[2] * lhs[12] + rhs[6] * lhs[13] + rhs[10] * lhs[14] + rhs[14] * lhs[15];
        	m[15] = rhs[3] * lhs[12] + rhs[7] * lhs[13] + rhs[11] * lhs[14] + rhs[15] * lhs[15];
        	
            return result;
        }
        
        template< typename TReal >
        Matrix4x4< TReal > operator/(Matrix4x4< TReal > const& matrix, TReal scalar) {
            return Matrix4x4< TReal >(
                matrix.column0 / scalar, 
                matrix.column1 / scalar,
                matrix.column2 / scalar,
                matrix.column3 / scalar
            );
        }
        
        template< typename TReal >
        Matrix4x4< TReal > operator+(Matrix4x4< TReal > const& matrix1, Matrix4x4< TReal > const& matrix2) {
            return Matrix4x4< TReal >(
                matrix1.column0 + matrix2.column0, 
                matrix1.column1 + matrix2.column1,
                matrix1.column2 + matrix2.column2,
                matrix1.column3 + matrix2.column3
            );
        }
        
        template< typename TReal >
        Matrix4x4< TReal > operator-(Matrix4x4< TReal > const& matrix1, Matrix4x4< TReal > const& matrix2) {
            return Matrix4x4< TReal >(
                matrix1.column0 - matrix2.column0, 
                matrix1.column1 - matrix2.column1,
                matrix1.column2 - matrix2.column2,
                matrix1.column3 - matrix2.column3
            );
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal > Matrix4x4< TReal >::OrthographicProjection(TReal width, TReal height, TReal near, TReal far) {
            TReal halfWidth = width * TReal(0.5f);
            TReal halfHeight = height * TReal(0.5f);
            TReal left = -halfWidth;
            TReal right = halfWidth;
            TReal bottom = -halfHeight;
            TReal top = halfHeight;
            TReal invWidth = TReal(1.0f) / width;
            TReal invHeight = TReal(1.0f) / height;
            TReal invDepth = TReal(1.0f) / (far - near);
            Vector4< TReal > column4(
                -(left + right) / (right - left),
                -(top + bottom) / (top - bottom),
                -(far + near) / (far - near),
                1.0f
            );
            
            return Matrix4x4< TReal >(
                Vector4< TReal >(TReal(2.0f) * invWidth, 0.0f, 0.0f, 0.0f),
                Vector4< TReal >(0.0f, TReal(2.0f) * invHeight, 0.0f, 0.0f),
                Vector4< TReal >(0.0f, 0.0f, TReal(-2.0f) * invDepth, 0.0f),
                column4
            );
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal > Matrix4x4< TReal >::PerspectiveProjection(TReal width, TReal height, TReal near, TReal far) {
            TReal halfWidth = width * TReal(0.5f);
            TReal halfHeight = height * TReal(0.5f);
            TReal left = -halfWidth;
            TReal right = halfWidth;
            TReal bottom = -halfHeight;
            TReal top = halfHeight;
            TReal invWidth = TReal(1.0f) / width;
            TReal invHeight = TReal(1.0f) / height;
            TReal invDepth = TReal(1.0f) / (far - near);
            
            return Matrix4x4< TReal >(
                Vector4< TReal >(TReal(2.0f) * near * invWidth, 0.0f, 0.0f, 0.0f),
                Vector4< TReal >(0.0f, TReal(2.0f) * near * invHeight, 0.0f, 0.0f),
                Vector4< TReal >(0.0f, 0.0f, -(far + near) * invDepth, -1.0f),
                Vector4< TReal >(0.0f, 0.0f, TReal(-2.0f) * far * near * invDepth, 0.0f)
            );
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal > Matrix4x4< TReal >::PerspectiveFieldOfViewProjection(TReal fieldOfView, TReal aspect, TReal near, TReal far) {
            TReal right = near * Mathf::Tan(fieldOfView * TReal(0.5));
            TReal left = -right;
            TReal top = right / aspect;
            TReal bottom = left / aspect;
            TReal invDepth = TReal(1.0f) / (far - near);
            
            return Matrix4x4< TReal >(
                Vector4< TReal >(near / right, 0.0f, 0.0f, 0.0f),
                Vector4< TReal >(0.0f, near / top, 0.0f, 0.0f),
                Vector4< TReal >(0.0f, 0.0f, -(far + near) * invDepth, -1.0f),
                Vector4< TReal >(0.0f, 0.0f, TReal(-2.0f) * far * near * invDepth, 0.0f)
            );
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal >::Matrix4x4()
        : column0(0.0, 0.0, 0.0, 0.0),
          column1(0.0, 0.0, 0.0, 0.0),
          column2(0.0, 0.0, 0.0, 0.0),
          column3(0.0, 0.0, 0.0, 0.0) {
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal >::Matrix4x4(Vector4< TReal > const& x, Vector4< TReal > const& y, Vector4< TReal > const& z, Vector4< TReal > const& w)
        : column0(x),
          column1(y),
          column2(z),
          column3(w) {
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal > Matrix4x4< TReal >::Cofactor() const {
            return Matrix4x4< TReal >(
                CofactorColumn(*this, 0),
                CofactorColumn(*this, 1),
                CofactorColumn(*this, 2),
                CofactorColumn(*this, 3)                    
            );
        }
        
        template< typename TReal >
        inline TReal Matrix4x4< TReal >::Determinant(Matrix4x4 const& cofactors) const {
            return GetColumn(0)[0] * cofactors.GetColumn(0)[0] +
                   GetColumn(1)[0] * cofactors.GetColumn(1)[0] +
                   GetColumn(2)[0] * cofactors.GetColumn(2)[0] +
                   GetColumn(3)[0] * cofactors.GetColumn(3)[0];
        }
        
        template< typename TReal >
        inline Vector4< TReal > Matrix4x4< TReal >::GetColumn(uint32_t index) const {
            JOH_ASSERT(index < 4, "Index is out of range");
            return *((&column0) + index);
        }
        
        template< typename TReal >
        inline TReal const* Matrix4x4< TReal >::GetData() const {
            return &column0.x;
        }
        
        template< typename TReal >
        inline Vector4< TReal > const& Matrix4x4< TReal >::GetRow(uint32_t index) const {
            JOH_ASSERT(index < 4, "Index is out of range");
            switch(index) {
                case 0: return Vector4< TReal >(column0.x, column1.x, column2.x, column3.x);
                case 1: return Vector4< TReal >(column0.y, column1.y, column2.y, column3.y);
                case 2: return Vector4< TReal >(column0.z, column1.z, column2.z, column3.z);
                case 3: return Vector4< TReal >(column0.w, column1.w, column2.w, column3.w);
            }
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal > Matrix4x4< TReal >::Inverse() const {
            Matrix4x4< TReal > cofactors(Cofactor());
            TReal oodet = TReal(1.0) / Determinant(cofactors);
            Matrix4x4< TReal > adjoint(cofactors.Transpose());
            return adjoint * oodet;
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal >& Matrix4x4< TReal >::Invert() {
            *this = Inverse();
        }
        
        template< typename TReal >
        inline void Matrix4x4< TReal >::SetColumn(uint32_t index, Vector4< TReal > const& column) {
            JOH_ASSERT(index < 4, "Index out of range");
            *(&column0 + index) = column;
        }
                    
        template< typename TReal >
        inline void Matrix4x4< TReal >::SetRow(uint32_t index, Vector4< TReal > const& row) {
            JOH_ASSERT(index < 4, "Index out of range");
            switch(index) {
                case 0: {
                    column0.x = row[0];
                    column1.x = row[1];
                    column2.x = row[2];
                    column3.x = row[3];
                    break;
                }
                
                case 1: {
                    column0.y = row[0];
                    column1.y = row[1];
                    column2.y = row[2];
                    column3.y = row[3];
                    break;
                }
                
                case 2: {
                    column0.z = row[0];
                    column1.z = row[1];
                    column2.z = row[2];
                    column3.z = row[3];
                    break;
                }
                
                case 3: {
                    column0.w = row[0];
                    column1.w = row[1];
                    column2.w = row[2];
                    column3.w = row[3];
                    break;
                }
            }
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal > Matrix4x4< TReal >::Transpose() const {
            Matrix4x4< TReal > result(
                Vector4< TReal >(GetColumn(0)[0], GetColumn(1)[0], GetColumn(2)[0], GetColumn(3)[0]),
                Vector4< TReal >(GetColumn(0)[1], GetColumn(1)[1], GetColumn(2)[1], GetColumn(3)[1]),
                Vector4< TReal >(GetColumn(0)[2], GetColumn(1)[2], GetColumn(2)[2], GetColumn(3)[2]),
                Vector4< TReal >(GetColumn(0)[3], GetColumn(1)[3], GetColumn(2)[3], GetColumn(3)[3])                
            );
            
            return result;
        }
        
        template< typename TReal >
        Vector4< TReal > Matrix4x4< TReal >::TransformPoint(Vector4< TReal > const& point) const {
            Vector4< TReal > row0(column0.x, column1.x, column2.x, column3.x);
            Vector4< TReal > row1(column0.y, column1.y, column2.y, column3.y);
            Vector4< TReal > row2(column0.z, column1.z, column2.z, column3.z);
            Vector4< TReal > row3(column0.w, column1.w, column2.w, column3.w);
            Vector4< TReal > result(
                Vector4< TReal >::DotProduct(row0, point),
                Vector4< TReal >::DotProduct(row1, point),
                Vector4< TReal >::DotProduct(row2, point),
                Vector4< TReal >::DotProduct(row3, point)
            );
            
            return result + row3;
        }

        template< typename TReal >
        Vector4< TReal > Matrix4x4< TReal >::InverseTransformPoint(Vector4< TReal > const& point) const {
            Vector4< TReal > result(
                Vector4< TReal >::DotProduct(column0, point),
                Vector4< TReal >::DotProduct(column1, point),
                Vector4< TReal >::DotProduct(column2, point),
                Vector4< TReal >::DotProduct(column3, point)
            );
            
            return result + column3;
        }
        
        template< typename TReal >
        Vector4< TReal > Matrix4x4< TReal >::TransformDirection(Vector4< TReal > const& direction) const {
            Vector4< TReal > row0(column0.x, column1.x, column2.x, column3.x);
            Vector4< TReal > row1(column0.y, column1.y, column2.y, column3.y);
            Vector4< TReal > row2(column0.z, column1.z, column2.z, column3.z);
            Vector4< TReal > row3(column0.w, column1.w, column2.w, column3.w);
            return Vector4< TReal >(
                Vector4< TReal >::DotProduct(row0, direction),
                Vector4< TReal >::DotProduct(row1, direction),
                Vector4< TReal >::DotProduct(row2, direction),
                Vector4< TReal >::DotProduct(row3, direction)
            );
        }

        template< typename TReal >
        Vector4< TReal > Matrix4x4< TReal >::InverseTransformDirection(Vector4< TReal > const& direction) const {
            return Vector4< TReal >(
                Vector4< TReal >::DotProduct(column0, direction),
                Vector4< TReal >::DotProduct(column1, direction),
                Vector4< TReal >::DotProduct(column2, direction),
                Vector4< TReal >::DotProduct(column3, direction)
            );
        }
        
        template< typename TReal >
        Vector3< TReal > Matrix4x4< TReal >::TransformPoint(Vector3< TReal > const& point) const {
            TReal const* m = GetData();
            Vector3< TReal > result(
                point.x * m[0] + point.y * m[4] + point.z * m[8]  + m[12],
                point.x * m[1] + point.y * m[5] + point.z * m[9]  + m[13],
                point.x * m[2] + point.y * m[6] + point.z * m[10] + m[14]
            );
            
            return (result / (point.x * m[3] + point.y * m[7] + point.z * m[11] + m[15]));
        }

        template< typename TReal >
        Vector3< TReal > Matrix4x4< TReal >::InverseTransformPoint(Vector3< TReal > const& point) const {
            TReal const* m = GetData();
            Vector3< TReal > result(
                point.x * m[0] + point.y * m[1] + point.z * m[2]  + m[3],
                point.x * m[4] + point.y * m[5] + point.z * m[6]  + m[7],
                point.x * m[8] + point.y * m[9] + point.z * m[10] + m[11]
            );
            
            return (result / (point.x * m[12] + point.y * m[13] + point.z * m[14] + m[15]));
        }
        
        template< typename TReal >
        Vector3< TReal > Matrix4x4< TReal >::TransformDirection(Vector3< TReal > const& direction) const {
            TReal const* m = GetData();
            return Vector3< TReal >(
                direction.x * m[0] + direction.y * m[4] + direction.z * m[8],
                direction.x * m[1] + direction.y * m[5] + direction.z * m[9],
                direction.x * m[2] + direction.y * m[6] + direction.z * m[10]
            );
        }

        template< typename TReal >
        Vector3< TReal > Matrix4x4< TReal >::InverseTransformDirection(Vector3< TReal > const& direction) const {
            TReal const* m = GetData();
            return Vector3< TReal >(
                direction.x * m[0] + direction.y * m[1] + direction.z * m[2],
                direction.x * m[4] + direction.y * m[5] + direction.z * m[6],
                direction.x * m[8] + direction.y * m[9] + direction.z * m[10]
            );
        }
        
        template< typename TReal >
        inline Vector4< TReal >& Matrix4x4< TReal >::operator[](uint32_t index) {
            return GetColumn(index);
        }

        template< typename TReal >
        inline Vector4< TReal > const& Matrix4x4< TReal >::operator[](uint32_t index) const {
            return GetColumn(index);
        }
        
        template< typename TReal >
        inline Matrix4x4< TReal >& Matrix4x4< TReal >::operator+=(Matrix4x4< TReal > const& matrix) {
            column0 += matrix.GetColumn(0);
            column1 += matrix.GetColumn(1);
            column2 += matrix.GetColumn(2);
            column3 += matrix.GetColumn(3);
            return *this;
        }

        template< typename TReal >
        inline Matrix4x4< TReal >& Matrix4x4< TReal >::operator-=(Matrix4x4< TReal > const& matrix) {
            column0 -= matrix.GetColumn(0);
            column1 -= matrix.GetColumn(1);
            column2 -= matrix.GetColumn(2);
            column3 -= matrix.GetColumn(3);
            return *this;
        }

        template< typename TReal >
        inline Matrix4x4< TReal >& Matrix4x4< TReal >::operator*=(Matrix4x4< TReal > const& matrix) {
            *this = *this * matrix;
            return *this;
        }

        template< typename TReal >
        inline Matrix4x4< TReal >& Matrix4x4< TReal >::operator*=(TReal scalar) {
            column0 *= scalar;
            column1 *= scalar;
            column2 *= scalar;
            column3 *= scalar;
            return *this;
        }

        template< typename TReal >
        inline Matrix4x4< TReal >& Matrix4x4< TReal >::operator/=(TReal scalar) {
            column0 /= scalar;
            column1 /= scalar;
            column2 /= scalar;
            column3 /= scalar;
            return *this;
        }
    }
}

#endif // JOH_MATH_MATRIX4X4_IPP
