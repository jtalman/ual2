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
#ifndef MU_PARSER_CALLBACK_H
#define MU_PARSER_CALLBACK_H


namespace mu
{

/** \brief Encapsulation of prototypes for a numerical parser function.

    Encapsulates the prototyp for numerical parser functions. The class
    stores the number of arguments for parser functions as well
    as additional flags indication the function is non optimizeable.
    The pointer to the callback function pointer is stored as void* 
    and needs to be casted according to the argument count.
    Negative argument counts indicate a parser function with a variable number
    of arguments. 
    This class is not used for string function prototyping.

    \author (C) 2004, 2005 Ingo Berg
*/
template<typename TVal, typename TString>
class ParserCallback
{
private:
    typedef TVal value_type;                
    typedef TString  string_type;
    typedef typename string_type::value_type char_type;
    typedef value_type (*fun_type1)(value_type); 
    typedef value_type (*fun_type2)(value_type, value_type); 
    typedef value_type (*fun_type3)(value_type, value_type, value_type); 
    typedef value_type (*fun_type4)(value_type, value_type, value_type, value_type); 
    typedef value_type (*fun_type5)(value_type, value_type, value_type, value_type, value_type); 
    typedef value_type (*pfun_type2)(value_type*, value_type); 
    typedef value_type (*multfun_type)(const value_type*, int);
    typedef value_type (*strfun_type1)(const char_type*);

public:
    
    ParserCallback(fun_type1 a_pFun, bool a_bAllowOpti)
    :m_pFun((void*)a_pFun)
    ,m_iArgc(1)
    ,m_iPri(-1)
    ,m_iCode(cmFUNC)
    ,m_iType(tpDBL)
    ,m_bAllowOpti(a_bAllowOpti)
    {}

    /** \brief This constructor is used either for constructing a binary operator or a binary function. 
    
        \param a_pFun pointer to the callback function taking two value parameters
        \param a_bAllowOpti Signals if the operator/function always returns the same 
                            result for the same arguments.
        \param a_iPri The priority. If this optional parameter is present a binary operator 
                      token will be constructed.
    */
    ParserCallback(fun_type2 a_pFun, bool a_bAllowOpti, int a_iPri = -999)
    :m_pFun((void*)a_pFun)
    ,m_iArgc(2)
    ,m_iPri(a_iPri)
    ,m_iCode( (a_iPri!=-999) ? cmBINOP : cmFUNC)
    ,m_iType(tpDBL)
    ,m_bAllowOpti(a_bAllowOpti)
    {}


    ParserCallback(fun_type3 a_pFun, bool a_bAllowOpti)
    :m_pFun((void*)a_pFun)
    ,m_iArgc(3)
    ,m_iPri(-1)
    ,m_iCode(cmFUNC)
    ,m_iType(tpDBL)
    ,m_bAllowOpti(a_bAllowOpti)
    {}

    ParserCallback(fun_type4 a_pFun, bool a_bAllowOpti)
    :m_pFun((void*)a_pFun)
    ,m_iArgc(4)
    ,m_iPri(-1)
    ,m_iCode(cmFUNC)
    ,m_iType(tpDBL)
    ,m_bAllowOpti(a_bAllowOpti)
    {}

    ParserCallback(fun_type5 a_pFun, bool a_bAllowOpti)
    :m_pFun((void*)a_pFun)
    ,m_iArgc(5)
    ,m_iPri(-1)
    ,m_iCode(cmFUNC)
    ,m_iType(tpDBL)
    ,m_bAllowOpti(a_bAllowOpti)
    {}

    ParserCallback(multfun_type a_pFun, bool a_bAllowOpti)
    :m_pFun((void*)a_pFun)
    ,m_iArgc(-1)
    ,m_iPri(-1)
    ,m_iCode(cmFUNC)
    ,m_iType(tpDBL)
    ,m_bAllowOpti(a_bAllowOpti)
    {}

    ParserCallback(strfun_type1 a_pFun, bool a_bAllowOpti)
    :m_pFun((void*)a_pFun)
    ,m_iArgc(1)
    ,m_iPri(-1)
    ,m_iCode(cmFUNC_STR)
    ,m_iType(tpSTR)
    ,m_bAllowOpti(a_bAllowOpti)
    {}

    /** \brief Default constructor. 

        \throw nothrow
    */
    ParserCallback()
    :m_pFun(0)
    ,m_iArgc(0)
    ,m_iCode(cmUNKNOWN)
    ,m_iType(tpVOID)
    ,m_bAllowOpti(0)
    {}

    /** \brief Copy constructor. 
    
        \throw nothrow
    */
    ParserCallback(const ParserCallback &a_Fun)
    {
      m_pFun = a_Fun.m_pFun;
      m_iArgc = a_Fun.m_iArgc;
      m_bAllowOpti = a_Fun.m_bAllowOpti;
      m_iCode = a_Fun.m_iCode;
      m_iType = a_Fun.m_iType;
      m_iPri = a_Fun.m_iPri;
    }
    
    /** \brief Clone this instance and return a pointer to the new instance. */
    ParserCallback* Clone() const
    {
      return new ParserCallback(*this);
    }


    /** \brief Return tru if the function is conservative.

        Conservative functions return always the same result for the same argument.
        \throw nothrow
    */
    bool IsOptimizable() const  { return m_bAllowOpti; }

    /** \brief Get the callback address for the parser function. 
    
        The type of the address is void. It needs to be recasted according to the
        argument number to the right type.

        \throw nothrow
        \return #pFun
    */
    void     *GetAddr() const { return m_pFun;  }
    ECmdCode  GetCode() const { return m_iCode; }
    ETypeCode GetType() const { return m_iType; }
    int GetPri()  const { return m_iPri;  }
    int GetArgc() const { return m_iArgc; }

private:
    void *m_pFun;       ///< Pointer to the callback function, casted to void
    
    /** \brief Number of function arguments
    
        This number is negative for functions with variable number of arguments. in this cases
        they represent the actual number of arguments found.
    */
    int   m_iArgc;    
    int   m_iPri;
    ECmdCode  m_iCode;
    ETypeCode m_iType;
    bool  m_bAllowOpti; ///< Flag indication optimizeability 
};

} // namespace MathIUtils

#endif

