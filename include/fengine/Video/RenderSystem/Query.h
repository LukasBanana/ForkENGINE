/*
 * Renderer query header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDERSYSTEM_QUERY_H__
#define __FORK_RENDERSYSTEM_QUERY_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(Query);

/**
Render system query interface. Query objects can be used to query information from the GPU,
e.g. how many pixels have been drawn from the previous render pass.
\code
auto query = renderSystem->CreateQuery(Video::Query::Types::SamplesPassed);
//...
renderContext->Begin(query.get());
{
    // render scene ...
}
renderContext->End(query.get());
//...
auto numSamplesPassed = renderContext->Result(query.get());
\endcode
*/
class FORK_EXPORT Query
{
    
    public:
        
        //! Query object types.
        enum class Types
        {
            SamplesPassed,          //!< Queries how many fragment (or rather pixel) samples have passed the depth test.
            AnySamplesPassed,       //!< Queries if any fragment (or rather pixel) samples have passed the depth test.
            PrimitivesGenerated,    //!< Records the number of primitives sent to a particular geometry shader output stream.
            TimeStamp,              //!< Records the current timestamp on the GPU.
        };

        /**
        Query states enumeration. This is used by the RenderContext.
        \see RenderContext::Begin(Query*)
        \see RenderContext::End(Query*)
        \see RenderContext::Result(Query*)
        */
        struct StateFlags
        {
            typedef int DataType;
            enum : DataType
            {
                /**
                The query is ready to begin. This is the initial state.
                At this point "RenderContext::End" and "RenderContext::Result" can not be called.
                */
                ReadyToBegin    = (1 << 0),
                /**
                The query waits to end. At this point "RenderContext::Begin"
                and "RenderContext::Result" can not be called!
                */
                WaitingForEnd   = (1 << 1),
                /**
                The result is available. At this point "RenderContext::Begin"
                or "RenderContext::Result" can be called.
                */
                ResultAvailable = (1 << 2),
            };
        };

        virtual ~Query()
        {
        }

        Query(const Query&) = delete;
        Query& operator = (const Query&) = delete;

        //! Returns the query object type.
        inline Types GetType() const
        {
            return type_;
        }

    protected:
        
        Query(const Types type) :
            type_{ type }
        {
        }

    private:
        
        Types type_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================