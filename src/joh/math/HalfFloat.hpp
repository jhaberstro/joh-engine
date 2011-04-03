#ifndef JOH_MATH_HALFFLOAT_HPP
#define JOH_MATH_HALFFLOAT_HPP

#include "joh/joh.hpp"

namespace joh
{
    namespace math
    {
        class HalfFloat
        {
        private:
            
            static uint32_t const kMantissaTable[2048];
            
            static uint32_t const kExponentTable[64];
            
            static uint16_t const kOffsetTable[64];
            
            static uint16_t const kBaseTable[512];
            
            static uint8_t const kShiftTable[512];
            
        public:
            
            static uint16_t FloatToHalfFloat(float value);
            
            static float HalfFloatToFloat(uint16_t value);
            
            HalfFloat();
            
            HalfFloat(float value);
            
            HalfFloat(HalfFloat const& other);
            
            operator float() const;
            
            operator uint16_t() const;
            
            HalfFloat& operator += (HalfFloat const other);

            HalfFloat& operator -= (HalfFloat const other);

            HalfFloat& operator *= (HalfFloat const other);

            HalfFloat& operator /= (HalfFloat const other);
            
            HalfFloat operator-() const;
            
            HalfFloat& operator++();
            
            HalfFloat operator++(int);
            
            HalfFloat& operator--();
            
            HalfFloat operator--(int);
            
            friend HalfFloat operator+(HalfFloat const lhs, HalfFloat const rhs);
            
            friend HalfFloat operator-(HalfFloat const lhs, HalfFloat const rhs);
            
            friend HalfFloat operator*(HalfFloat const lhs, HalfFloat const rhs);
            
            friend HalfFloat operator/(HalfFloat const lhs, HalfFloat const rhs);
            
            friend bool operator==(HalfFloat const lhs, HalfFloat const rhs);
            
            friend bool operator!=(HalfFloat const lhs, HalfFloat const rhs);
            
            friend bool operator<(HalfFloat const lhs, HalfFloat const rhs);

            friend bool operator<=(HalfFloat const lhs, HalfFloat const rhs);

            friend bool operator>(HalfFloat const lhs, HalfFloat const rhs);

            friend bool operator>=(HalfFloat const lhs, HalfFloat const rhs);

        private:
                            
            uint16_t data_;
        };
        
        
        inline uint16_t HalfFloat::FloatToHalfFloat(float value) {
            uint32_t f = *(reinterpret_cast< uint32_t* >(&value));
            return static_cast< uint16_t >(kBaseTable[f >> 23] + ((f & 0x007fffff) >> kShiftTable[f >> 23]));
        }
        
        inline float HalfFloat::HalfFloatToFloat(uint16_t value) {
            uint32_t f = kMantissaTable[kOffsetTable[value >> 10] + (value & 0x3ff)] + kExponentTable[value >> 10];
            return *(reinterpret_cast< float * >(&f));
        }
        
        inline HalfFloat::HalfFloat()
        : data_(0) {
        }
        
        inline HalfFloat::HalfFloat(float value)
        : data_(FloatToHalfFloat(value)) {
        }
        
        inline HalfFloat::HalfFloat(HalfFloat const& other)
        : data_(other.data_) {
        }
        
        inline HalfFloat::operator float() const {
            return HalfFloatToFloat(data_);
        }
        
        inline HalfFloat::operator uint16_t() const {
            return data_;
        }
        
        inline HalfFloat& HalfFloat::operator += (HalfFloat const other) {
            *this = *this + other;
            return *this;
        }

        inline HalfFloat& HalfFloat::operator -= (HalfFloat const other) {
            *this = *this - other;
            return *this;
        }

        inline HalfFloat& HalfFloat::operator *= (HalfFloat const other) {
            *this = *this * other;
            return *this;
        }

        inline HalfFloat& HalfFloat::operator /= (HalfFloat const other) {
            *this = *this / other;
            return *this;
        }
        
        inline HalfFloat HalfFloat::operator-() const {
            HalfFloat result;
            result.data_ = static_cast< uint16_t >(data_ ^ 0x8000);
            return result;
        }
        
        inline HalfFloat& HalfFloat::operator++() {
            float f = HalfFloatToFloat(data_);
            data_ = FloatToHalfFloat(++f);
            return *this;
        }
        
        inline HalfFloat HalfFloat::operator++(int) {
            HalfFloat result(*this);
            ++*this;
            return result;
        }
        
        inline HalfFloat& HalfFloat::operator--() {
            float f = HalfFloatToFloat(data_);
            data_ = FloatToHalfFloat(--f);
            return *this;
        }
        
        inline HalfFloat HalfFloat::operator--(int) {
            HalfFloat result(*this);
            ++*this;
            return result;
        }
        
        inline HalfFloat operator+(HalfFloat const lhs, HalfFloat const rhs) {
            return HalfFloat(float(lhs) + float(rhs));
        }
        
        inline HalfFloat operator-(HalfFloat const lhs, HalfFloat const rhs) {
            return HalfFloat(float(lhs) - float(rhs));
        }
        
        inline HalfFloat operator*(HalfFloat const lhs, HalfFloat const rhs) {
            return HalfFloat(float(lhs) * float(rhs));
        }
        
        inline HalfFloat operator/(HalfFloat const lhs, HalfFloat const rhs) {
            return HalfFloat(float(lhs) / float(rhs));
        }
        
        inline bool operator==(HalfFloat const lhs, HalfFloat const rhs) {
            return lhs.data_ == rhs.data_;
        }
        
        inline bool operator!=(HalfFloat const lhs, HalfFloat const rhs) {
            return lhs.data_ != rhs.data_;
        }
        
        inline bool operator<(HalfFloat const lhs, HalfFloat const rhs) {
            return float(lhs) < float(rhs);
        }

        inline bool operator<=(HalfFloat const lhs, HalfFloat const rhs) {
            return float(lhs) <= float(rhs);
        }

        inline bool operator>(HalfFloat const lhs, HalfFloat const rhs) {
            return float(lhs) > float(rhs);
        }

        inline bool operator>=(HalfFloat const lhs, HalfFloat const rhs) {
            return float(lhs) >= float(rhs);
        }
    }
}

#endif // JOH_MATH_HALFFLOAT_HPP
