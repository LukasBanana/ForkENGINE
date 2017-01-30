/*
 * flexible_stack.hpp file
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef _CPPLIBEXT_FLEXIBLE_STACK_H_
#define _CPPLIBEXT_FLEXIBLE_STACK_H_


#include <vector>
#include <string>
#include <exception>
#include <type_traits>


namespace ext
{


/**
The flexible stack allows you to store elements of different sizes.
\code
enum TypeIds
{
    TYPEID_CHAR = 0,
    TYPEID_FLOAT,
    TYPEID_DOUBLE
};

flexible_stack<TypeIds> myStack;

myStack.push('x', TYPEID_CHAR); // push char
myStack.push(3.14f, TYPEID_FLOAT); // push float
myStack.push(4.5, TYPEID_DOUBLE); // push double

//...

while (!myStack.empty())
{
    switch (myStack.top_id())
    {
        case TYPEID_CHAR:
            char x = myStack.top<char>();
            break;
        case TYPEID_FLOAT:
            float x = myStack.top<float>();
            break;
        case TYPEID_DOUBLE:
            double x = myStack.top<double>();
            break;
    }
    myStack.pop();
}
\endcode
*/
template <typename IDType> class flexible_stack
{
    
    public:
        
        /* --- Type definitions --- */

        typedef std::vector<char>           storage_type;
        typedef storage_type::size_type     size_type;
        typedef IDType                      id_type;

    private:
        
        /* --- Members --- */

        storage_type    data_;              //!< Data storage.
        size_type       size_       = 0;    //!< Number of elements.
        size_type       valOffset_  = 0;    //!< Offset of the top element.
        size_type       valSize_    = 0;    //!< Size of the top element.
        id_type         valId_;             //!< ID of the top element.

        /* --- Functions --- */

        template <typename T> void assert_element_size()
        {
            if (sizeof(T) != valSize_)
            {
                throw std::length_error(
                    "data type size mismatch: stored " + std::to_string(valSize_) +
                    " bytes but required " + std::to_string(sizeof(T)) + " bytes"
                );
            }
        }

    public:
        
        /* --- Functions --- */

        //! Returns the number of elements in the stack.
        size_type size() const
        {
            return size_;
        }

        //! Returns true if this stack is empty.
        bool empty() const
        {
            return size_ == 0;
        }

        /**
        Returns the ID of the top element.
        If the stack is empty, this value is undefined!
        */
        id_type top_id() const
        {
            return valId_;
        }

        /**
        Returns the size (in bytes) of the top element.
        If the stack is empty, this value is 0.
        */
        size_type top_size() const
        {
            return valSize_;
        }

        //! Returns the additional size for the meta data of each element.
        size_type meta_size() const
        {
            return sizeof(id_type) + sizeof(size_type);
        }

        /**
        Pushes the specified value onto the stack.
        \tparam T Specifies the data type. This can be a primitive type or any class or structure type.
        \param[in] val Specifies the value which is to be pushed onto the stack.
        \param[in] id Specifies the data type id. This should be a unique identifier for each data type.
        */
        template <typename T> void push(const T& val, const id_type& id)
        {
            static_assert(std::is_trivially_copyable<T>::value, "flexible_stack can only store trivially copyable objects");

            /* Increase storage */
            auto pos = data_.size();
            data_.resize(pos + sizeof(T) + meta_size());

            /* Store value offset */
            valOffset_ = pos;
            
            /* Store value */
            memcpy(&data_[pos], &val, sizeof(T));
            pos += sizeof(T);

            /* Store id */
            *reinterpret_cast<id_type*>(&data_[pos]) = id;
            pos += sizeof(id_type);

            /* Store size */
            *reinterpret_cast<size_type*>(&data_[pos]) = sizeof(T);

            /* Store current value state */
            valSize_ = sizeof(T);
            valId_  = id;
            ++size_;
        }

        /**
        Pops the previous element form the stack.
        If the stack is already empty, the behaviour of this function is undefined!
        */
        void pop()
        {
            /* Decrease storage */
            data_.resize(valOffset_);

            /* Restore id and size of the previous element */
            if (!data_.empty())
            {
                auto pos = data_.size();

                /* Restore value size */
                pos -= sizeof(size_type);
                valSize_ = *reinterpret_cast<size_type*>(&data_[pos]);

                /* Restore value id */
                pos -= sizeof(id_type);
                valId_ = *reinterpret_cast<id_type*>(&data_[pos]);

                /* Restore value offset */
                valOffset_ -= (valSize_ + meta_size());
            }
            else
            {
                /* Reset values */
                valOffset_ = 0;
                valSize_ = 0;
            }

            --size_;
        }

        /**
        Returns a reference to the top element.
        \tparam T Specifies the data type which has been stored when "push" was called.
        \throws std::length_error When the size of 'T' is not equal to the size of the top element.
        */
        template <typename T> T& top()
        {
            assert_element_size<T>();
            return *reinterpret_cast<T*>(&data_[valOffset_]);
        }

        /**
        Returns a constant reference to the top element.
        \tparam T Specifies the data type which has been stored when "push" was called.
        \throws std::length_error When the size of 'T' is not equal to the size of the top element.
        */
        template <typename T> const T& top() const
        {
            assert_element_size<T>();
            return *reinterpret_cast<const T*>(&data_[valOffset_]);
        }

};


} // /namespace ext


#endif


