/*
 * Direct3D 11 query file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11Query.h"
#include "D3D11Core.h"
#include "D3D11ParamMapper.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Video
{


D3D11Query::D3D11Query(ID3D11Device* device, const Types type) :
    Query{ type }
{
    ASSERT_POINTER(device);

    /* Setup query description */
    D3D11_QUERY_DESC queryDesc;

    queryDesc.Query     = D3D11ParamMapper::Map(type);
    queryDesc.MiscFlags = 0;

    /* Create query object */
    auto result = device->CreateQuery(&queryDesc, &queryObject_);
    DXAssert(result, "Creating D3D11 query failed");
}
D3D11Query::~D3D11Query()
{
    DXSafeRelease(queryObject_);
}


} // /namespace Video

} // /namespace Fork



// ========================