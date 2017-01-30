/*
 * Model reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MODEL_READER_H__
#define __FORK_MODEL_READER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Scene/Node/GeometryNode.h"
#include "Scene/Geometry/Skeleton.h"
#include "IO/FileSystem/PathDictionary.h"


namespace Fork
{

namespace Scene
{


DECL_SHR_PTR(ModelReader);

//! Model reader interface.
class FORK_EXPORT ModelReader
{
    
    public:
        
        //! Model reading flags.
        struct Flags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                /**
                Generates tangent-space meshes (TangentSpaceMeshGeometry).
                Simple 3D meshes (Simple3DMeshGeoemtry) are the default.
                \remarks This can be used for normal mapping effects such as per-pixel bump mapping.
                \see TangentSpaceMeshGeometry
                \see Simple3DMeshGeoemtry
                */
                GenerateTangentSpace    = (1 << 0),
                /**
                Optimizes the geometry node graph. This may reduces unnecessary texture bindings dramatically!
                \see GeometryConverter::OptimizeGeometryGraph
                */
                OptimizeGeometryGraph   = (1 << 1),
            };
        };

        //! Simple structure to hold a reference to a model and its animation.
        struct AnimatedModel
        {
            GeometryPtr         model;
          //Anim::AnimationPtr  animation;

            //! Returns true if 'model' is not null.
            inline operator bool () const
            {
                return model != nullptr;
            }
        };

        virtual ~ModelReader();

        /**
        Reads the model data from the specified file.
        \param[in] filename Specifies the filename from which the model is to be read.
        \param[in] texPathDict Optional texture path dictionary. This can be used
        to add several search paths to find the textures used by the model. By default the filename's path is used only.
        \param[in] flags Optional model reading flags. This can be a bitwise combination of the "Flags" entries. By default 0.
        \return Shared pointer to the geometry scene node. If an error occured while loading the model,
        the return value is null and the error message will be printed into the log output.
        \remarks To load the skeleton and the animation for this model, use "ReadAnimatedModel".
        \see Flags
        \see ReadAnimatedModel
        */
        virtual GeometryPtr ReadModel(
            const std::string& filename,
            const IO::PathDictionary& texPathDict = {},
            const Flags::DataType flags = 0
        ) = 0;
        /**
        Reads the model data, its skeleton and its animation from the specified file.
        \remarks If the model has no animation, 'skeleton' and 'animation' will be null.
        \see ReadModel
        */
        virtual AnimatedModel ReadAnimatedModel(
            const std::string& filename,
            const IO::PathDictionary& texPathDict = {},
            const Flags::DataType flags = 0
        );

    protected:

        ModelReader() = default;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================