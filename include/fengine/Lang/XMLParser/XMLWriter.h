/*
 * XML writer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XML_WRITER_H__
#define __FORK_XML_WRITER_H__


#include "Lang/XMLParser/XMLParser.h"

#include <ostream>


namespace Fork
{

namespace Lang
{

namespace XMLWriter
{


/**
Writes the specified XML tags to the given file.
\param[in] stream Specifies the output stream to which the XML tags are to be written.
This file must be opend in text mode, i.e. don't use "std::ios_base::binary"!
\param[in] xmlTags Specifies the list of XML tags which are to be written to the file.
\see AbstractSyntaxTrees::XMLTag
*/
FORK_EXPORT void WriteXMLTagsToFile(
    std::ostream& stream, const std::vector<AbstractSyntaxTrees::XMLTagPtr>& xmlTags
);


} // /namespace XMLWriter

} // /namespace Lang

} // /namespace Fork


#endif



// ========================