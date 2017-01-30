/*
 * Image header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_H__
#define __FORK_IMAGE_H__


#include "Math/Core/Size3.h"
#include "Math/Core/Size2.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Video/Core/ColorRGBA.h"
#include "Video/Image/ImageAttributes.h"
#include "Video/Image/ImageConverter.h"
#include "Core/Export.h"
#include "Core/STLHelper.h"
#include "Plugins/cpplibext/make_array.hpp"

#include <memory>


namespace Fork
{

namespace Video
{


//! Simple structure for implicit image coordinate conversion.
struct ImagePoint
{
    ImagePoint() :
        u{ 0 },
        v{ 0 },
        w{ 0 }
    {
    }
    ImagePoint(size_t x) :
        u{ x },
        v{ 0 },
        w{ 0 }
    {
    }
    ImagePoint(size_t x, size_t y) :
        u{ x },
        v{ y },
        w{ 0 }
    {
    }
    ImagePoint(size_t x, size_t y, size_t z) :
        u{ x },
        v{ y },
        w{ z }
    {
    }
    ImagePoint(const Math::Vector2st& pos) :
        u{ pos.x },
        v{ pos.y },
        w{ 0     }
    {
    }
    ImagePoint(const Math::Vector3st& pos) :
        u{ pos.x },
        v{ pos.y },
        w{ pos.z }
    {
    }

    size_t u, v, w;
};


//! Image base class.
template <typename T> class Image
{
    
    public:
        
        /* === Types === */

        //! Image size type.
        typedef Math::Size3st SizeType;

        //! Image buffer type.
        typedef std::shared_ptr<T> BufferType;

        /* === Iterators === */

        /**
        Pixel iterator class.
        \tparam T Specifies the pixel components data type. Could be unsigned char, int, float or double.
        */
        class PixelIterator
        {
            
            public:
                
                typedef PixelIterator ThisType;

                PixelIterator() = default;
                PixelIterator(T* ptr, int size) :
                    ptr_    (ptr    ),
                    size_   (size   )
                {
                }
                
                inline bool operator == (const ThisType& other) const
                {
                    return ptr_ == other.ptr_ && size_ == other.size_;
                }
                inline bool operator != (const ThisType& other) const
                {
                    return ptr_ != other.ptr_ || size_ != other.size_;
                }

                inline ThisType& operator ++ ()
                {
                    ptr_ += size_;
                    return *this;
                }
                inline ThisType operator ++ (int)
                {
                    auto prevPtr = ptr_;
                    ptr_ += size_;
                    return ThisType(prevPtr, size_);
                }

                inline ThisType& operator -- ()
                {
                    ptr_ -= size_;
                    return *this;
                }
                inline ThisType operator -- (int)
                {
                    auto prevPtr = ptr_;
                    ptr_ -= size_;
                    return ThisType(prevPtr, size_);
                }

                inline ThisType& operator += (int offset)
                {
                    ptr_ += offset * size_;
                    return *this;
                }
                inline ThisType& operator -= (int offset)
                {
                    ptr_ -= offset * size_;
                    return *this;
                }

                inline ColorRGBA<T>& operator * () const
                {
                    return *reinterpret_cast<ColorRGBA<T>*>(ptr_);
                }
                inline ColorRGBA<T>* operator -> () const
                {
                    return reinterpret_cast<ColorRGBA<T>*>(ptr_);
                }

                //! Returns true if this iterator is valid.
                inline operator bool () const
                {
                    return ptr_ != nullptr && size_ > 0;
                }

            private:
                
                T* ptr_ = nullptr;
                int size_ = 0;
                
        };

        /**
        Constant pixel iterator class.
        \tparam T Specifies the pixel components data type. Could be unsigned char, int, float or double.
        */
        class ConstPixelIterator
        {
            
            public:
                
                typedef ConstPixelIterator ThisType;

                ConstPixelIterator() = default;
                ConstPixelIterator(const T* ptr, int size) :
                    ptr_    (ptr    ),
                    size_   (size   )
                {
                }
                
                inline bool operator == (const ThisType& other) const
                {
                    return ptr_ == other.ptr_ && size_ == other.size_;
                }
                inline bool operator != (const ThisType& other) const
                {
                    return ptr_ != other.ptr_ || size_ != other.size_;
                }

                inline ThisType& operator ++ ()
                {
                    ptr_ += size_;
                    return *this;
                }
                inline ThisType operator ++ (int)
                {
                    auto prevPtr = ptr_;
                    ptr_ += size_;
                    return ThisType(prevPtr, size_);
                }

                inline ThisType& operator -- ()
                {
                    ptr_ -= size_;
                    return *this;
                }
                inline ThisType operator -- (int)
                {
                    auto prevPtr = ptr_;
                    ptr_ -= size_;
                    return ThisType(prevPtr, size_);
                }

                inline ThisType& operator += (int offset)
                {
                    ptr_ += offset * size_;
                    return *this;
                }
                inline ThisType& operator -= (int offset)
                {
                    ptr_ -= offset * size_;
                    return *this;
                }

                inline const ColorRGBA<T>& operator * () const
                {
                    return *reinterpret_cast<const ColorRGBA<T>*>(ptr_);
                }
                inline const ColorRGBA<T>* operator -> () const
                {
                    return reinterpret_cast<const ColorRGBA<T>*>(ptr_);
                }

                //! Returns true if this iterator is valid.
                inline operator bool () const
                {
                    return ptr_ != nullptr && size_ > 0;
                }

            private:
                
                const T* ptr_ = nullptr;
                int size_ = 0;
                
        };

        /* === STL conformance types === */

        typedef T                                       value_type;
        typedef value_type&                             reference;
        typedef const value_type&                       const_reference;
        typedef T*                                      pointer;
        typedef const T*                                const_pointer;
        typedef PixelIterator                           iterator;
        typedef ConstPixelIterator                      const_iterator;
        typedef std::reverse_iterator<iterator>         reverse_iterator;
        typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
        typedef std::ptrdiff_t                          difference_type;
        typedef unsigned int                            size_type;

        /* === STL conformance functions === */

        /*
        Theses functions are an exception of the engine's coding conventions.
        This is due to STL conformance with the container template classes such as std::vector.
        */

        iterator begin()
        {
            return Begin();
        }
        const_iterator begin() const
        {
            return Begin();
        }

        iterator end()
        {
            return End();
        }
        const_iterator end() const
        {
            return End();
        }

        //to be continued ...

        /* === Functions === */

        /**
        Image constructor.
        \param[in] size Specifies the initial image size. This can be changed with "Resize".
        \param[in] format Specifies the initial image color format. This can be changed with "ConvertFormat".
        \param[in] fillBuffer Specifies whether the image buffer is to be filled.
        If false, the image buffer is uninitialized! By default true.
        \see Resize
        \see ConvertFormat
        */
        Image(const SizeType& size, const ImageColorFormats format, bool fillBuffer = true) :
            format_ { format },
            size_   { size   }
        {
            /* Initialize image buffer */
            ResizeBuffer(NumElements());
            if (fillBuffer)
                std::fill(buffer_.get(), buffer_.get() + NumElements(), T(0));
        }

        /**
        Resizes the image buffer.
        \param[in] size Specifies the new image size.
        \param[in] mode Specifies the resize mode. If this is ImageResizeModes::None,
        no pixel interpolation will be computed, i.e. the new image buffer will be uninitialized!
        By default ImageResizeModes::Nearest.
        \see ImageResizeModes
        \see ImageConverter::ResizeImageNearest
        \see ImageConverter::ResizeImageLinear
        \todo 'ImageResizeModes::Linear' mode not yet implemented!
        */
        void Resize(const SizeType& size, const ImageResizeModes mode = ImageResizeModes::Nearest)
        {
            if (size == size_)
                return;

            /* Store new settings */
            const auto prevSize = size_;
            size_ = size;

            /* Create new image buffer */
            auto scaledBuffer = ext::make_shared_array<T>(NumElements());

            /* Copy scaled image to the new buffer */
            switch (mode)
            {
                case ImageResizeModes::None:
                    // no pixel interpolation (unintialized buffer)
                    break;
                case ImageResizeModes::Nearest:
                    ImageConverter::ResizeImageNearest(buffer_.get(), prevSize, scaledBuffer.get(), size_, NumColorComponents());
                    break;
                case ImageResizeModes::Linear: //!!!
                    ImageConverter::/*ResizeImageLinear*/ResizeImageNearest(buffer_.get(), prevSize, scaledBuffer.get(), size_, NumColorComponents());
                    break;
            }

            /* Use the new image buffer */
            buffer_ = scaledBuffer;
        }

        /**
        Converts the color format of the entire image buffer.
        \param[in] format Specifies the new image format.
        \param[in] defaultAlpha Specifies the default alpha value. By default MaxColorValue<T>().
        \return True on success, otherwise an invalid format was used (see 'ImageConverter::ConvertImageFormat' for more details).
        \see ImageConverter::ConvertImageFormat
        \see MaxColorValue
        */
        bool ConvertFormat(const ImageColorFormats format, const T& defaultAlpha = MaxColorValue<T>())
        {
            if (format > ImageColorFormats::BGRA || format_ > ImageColorFormats::BGRA)
                return false;
            
            /* Create new image buffer */
            auto destBufferSize = NumPixels() * Video::NumColorComponents(format);
            auto destBuffer = ext::make_shared_array<T>(destBufferSize);

            /* Convert image format */
            ImageConverter::ConvertImageFormat(
                buffer_.get(), format_,
                destBuffer.get(), format,
                NumPixels(), defaultAlpha
            );

            /* Store new data */
            buffer_ = destBuffer;
            format_ = format;

            return true;
        }

        /**
        Adjusts the format alignment if the color format has 3 components.
        \remarks Some renderers (Direct3D 11 for instance) don't allow 3-component color formats.
        In this case RGB must be converted to RGBA and BGR must be converted to BGRA.
        This format alignment will be adjusted by this function.
        \remarks The behavior of this function is equivalent to the following code:
        \code
        if (this->GetFormat() == Video::ImageColorFormats::RGB)
            this->ConvertFormat(Video::ImageColorFormats::RGBA);
        else if (this->GetFormat() == Video::ImageColorFormats::BGR)
            this->ConvertFormat(Video::ImageColorFormats::BGRA);
        \endcode
        \remarks This function will be used automatically for each loaded image by the TextureManager.
        \see ConvertFormat
        \see TextureManager
        */
        void AdjustFormatAlignment()
        {
            if (GetFormat() == ImageColorFormats::RGB)
                ConvertFormat(ImageColorFormats::RGBA);
            else if (GetFormat() == ImageColorFormats::BGR)
                ConvertFormat(ImageColorFormats::BGRA);
        }
        
        //! Copies this image object and returns a shared pointer to the new image object.
        std::shared_ptr<Image<T>> Copy() const
        {
            /* Create new image object */
            auto newImage = std::make_shared<Image<T>>(format_, size_);

            newImage->ResizeBuffer(newImage->BufferSize());
            std::copy(buffer_.get(), buffer_.get() + NumElements(), newImage->buffer_.get());

            return newImage;
        }

        //! Converts the specifies image image (or rather position) into a pixel index.
        size_t ToPixelIndex(const ImagePoint& position) const
        {
            return size_.width * (position.w*size_.height + position.v) + position.u;
        }

        /**
        Writes the specified color into the image buffer.
        \param[in] position Specifies the 3D position in the image buffer.
        The constructor or 'ImagePoint' allows simple 1D-, 2D- and 3D coordinates (see example).
        \param[in] color Specifies the RGBA color which is to be written to the image buffer.
        remarks Usage example:
        \code
        imageUByte2D->WriteColor({ x, y }, { 255, 128, 0, 255 });
        imageFloat3D->WriteColor({ x, y, z }, { 0.5f, 1.0f, 0.25f, 1.0f });
        \endcode
        \see ReadColor
        */
        void WriteColor(const ImagePoint& position, const ColorRGBA<T>& color)
        {
            size_t index = ToPixelIndex(position);
            T* rawBuffer = RawBuffer();

            switch (format_)
            {
                case ImageColorFormats::Gray:
                    rawBuffer[index] = color.r;
                    break;

                case ImageColorFormats::GrayAlpha:
                    index *= 2;
                    rawBuffer[index    ] = color.r;
                    rawBuffer[index + 1] = color.a;
                    break;

                case ImageColorFormats::RGB:
                    index *= 3;
                    rawBuffer[index    ] = color.r;
                    rawBuffer[index + 1] = color.g;
                    rawBuffer[index + 2] = color.b;
                    break;

                case ImageColorFormats::BGR:
                    index *= 3;
                    rawBuffer[index    ] = color.b;
                    rawBuffer[index + 1] = color.g;
                    rawBuffer[index + 2] = color.r;
                    break;

                case ImageColorFormats::RGBA:
                    index *= 4;
                    rawBuffer[index    ] = color.r;
                    rawBuffer[index + 1] = color.g;
                    rawBuffer[index + 2] = color.b;
                    rawBuffer[index + 3] = color.a;
                    break;

                case ImageColorFormats::BGRA:
                    index *= 4;
                    rawBuffer[index    ] = color.b;
                    rawBuffer[index + 1] = color.g;
                    rawBuffer[index + 2] = color.r;
                    rawBuffer[index + 3] = color.a;
                    break;

                default:
                    break;
            }
        }

        /**
        Reads the color from the image buffer.
        \param[in] position Specifies the 3D position in the image buffer.
        \return RGBA color from the image buffer at the specified position.
        \see WriteColor
        */
        ColorRGBA<T> ReadColor(const ImagePoint& position) const
        {
            ColorRGBA<T> color;

            size_t index = ToPixelIndex(position);
            const T* rawBuffer = RawBuffer();

            switch (format_)
            {
                case ImageColorFormats::Gray:
                    color.r = rawBuffer[index];
                    break;

                case ImageColorFormats::GrayAlpha:
                    index *= 2;
                    color.r = rawBuffer[index    ];
                    color.a = rawBuffer[index + 1];
                    break;

                case ImageColorFormats::RGB:
                    index *= 3;
                    color.r = rawBuffer[index    ];
                    color.g = rawBuffer[index + 1];
                    color.b = rawBuffer[index + 2];
                    break;

                case ImageColorFormats::BGR:
                    index *= 3;
                    color.b = rawBuffer[index    ];
                    color.g = rawBuffer[index + 1];
                    color.r = rawBuffer[index + 2];
                    break;

                case ImageColorFormats::RGBA:
                    index *= 4;
                    color.r = rawBuffer[index    ];
                    color.g = rawBuffer[index + 1];
                    color.b = rawBuffer[index + 2];
                    color.a = rawBuffer[index + 3];
                    break;

                case ImageColorFormats::BGRA:
                    index *= 4;
                    color.b = rawBuffer[index    ];
                    color.g = rawBuffer[index + 1];
                    color.r = rawBuffer[index + 2];
                    color.a = rawBuffer[index + 3];
                    break;

                default:
                    break;
            }

            return color;
        }

        //! Returns the image size rounded to the next power-of-two size (e.g. 120 to 128, 260 to 256 etc.).
        SizeType SizePow2() const
        {
            return SizeType(
                Math::RoundPow2( size_.width  ),
                Math::RoundPow2( size_.height ),
                Math::RoundPow2( size_.depth  )
            );
        }

        /**
        Returns true if this image has a power-of-two size.
        \see SizePow2
        */
        inline bool HasSizePow2() const
        {
            return GetSize() == SizePow2();
        }

        /**
        Short version of the "Resize" function. This is equivalent to the following code:
        \code
        Resize(Math::Size3Dui(size.width, size.height, GetSize().depth));
        \endcode
        \see Resize(const Math::Size3Dui&)
        */
        inline void Resize(const Math::Size2st& size)
        {
            Resize(SizeType(size.width, size.height, GetSize().depth));
        }

        //! Returns the image size.
        inline const SizeType& GetSize() const
        {
            return size_;
        }
        //! Returns the image color format.
        inline ImageColorFormats GetFormat() const
        {
            return format_;
        }

        //! Returns the number of image pixels.
        inline size_t NumPixels() const
        {
            return size_.Volume();
        }
        //! Returns the number of elements (color components) in the whole image buffer.
        inline size_t NumElements() const
        {
            return NumPixels() * NumColorComponents();
        }
        //! Returns the size (in bytes) of the whole image buffer.
        inline size_t BufferSize() const
        {
            return NumPixels() * FormatSize();
        }
        
        /**
        Returns the number of color components of the image color format. This will be 1, 2, 3 or 4.
        \see Video::NumColorComponents
        */
        inline size_t NumColorComponents() const
        {
            return Video::NumColorComponents(format_);
        }
        //! Returns the format size (in bytes) of the image color format. This is the size of a single pixel.
        inline size_t FormatSize() const
        {
            return NumColorComponents() * sizeof(T);
        }
        //! Returns the size (in bytes) of a single row of pixels.
        inline size_t Stride() const
        {
            return GetSize().width * FormatSize();
        }

        /*
        Returns the image buffer type. This is a shared pointer (std::shared_ptr<T>).
        \see BufferType
        */
        inline const BufferType& GetBuffer() const
        {
            return buffer_;
        }

        //! Returns the raw image buffer.
        inline T* RawBuffer()
        {
            return buffer_.get();
        }
        //! Returns the constant raw image buffer.
        inline const T* RawBuffer() const
        {
            return buffer_.get();
        }

        //! Returns a pixel iterator to the beginning of the image buffer.
        inline PixelIterator Begin()
        {
            return PixelIterator(buffer_.get(), static_cast<int>(NumColorComponents()));
        }
        //! Returns a constant pixel iterator to the beginning of the image buffer.
        inline ConstPixelIterator Begin() const
        {
            return ConstPixelIterator(buffer_.get(), static_cast<int>(NumColorComponents()));
        }

        //! Returns a pixel iterator to the end (after the last pixel) of the image buffer.
        inline PixelIterator End()
        {
            return PixelIterator(buffer_.get() + NumElements(), static_cast<int>(NumColorComponents()));
        }
        //! Returns a constant pixel iterator to the end (after the last pixel) of the image buffer.
        inline ConstPixelIterator End() const
        {
            return ConstPixelIterator(buffer_.get() + NumElements(), static_cast<int>(NumColorComponents()));
        }

    private:

        /* === Functions === */

        void ResizeBuffer(size_t size)
        {
            buffer_ = ext::make_shared_array<T>(size);
        }

        /* === Members === */

        ImageColorFormats format_;
        SizeType size_;
        
        BufferType buffer_;

};


typedef Image<unsigned char> ImageUByte;
typedef Image<float> ImageFloat;

typedef std::shared_ptr<ImageUByte> ImageUBytePtr;
typedef std::shared_ptr<ImageFloat> ImageFloatPtr;


} // /namespace Video

} // /namespace Fork


#endif



// ========================