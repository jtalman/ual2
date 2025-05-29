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
#ifndef MU_PARSER_TYPES_H
#define MU_PARSER_TYPES_H

#include <map>
#include <string>

#include "muParserError.h"
#include "muParserCallback.h"

//---------------------------------------------------------------------------
namespace mu
{

template<typename TVal, typename TStr>
class IParserTypes
{

public:
  typedef TVal value_type;                
  typedef TStr string_type;
  typedef typename string_type::value_type char_type;
  typedef ParserError<string_type> exception_type;
  typedef ParserCallback<value_type, string_type> callback_type;
  typedef value_type (*fun_type1)(value_type); 
  typedef value_type (*fun_type2)(value_type, value_type); 
  typedef value_type (*fun_type3)(value_type, value_type, value_type); 
  typedef value_type (*fun_type4)(value_type, value_type, value_type, value_type); 
  typedef value_type (*fun_type5)(value_type, value_type, value_type, value_type, value_type); 
  typedef value_type (*multfun_type)(const value_type*, int);
  typedef value_type (*strfun_type1)(const char *);

  typedef std::map<string_type, value_type*> varmap_type;  
  typedef std::map<string_type, value_type> valmap_type;
  typedef std::map<string_type, std::size_t> strmap_type;
  typedef std::map<string_type, callback_type> funmap_type; 

  virtual ~IParserTypes() {};
  IParserTypes() {};

protected:
  typedef std::basic_stringstream<char_type, 
                                  std::char_traits<char_type>,
                                  std::allocator<char_type> > stringstream_type;

//typedef ParserCallback<value_type, string_type> callback_type;
  typedef bool (*identfun_type)(const char_type*, int&, value_type&);
  typedef value_type* (*facfun_type)(const char_type*);
};

} // namespace MathUtils

#endif

