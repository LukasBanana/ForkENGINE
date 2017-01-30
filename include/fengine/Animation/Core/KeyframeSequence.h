/*
 * Keyframe sequence header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_KEYFRAME_SEQUENCE_H__
#define __FORK_KEYFRAME_SEQUENCE_H__


#include "Core/Export.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Quaternion.h"
#include "Math/Core/Transform3D.h"
#include "Animation/Core/Playback.h"
#include "IO/FileSystem/File.h"

#include <vector>


namespace Fork
{

namespace Anim
{


/**
Animation keyframe sequence class. This class stores 3D transformation animation keyframes.
\ingroup animation
*/
class FORK_EXPORT KeyframeSequence
{

    public:
        
        //! Vector keyframe structure.
        struct VectorKeyframe
        {
            VectorKeyframe() = default;
            VectorKeyframe(size_t frame, const Math::Vector3f& vector) :
                frame   { frame  },
                vector  { vector }
            {
            }

            size_t          frame = 0;  //!< Frame index. By default 0.
            Math::Vector3f  vector;     //!< General purpose 3D vector. Mainly used for position and scale.
        };

        //! Quaternion keyframe structure.
        struct QuaternionKeyframe
        {
            QuaternionKeyframe() = default;
            QuaternionKeyframe(size_t frame, const Math::Quaternionf& quaternion) :
                frame       { frame      },
                quaternion  { quaternion }
            {
            }

            size_t              frame = 0;  //!< Frame index. By default 0.
            Math::Quaternionf   quaternion; //!< General purpose quaternion. Mainly used for rotations.
        };

        /**
        Adds a new transformation to the sub keyframe lists.
        \param[in] frame Specifies the frame index.
        \param[in] transform Specifies the new 3D transformation.
        \remarks This will add a new entry to all three keyframe
        lists ('positionKeyframes', 'rotationKeyframes' and 'scaleKeyframes').
        \see positionKeyframes
        \see rotationKeyframes
        \see scaleKeyframes
        */
        void AddTransform(size_t frame, const Math::Transform3Df& transform);

        /**
        Builds the main keyframe list out of the other three keyframe
        sub lists ('positionKeyframes', 'rotationKeyframes' and 'scaleKeyframes').
        \param[in] clampEdges Specifies whether the edges (start and end frames) are to be clamped or not.
        If this is true, the first and last frames are taken from the sub-keyframe lists,
        before the frame interpolation reaches their frame indices. Otherwise the keyframes
        are unmodified until the frame interpolation reaches their frame indices. By default true.
        \remarks This function will generate new keyframes by interpolating them depending on the sub-lists frame indices.
        \code
        KeyframeSequence seq;
        // Add two transformations with frame index 1 and 3.
        seq.AddTransform(transformA, 1);
        seq.AddTransform(transformB, 3);
        // This function call will generate 4 keyframes.
        //   The 1st keyframe (0) has the same (non-interpolated) transformation as 'transformA',
        //     because the keyframe list must begin with index 0 (but we added frame index 1).
        //   The 2nd keyframe (1) has the same transformation as 'transformA' since this is the
        //     transformation we added to exactly this index (frame index 1).
        //   The 3rd keyframe (2) has a 50/50 interpolation between 'transformA' and 'transformB',
        //     because its frame index is between 1 and 3.
        //   The 4th keyframe (3) has the same transformation as 'transformB' since this is the
        //     transformation we added to exactly this index (frame index 3).
        seq.BuildKeyframes();
        \endcode
        \see positionKeyframes
        \see rotationKeyframes
        \see scaleKeyframes
        */
        void BuildKeyframes(bool clampEdges = true);

        /**
        Returns the first frame index.
        \note This may take a little time because all three keyframe lists
        must be searched, so don't call this function every frame.
        */
        size_t FirstFrame() const;
        /**
        Returns the last frame index.
        \note This may take a little time because all three keyframe lists
        must be searched, so don't call this function every frame.
        */
        size_t LastFrame() const;

        /**
        Interpolates the keyframes with the specified frame and interpolator.
        \param[out] transform Specifies the resulting 3D transformation. The interpolated transformation will be stored in this field.
        \param[in] frameFrom Specifies the first keyframe index. This should be in the range [0 .. number-of-keyframes).
        \param[in] frameTo Specifies the second keyframe index. This should be in the range [0 .. number-of-keyframes).
        \param[in] interpolator Specifies the frame interpolator. This should be in the range [0.0 .. 1.0].
        This will be used to interpolate between the two frames specified by 'frameFrom' and 'frameTo'.
        \remarks If 'frameFrom' or 'frameTo' is out of range, the resulting transformation 'transform' will not be modified.
        */
        void Interpolate(Math::Transform3Df& transform, size_t frameFrom, size_t frameTo, float interpolator) const;

        /**
        Writes the entire keyframe sequence to the specified file.
        \see ReadFromFile
        */
        void WriteToFile(IO::File& file) const;
        /**
        Reads an entire keyframe sequence from the specified file.
        \see BuildKeyframes
        \see WriteToFile
        */
        void ReadFromFile(IO::File& file, bool clampEdges = true);
        
        /**
        Comfort function to directly use the 'frame' and 'interpolator' fields from the specified Playback instance.
        \remarks This is equivalent to the following code:
        \code
        Interpolate(playback.frame, playback.interpolator);
        \endcode
        \see Interpolate(size_t, float)
        */
        inline void Interpolate(Math::Transform3Df& transform, const Playback& playback) const
        {
            Interpolate(transform, playback.frame, playback.nextFrame, static_cast<float>(playback.interpolator));
        }

        /**
        Returns the final keyframe list. Call "BuildKeyframes" to build the keyframe list
        out of the other three keyframe sub lists ('positionKeyframes', 'rotationKeyframes' and 'scaleKeyframes').
        \see BuildKeyframes
        */
        inline const std::vector<Math::Transform3Df>& GetKeyframes() const
        {
            return keyframes_;
        }

        //! Position keyframe list.
        std::vector<VectorKeyframe>     positionKeyframes;

        //! Rotation keyframe list.
        std::vector<QuaternionKeyframe> rotationKeyframes;

        //! Scale keyframe list.
        std::vector<VectorKeyframe>     scaleKeyframes;

    private:
        
        std::vector<Math::Transform3Df> keyframes_;

};


} // /namespace Anim

} // /namespace Fork


#endif



// ========================