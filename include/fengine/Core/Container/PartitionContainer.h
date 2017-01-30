/*
 * Partition container header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PARTITION_CONTAINER_H__
#define __FORK_PARTITION_CONTAINER_H__


#include "Core/Export.h"
#include "Core/Exception/PartitionContainerException.h"

#include <vector>
#include <map>
#include <algorithm>


namespace Fork
{


/**
The partition container is used to store several independent buffers in one single buffer.
The "Video::PrimitiveRenderer" for instance stores all font-vertex-buffers in a single one.
*/
template <typename Key> class PartitionContainer
{

    public:
        
        //! Partition meta data class.
        class Partition
        {

            public:

                Partition() = default;
                Partition(size_t offset, size_t size) :
                    offset_ (offset ),
                    size_   (size   )
                {
                }

                //! Returns the partitions's offset (in bytes).
                inline size_t GetOffset() const
                {
                    return offset_;
                }
                //! Returns the partitions's size (in bytes).
                inline size_t GetSize() const
                {
                    return size_;
                }

                //! Returns true if this is a valid partition. This is equivalent to "GetSize() > 0".
                inline bool IsValid() const
                {
                    return size_ > 0;
                }

            private:

                friend class PartitionContainer;

                size_t offset_ = 0;
                size_t size_ = 0;

        };

        typedef std::map<Key, Partition> PartitionListType;

        /**
        Adds a new partition to the container.
        \param[in] key Specifies the partition key.
        \param[in] data Raw-pointer to the data from the the partition will be initialized.
        If this is null, the partition will be uninitialized.
        \param[in] size Specifies the partition size (in bytes).
        \return The new created partition.
        \throws PartitionContainerException If the size is zero.
        \throws PartitionContainerException If the partition already exists.
        */
        Partition Add(const Key& key, const void* data, const size_t size)
        {
            if (!size)
                throw PartitionContainerException(__FUNCTION__, "Invalid size for partition");

            /* Check if partition already exists */
            auto it = partitions_.find(key);
            if (it != partitions_.end())
                throw PartitionContainerException(__FUNCTION__, "Partition already exists");

            /* Append partition to the hash-map */
            Partition partition(buffer_.size(), size);
            partitions_[key] = partition;

            /* Update buffer */
            buffer_.resize(buffer_.size() + size);

            if (data)
            {
                auto byteAlignedData = reinterpret_cast<const char*>(data);
                std::copy(byteAlignedData, byteAlignedData + size, buffer_.begin() + partition.GetOffset());
            }

            return partition;
        }

        /**
        Removes the specified partition from the container.
        \param[in] key Specifies the partition which is to be removed.
        \throws PartitionContainerException If the partition does not exist.
        */
        void Remove(const Key& key)
        {
            /* Find partition */
            auto it = partitions_.find(key);
            if (it == partitions_.end())
                throw PartitionContainerException(__FUNCTION__, "Partition does not exist");
            
            /* Remove partition from the hash-map */
            const auto partitionToRemove = it->second;

            if (buffer_.size() < partitionToRemove.GetSize())
                throw PartitionContainerException(__FUNCTION__, "Partition to remove is larger than the entire buffer");

            partitions_.erase(it);

            /* Copy subsequent partitions to the removed place */
            auto dest = buffer_.begin() + partitionToRemove.GetOffset();
            auto destEnd = buffer_.end() - partitionToRemove.GetSize();

            auto src = buffer_.begin() + partitionToRemove.GetOffset() + partitionToRemove.GetSize();

            for (; dest != destEnd; ++dest, ++src)
                *dest = *src;

            /* Update subsequent partition offsets */
            for (auto it : partitions_)
            {
                if (it.second.GetOffset() > partitionToRemove.GetOffset())
                    it.second.offset_ -= partitionToRemove.GetSize();
            }

            /* Shrink container size */
            buffer_.resize(buffer_.size() - partitionToRemove.GetSize());
        }

        //! Clears the entire contianer with all its partitions.
        void Clear()
        {
            buffer_.clear();
            partitions_.clear();
        }

        //! Returns the size of the entire container.
        inline size_t Size() const
        {
            return buffer_.size();
        }

        /*void Resize(const Key& key, size_t newSize)
        {
            //todo ...
        }*/

        //! Trys to find the specified partition.
        Partition Find(const Key& key) const
        {
            auto it = partitions_.find(key);
            return it != partitions_.end() ? it->second : Partition();
        }

        /**
        Returns a raw-pointer to the buffer where the specified partition begins.
        \throws PartitionContainerException If the specified partition does not exist.
        */
        void* GetRawBuffer(const Key& key)
        {
            auto partition = Find(key);
            if (!partition.IsValid())
                throw PartitionContainerException(__FUNCTION__, "Partition does not exist");
            return buffer_.data() + partition.GetOffset();
        }
        /**
        Returns a constant raw-pointer to the buffer where the specified partition begins.
        \throws PartitionContainerException If the specified partition does not exist.
        */
        const void* GetRawBuffer(const Key& key) const
        {
            auto partition = Find(key);
            if (!partition.IsValid())
                throw PartitionContainerException(__FUNCTION__, "Partition does not exist");
            return buffer_.data() + partition.GetOffset();
        }

        //! Returns a raw-pointer to the entire buffer.
        inline void* GetRawBuffer()
        {
            return buffer_.data();
        }
        //! Returns a constant raw-pointer to the entire buffer.
        inline const void* GetRawBuffer() const
        {
            return buffer_.data();
        }

        //! Returns the partition list (or rather hash-map).
        inline const PartitionListType& GetPartitions() const
        {
            return partitions_;
        }

    private:

        std::vector<char> buffer_;

        PartitionListType partitions_;

};


} // /namespace Fork


#endif



// ========================