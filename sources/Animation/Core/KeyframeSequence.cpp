/*
 * Keyframe sequence file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Animation/Core/KeyframeSequence.h"

#include <limits>


namespace Fork
{

namespace Anim
{


void KeyframeSequence::AddTransform(size_t frame, const Math::Transform3Df& transform)
{
    positionKeyframes.push_back({ frame, transform.GetPosition() });
    rotationKeyframes.push_back({ frame, transform.GetRotation() });
    scaleKeyframes   .push_back({ frame, transform.GetScale   () });
}

void KeyframeSequence::BuildKeyframes(bool clampEdges /* <--- TODO!!! */)
{
    /* If all sub-keyframe lists are empty -> break */
    if (positionKeyframes.empty() && rotationKeyframes.empty() && scaleKeyframes.empty())
        return;

    /* Get number of keyframes -> (last frame index) + 1 */
    const auto numFrames = LastFrame() + 1;

    keyframes_.resize(numFrames);

    auto FrameInterpolator = [](size_t from, size_t to, size_t current)
    {
        return static_cast<float>(current - from) / (to - from);
    };

    /* --- Position keyframes --- */

    if (!positionKeyframes.empty())
    {
        auto from   = positionKeyframes.begin();
        auto to     = from;

        for (size_t frame = 0; frame < numFrames; ++frame)
        {
            auto& keyframe = keyframes_[frame];

            /* Get next keyframes */
            if (to != positionKeyframes.end() && frame == to->frame)
            {
                from = to;
                ++to;
            }

            if (frame == from->frame || from == to || to == positionKeyframes.end())
            {
                /* Set non-interpolated keyframe */
                keyframe.SetPosition(from->vector);
            }
            else
            {
                /* Interpolate keyframes */
                auto t = FrameInterpolator(from->frame, to->frame, frame);
                keyframe.SetPosition(Math::Lerp(from->vector, to->vector, t));
            }
        }
    }

    /* --- Rotation keyframes --- */

    if (!rotationKeyframes.empty())
    {
        auto from   = rotationKeyframes.begin();
        auto to     = from;

        for (size_t frame = 0; frame < numFrames; ++frame)
        {
            auto& keyframe = keyframes_[frame];

            /* Get next keyframes */
            if (to != rotationKeyframes.end() && frame == to->frame)
            {
                from = to;
                ++to;
            }

            if (frame == from->frame || from == to || to == rotationKeyframes.end())
            {
                /* Set non-interpolated keyframe */
                keyframe.SetRotation(from->quaternion);
            }
            else
            {
                /* Interpolate keyframes */
                auto t = FrameInterpolator(from->frame, to->frame, frame);
                Math::Quaternionf rotation;
                rotation.SLerp(from->quaternion, to->quaternion, t);
                keyframe.SetRotation(rotation);
            }
        }
    }

    /* --- Scale keyframes --- */

    if (!scaleKeyframes.empty())
    {
        auto from   = scaleKeyframes.begin();
        auto to     = from;

        for (size_t frame = 0; frame < numFrames; ++frame)
        {
            auto& keyframe = keyframes_[frame];

            /* Get next keyframes */
            if (to != scaleKeyframes.end() && frame == to->frame)
            {
                from = to;
                ++to;
            }

            if (frame == from->frame || from == to || to == scaleKeyframes.end())
            {
                /* Set non-interpolated keyframe */
                keyframe.SetScale(from->vector);
            }
            else
            {
                /* Interpolate keyframes */
                auto t = FrameInterpolator(from->frame, to->frame, frame);
                keyframe.SetScale(Math::Lerp(from->vector, to->vector, t));
            }
        }
    }
}

/*
This internal template function is used by the "FirstFrame".
If the index is zero, the function returns immediately since there is no smaller value.
*/
template <class T> bool FindMinFrameIndex(const T& keyframeList, size_t& firstFrame)
{
    const size_t minFrame = 0;

    for (const auto& keyframe : keyframeList)
    {
        if (firstFrame > keyframe.frame)
        {
            firstFrame = keyframe.frame;
            if (firstFrame == minFrame)
                return true;
        }
    }

    return false;
}

/*
This function searched the first frame index by iterating over all three keyframe lists.
The minimal frame index will be searched in each iteration.
*/
size_t KeyframeSequence::FirstFrame() const
{
    size_t firstFrame = std::numeric_limits<size_t>::max();

    if (FindMinFrameIndex(positionKeyframes, firstFrame))
        return firstFrame;
    if (FindMinFrameIndex(rotationKeyframes, firstFrame))
        return firstFrame;
    if (FindMinFrameIndex(scaleKeyframes, firstFrame))
        return firstFrame;

    return firstFrame;
}

/*
This internal template function is used by the "LastFrame".
If the index is the maximal value for unsigned integers,
the function returns immediately since there is no higher value.
*/
template <class T> bool FindMaxFrameIndex(const T& keyframeList, size_t& lastFrame)
{
    const size_t maxFrame = std::numeric_limits<size_t>::max();

    for (const auto& keyframe : keyframeList)
    {
        if (lastFrame < keyframe.frame)
        {
            lastFrame = keyframe.frame;
            if (lastFrame == maxFrame)
                return true;
        }
    }

    return false;
}

/*
This function works similiar to the "FirstFrame" function.
In this case we search the maximal frame index.
*/
size_t KeyframeSequence::LastFrame() const
{
    size_t lastFrame = 0;

    if (FindMaxFrameIndex(positionKeyframes, lastFrame))
        return lastFrame;
    if (FindMaxFrameIndex(rotationKeyframes, lastFrame))
        return lastFrame;
    if (FindMaxFrameIndex(scaleKeyframes, lastFrame))
        return lastFrame;

    return lastFrame;
}

void KeyframeSequence::Interpolate(
    Math::Transform3Df& transform, size_t frameFrom, size_t frameTo, float interpolator) const
{
    /* Check if frame indices are out-of-range */
    const auto numFrames = keyframes_.size();
    if (frameFrom >= numFrames || frameTo >= numFrames)
        return;

    /* Get keyframe transformations */
    const auto& transformFrom   = keyframes_[ frameFrom ];
    const auto& transformTo     = keyframes_[ frameTo   ];

    /* Interpolate keyframes */
    transform.Interpolate(transformFrom, transformTo, interpolator);
}

/* Maximal size type (to be consistent along with 32- and 64 bit applications) for the <keyframe-sequence file format> */
typedef unsigned long long FormatSizeType;

void KeyframeSequence::WriteToFile(IO::File& file) const
{
    /* Write all positions */
    file.Write(static_cast<FormatSizeType>(positionKeyframes.size()));
    
    for (const auto& keyframe : positionKeyframes)
    {
        file.Write(static_cast<FormatSizeType>(keyframe.frame));
        file.Write(keyframe.vector);
    }

    /* Write all rotations */
    file.Write(static_cast<FormatSizeType>(rotationKeyframes.size()));
    
    for (const auto& keyframe : rotationKeyframes)
    {
        file.Write(static_cast<FormatSizeType>(keyframe.frame));
        file.Write(keyframe.quaternion);
    }

    /* Write all scales */
    file.Write(static_cast<FormatSizeType>(scaleKeyframes.size()));
    
    for (const auto& keyframe : scaleKeyframes)
    {
        file.Write(static_cast<FormatSizeType>(keyframe.frame));
        file.Write(keyframe.vector);
    }
}

void KeyframeSequence::ReadFromFile(IO::File& file, bool clampEdges)
{
    /* Read all positions */
    positionKeyframes.resize(static_cast<size_t>(file.Read<FormatSizeType>()));

    for (auto& keyframe : positionKeyframes)
    {
        keyframe.frame = static_cast<size_t>(file.Read<FormatSizeType>());
        keyframe.vector = file.ReadVec3<float>();
    }

    /* Read all rotations */
    rotationKeyframes.resize(static_cast<size_t>(file.Read<FormatSizeType>()));

    for (auto& keyframe : rotationKeyframes)
    {
        keyframe.frame = static_cast<size_t>(file.Read<FormatSizeType>());
        keyframe.quaternion = file.Read<Math::Quaternionf>();
    }

    /* Read all scales */
    scaleKeyframes.resize(static_cast<size_t>(file.Read<FormatSizeType>()));

    for (auto& keyframe : scaleKeyframes)
    {
        keyframe.frame = static_cast<size_t>(file.Read<FormatSizeType>());
        keyframe.vector = file.ReadVec3<float>();
    }

    /* Build keyframes */
    BuildKeyframes(clampEdges);
}


} // /namespace Anim

} // /namespace Fork



// ========================