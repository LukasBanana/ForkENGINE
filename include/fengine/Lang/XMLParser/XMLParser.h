/*
 * XML parser header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XML_PARSER_H__
#define __FORK_XML_PARSER_H__


#include "Lang/SyntaxAnalyzer/Parser.h"
#include "Lang/XMLParser/XMLTag.h"
#include "Lang/XMLParser/XMLAttribute.h"
#include "Lang/XMLParser/XMLContent.h"


namespace Fork
{

namespace Lang
{


//! XML parser (or syntax analyzer).
class FORK_EXPORT XMLParser : public SyntaxAnalyzer::Parser
{
    
    public:
        
        typedef std::vector<AbstractSyntaxTrees::XMLTagPtr> XMLTagList;

        XMLParser();

        XMLTagList ParseXMLFile(const std::string& filename);
        XMLTagList ParseXMLString(const std::string& xmlContent);

    private:
        
        bool IsTokenContent() const;

        AbstractSyntaxTrees::XMLTagPtr          ParseTag        (std::string& tagEnd);
        AbstractSyntaxTrees::XMLAttributePtr    ParseAttribute  ();
        AbstractSyntaxTrees::XMLContentPtr      ParseContent    ();

};


} // /namespace Lang

} // /namespace Fork


#endif



// ========================