/*
 * Direct3D 11 query header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_RENDERSYSTEM_QUERY_H__
#define __FORK_D3D11_RENDERSYSTEM_QUERY_H__


#include "Video/RenderSystem/Query.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


//! Direct3D 11 query implementation.
class D3D11Query : public Query
{
    
    public:
        
        /**
        \throws NullPointerException If 'device' is null.
        \throws HRESULT If creating D3D11 query failed.
        */
        D3D11Query(ID3D11Device* device, const Types type);
        ~D3D11Query();

        D3D11Query(const D3D11Query&) = delete;
        D3D11Query& operator = (const D3D11Query&) = delete;

        //! Returns the asynchronous D3D11 query object.
        inline ID3D11Query* GetQueryObject() const
        {
            return queryObject_;
        }

    private:
        
        ID3D11Query* queryObject_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================