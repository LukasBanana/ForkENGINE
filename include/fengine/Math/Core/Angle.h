/*
 * Angle header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ANGLE_H__
#define __FORK_ANGLE_H__


#include "DefaultMathOperators.h"
#include "MathConstants.h"


namespace Fork
{

namespace Math
{


/**
Angle base class. This class stores a value in common angle units which can be converted into radians or degrees.
The internal angle units are in the range [0.0 .. 1.0), i.e. 1.0 is a 360 degrees, or rather a 2*pi radians rotation.
\tparam T Specifies the data type. This should be float or double.
\tparam Definition Specifies the definition of a full rotation. This must be a struct that implements the following interface:
\code
struct ExampleDefinition
{
    // Full rotation. This can be 1.0 (for abstract abgles), 360.0 (for degrees) or 2*PI<T>() (for radians).
    static T FullRotation();
};
\endcode
See "Math::AngleRotation" for the default definition.
\see Math::AngleRotation
\see Math::Degree
\see Math::Radian
*/
template <typename T, class Definition> class AngleBase
{
    
    public:
        
        typedef AngleBase<T, Definition> ThisType;

        AngleBase() = default;
        //! Constructor for implicit conversion between angle definitions.
        template <typename OtherDefinition> AngleBase(const AngleBase<T, OtherDefinition>& other) :
            angle_{ other.NormalizedAngle() * ThisType::FullRotation() }
        {
        }
        explicit AngleBase(const T& angleUnits) :
            angle_{ angleUnits }
        {
        }

        inline ThisType& operator += (const ThisType& other)
        {
            angle_ += other.angle_;
            return *this;
        }
        inline ThisType& operator -= (const ThisType& other)
        {
            angle_ -= other.angle_;
            return *this;
        }

        inline ThisType& operator *= (const T& factor)
        {
            angle_ *= factor;
            return *this;
        }
        inline ThisType& operator /= (const T& factor)
        {
            angle_ /= factor;
            return *this;
        }

        /**
        Converts this angle to an angle of the other angle type.
        \tparam OtherType This must be an "AngleBase" class, e.g. Angle<>, Degree<> or Radian<>.
        \return The converted angle.
        \note Because of a BUG inside the MSVC12 compiler this function was simplified.
        MSVC12 instantiates this template wrong! This happens when this function is called
        consecutively with different template arguments. This totally works for GCC 4.7+
        but unfortunately not correctly for MSVC12. This was the original code:
        \code
        template <template <typename> class OtherType> inline T Get() const
        {
            return OtherType<T>(*this).GetAngle();
        }
        \endcode
        */
        template <class OtherType> inline T Get() const
        {
            return NormalizedAngle() * OtherType::FullRotation();
        }

        //! Returns the internal angle. This value depends on the angle definition.
        inline T GetAngle() const
        {
            return angle_;
        }

        //! Returns the normalized angle, i.e. in the range [0.0 .. 1.0).
        inline T NormalizedAngle() const
        {
            return angle_ / ThisType::FullRotation();
        }

        /**
        Returns the full rotation of this angle definition.
        This can be 1.0 (for unit angles), 360 (for degrees) or 2*pi (for radians).
        */
        static inline T FullRotation()
        {
            return Definition::FullRotation();
        }

    private:

        T angle_ { 0 };

};

/* --- Global binary operators --- */

template <typename T, class DefinitionLHS, class DefinitionRHS>
AngleBase<T, DefinitionLHS> operator + (const AngleBase<T, DefinitionLHS>& lhs, const AngleBase<T, DefinitionRHS>& rhs)
{
    auto result = lhs;
    result += AngleBase<T, DefinitionLHS>(rhs);
    return result;
}

template <typename T, class DefinitionLHS, class DefinitionRHS>
AngleBase<T, DefinitionLHS> operator - (const AngleBase<T, DefinitionLHS>& lhs, const AngleBase<T, DefinitionRHS>& rhs)
{
    auto result = lhs;
    result -= AngleBase<T, DefinitionLHS>(rhs);
    return result;
}

template <typename T, class Definition> AngleBase<T, Definition> operator * (const AngleBase<T, Definition>& lhs, const T& rhs)
{
    auto result = lhs;
    result *= rhs;
    return result;
}
template <typename T, class Definition> AngleBase<T, Definition> operator * (const T& lhs, const AngleBase<T, Definition>& rhs)
{
    auto result = rhs;
    result *= lhs;
    return result;
}

template <typename T, class Definition> AngleBase<T, Definition> operator / (const AngleBase<T, Definition>& lhs, const T& rhs)
{
    auto result = lhs;
    result /= rhs;
    return result;
}
template <typename T, class Definition> AngleBase<T, Definition> operator / (const T& lhs, const AngleBase<T, Definition>& rhs)
{
    auto result = rhs;
    result /= lhs;
    return result;
}

/* --- Global comparision operators --- */

template <typename T, class DefinitionLHS, class DefinitionRHS>
bool operator == (const AngleBase<T, DefinitionLHS>& lhs, const AngleBase<T, DefinitionRHS>& rhs)
{
    return lhs.GetAngle() == rhs.Get<AngleBase<T, DefinitionLHS>>();
}

template <typename T, class DefinitionLHS, class DefinitionRHS>
bool operator != (const AngleBase<T, DefinitionLHS>& lhs, const AngleBase<T, DefinitionRHS>& rhs)
{
    return lhs.GetAngle() != rhs.Get<AngleBase<T, DefinitionLHS>>();
}

template <typename T, class DefinitionLHS, class DefinitionRHS>
bool operator < (const AngleBase<T, DefinitionLHS>& lhs, const AngleBase<T, DefinitionRHS>& rhs)
{
    return lhs.GetAngle() < rhs.Get<AngleBase<T, DefinitionLHS>>();
}

template <typename T, class DefinitionLHS, class DefinitionRHS>
bool operator <= (const AngleBase<T, DefinitionLHS>& lhs, const AngleBase<T, DefinitionRHS>& rhs)
{
    return lhs.GetAngle() <= rhs.Get<AngleBase<T, DefinitionLHS>>();
}

template <typename T, class DefinitionLHS, class DefinitionRHS>
bool operator > (const AngleBase<T, DefinitionLHS>& lhs, const AngleBase<T, DefinitionRHS>& rhs)
{
    return lhs.GetAngle() > rhs.Get<AngleBase<T, DefinitionLHS>>();
}

template <typename T, class DefinitionLHS, class DefinitionRHS>
bool operator >= (const AngleBase<T, DefinitionLHS>& lhs, const AngleBase<T, DefinitionRHS>& rhs)
{
    return lhs.GetAngle() >= rhs.Get<AngleBase<T, DefinitionLHS>>();
}

/* --- Angle --- */

/**
Angle rotation definition.
\see Math::Angle
*/
template <typename T> struct AngleDefinition
{
    static inline T FullRotation()
    {
        return T(1);
    }
};

//! AngleBase specialization for unit angles in the range [0.0 .. 1.0).
template <typename T = float> using Angle = AngleBase<T, AngleDefinition<T>>;

/* --- Degree --- */

/**
Degree rotation definition.
\see Math::Degree
*/
template <typename T> struct DegreeDefinition
{
    static inline T FullRotation()
    {
        return T(360);
    }
};

//! AngleBase specialization for degrees in the range [0.0 .. 360).
template <typename T = float> using Degree = AngleBase<T, DegreeDefinition<T>>;

/* --- Radian --- */

/**
Radian rotation definition.
\see Math::Radian
*/
template <typename T> struct RadianDefinition
{
    static inline T FullRotation()
    {
        return T(PI<T>()*2);
    }
};

//! AngleBase specialization for radians in the range [0.0 .. 2*pi).
template <typename T = float> using Radian = AngleBase<T, RadianDefinition<T>>;


} // /namespace Math

} // /namespace Fork


#endif



// ========================