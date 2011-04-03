#ifndef JOH_MATH_MATHF_HPP
#define JOH_MATH_MATHF_HPP

#include "joh/joh.hpp"
#include <math.h>

namespace joh
{
    namespace math
    {
        template< typename T >
        class Math
        {
        public:
            
            static T const kPI;
            static T const kEpsilon;
            static T const kToRadians;
            static T const kToDegrees;
    
        public:
            
            static T Sin(T x);
    
            static T Cos(T x);
    
            static T Tan(T x);

            static T ArcSin(T x);
    
            static T ArcCos(T x);
    
            static T ArcTan(T x);
    
            static T ArcTan2(T y, T x);
  
            static T Sqrt(T x);
    
            static T Abs(T x);
    
            static T Min(T x, T y);
    
            static T Max(T x, T y);
    
            static T Pow(T x, T power);
    
            static T Exp(T x);
    
            static T Log(T x);
    
            static T Log10(T x);
    
            static T Ceil(T x);
    
            static T Floor(T x);
    
            static T Round(T x);
    
            static int32_t CeilToInt(T x);
    
            static int32_t FloorToInt(T x);
    
            static int32_t RoundToInt(T x);
    
            static T Sign(T x);
    
            static T Clamp(T value, T min, T max);
    
            static T Lerp(T start, T end, T t);
    
            static bool ApproximatelyEqual(T left, T right, T delta);
    
            static T ToDegrees(T rad);
    
            static T ToRadians(T deg);
    
            static T PingPong(T value, T length);
    
            static T InverseLerp(T start, T end, T value);
        };


        template< >
        class Math< float >
        {
        public:
            
            static float const kPI = 3.141592653589793f;
            static float const kEpsilon = 1.0e-06f;
            static float const kToRadians = 1.7453292519943295e-02f;
            static float const kToDegrees = 57.29577951308232f;
            
        public:
            
            static JOH_FORCE_INLINE float Sin(float x) {
                return sinf(x);
            }
    
            static JOH_FORCE_INLINE float Cos(float x) {
                return cosf(x);
            }
    
            static JOH_FORCE_INLINE float Tan(float x) {
                return tanf(x);
            }

            static JOH_FORCE_INLINE float ArcSin(float x) {
                return asinf(x);
            }
    
            static JOH_FORCE_INLINE float ArcCos(float x) {
                return acosf(x);
            }
    
            static JOH_FORCE_INLINE float ArcTan(float x) {
                return atanf(x);
            }
    
            static JOH_FORCE_INLINE float ArcTan2(float y, float x) {
                return atan2f(y, x);
            }
  
            static JOH_FORCE_INLINE float Sqrt(float x) {
                return sqrtf(x);
            }
    
            static JOH_FORCE_INLINE float Abs(float x) {
                return fabsf(x);
            }
    
            static JOH_FORCE_INLINE float Min(float x, float y) {
                return (x < y) ? x : y;
            }
    
            static JOH_FORCE_INLINE float Max(float x, float y) {
                return (x > y) ? x : y;
            }
    
            static JOH_FORCE_INLINE float Pow(float x, float power) {
                return powf(x, power);
            }
    
            static JOH_FORCE_INLINE float Exp(float x) {
                return expf(x);
            }
    
            static JOH_FORCE_INLINE float Log(float x) {
                return logf(x);
            }
    
            static JOH_FORCE_INLINE float Log10(float x) {
                return log10f(x);
            }
    
            static JOH_FORCE_INLINE float Ceil(float x) {
                return ceilf(x);
            }
    
            static JOH_FORCE_INLINE float Floor(float x) {
                return floorf(x);
            }
    
            static JOH_FORCE_INLINE float Round(float x) {
                return roundf(x);
            }
    
            static JOH_FORCE_INLINE int32_t CeilToInt(float x) {
                return static_cast< int32_t >(ceilf(x));
            }
    
            static JOH_FORCE_INLINE int32_t FloorToInt(float x) {
                return static_cast< int32_t >(floorf(x));
            }
    
            static JOH_FORCE_INLINE int32_t RoundToInt(float x) {
                return static_cast< int32_t >(roundf(x));
            }
    
            static JOH_FORCE_INLINE float Sign(float x) {
                return (x < 0.0f) ? -1.0f : 1.0f;
            }
    
            static JOH_FORCE_INLINE float Clamp(float value, float min, float max) {
                if (value > max) {
                    return max;
                }
                else if (value < min) {
                    return min;
                }
        
                return value;
            }
    
            static JOH_FORCE_INLINE float Lerp(float start, float end, float t) {
                return start + ((end - start) * t);
            }
    
            static JOH_FORCE_INLINE bool ApproximatelyEqual(float left, float right, float delta) {
                return Abs(left - right) <= delta;
            }
    
            static JOH_FORCE_INLINE float ToDegrees(float rad) {
                return rad * kToDegrees;
            }
    
            static JOH_FORCE_INLINE float ToRadians(float deg) {
                return deg * kToRadians;
            }
    
            static JOH_FORCE_INLINE float PingPong(float value, float length) {
                value = (value - (floorf(value / (length * 2.0f)) * (length * 2.0f)));
                return (length - fabsf(value - length));
            }
    
            static JOH_FORCE_INLINE float InverseLerp(float start, float end, float value) {
                return (value - start) / (end - start);
            }
        };

        typedef Math< float > Mathf;
    }
}
#endif // JOH_MATH_MATHF_HPP
