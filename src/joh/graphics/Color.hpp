#ifndef JOH_GRAPHICS_COLOR_HPP
#define JOH_GRAPHICS_COLOR_HPP

#include "joh/joh.hpp"

namespace joh
{
	namespace graphics
	{
		template< typename TReal >
		class Color
		{
		public:
			
			typedef TReal RealType;
			
		public:
			
			static Color Lerp(Color const& start, Color const& end, float t);
            
            Color();
			
			Color(RealType r, RealType g, RealType b, RealType a);
			
			Color const& operator+=(Color const& rhs);

			Color const& operator-=(Color const& rhs);
			
			Color const& operator*=(Color const& rhs);

			Color const& operator/=(Color const& rhs);			

			Color const& operator*=(RealType rhs);

			Color const& operator/=(RealType rhs);		
			
			RealType& operator[](uint32_t index);
            
            RealType operator[](uint32_t index) const;	
			
		public:
			
			TReal r;
			TReal g;
			TReal b;
			TReal a;
		};
		
		template< typename TReal >
        Color< TReal > operator+(Color< TReal > const& lhs, Color< TReal > const& rhs);
		
		template< typename TReal >
        Color< TReal > operator-(Color< TReal > const& lhs, Color< TReal > const& rhs);

        template< typename TReal >
		Color< TReal > operator*(Color< TReal > const& lhs, Color< TReal > const& rhs);

        template< typename TReal >
		Color< TReal > operator/(Color< TReal > const& lhs, Color< TReal > const& rhs);
		
		template< typename TReal >
        Color< TReal > operator*(Color< TReal > const& lhs, typename Color< TReal >::RealType rhs);
        
		template< typename TReal >
        Color< TReal > operator/(Color< TReal > const& lhs, typename Color< TReal >::RealType rhs);
		
		template< typename TReal >
        Color< TReal > operator*(typename Color< TReal >::RealType lhs, Color< TReal > const& rhs);

		
		template< typename TReal >
		Color< TReal > Color< TReal >::Lerp(Color< TReal > const& start, Color< TReal > const& end, float t) {
			return (start + ((end - start) * t));
		}
		
		template< typename TReal >
		inline Color< TReal >::Color()
		: r(TReal(0.0f)),
          g(TReal(0.0f)),
          b(TReal(0.0f)),
          a(TReal(0.0f)) {
		}
        
        template< typename TReal >
		inline Color< TReal >::Color(typename Color< TReal >::RealType r, typename Color< TReal >::RealType g, typename Color< TReal >::RealType b, typename Color< TReal >::RealType a)
		: r(r),
		  g(g),
		  b(b),
		  a(a) {
		}
		
		template< typename TReal >
		inline Color< TReal > const& Color< TReal >::operator+=(Color< TReal > const& rhs) {
			this->r += rhs.r;
			this->g += rhs.g;
			this->b += rhs.b;
			this->a += rhs.a;
			return *this;
		}

		template< typename TReal >
		inline Color< TReal > const& Color< TReal >::operator-=(Color< TReal > const& rhs) {
			this->r -= rhs.r;
			this->g -= rhs.g;
			this->b -= rhs.b;
			this->a -= rhs.a;
			return *this;
		}
		
		template< typename TReal >
		inline Color< TReal > const& Color< TReal >::operator*=(Color< TReal > const& rhs) {
			this->r *= rhs.r;
			this->g *= rhs.g;
			this->b *= rhs.b;
			this->a *= rhs.a;
			return *this;
		}

		template< typename TReal >
		inline Color< TReal > const& Color< TReal >::operator/=(Color< TReal > const& rhs) {
			this->r /= rhs.r;
			this->g /= rhs.g;
			this->b /= rhs.b;
			this->a /= rhs.a;
			return *this;
		}
		
		template< typename TReal >
		inline Color< TReal > const& Color< TReal >::operator*=(typename Color< TReal >::RealType rhs) {
			this->r *= rhs;
			this->g *= rhs;
			this->b *= rhs;
			this->a *= rhs;
			return *this;
		}

		template< typename TReal >
		inline Color< TReal > const& Color< TReal >::operator/=(typename Color< TReal >::RealType rhs) {
			TReal invRhs = TReal(1.0f) / rhs; 
			this->r *= invRhs;
			this->g *= invRhs;
			this->b *= invRhs;
			this->a *= invRhs;
			return *this;
		}
		
		template< typename TReal >
		inline Color< TReal > operator+(Color< TReal > const& lhs, Color< TReal > const& rhs) {
			return Color< TReal >(
				lhs.r + rhs.r,
				lhs.g + rhs.g,
				lhs.b + rhs.b,
				lhs.a + rhs.a				
			);
		}
		
		template< typename TReal >
		inline Color< TReal > operator-(Color< TReal > const& lhs, Color< TReal > const& rhs) {
			return Color< TReal >(
				lhs.r - rhs.r,
				lhs.g - rhs.g,
				lhs.b - rhs.b,
				lhs.a - rhs.a				
			);
		}
		
		template< typename TReal >
		inline Color< TReal > operator*(Color< TReal > const& lhs, typename Color< TReal >::RealType rhs) {
			return Color< TReal >(
				lhs.r * rhs,
				lhs.g * rhs,
				lhs.b * rhs,
				lhs.a * rhs				
			);
		}
		
		template< typename TReal >
		inline Color< TReal > operator*(Color< TReal > const& lhs, Color< TReal > const& rhs) {
			return Color< TReal >(
				lhs.r * rhs.r,
				lhs.g * rhs.g,
				lhs.b * rhs.b,
				lhs.a * rhs.a				
			);
		}

        template< typename TReal >
		inline Color< TReal > operator/(Color< TReal > const& lhs, Color< TReal > const& rhs) {
			return Color< TReal >(
				lhs.r / rhs.r,
				lhs.g / rhs.g,
				lhs.b / rhs.b,
				lhs.a / rhs.a				
			);
		}
        
		template< typename TReal >
		inline Color< TReal > operator/(Color< TReal > const& lhs, typename Color< TReal >::RealType rhs) {
			TReal invRhs = TReal(1.0) / rhs;
			return Color< TReal >(
				lhs.r * invRhs,
				lhs.g * invRhs,
				lhs.b * invRhs,
				lhs.a * invRhs				
			);
		}
		
		template< typename TReal >
		inline Color< TReal > operator*(typename Color< TReal >::RealType lhs, Color< TReal > const& rhs) {
			return Color< TReal >(
				lhs * rhs.r,
				lhs * rhs.g,
				lhs * rhs.b,
				lhs * rhs.a
			);
		}
		
		template< typename TReal >
        inline typename Color< TReal >::RealType& Color< TReal >::operator[](uint32_t index) {
            JOH_ASSERT(index < 2, "index is out of range");
            return *((&r) + index);
        }
        
        template< typename TReal >
        inline typename Color< TReal >::RealType Color< TReal >::operator[](uint32_t index) const {
            JOH_ASSERT(index < 2, "index is out of range");
            return *((&r) + index);
        }
	}
}

#endif // JOH_GRAPHICS_COLOR_HPP