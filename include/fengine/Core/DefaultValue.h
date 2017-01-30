/*
 * Default value header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DEFAULT_VALUE_H__
#define __FORK_DEFAULT_VALUE_H__


#include <string>
#include <memory>


namespace Fork
{


/**
Default value structure.
\code
// Example usage:
template <typename T> struct MyAbstractRange
{
    T left = DefaultValue<T>::value;
    T right = DefaultValue<T>::value;
};
MyAbstractRange<int> integralRange; // Initialized to { 0, 0 }
MyAbstractRange<double> realRange; // Initialized to { 0.0, 0.0 }
MyAbstractRange<std::string> rangeNames; // Initialized to { "", "" }
MyAbstractRange<ObjectClass*> rangeReferences; // Initialized to { nullptr, nullptr }
\endcode
\remarks There are specialized version of this structure for the following types:
- float
- double
- std::shared_ptr<T>
- std::unique_ptr<T>
- std::basic_string<T>
- T*
*/
template <typename T> struct DefaultValue
{
    static const T value = T(0);
};


//! Specialized version of the "DefaultValue" structure for pointers.
template <typename T> struct DefaultValue<T*>
{
    static const T* value;
};
template <typename T> const T* DefaultValue<T*>::value = nullptr;


//! Specialized version of the "DefaultValue" structure for shared pointers.
template <typename T> struct DefaultValue<std::shared_ptr<T>>
{
    static const std::shared_ptr<T> value;
};
template <typename T> const std::shared_ptr<T> DefaultValue<std::shared_ptr<T>>::value;


//! Specialized version of the "DefaultValue" structure for unique pointers.
template <typename T> struct DefaultValue<std::unique_ptr<T>>
{
    static const std::unique_ptr<T> value;
};
template <typename T> const std::unique_ptr<T> DefaultValue<std::unique_ptr<T>>::value;


//! Specialized version of the "DefaultValue" structure for floats.
template <> struct DefaultValue<float>
{
    static const float value;
};
const float DefaultValue<float>::value = 0.0f;


//! Specialized version of the "DefaultValue" structure for doubles.
template <> struct DefaultValue<double>
{
    static const double value;
};
const double DefaultValue<double>::value = 0.0;


//! Specialized version of the "DefaultValue" structure for strings.
template <typename T> struct DefaultValue<std::basic_string<T>>
{
    static const std::basic_string<T> value;
};
template <typename T> const std::basic_string<T> DefaultValue<std::basic_string<T>>::value;


} // /namespace Fork


#endif



// ========================