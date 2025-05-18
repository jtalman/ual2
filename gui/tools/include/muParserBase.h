/** 
  Copyright (C) 2004, 2005 Ingo Berg

  Permission is hereby granted, free of charge, to any person obtaining a copy of this 
  software and associated documentation files (the "Software"), to deal in the Software
  without restriction, including without limitation the rights to use, copy, modify, 
  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
  permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or 
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
*/
#ifndef MU_PARSER_BASE_H
#define MU_PARSER_BASE_H

#include <cmath>
#include <string>
#include <iostream>
#include <map>
#include <memory>

#include "muParserDef.h"
#include "muParserStack.h"
#include "muBasicParserTokenReader.h"
#include "muParserBytecode.h"
#include "muParserError.h"
#include "muIParserTypes.h"


namespace mu
{

/** \brief Mathematical expressions parser.
  
  Version 1.2

  This is the implementation of a bytecode based mathematical expressions parser. 
  The formula will be parsed from string and converted into a bytecode. 
  Future calculations will be done with the bytecode instead the formula string
  resulting in a significant performance increase. 
  Complementary to a set of internally implemented functions the parser is able to handle 
  user defined functions and variables. 

  \author (C) 2004, 2005 Ingo Berg
*/
class ParserBase : public IParserTypes<MU_PARSER_BASETYPE, MU_PARSER_STRINGTYPE>
{
  // friend class ParserTokenReader<MU_PARSER_BASETYPE, MU_PARSER_STRINGTYPE>;

public:
    typedef IParserTypes<MU_PARSER_BASETYPE, MU_PARSER_STRINGTYPE> traits_type;

private:
    typedef value_type (ParserBase::*ParseFunction)() const;  
    typedef ParserToken<value_type, MU_PARSER_STRINGTYPE> token_type;
    typedef ParserByteCode<value_type> bytecode_type;
    typedef std::vector<string_type> stringbuf_type;
    typedef BasicParserTokenReader<value_type, string_type> token_basic_reader_type;

    static const char_type *c_DefaultOprt[]; 

 public:
    ParserBase(); 
    ParserBase( const ParserBase &a_Parser );
    ParserBase& operator=(const ParserBase &a_Parser);

    //---------------------------------------------------------------------------
    /** \brief Destructor. (trivial) 

        \throw nothrow
    */
    virtual ~ParserBase()
    {}
    
    //---------------------------------------------------------------------------
    /** \brief Calculate the result.

      A note on const correctness: 
      I consider it important that Calc is a const function.
      Due to caching operations Calc changes only the state of internal variables with one exception
      m_UsedVar this is reset during string parsing and accessible from the outside. Instead of making
      Calc non const GetUsedVar is non const because it explicitely calls Eval() forcing this update. 

      \pre A formula must be set.
      \pre Variables must have been set (if needed)
  
      \sa #m_pParseFormula
      \return The evaluation result
      \throw ParseException if no Formula is set or in case of any other error related to the formula.
    */
  	inline value_type Eval() const
    {
      return (this->*m_pParseFormula)(); 
    }

    void SetExpr(const std::string &a_sExpr);
    void SetVarFactory(facfun_type a_pFactory);

    void EnableOptimizer(bool a_bIsOn=true);
    void EnableByteCode(bool a_bIsOn=true);
    void EnableBuiltInOprt(bool a_bIsOn=true);

    bool HasBuiltInOprt() const;
    void AddValIdent(identfun_type a_pCallback);

    template<typename TCallBack>
    void DefineFun(const string_type &a_strName, TCallBack *a_pFun, bool a_bAllowOpt = true)
    {
      AddCallback(a_strName, callback_type(a_pFun, a_bAllowOpt), m_FunDef, ValidNameChars());
    }

    void DefineOprt(const string_type &a_strName, fun_type2 a_pFun, unsigned a_iPri=0, bool a_bAllowOpt = false);
    void DefineConst(const string_type &a_sName, value_type a_fVal);
    void DefineStrConst(const string_type &a_sName, const string_type &a_strVal);
    void DefineVar(const string_type &a_sName, value_type *a_fVar);
    void DefinePostfixOprt(const string_type &a_strFun, fun_type1 a_pOprt, bool a_bAllowOpt=true);
    void DefineInfixOprt(const string_type &a_strName, fun_type1 a_pOprt, bool a_bAllowOpt=true);

    // Clear user defined variables, constants or functions
  	void ClearVar();
    void ClearFun();
    void ClearConst();
	  void ClearInfixOprt();
    void ClearPostfixOprt();
    void ClearOprt();
    
    void RemoveVar(const string_type &a_strVarName);
    const varmap_type& GetUsedVar() const;
    const varmap_type& GetVar() const;
    const valmap_type& GetConst() const;
    const string_type& GetExpr() const;
    const funmap_type& GetFunDef() const;

    //---------------------------------------------------------------------------
    /** \brief Return the strings of all Operator identifiers. 
    
        GetOprt is a const function returning a pinter to an array of const char pointers.
        
        \return Returns a pointer to the c_DefaultOprt array of const char *.
        \throw nothrow
    */
    const char_type ** GetOprtDef() const
    {
      return (const char **)(&c_DefaultOprt[0]);
    }

    //---------------------------------------------------------------------------
    /** \brief Define the set of valid characters to be used in names of
               functions, variables, constants.
    */
    void DefineNameChars(const char_type *a_szCharset)
    {
      m_sNameChars = a_szCharset;
    }

    //---------------------------------------------------------------------------
    /** \brief Define the set of valid characters to be used in names of
               binary operators and postfix operators.
    */
    void DefineOprtChars(const char_type *a_szCharset)
    {
      m_sOprtChars = a_szCharset;
    }

    //---------------------------------------------------------------------------
    /** \brief Define the set of valid characters to be used in names of
               infix operators.
    */
    void DefineInfixOprtChars(const char_type *a_szCharset)
    {
      m_sInfixOprtChars = a_szCharset;
    }

    //---------------------------------------------------------------------------
    /** \brief Virtual function that defines the characters allowed in name identifiers. 
        \sa #ValidOprtChars, #ValidPrefixOprtChars
    */ 
    const char_type* ValidNameChars() const
    {
      return m_sNameChars.c_str();
    }

    //---------------------------------------------------------------------------
    /** \brief Virtual function that defines the characters allowed in operator definitions. 
        \sa #ValidNameChars, #ValidPrefixOprtChars
    */
    const char_type* ValidOprtChars() const
    {
      return m_sOprtChars.c_str();
    }

    //---------------------------------------------------------------------------
    /** \brief Virtual function that defines the characters allowed in infix operator definitions.
        \sa #ValidNameChars, #ValidOprtChars
    */
    const char_type* ValidInfixOprtChars() const
    {
      return m_sInfixOprtChars.c_str();
    }

    void  Error( EErrorCodes a_iErrc, 
                 int a_iPos = traits_type::string_type::npos, 
                 const string_type &a_strTok = traits_type::string_type() ) const;

 protected:
	  
    //---------------------------------------------------------------------------
    /** \brief Initialize user defined functions. 
     
      Calls the virtual functions InitFun(), InitConst() and InitOprt().
    */
    void Init()
    {
      InitCharSets();
      InitFun();
      InitConst();
      InitOprt();
    }

    //---------------------------------------------------------------------------
    virtual void InitCharSets() = 0;
    virtual void InitFun() = 0;
	  virtual void InitConst() = 0;
    virtual void InitOprt() = 0; 

 private:
    void Assign(const ParserBase &a_Parser);
    void InitTokenReader();
    void ReInit() const;

    void AddCallback(const string_type &a_strName, 
                     const callback_type &a_Callback, 
                     funmap_type &a_Storage,
                     const char_type *a_szCharSet);

    void ApplyBinOprt(ParserStack<token_type> &a_stOpt,
                      ParserStack<token_type> &a_stVal) const;

    void ApplyFunc(ParserStack<token_type> &a_stOpt,
                   ParserStack<token_type> &a_stVal, 
                   int iArgCount) const; 

    token_type ApplyNumFunc(const token_type &a_FunTok,
                            const std::vector<token_type> &a_vArg) const;

    token_type ApplyStrFunc(const token_type &a_FunTok,
                            token_type &a_Arg) const;

    int GetOprtPri(const token_type &a_Tok) const;

    value_type ParseString() const; 
    value_type ParseCmdCode() const;
    value_type ParseValue() const;

    void  ClearFormula();
    void  CheckName(const string_type &a_strName, const string_type &a_CharSet) const;

#if defined(MU_PARSER_DUMP_STACK) | defined(MU_PARSER_DUMP_CMDCODE)
    void StackDump(const ParserStack<token_type > &a_stVal, 
                   const ParserStack<token_type > &a_stOprt) const;
#endif

    /** \brief Pointer to the parser function. 
    
      Eval() calls the function whose address is stored there.
    */
    mutable ParseFunction m_pParseFormula;
    mutable const int *m_pCmdCode;        ///< Formula converted to bytecode, points to the data of the bytecode class.
    mutable bytecode_type m_vByteCode;    ///< The Bytecode class.

 public:
    mutable stringbuf_type  m_vStringBuf; ///< String buffer, used for storing string function arguments
    stringbuf_type  m_vStringVarBuf;

 private:

    /** \brief Managed pointer to the token reader object. */
    std::auto_ptr<token_basic_reader_type> m_pTokenReader; 

    funmap_type  m_FunDef;        ///< Map of function names and pointers.
    funmap_type  m_PostOprtDef;   ///< Postfix operator callbacks
    funmap_type  m_InfixOprtDef;  ///< unary infix operator.
    funmap_type  m_OprtDef;       ///< Binary operator callbacks
    valmap_type  m_ConstDef;      ///< user constants.
    strmap_type  m_StrVarDef;     ///< user defined string constants
    varmap_type  m_VarDef;        ///< user defind variables.

    stringbuf_type  m_vErrMsg;    ///< Storage for error messages
    bool m_bOptimize;             ///< Flag that indicates if the optimizer is on or off.
    bool m_bUseByteCode;          ///< Flag that indicates if bytecode parsing is on or off.
    bool m_bBuiltInOp;            ///< Flag that can be used for switching built in operators on and off

    string_type m_sNameChars;      ///< Charset for names
    string_type m_sOprtChars;      ///< Charset for postfix/ binary operator tokens
    string_type m_sInfixOprtChars; ///< Charset for infix operator tokens
};

} // namespace mu

#endif

