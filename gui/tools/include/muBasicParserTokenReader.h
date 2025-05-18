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
#ifndef MU_BASIC_PARSER_TOKEN_READER_H
#define MU_BASIC_PARSER_TOKEN_READER_H

#include <cassert>
#include <cstdio>
#include <cstring>
#include <map>
#include <stack>
#include <string>

#include "muParserDef.h"
#include "muParserToken.h"
#include "muIParserTypes.h"


namespace mu
{

// Forward declaration
// class ParserBase;

/** \brief Token reader for the ParserBase class.

*/
template<typename TVal, typename TStr>
class BasicParserTokenReader : public IParserTypes<TVal, TStr>
{

 protected:
    typedef ParserToken<TVal, TStr> token_type;
    typedef IParserTypes<TVal, TStr> traits_type;

protected:

    BasicParserTokenReader() 
      :IParserTypes<typename traits_type::value_type, 
                    typename traits_type::string_type>()
    { 
    }


    //---------------------------------------------------------------------------
    /** \brief Copy constructor.

        \sa Assign
        \throw nothrow
    */
    BasicParserTokenReader(const BasicParserTokenReader &a_Reader) 
      :IParserTypes<typename traits_type::value_type, 
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
    virtual BasicParserTokenReader& operator=(const BasicParserTokenReader &a_Reader) 
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
    virtual void Assign(const BasicParserTokenReader &a_Reader) { }

public:


    
    //---------------------------------------------------------------------------
    /** \brief Destructor (trivial).
    
        \throw nothrow
    */
   virtual ~BasicParserTokenReader()
    {
    }

    //---------------------------------------------------------------------------
    /** \brief Create instance of a ParserTokenReader identical with this 
               and return its pointer. 
        This is a factory method the calling function must take care of the object destruction.
        \return A new ParserTokenReader object.
        \throw nothrow
    */
    virtual BasicParserTokenReader* Clone() const
    {
      return new BasicParserTokenReader(*this);
    }


};

} // namespace mu

#endif


