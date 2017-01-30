/*
 * FKS reader header
 * 
 * This file is part of the "FKS-FileHandler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FKS_READER_H__
#define __FKS_READER_H__


#include "FKSMainStructures.h"
#include "FKSExport.h"

#include <fstream>
#include <memory>
#include <string>


namespace FKS
{


typedef std::shared_ptr<ChunkList> ChunkListPtr;

class FKS_EXPORT SceneReader
{
    
    public:
        
        ChunkListPtr ReadSceneFile(const std::string& filename);

        //! Returns the error message when reading the scene has failed.
        inline std::string GetErrorMessage() const
        {
            return errorMsg_;
        }

    private:
        
        /* === Functions === */

        void Error(const std::string& err);

        /* --- Base reading functions --- */

        template <typename T> void Read(T& val)
        {
            file_.read(reinterpret_cast<char*>(&val), sizeof(val));
        }
        template <typename T> T Read()
        {
            T val;
            Read<T>(val);
            return val;
        }

        /* --- Base structures --- */

        Boolean         ReadBoolean     ();

        Char            ReadChar        ();
        WChar           ReadWChar       ();

        Int8            ReadInt8        ();
        UInt8           ReadUInt8       ();
        Int16           ReadInt16       ();
        UInt16          ReadUInt16      ();
        Int32           ReadInt32       ();
        UInt32          ReadUInt32      ();
        Int64           ReadInt64       ();
        UInt64          ReadUInt64      ();

        Float32         ReadFloat32     ();
        Float64         ReadFloat64     ();

        std::string     ReadString      ();
        std::wstring    ReadWString     ();

        Vector2f        ReadVector2f    ();
        Vector3f        ReadVector3f    ();
        Vector4f        ReadVector4f    ();

        Vector2i        ReadVector2i    ();
        Vector3i        ReadVector3i    ();
        Vector4i        ReadVector4i    ();

        Matrix2f        ReadMatrix2f    ();
        Matrix3f        ReadMatrix3f    ();
        Matrix4f        ReadMatrix4f    ();

        Quaternion      ReadQuaternion  ();

        Transform2Df    ReadTransform2Df();
        Transform3Df    ReadTransform3Df();

        Viewport        ReadViewport    ();
        Projection      ReadProjection  ();

        ChunkDesc       ReadChunkDesc   ();

        /* --- Main structures --- */

        void ReadFileHeader();
        void ReadChunkDescList();
        void ReadChunks();

        /* --- Chunks --- */



        /* === Members === */

        std::ifstream file_;
        size_t fileSize_ = 0;

        ChunkListPtr chunkList_;

        std::string stateDesc_;
        std::string errorMsg_;

        UInt32 formatVersion_ = 0;

};


} // /namespace FKS


#endif



// ========================