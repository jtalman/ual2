/*
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
#ifndef MU_PARSER_ERROR_H
#define MU_PARSER_ERROR_H

#include <cassert>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <memory>


namespace mu
{

/** \brief Error codes. */
enum EErrorCodes
{
  // Formula syntax errors
  ecUNEXPECTED_OPERATOR =  0, ///< Unexpected binary operator found
  ecUNASSIGNABLE_TOKEN,       ///< Token cant be identified.
  ecUNEXPECTED_EOF,           ///< Unexpected end of formula. (Example: "2+sin(")
  ecUNEXPECTED_COMMA,         ///< An unexpected comma has been found. (Example: "1,23")
  ecUNEXPECTED_ARG,           ///< An unexpected argument has been found
  ecUNEXPECTED_VAL,           ///< An unexpected value token has been found
  ecUNEXPECTED_VAR,           ///< An unexpected variable token has been found
  ecUNEXPECTED_PARENS,        ///< Unexpected Parenthesis, opening or closing
  ecUNEXPECTED_STR,           ///< A string has been found at an inapropriate position
  ecSTRING_EXPECTED,          ///< A string function has been called with a different type of argument
  ecVAL_EXPECTED,             ///< A numerical function has been called with a non value type of argument
  ecMISSING_PARENS,           ///< Missing parens. (Example: "3*sin(3")
  ecUNEXPECTED_FUN,           ///< Unexpected function found. (Example: "sin(8)cos(9)")
  ecUNTERMINATED_STRING,      ///< unterminated string constant. (Example: "3*valueof("hello)")
  ecTOO_MANY_PARAMS,          ///< Too many function parameters
  ecTOO_FEW_PARAMS,           ///< Too few function parameters. (Example: "ite(1<2,2)")
  ecOPRT_TYPE_CONFLICT,       ///< binary operators may only be applied to value items of the same type
  ecSTR_RESULT,               ///< result is a string

  // Invalid Parser input Parameters
  ecINVALID_NAME,             ///< Invalid function, variable or constant name.
  ecBUILTIN_OVERLOAD,         ///< Trying to overload builtin operator
  ecINVALID_FUN_PTR,          ///< Invalid callback function pointer 
  ecINVALID_VAR_PTR,          ///< Invalid variable pointer 

  ecNAME_CONFLICT,            ///< Name conflict
  ecOPT_PRI,                  ///< Invalid operator priority
  // 
  ecDOMAIN_ERROR,             ///< catch division by zero, sqrt(-1), log(0) (currently unused)
  ecDIV_BY_ZERO,              ///< Division by zero (currently unused)
  ecGENERIC,                  ///< Generic error

  // internal errors
  ecINTERNAL_ERROR,           ///< Internal error of any kind.

  // The last two are special entries 
  ecCOUNT,                    ///< This is no error code, It just stores just the total number of error codes
  ecUNDEFINED           = -1 ///< Undefined message, placeholder to detect unassigned error messages
};

//---------------------------------------------------------------------------
template<typename TString>
class ParserErrorMsg
{

public:
  typedef ParserErrorMsg<TString> self_type;

  static ParserErrorMsg& Instance()
  {
    if(m_Instance == 0) m_Instance = new  ParserErrorMsg<TString>();
    return *m_Instance;
  }

  //------------------------------------------------------------------------------
  TString operator[](unsigned a_iIdx) const
  {
    return (a_iIdx<m_vErrMsg.size()) ? m_vErrMsg[a_iIdx] : TString();
  }

private:
  std::vector<TString>  m_vErrMsg;
  static ParserErrorMsg<TString>* m_Instance;

  //---------------------------------------------------------------------------
 ~ParserErrorMsg()
  {}

  //---------------------------------------------------------------------------
  ParserErrorMsg& operator=(const ParserErrorMsg &)
  {}

  //---------------------------------------------------------------------------
  ParserErrorMsg(const ParserErrorMsg&)
  {}

  //---------------------------------------------------------------------------
  ParserErrorMsg()
    :m_vErrMsg(0)
  {
    m_vErrMsg.resize(ecCOUNT);

    m_vErrMsg[ecUNASSIGNABLE_TOKEN]  = "Undefined token \"$TOK$\" found at position $POS$.";
    m_vErrMsg[ecINTERNAL_ERROR]      = "Internal error";
    m_vErrMsg[ecINVALID_NAME]        = "Invalid function-, variable- or constant name.";
    m_vErrMsg[ecINVALID_FUN_PTR]     = "Invalid pointer to callback function.";
    m_vErrMsg[ecINVALID_VAR_PTR]     = "Invalid pointer to variable.";
    m_vErrMsg[ecUNEXPECTED_OPERATOR] = "Unexpected operator \"$TOK$\" found at position $POS$";
    m_vErrMsg[ecUNEXPECTED_EOF]      = "Unexpected end of formula at position $POS$";
    m_vErrMsg[ecUNEXPECTED_COMMA]    = "Unexpected comma at position $POS$";
    m_vErrMsg[ecUNEXPECTED_PARENS]   = "Unexpected parenthesis \"$TOK$\" at position $POS$";
    m_vErrMsg[ecUNEXPECTED_FUN]      = "Unexpected function \"$TOK$\" at position $POS$";
    m_vErrMsg[ecUNEXPECTED_VAL]      = "Unexpected value \"$TOK$\" found at position $POS$";
    m_vErrMsg[ecUNEXPECTED_VAR]      = "Unexpected variable \"$TOK$\" found at position $POS$";
    m_vErrMsg[ecUNEXPECTED_ARG]      = "Function arguments used without a function (position: $POS$)";
    m_vErrMsg[ecMISSING_PARENS]      = "Missing parenthesis";
    m_vErrMsg[ecTOO_MANY_PARAMS]     = "Too many parameters for function \"$TOK$\" at formula position $POS$";
    m_vErrMsg[ecTOO_FEW_PARAMS]      = "Too few parameters for function \"$TOK$\" at formula position $POS$";
    m_vErrMsg[ecDIV_BY_ZERO]         = "Divide by zero";
    m_vErrMsg[ecDOMAIN_ERROR]        = "Domain error";
    m_vErrMsg[ecNAME_CONFLICT]       = "Name conflict";
    m_vErrMsg[ecOPT_PRI]             = "Invalid value for operator priority (must be greater or equal to zero).";
    m_vErrMsg[ecBUILTIN_OVERLOAD]    = "Binary operator identifier conflicts with a built in operator.";
    m_vErrMsg[ecUNEXPECTED_STR]      = "Unexpected string token found at position $POS$.";
    m_vErrMsg[ecUNTERMINATED_STRING] = "Unterminated string starting at position $POS$.";
    m_vErrMsg[ecSTRING_EXPECTED]     = "String function called with a non string type of argument.";
    m_vErrMsg[ecVAL_EXPECTED]        = "Numerical function called with a non value type of argument.";
    m_vErrMsg[ecOPRT_TYPE_CONFLICT]  = "No suitable overload for operator \"$TOK$\" at position $POS$.";
    m_vErrMsg[ecGENERIC]             = "Parser error.";
    m_vErrMsg[ecSTR_RESULT]          = "Function result is a string.";

    #if defined(_DEBUG)
      for (int i=0; i<ecCOUNT; ++i)
        if (!m_vErrMsg[i].length())
          assert(false);
    #endif
  }
};

template<typename TString>
ParserErrorMsg<TString>* ParserErrorMsg<TString>::m_Instance = 0;

//---------------------------------------------------------------------------
/** \brief Error class of the parser. 

  Part of the math parser package.

  \author Ingo Berg
*/
template<typename TStr>
/* final */ class ParserError
{
private:

    typedef TStr string_type;

    /** \brief Type of the string characters. */
    typedef typename string_type::value_type char_type;

    /** \brief Stringstream type depending on string_type. */
    typedef std::basic_stringstream<char_type, 
                                    std::char_traits<char_type>,
                                    std::allocator<char_type> > stringstream_type;

    //------------------------------------------------------------------------------
    /** \brief Replace all ocuurences of a substring with another string. */
    void ReplaceSubString( string_type &strSource, 
                           const string_type &strFind,
                           const string_type &strReplaceWith)
    {
      string_type strResult;
      typename string_type::size_type iPos(0), iNext(0);

      for(;;)
      {
        iNext = strSource.find(strFind, iPos);
        strResult.append(strSource, iPos, iNext-iPos);

        if( iNext==string_type::npos )
          break;

        strResult.append(strReplaceWith);
        iPos = iNext + strFind.length();
      } 

      strSource.swap(strResult);
    }

    //------------------------------------------------------------------------------
    void Reset()
    {
      m_strMsg = "";
      m_strFormula = "";
      m_strTok = "";
      m_iPos = -1;
      m_iErrc = ecUNDEFINED;
    }

public:

    //------------------------------------------------------------------------------
    ParserError()
      :m_strMsg()
      ,m_strFormula()
      ,m_strTok()
      ,m_iPos(-1)
      ,m_iErrc(ecUNDEFINED)
      ,m_ErrMsg(ParserErrorMsg<TStr>::Instance())
    {
    }

    //------------------------------------------------------------------------------
    /** \brief This Constructor is used for internal exceptions only. 
    
      It does not contain any information but the error code.
    */
    explicit ParserError(EErrorCodes a_iErrc) 
      :m_ErrMsg(ParserErrorMsg<string_type>::Instance())
    {
      assert(a_iErrc==ecINTERNAL_ERROR);
      Reset();
      m_strMsg = "internal error";
    }

    //------------------------------------------------------------------------------
    explicit ParserError(const string_type &sMsg) 
      :m_ErrMsg(ParserErrorMsg<string_type>::Instance())
    {
      Reset();
      m_strMsg = sMsg;
    }

    //------------------------------------------------------------------------------
    ParserError( EErrorCodes a_iErrc,
                 const string_type &sTok,
                 const string_type &sFormula = TStr("(formula is not available)"),
                 int a_iPos = -1)
      :m_strMsg()
      ,m_strFormula(sFormula)
      ,m_strTok(sTok)
      ,m_iPos(a_iPos)
      ,m_iErrc(a_iErrc)
      ,m_ErrMsg(ParserErrorMsg<string_type>::Instance())
    {
      m_strMsg = m_ErrMsg[m_iErrc];
      stringstream_type stream;
      stream << (int)m_iPos;
      ReplaceSubString(m_strMsg, "$POS$", stream.str());
      ReplaceSubString(m_strMsg, "$TOK$", m_strTok);
    }

    //------------------------------------------------------------------------------
    ParserError( EErrorCodes a_iErrc, int a_iPos, const string_type &sTok) 
      :m_strMsg()
      ,m_strFormula()
      ,m_strTok(sTok)
      ,m_iPos(a_iPos)
      ,m_iErrc(a_iErrc)
      ,m_ErrMsg(ParserErrorMsg<string_type>::Instance())
    {
      m_strMsg = m_ErrMsg[m_iErrc];
      stringstream_type stream;
      stream << (int)m_iPos;
      ReplaceSubString(m_strMsg, "$POS$", stream.str());
      ReplaceSubString(m_strMsg, "$TOK$", m_strTok);
    }

    //------------------------------------------------------------------------------
    ParserError( const char *a_szMsg, int a_iPos = -1, const string_type &sTok = string_type()) 
      :m_strMsg(a_szMsg)
      ,m_strFormula()
      ,m_strTok(sTok)
      ,m_iPos(a_iPos)
      ,m_iErrc(ecGENERIC)
      ,m_ErrMsg(ParserErrorMsg<string_type>::Instance())
    {
      stringstream_type stream;
      stream << (int)m_iPos;
      ReplaceSubString(m_strMsg, "$POS$", stream.str());
      ReplaceSubString(m_strMsg, "$TOK$", m_strTok);
    }

    //------------------------------------------------------------------------------
    ParserError(const ParserError &a_Obj)
      :m_strMsg(a_Obj.m_strMsg)
      ,m_strFormula(a_Obj.m_strFormula)
      ,m_strTok(a_Obj.m_strTok)
      ,m_iPos(a_Obj.m_iPos)
      ,m_iErrc(a_Obj.m_iErrc)
      ,m_ErrMsg(ParserErrorMsg<string_type>::Instance())
    {
    }

    //------------------------------------------------------------------------------
    ParserError& operator=(const ParserError &a_Obj)
    {
      if (this==&a_Obj)
        return *this;

      m_strMsg = a_Obj.m_strMsg;
      m_strFormula = a_Obj.m_strFormula;
      m_strTok = a_Obj.m_strTok;
      m_iPos = a_Obj.m_iPos;
      m_iErrc = a_Obj.m_iErrc;
      return *this;
    }

    //------------------------------------------------------------------------------
   ~ParserError()
    {};
    
    //------------------------------------------------------------------------------
    void SetFormula(const string_type &a_strFormula)
    {
      m_strFormula = a_strFormula;
    }

    //------------------------------------------------------------------------------
    const string_type& GetExpr() const 
    {
  	  return m_strFormula;
    }

    //------------------------------------------------------------------------------
    const string_type& GetMsg() const
    {
      return m_strMsg;
    }

    //------------------------------------------------------------------------------
    /** \brief Return the formula position related to the error. 
    
      If the error is not related to a distinct position this will return -1
    */
    std::size_t GetPos() const
    {
      return m_iPos;
    }

    //------------------------------------------------------------------------------
    /** \brief Return string related with this token (if available). */
    const string_type& GetToken() const
    {
      return m_strTok;
    }

    //------------------------------------------------------------------------------
    /** \brief Return the error code. */
    EErrorCodes GetCode() const
    {
      return m_iErrc;
    }

private:
    string_type  m_strMsg;     ///< The message string
    string_type  m_strFormula; ///< Formula string
    string_type  m_strTok;     ///< Token related with the error
    int m_iPos;                ///< Formula position related to the error 
    EErrorCodes m_iErrc;       ///< Error code
    const ParserErrorMsg<string_type> &m_ErrMsg;
};		

} // namespace MathUtils

#endif

