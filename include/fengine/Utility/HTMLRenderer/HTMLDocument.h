/*
 * HTML document header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_HTML_DOCUMENT_H__
#define __FORK_UTILITY_HTML_DOCUMENT_H__


#include "Lang/XMLParser/XMLParser.h"

#include <vector>


namespace Fork
{

namespace Utility
{


//! HTML document structure. Contains array list of XML tags.
struct HTMLDocument
{
    Lang::XMLParser::XMLTagList tags;
};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================