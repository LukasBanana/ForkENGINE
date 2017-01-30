/*
 * FKS reader file
 * 
 * This file is part of the "FKS-FileHandler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "FKSReader.h"
#include "FKSVersion.h"
#include "FKSStringHelper.h"


namespace FKS
{


/*
 * Internal macros
 */

#define BOOLEAN(m)  m = ReadBoolean();

#define CHAR(m)     m = ReadChar();
#define WCHAR(m)    m = ReadWChar();

#define INT8(m)     m = ReadInt8();
#define UINT8(m)    m = ReadUInt8();
#define INT16(m)    m = ReadInt16();
#define UINT16(m)   m = ReadUInt16();
#define INT32(m)    m = ReadInt32();
#define UINT32(m)   m = ReadUInt32();
#define INT64(m)    m = ReadInt64();
#define UINT64(m)   m = ReadUInt64();

#define FLOAT32(m)  m = ReadFloat32();
#define FLOAT64(m)  m = ReadFloat64();

#define STRING(m)   m = ReadString();
#define WSTRING(m)  m = ReadWString();

#define VEC2F(m)    m = ReadVector2f();
#define VEC3F(m)    m = ReadVector3f();
#define VEC4F(m)    m = ReadVector4f();

#define VEC2I(m)    m = ReadVector2i();
#define VEC3I(m)    m = ReadVector3i();
#define VEC4I(m)    m = ReadVector4i();

#define MAT2F(m)    m = ReadMatrix2f();
#define MAT3F(m)    m = ReadMatrix3f();
#define MAT4F(m)    m = ReadMatrix4f();

#define QUAT(m)     m = ReadQuaternion();

#define VERSION(v) if (formatVersion_ >= v)


/*
 * SceneReader class
 */

ChunkListPtr SceneReader::ReadSceneFile(const std::string& filename)
{
    errorMsg_.clear();

    chunkList_ = std::make_shared<ChunkList>();

    /* Open file for reading */
    file_.open(filename, std::ios_base::binary);

    /* Query file size */
    file_.seekg(0, std::ios_base::end);
    fileSize_ = static_cast<size_t>(file_.tellg());
    file_.seekg(0, std::ios_base::beg);

    /* Start reading file */
    try
    {
        ReadFileHeader();
        ReadChunkDescList();
        ReadChunks();
    }
    catch (const std::string& err)
    {
        errorMsg_ = err;
        chunkList_ = nullptr;
    }

    file_.close();

    return chunkList_;
}


/*
 * ======= Private: =======
 */

void SceneReader::Error(const std::string& err)
{
    if (stateDesc_.empty())
        throw std::string(err);
    else
        throw std::string(err + " (in state: " + stateDesc_ + ")");
}

/* --- Base structures --- */

Boolean SceneReader::ReadBoolean()
{
    return Read<Boolean>();
}

Char SceneReader::ReadChar()
{
    return Read<Char>();
}

WChar SceneReader::ReadWChar()
{
    return Read<WChar>();
}

Int8 SceneReader::ReadInt8()
{
    return Read<Int8>();
}

UInt8 SceneReader::ReadUInt8()
{
    return Read<UInt8>();
}

Int16 SceneReader::ReadInt16()
{
    return Read<Int16>();
}

UInt16 SceneReader::ReadUInt16()
{
    return Read<UInt16>();
}

Int32 SceneReader::ReadInt32()
{
    return Read<Int32>();
}

UInt32 SceneReader::ReadUInt32()
{
    return Read<UInt32>();
}

Int64 SceneReader::ReadInt64()
{
    return Read<Int64>();
}

UInt64 SceneReader::ReadUInt64()
{
    return Read<UInt64>();
}

Float32 SceneReader::ReadFloat32()
{
    return Read<Float32>();
}

Float64 SceneReader::ReadFloat64()
{
    return Read<Float64>();
}

std::string SceneReader::ReadString()
{
    /* Read string length */
    size_t len = static_cast<size_t>(Read<UInt32>());

    if (len >= fileSize_)
        Error("UTF-8 string is too long");

    /* Read string */
    std::string str;
    str.resize(len);

    file_.read(&str[0], len);

    return str;
}

std::wstring SceneReader::ReadWString()
{
    /* Read string length */
    size_t len = static_cast<size_t>(Read<UInt32>());
    const size_t size = len*sizeof(WChar);

    if (size >= fileSize_)
        Error("UTF-16 string is too long");

    /* Read string */
    std::wstring str;
    str.resize(len);

    file_.read(reinterpret_cast<char*>(&str[0]), size);

    return str;
}

Vector2f SceneReader::ReadVector2f()
{
    Vector2f vec;

    FLOAT32( vec.x )
    FLOAT32( vec.y )

    return vec;
}

Vector3f SceneReader::ReadVector3f()
{
    Vector3f vec;

    FLOAT32( vec.x )
    FLOAT32( vec.y )
    FLOAT32( vec.z )

    return vec;
}

Vector4f SceneReader::ReadVector4f()
{
    Vector4f vec;

    FLOAT32( vec.x )
    FLOAT32( vec.y )
    FLOAT32( vec.z )
    FLOAT32( vec.w )

    return vec;
}

Vector2i SceneReader::ReadVector2i()
{
    Vector2i vec;

    INT32( vec.x )
    INT32( vec.y )

    return vec;
}

Vector3i SceneReader::ReadVector3i()
{
    Vector3i vec;

    INT32( vec.x )
    INT32( vec.y )
    INT32( vec.z )

    return vec;
}

Vector4i SceneReader::ReadVector4i()
{
    Vector4i vec;

    INT32( vec.x )
    INT32( vec.y )
    INT32( vec.z )
    INT32( vec.w )

    return vec;
}

Matrix2f SceneReader::ReadMatrix2f()
{
    Matrix2f mat;

    VEC2F( mat.x )
    VEC2F( mat.y )

    return mat;
}

Matrix3f SceneReader::ReadMatrix3f()
{
    Matrix3f mat;

    VEC3F( mat.x )
    VEC3F( mat.y )
    VEC3F( mat.z )

    return mat;
}

Matrix4f SceneReader::ReadMatrix4f()
{
    Matrix4f mat;

    VEC4F( mat.x )
    VEC4F( mat.y )
    VEC4F( mat.z )
    VEC4F( mat.w )

    return mat;
}

Quaternion SceneReader::ReadQuaternion()
{
    Quaternion quat;

    FLOAT32( quat.x )
    FLOAT32( quat.y )
    FLOAT32( quat.z )
    FLOAT32( quat.w )

    return quat;
}

Transform2Df SceneReader::ReadTransform2Df()
{
    Transform2Df transform;

    VEC2F  ( transform.position )
    FLOAT32( transform.rotation )
    VEC2F  ( transform.scale    )

    return transform;
}

Transform3Df SceneReader::ReadTransform3Df()
{
    Transform3Df transform;

    VEC3F( transform.position )
    QUAT ( transform.rotation )
    VEC3F( transform.scale    )

    return transform;
}

Viewport SceneReader::ReadViewport()
{
    Viewport viewport;

    VEC2I  ( viewport.position )
    VEC2I  ( viewport.size     )
    FLOAT32( viewport.minDepth )
    FLOAT32( viewport.maxDepth )

    return viewport;
}

Projection SceneReader::ReadProjection()
{
    Projection projection;

    projection.viewport = ReadViewport();

    FLOAT32( projection.nearPlane   )
    FLOAT32( projection.farPlane    )
    FLOAT32( projection.fieldOfView )
    BOOLEAN( projection.isOrtho     )

    return projection;
}

ChunkDesc SceneReader::ReadChunkDesc()
{
    ChunkDesc chunkDesc;

    UINT32( chunkDesc.id     )
    UINT32( chunkDesc.offset )
    UINT32( chunkDesc.size   )

    return chunkDesc;
}

/* --- Main structures --- */

void SceneReader::ReadFileHeader()
{
    /* Check magic number */
    UINT32( auto magic )

    if (magic != __FKS_FORMAT_MAGIC_NUMBER__)
        Error("Wrong magic number");

    /* Check format versions */
    UINT32( formatVersion_             )
    UINT32( auto lastCompatibleVersion )

    if (lastCompatibleVersion > __FKS_FORMAT_VERSION__)
        Error("Last compatible format version (" + ToStr(lastCompatibleVersion) + ") not supported");
}

void SceneReader::ReadChunkDescList()
{
    
}

void SceneReader::ReadChunks()
{
    
}


} // /namespace FKS



// ========================