 /*
  Copyright (C) 2005 Ingo Berg

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
#ifndef MU_PARSER_TOKEN_READER_H
#define MU_PARSER_TOKEN_READER_H

#include <cassert>
#include <cstdio>
#include <cstring>
#include <map>
#include <stack>
#include <string>

#include "muParserDef.h"
#include "muParserToken.h"
#include "muIParserTypes.h"
#include "muBasicParserTokenReader.h"
#include "muParserBase.h"


namespace mu
{

// Forward declaration
// class ParserBase;

/** \brief Token reader for the ParserBase class.

*/
template<typename TVal, typename TStr>
class ParserTokenReader : public BasicParserTokenReader<TVal, TStr>
{
 protected:
    typedef TStr string_type;
    typedef ParserToken<TVal, TStr> token_type;
    typedef IParserTypes<TVal, TStr> traits_type;

 protected:

    ParserBase *m_pParser;
    typename IParserTypes<TVal, TStr>::string_type m_strFormula;
    int  m_iPos;
    int  m_iSynFlags;
    bool m_bIgnoreUndefVar;

    const typename traits_type::funmap_type *m_pFunDef;
    const typename traits_type::funmap_type *m_pPostOprtDef;
    const typename traits_type::funmap_type *m_pInfixOprtDef;
    const typename traits_type::funmap_type *m_pOprtDef;
    const typename traits_type::valmap_type *m_pConstDef;
    const typename traits_type::strmap_type *m_pStrVarDef;

    typename IParserTypes<TVal, TStr>::varmap_type *m_pVarDef;  
///< The only non const pointer to parser internals

    typename IParserTypes<TVal, TStr>::facfun_type m_pFactory;

    std::vector<typename IParserTypes<TVal, TStr>::identfun_type> m_vIdentFun; 
///< Value token identification function

    typename IParserTypes<TVal, TStr>::varmap_type m_UsedVar;

    typename IParserTypes<TVal, TStr>::value_type m_fZero;  
///< Dummy value of zero, referenced by undefined variables


//
// private Functions
//
 protected:

    /** \brief Syntax codes. 
	
	The syntax codes control the syntax check done during the first time parsing of 
        the expression string. They are flags that indicate which tokens are allowed next
        if certain tokens are identified.
  	*/
	  enum ESynCodes
    {
      noBO      = 1 << 0,  ///< to avoid i.e. "cos(7)(" 
      noBC      = 1 << 1,  ///< to avoid i.e. "sin)" or "()"
      noVAL     = 1 << 2,  ///< to avoid i.e. "tan 2" or "sin(8)3.14"
      noVAR     = 1 << 3,  ///< to avoid i.e. "sin a" or "sin(8)a"
      noCOMMA   = 1 << 4,  ///< to avoid i.e. ",," or "+," ...
      noFUN     = 1 << 5,  ///< to avoid i.e. "sqrt cos" or "(1)sin"	
      noOPT     = 1 << 6,  ///< to avoid i.e. "(+)"
      noPOSTOP  = 1 << 7,  ///< to avoid i.e. "(5!!)" "sin!"
      noINFIXOP = 1 << 8,  ///< to avoid i.e. "++4" "!!4"
      noEND     = 1 << 9,  ///< to avoid unexpected end of formula
      noSTR     = 1 << 10, ///< to block numeric arguments on string functions
      noASSIGN  = 1 << 11, ///< to block assignement to constant i.e. "4=7"
      noANY     = ~0       ///< All of he above flags set
    };	

    //---------------------------------------------------------------------------
    /** \brief Copy constructor.

        \sa Assign
        \throw nothrow
    */
    ParserTokenReader(const ParserTokenReader &a_Reader) 
      : BasicParserTokenReader<typename traits_type::value_type, 
                    typename traits_type::string_type>()
    { 
       Assign(a_Reader);
    }
    
    //---------------------------------------------------------------------------
    /** \brief Assignement operator.
    
        Self assignement will be suppressed otherwise #Assign is called.

        \param a_Reader Object to copy to this token reader.
        \throw nothrow
    */
    BasicParserTokenReader<TVal, TStr>& operator=(const BasicParserTokenReader<TVal, TStr> &a_Reader) 
    {
      if (&a_Reader!=this)
        Assign(a_Reader);

      return *this;
    }

    //---------------------------------------------------------------------------
    /** \brief Assign state of a token reader to this token reader. 
        
        \param a_Reader Object from which the state should be copied.
        \throw nothrow
    */
    void Assign(const BasicParserTokenReader<TVal, TStr> &i_Reader)
    {
      const ParserTokenReader<TVal, TStr>& a_Reader = static_cast< const ParserTokenReader<TVal, TStr>& >(i_Reader);
      m_pParser = a_Reader.m_pParser;
      m_strFormula = a_Reader.m_strFormula;
      m_iPos = a_Reader.m_iPos;
      m_iSynFlags = a_Reader.m_iSynFlags;
      
      m_UsedVar = a_Reader.m_UsedVar;
      m_pFunDef = a_Reader.m_pFunDef;
      m_pConstDef = a_Reader.m_pConstDef;
      m_pVarDef = a_Reader.m_pVarDef;
      m_pStrVarDef = a_Reader.m_pStrVarDef;
      m_pPostOprtDef = a_Reader.m_pPostOprtDef;
      m_pInfixOprtDef = a_Reader.m_pInfixOprtDef;
      m_pOprtDef = a_Reader.m_pOprtDef;
      m_bIgnoreUndefVar = a_Reader.m_bIgnoreUndefVar;
      m_vIdentFun = a_Reader.m_vIdentFun;
      m_pFactory = a_Reader.m_pFactory;
    }


public:

    //---------------------------------------------------------------------------
    /** \brief Constructor. 
        
        \pre [assert] a_pParser may not be NULL
        \post #m_pParser==a_pParser
        \param a_pParent Parent parser object of the token reader.
    */
    ParserTokenReader(ParserBase *a_pParent)
      : BasicParserTokenReader<typename traits_type::value_type, 
                    typename traits_type::string_type>()
      ,m_pParser(a_pParent)
      ,m_strFormula()
      ,m_iPos(0)
      ,m_iSynFlags(0)
      ,m_bIgnoreUndefVar(false)
      ,m_pFunDef(0)
      ,m_pPostOprtDef(0)
      ,m_pInfixOprtDef(0)
      ,m_pOprtDef(0)
      ,m_pConstDef(0)
      ,m_pStrVarDef(0)
      ,m_pVarDef(0)

      ,m_pFactory(0)
      ,m_vIdentFun()
      ,m_UsedVar()
      ,m_fZero(0)
    {
      assert(m_pParser);
    }
    
    //---------------------------------------------------------------------------
    /** \brief Destructor (trivial).
    
        \throw nothrow
    */
   ~ParserTokenReader()
    {
    }

    //---------------------------------------------------------------------------
    /** \brief Create instance of a ParserTokenReader identical with this 
               and return its pointer. 

        This is a factory method the calling function must take care of the object destruction.

        \return A new ParserTokenReader object.
        \throw nothrow
    */
    BasicParserTokenReader<TVal, TStr>* Clone() const
    {
      return new ParserTokenReader(*this);
    }

    //---------------------------------------------------------------------------
    void AddValIdent(typename traits_type::identfun_type a_pCallback)
    {
      m_vIdentFun.push_back(a_pCallback);
    }

    //---------------------------------------------------------------------------
    void SetVarCreator(typename traits_type::facfun_type a_pFactory)
    {
      m_pFactory = a_pFactory;
    }

    //---------------------------------------------------------------------------
    /** \brief Return the current position of the token reader in the formula string. 

        \return #m_iPos
        \throw nothrow
    */
    int GetPos() const
    {
      return m_iPos;
    }

    //---------------------------------------------------------------------------
    /** \brief Return a reference to the formula. 

        \return #m_strFormula
        \throw nothrow
    */
    const typename traits_type::string_type& GetFormula() const
    {
      return m_strFormula;
    }

    //---------------------------------------------------------------------------
    /** \brief Return a map containing the used variables only. */
    const typename traits_type::varmap_type& GetUsedVar() const
    {
      return m_UsedVar;
    }

    //---------------------------------------------------------------------------
    /** \brief Initialize the token Reader. 
    
        Sets the formula position index to zero and set Syntax flags to default for initial formula parsing.
        \pre [assert] triggered if a_szFormula==0
    */
    void SetFormula(const typename traits_type::string_type &a_strFormula)
    {
      m_strFormula = a_strFormula;
      ReInit();
    }

    //---------------------------------------------------------------------------
    void SetDefs(const typename traits_type::funmap_type *a_pFunDef, 
                 const typename traits_type::funmap_type *a_pOprtDef,
                 const typename traits_type::funmap_type *a_pInfixOprtDef,
                 const typename traits_type::funmap_type *a_pPostOprtDef,
                 typename traits_type::varmap_type *a_pVarDef,
                 const typename traits_type::strmap_type *a_pStrVarDef,
                 const typename traits_type::valmap_type *a_pConstDef)
    {
      m_pFunDef = a_pFunDef;
      m_pOprtDef = a_pOprtDef;
      m_pInfixOprtDef = a_pInfixOprtDef;
      m_pPostOprtDef = a_pPostOprtDef;
      m_pVarDef = a_pVarDef;
      m_pStrVarDef = a_pStrVarDef;
      m_pConstDef = a_pConstDef;
    }

    //---------------------------------------------------------------------------
    /** \brief Set Flag that contronls behaviour in case of undefined variables beeing found. 
    
      If true, the parser does not throw an exception if an undefined variable is found. 
      otherwise it does. This variable is used internally only!
      It supresses a "undefined variable" exception in GetUsedVar().  
      Those function should return a complete list of variables including 
      those the are not defined by the time of it's call.
    */
    void IgnoreUndefVar(bool bIgnore)
    {
      m_bIgnoreUndefVar = bIgnore;
    }

    //---------------------------------------------------------------------------
    /** \brief Reset the token reader to the start of the formula. 

        The syntax flags will be reset to a value appropriate for the 
        start of a formula.
        \post #m_iPos==0, #m_iSynFlags = noOPT | noBC | noPOSTOP | noSTR
        \throw nothrow
        \sa ESynCodes
    */
    void ReInit()
    {
      m_iPos = 0;
      m_iSynFlags = noOPT | noBC | noPOSTOP | noASSIGN;
      m_UsedVar.clear();
    }

    //---------------------------------------------------------------------------
    /** \brief Read the next token from the string. */ 
    token_type ReadNextToken()
    {
      assert(m_pParser);

      std::stack<int> FunArgs;
      const typename traits_type::char_type *szFormula = m_strFormula.c_str();
      token_type tok;

      while (szFormula[m_iPos] == ' ') {
	++m_iPos;
      }

      if ( IsEOF(tok) ) return tok;        // Check for end of formula
      if ( IsOprt(tok) )   return tok;     // Check for user defined binary operator
      if ( IsBuiltIn(tok) ) return tok;    // Check built in operators / tokens
      if ( IsFunTok(tok) ) return tok;     // Check for function token
      if ( IsValTok(tok) ) return tok;     // Check for values / constant tokens
      if ( IsVarTok(tok) ) return tok;     // Check for variable tokens
      if ( IsStrVarTok(tok) ) return tok;  // Check for string variables
      if ( IsString(tok) ) return tok;     // Check for String tokens
      if ( IsInfixOpTok(tok) ) return tok; // Check for unary operators
      if ( IsPostOpTok(tok) )  return tok; // Check for unary operators

      // Check String for undefined variable token. Done only if a 
      // flag is set indicating to ignore undefined variables.
      // This is a way to conditionally avoid an error if 
      // undefined variables occur. 
      // The GetUsedVar function must supress the error for
      // undefined variables in order to collect all variable 
      // names including the undefined ones.
      if ( (m_bIgnoreUndefVar || m_pFactory) && IsUndefVarTok(tok) )  return tok;

      // Check for unknown token
      // 
      // !!! From this point on there is no exit without an exception possible...
      // 
      typename traits_type::string_type strTok;
      int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
      if (iEnd!=m_iPos)
        Error(ecUNASSIGNABLE_TOKEN, m_iPos, strTok);

      Error(ecUNASSIGNABLE_TOKEN, m_iPos, m_strFormula.substr(m_iPos));
      return token_type(); // never reached
    }

//
// private functions
//
private:

    //---------------------------------------------------------------------------
    /** \brief Extract all characters that belong to a certain charset.

      \param a_szCharSet [in] Const char array of the characters allowed in the token. 
      \param a_strTok [out]  The string that consists entirely of characters listed in a_szCharSet.
      \param a_iPos [in] Position in the string from where to start reading.
      \return The Position of the first character not listed in a_szCharSet.
      \throw nothrow
    */
    int ExtractToken(const typename traits_type::char_type *a_szCharSet, 
                     typename traits_type::string_type &a_strTok, int a_iPos) const
    {
      int iEnd = (int)m_strFormula.find_first_not_of(a_szCharSet, a_iPos);

      if (iEnd==(int)std::string::npos)
          iEnd = (int)m_strFormula.length();  

      a_strTok = string_type( m_strFormula.begin()+a_iPos, m_strFormula.begin()+iEnd);
      a_iPos = iEnd;
      return iEnd;
    }

    //---------------------------------------------------------------------------
    /** \brief Check if a built in operator or other token can be found
        \param a_Tok  [out] Operator token if one is found. This can either be a binary operator or an infix operator token.
        \return true if an operator token has been found.
    */
    bool IsBuiltIn(token_type &a_Tok)
    {
      const typename traits_type::char_type **pOprtDef = m_pParser->GetOprtDef();
      const typename traits_type::char_type* szFormula = m_strFormula.c_str();

      // Compare token with function and operator strings
      // check string for operator/function
      for (int i=0; pOprtDef[i]; i++)
      {
        std::size_t len = std::strlen( pOprtDef[i] );

        if (!std::strncmp(&szFormula[m_iPos], pOprtDef[i], len))
	      {
	        switch(i)
	        {
          case cmAND:
          case cmOR:
          case cmXOR:
          case cmLT:
		      case cmGT:
		      case cmLE:
		      case cmGE:
		      case cmNEQ:  
		      case cmEQ:
		      case cmADD:
		      case cmSUB:
		      case cmMUL:
		      case cmDIV:
		      case cmPOW:
          case cmASSIGN:
                        // The assignement operator need special treatment
                        if (i==cmASSIGN && m_iSynFlags & noASSIGN)
                          Error(ecUNEXPECTED_OPERATOR, m_iPos, pOprtDef[i]);

                        if (!m_pParser->HasBuiltInOprt()) continue;
                        if (m_iSynFlags & noOPT) 
                        {
                          // Maybe its an infix operator not an operator
                          // Both operator types can share characters in 
                          // their identifiers
                          if ( IsInfixOpTok(a_Tok) ) 
                            return true;

                          Error(ecUNEXPECTED_OPERATOR, m_iPos, pOprtDef[i]);
                        }

                        m_iSynFlags  = noBC | noOPT | noCOMMA | noPOSTOP | noASSIGN;
				                m_iSynFlags |= ( (i != cmEND) && ( i != cmBC) ) ? noEND : 0;
				                break;

		      case cmCOMMA:
				                if (m_iSynFlags & noCOMMA)
					                Error(ecUNEXPECTED_COMMA, m_iPos, pOprtDef[i]);
        				
				                m_iSynFlags  = noBC | noOPT | noEND | noCOMMA | noPOSTOP | noASSIGN;
			                  break;

		      case cmBO:
				                if (m_iSynFlags & noBO)
					                Error(ecUNEXPECTED_PARENS, m_iPos, pOprtDef[i]);

				                m_iSynFlags = noBC | noOPT | noEND | noCOMMA | noPOSTOP | noASSIGN;
				                break;

		      case cmBC:
				                if (m_iSynFlags & noBC)
					                Error(ecUNEXPECTED_PARENS, m_iPos, pOprtDef[i]);

				                m_iSynFlags  = noBO | noVAR | noVAL | noFUN | noINFIXOP | noSTR | noASSIGN;
				                break;
      		
		default:      // The operator is listed in c_DefaultOprt, but not here. This is a bad thing...
                  Error(ecINTERNAL_ERROR);
		  break;
	        } // switch operator id

          m_iPos += (int)len;
          a_Tok.Set( (ECmdCode)i, pOprtDef[i] );
          return true;
	      } // if operator string found
      } // end of for all operator strings
    
      return false;
    }

    //---------------------------------------------------------------------------
    /** \brief Check for End of Formula.
 
        \return true if an end of formula is found false otherwise.
        \param a_Tok [out] If an eof is found the corresponding token will be stored there.
        \throw nothrow
        \sa IsOprt, IsFunTok, IsStrFunTok, IsValTok, IsVarTok, IsString, IsInfixOpTok, IsPostOpTok
    */
    bool IsEOF(token_type &a_Tok)
    {
      // added by NM
      
      if(m_iPos < 0) {
	if ( m_iSynFlags & noEND ) Error(ecUNEXPECTED_EOF, m_iPos);

        m_iSynFlags = 0;
        a_Tok.Set(cmEND);
        return true;
      }
      

      const typename traits_type::char_type* szFormula = m_strFormula.c_str();

      // check for EOF
      if ( !szFormula[m_iPos] || szFormula[m_iPos] == '\n')
      {
        if ( m_iSynFlags & noEND ) Error(ecUNEXPECTED_EOF, m_iPos);

        m_iSynFlags = 0;
        a_Tok.Set(cmEND);
        return true;
      }

      return false;
    }

    //---------------------------------------------------------------------------
    /** \brief Check if a string position contains a unary infix operator. 
        \return true if a function token has been found false otherwise.
    */
    bool IsInfixOpTok(token_type &a_Tok)
    {
      typename traits_type::string_type strTok;
      int iEnd = ExtractToken(m_pParser->ValidInfixOprtChars(), strTok, m_iPos);
      if (iEnd==m_iPos)
        return false;

      typename traits_type::funmap_type::const_iterator item = m_pInfixOprtDef->find(strTok);
      if (item==m_pInfixOprtDef->end())
        return false;

      a_Tok.Set(item->second, strTok);
      m_iPos = (int)iEnd;

      if (m_iSynFlags & noINFIXOP) 
        Error(ecUNEXPECTED_OPERATOR, m_iPos, a_Tok.GetAsString());

      m_iSynFlags = noPOSTOP | noINFIXOP | noOPT | noBC | noSTR | noASSIGN; 
      return true;
    }

    //---------------------------------------------------------------------------
    /** \brief Check whether the token at a given position is a function token.
        \param a_Tok [out] If a value token is found it will be placed here.
        \throw ParserException if Syntaxflags do not allow a function at a_iPos
        \return true if a function token has been found false otherwise.
        \pre [assert] m_pParser!=0
    */
    bool IsFunTok(token_type &a_Tok)
    {
      typename traits_type::string_type strTok;
      int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
      if (iEnd==m_iPos)
        return false;

      typename traits_type::funmap_type::const_iterator item = m_pFunDef->find(strTok);
      if (item==m_pFunDef->end())
         return false;

      a_Tok.Set(item->second, strTok);

      m_iPos = (int)iEnd;
      if (m_iSynFlags & noFUN)
        Error(ecUNEXPECTED_FUN, m_iPos-(int)a_Tok.GetAsString().length(), a_Tok.GetAsString());

      m_iSynFlags = noANY ^ noBO;
      return true;
    }

    //---------------------------------------------------------------------------
    /** \brief Check if a string position contains a unary post value operator. */
    bool IsPostOpTok(token_type &a_Tok)
    {
      // Tricky problem with equations like "3m+5":
      //     m is a postfix operator, + is a valid sign for postfix operators and 
      //     for binary operators parser detects "m+" as operator string and 
      //     finds no matching postfix operator.
      // 
      // This is a special case so this routine slightly differs from the other
      // token readers.
      
      // Test if there could be a postfix operator
      typename traits_type::string_type strTok;
      int iEnd = ExtractToken(m_pParser->ValidOprtChars(), strTok, m_iPos);
      if (iEnd==m_iPos)
        return false;

      // iteraterate over all postfix operator strings
      typename traits_type::funmap_type::const_iterator item = m_pPostOprtDef->begin();
      for (item=m_pPostOprtDef->begin(); item!=m_pPostOprtDef->end(); ++item)
      {
        if (strTok.find(item->first)!=0)
          continue;

        a_Tok.Set(item->second, strTok);
  	    m_iPos += (int)item->first.length();

        if (m_iSynFlags & noPOSTOP)
          Error(ecUNEXPECTED_OPERATOR, m_iPos-(int)a_Tok.GetAsString().length(), a_Tok.GetAsString());

        m_iSynFlags = noVAL | noVAR | noFUN | noBO | noPOSTOP | noSTR | noASSIGN;
        return true;
      }

      return false;
    }

    //---------------------------------------------------------------------------
    /** \brief Check if a string position contains a binary operator.
        \param a_Tok  [out] Operator token if one is found. This can either be a binary operator or an infix operator token.
        \return true if an operator token has been found.
    */
    bool IsOprt(token_type &a_Tok)
    {
      const typename traits_type::char_type *szFormula = m_strFormula.c_str();
      int iVarEnd = (int)strspn(&szFormula[m_iPos], m_pParser->ValidOprtChars());
      if (!iVarEnd) return false;
      
      typename traits_type::string_type strOprt(&szFormula[m_iPos], &szFormula[m_iPos+iVarEnd]);

      typename traits_type::funmap_type::const_iterator item = m_pOprtDef->find(strOprt);
      if (item==m_pOprtDef->end())
        return false;

      a_Tok.Set(item->second, strOprt);

      if (m_iSynFlags & noOPT) 
      {
        // An operator was found but is not expected to occur at
        // this position of the formula, maybe it is an infix 
        // operator, not a binary operator. Both operator types
        // can share characters in their identifiers.
        if ( IsInfixOpTok(a_Tok) ) return true;
        // nope, no infix operator
        Error(ecUNEXPECTED_OPERATOR, m_iPos, a_Tok.GetAsString()); 
      }

	    m_iPos += iVarEnd;
      m_iSynFlags  = noBC | noOPT | noCOMMA | noPOSTOP | noEND | noBC | noASSIGN;
      return true;
    }

    //---------------------------------------------------------------------------
    /** \brief Check whether the token at a given position is a value token.

      Value tokens are either values or constants.

      \param a_Tok [out] If a value token is found it will be placed here.
      \return true if a value token has been found.
    */
    bool IsValTok(token_type &a_Tok)
    {
      assert(m_pConstDef);
      assert(m_pParser);

      #if defined(_MSC_VER)
        #pragma warning( disable : 4244 )
      #endif


      typename traits_type::stringstream_type stream(&m_strFormula[m_iPos]);
      typename traits_type::string_type strTok;
      typename traits_type::value_type fVal(0);
      int iEnd(0);
      
      // std::cout << "2.) Check for user defined constant" << std::endl;
      // Read everything that could be a constant name
      iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);

      if (iEnd!=m_iPos)
      {
        typename traits_type::valmap_type::const_iterator item = m_pConstDef->find(strTok);
        if (item!=m_pConstDef->end())
        {
          m_iPos = iEnd;
          a_Tok.SetVal(item->second, strTok);

          if (m_iSynFlags & noVAL)
            Error(ecUNEXPECTED_VAL, m_iPos - (int)strTok.length(), strTok);

          m_iSynFlags = noVAL | noVAR | noFUN | noBO | noINFIXOP | noSTR | noASSIGN; 
          return true;
        }
      }

      // std::cout << "3.call the value recognition functions provided by the user" << std::endl;
      // Call user defined value recognition functions
      typename std::vector<typename traits_type::identfun_type>::const_iterator item = m_vIdentFun.begin();
      for (item = m_vIdentFun.begin(); item!=m_vIdentFun.end(); ++item)
      {
        int iStart = m_iPos;
        if ( (*item)(m_strFormula.c_str() + m_iPos, m_iPos, fVal) )
        {
          strTok.assign(m_strFormula.c_str(), iStart, m_iPos);

          if (m_iSynFlags & noVAL) Error(ecUNEXPECTED_VAL, m_iPos - (int)strTok.length(), strTok);

          a_Tok.SetVal(fVal, strTok);
          m_iSynFlags = noVAL | noVAR | noFUN | noBO | noINFIXOP | noSTR | noASSIGN;
          return true;
        }
      }

      return false;

      #if defined(_MSC_VER)
        #pragma warning( default : 4244 )
      #endif
    }

    //---------------------------------------------------------------------------
    /** \brief Check wheter a token at a given position is a variable token. 
        \param a_Tok [out] If a variable token has been found it will be placed here.
	      \return true if a variable token has been found.
    */
    bool IsVarTok(token_type &a_Tok)
    {
      if (!m_pVarDef->size())
        return false;

      typename traits_type::string_type strTok;
      int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
      if (iEnd==m_iPos)
        return false;

      typename traits_type::varmap_type::const_iterator item =  m_pVarDef->find(strTok);
      if (item==m_pVarDef->end())
        return false;

      if (m_iSynFlags & noVAR)
        Error(ecUNEXPECTED_VAR, m_iPos, strTok);

      m_iPos = iEnd;
      a_Tok.SetVar(item->second, strTok);
      m_UsedVar[item->first] = item->second;  // Add variable to used-var-list

      m_iSynFlags = noVAL | noVAR | noFUN | noBO | noPOSTOP | noINFIXOP | noSTR;
      return true;
    }

    //---------------------------------------------------------------------------
    bool IsStrVarTok(token_type &a_Tok)
    {
      if (!m_pStrVarDef || !m_pStrVarDef->size())
        return false;

      typename traits_type::string_type strTok;
      int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
      if (iEnd==m_iPos)
        return false;

      typename traits_type::strmap_type::const_iterator item =  m_pStrVarDef->find(strTok);
      if (item==m_pStrVarDef->end())
        return false;

      if (m_iSynFlags & noSTR)
        Error(ecUNEXPECTED_VAR, m_iPos, strTok);

      m_iPos = iEnd;
      if (!m_pParser->m_vStringVarBuf.size())
        Error(ecINTERNAL_ERROR);

      a_Tok.SetString(m_pParser->m_vStringVarBuf[item->second], m_pParser->m_vStringVarBuf.size() );

      m_iSynFlags = m_iSynFlags = noANY ^ ( noBC | noOPT | noEND | noCOMMA);
      return true;
    }

    //---------------------------------------------------------------------------
    /** \brief Check wheter a token at a given position is an undefined variable. 

        \param a_Tok [out] If a variable tom_pParser->m_vStringBufken has been found it will be placed here.
	      \return true if a variable token has been found.
        \throw nothrow
    */
    bool IsUndefVarTok(token_type &a_Tok)
    {
      typename traits_type::string_type strTok;
      int iEnd = ExtractToken(m_pParser->ValidNameChars(), strTok, m_iPos);
      if (iEnd==m_iPos)
        return false;

      if (m_iSynFlags & noVAR)
        Error(ecUNEXPECTED_VAR, m_iPos - (int)a_Tok.GetAsString().length(), a_Tok.GetAsString());

      // If a factory is available implicitely create new variables
      if (m_pFactory)
      {
        typename traits_type::value_type *fVar = m_pFactory(strTok.c_str());
        a_Tok.SetVar(fVar, strTok );

        // Do not use m_pParser->DefineVar( strTok, fVar );
        // in order to define the new variable, it will clear the
        // m_UsedVar array which will kill previousely defined variables
        // from the list
        // This is safe because the new variable can never override an existing one
        // because they are checked first!
        (*m_pVarDef)[strTok] = fVar;
        m_UsedVar[strTok] = fVar;  // Add variable to used-var-list
      }
      else
      {
        a_Tok.SetVar((typename traits_type::value_type*)&m_fZero, strTok);
        m_UsedVar[strTok] = 0;  // Add variable to used-var-list
      }

      m_iPos = iEnd;

      // Call the variable factory in order to let it define a new parser variable
      m_iSynFlags = noVAL | noVAR | noFUN | noBO | noPOSTOP | noINFIXOP | noSTR;
      return true;
    }

    //---------------------------------------------------------------------------
    /** \brief Check wheter a token at a given position is a string.

        \param a_Tok [out] If a variable token has been found it will be placed here.
  	    \return true if a string token has been found.
        \sa IsOprt, IsFunTok, IsStrFunTok, IsValTok, IsVarTok, IsEOF, IsInfixOpTok, IsPostOpTok
        \throw nothrow
    */
    bool IsString(token_type &a_Tok)
    {
      if (m_strFormula[m_iPos]!='"') 
        return false;

      typename traits_type::string_type strBuf(&m_strFormula[m_iPos+1]);
      std::size_t iEnd(0), iSkip(0);

      // parser over escaped '\"' end replace them with '"'
      for(iEnd=(int)strBuf.find("\""); iEnd!=std::string::npos; iEnd=(int)strBuf.find("\"", iEnd))
      {
        if (strBuf[iEnd-1]!='\\') break;
        strBuf.replace(iEnd-1, 2, "\"");
        iSkip++;
      }

      if (iEnd==std::string::npos)
        Error(ecUNTERMINATED_STRING, m_iPos, "\"");

      typename traits_type::string_type strTok(strBuf.begin(), strBuf.begin()+iEnd);

      if (m_iSynFlags & noSTR)
        Error(ecUNEXPECTED_STR, m_iPos, strTok);

		  m_pParser->m_vStringBuf.push_back(strTok); // Store string in internal buffer
      a_Tok.SetString(strTok, m_pParser->m_vStringBuf.size());

      m_iPos += (int)strTok.length() + 2 + (int)iSkip;  // +2 wg Anführungszeichen; +iSkip für entfernte escape zeichen
      m_iSynFlags = m_iSynFlags = noANY ^ ( noBC | noOPT | noEND );

      return true;
    }

    //---------------------------------------------------------------------------
    /** \brief Create an error containing the parse error position.

      This function will create an Parser Exception object containing the error text and its position.

      \param a_iErrc [in] The error code of type #EErrorCodes.
      \param a_iPos [in] The position where the error was detected.
      \param a_strTok [in] The token string representation associated with the error.
      \throw ParserException always throws thats the only purpose of this function.
    */
    void  Error(EErrorCodes a_iErrc, int a_iPos = -1, 
                const typename traits_type::string_type &a_sTok = string_type()) const
    {
      m_pParser->Error(a_iErrc, a_iPos, a_sTok);
    }
};

} // namespace mu

#endif


