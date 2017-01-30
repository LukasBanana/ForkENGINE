/*
 * FSC interpreter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LANG_FSC_INTERPRETER_H__
#define __FORK_LANG_FSC_INTERPRETER_H__


#include "Lang/SyntaxAnalyzer/Parser.h"
#include "Lang/FSCInterpreter/FSCScopeManager.h"
#include "Lang/AbstractSyntaxTrees/FSC/StructNameIdent.h"


namespace Fork
{

namespace Lang
{


/**
The FSC (ForkSCript) interpreter can be used to read, parse and interpret a ForkSCript file.
This interpreter is a single-pass compiler. This class can be extended to extend the script features.
The "Parse..." functions follow exactly the ForkSCript grammar specification,
which can be found in "trunk/docu/Specs/ForkSCript Grammar Spec.txt".
\note Tha FSCInterpreter direclty parses the script and thus does not create an AST (Abstract Syntax Tree).
\ingroup lang_forkscript
*/
class FORK_EXPORT FSCInterpreter : public SyntaxAnalyzer::Parser
{
    
    public:
        
        FSCInterpreter();
        virtual ~FSCInterpreter();

        //! Runs the script read from the specified file.
        bool RunScriptFromFile(const std::string& filename);

        //! Runs the script read from the specified source code string.
        bool RunScript(const std::string& sourceCode);

        /**
        Trys to find the specified variable object in the global scope.
        \see VarObject
        */
        VarObjectPtr Fetch(const std::string& varName);

    private:
        
        /* === Functions === */

        void ReadSourceFile(const std::string& filename);

        /* --- Parsing functions --- */

        void ParseScript();

        void ParseStatementList();
        void ParseSingleStatement();
        void ParseSingleStmntKeyword();

        void ParseArgumentList();
        void ParseSingleArgument();

        void ParseInitializerList();
        void ParseSingleInitializer();

        void ParseArrayEntryList();
        void ParseSingleArrayEntry();

        void ParseArrayAccessList();
        void ParseSingleArrayAccess();

        void ParseStructNameStmnt();
        void ParseVarDeclStmnt();
        void ParseIncludeStmnt();
        void ParseForStmnt();
        void ParseIfStmnt();
        void ParseElIfStmnt();
        void ParseElseStmnt();
        void ParseBlockStmnt();

        void ParseExpression();
        void ParseSumExpr();
        void ParseProductExpr();
        void ParseValueExpr();
        void ParseStructNameExpr();
        void ParseNegationExpr();
        void ParseBracketExpr();
        void ParseLiteralExpr();

        int ParseIntegerLiteral();
        float ParseFloatLiteral();
        std::string ParseStringLiteral();

        void ParseAssignment();
        void ParseNullListAsmnt();
        void ParseListAsmnt();
        void ParseAllocAsmnt();
        void ParseExpressionAsmnt();

        SyntaxAnalyzer::TokenPtr ParseIdentifier();
        AbstractSyntaxTrees::FSCStructNameIdentPtr ParseStructNameIdent();
        void ParseObjectAllocation();

        /* === Members === */

        FSCScopeManager scopeMngr_;
        FSCScopePtr globalScope_;

};


} // /namespace Lang

} // /namespace Fork


#endif



// ========================