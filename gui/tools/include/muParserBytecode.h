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
#ifndef MU_PARSER_BYTECODE_H
#define MU_PARSER_BYTECODE_H

#include <cassert>
#include <string>
#include <stack>
#include <vector>

#include "muParserDef.h"
#include "muParserError.h"
#include "muParserToken.h"


namespace mu
{


/** \brief Bytecode implementation of the Math Parser.

  The bytecode contains the formula converted to revers polish notation stored in a continious
  memory area. Associated with this data are operator codes, variable pointers, constant 
  values and function pointers. Those are necessary in order to calculate the result.
  All those data items will be casted to the underlying datatype of the bytecode.

  \author (C) 2004, 2005 Ingo Berg 
*/
template<typename TBaseData>
class ParserByteCode
{
private:

    /** The value type of the parser*/
    typedef TBaseData value_type;

    /** \brief Underlying type of the storage. */
    typedef int map_type;

    /** \brief Token type for internal use only. */
    typedef ParserToken<TBaseData, MU_PARSER_STRINGTYPE> token_type;

    /** \brief Core type of the bytecode. */
    typedef std::vector<map_type> storage_type;

    /** \brief Position in the Calculation array. */
    unsigned m_iStackPos;

    /** \brief Core type of the bytecode. */
    storage_type m_vBase;

    /** \brief Size of a value entry in the bytecode, relative to TMapType size. */
    const int mc_iSizeVal;

    /** \brief Size of a pointer, relative to size of underlying TMapType.
       
        \attention The size is related to the size of TMapType not bytes!
    */
    const int mc_iSizePtr;

    /** \brief A value entry requires that much entires in the bytecode. 
        
        Value entry consists of:
        <ul>
          <li>One entry for Stack index</li>
          <li>One entry for Token identifier</li>
          <li>mc_iSizeVal entries for the value</li>
        <ul>

        \sa AddVal(TBaseData a_fVal)
    */
    const int mc_iSizeValEntry;

    /** \brief Store an address in bytecode.
        \param a_pAddr Address to be stored.
        \throw nothrow
    */
    void StorePtr(void *a_pAddr)
    {
      #if defined(_MSC_VER)
        #pragma warning( disable : 4311 )
      #endif

      for (int i=0; i<mc_iSizePtr; ++i)
        m_vBase.push_back( (map_type)( (map_type*)a_pAddr + i ) );

      #if defined(_MSC_VER)
        #pragma warning( default : 4311 )
      #endif
    }

public:
    
    /** \brief Bytecode default constructor. 
     
      \pre [assert] sizeof(TBaseData)>=sizeof(TMapType)
      \pre [assert] sizeof(TBaseData*)>=sizeof(TMapType)
    */
    ParserByteCode()
      :m_iStackPos(0)
      ,m_vBase()
      ,mc_iSizeVal( sizeof(TBaseData)/sizeof(map_type) )
      ,mc_iSizePtr( sizeof(TBaseData*) / sizeof(map_type) )
      ,mc_iSizeValEntry( 2 + mc_iSizeVal)
    {
      m_vBase.reserve(1000);
      assert( sizeof(TBaseData)>=sizeof(map_type) );
      assert( sizeof(TBaseData*)>=sizeof(map_type) );
    }

    /** \brief Destructor (trivial).*/
   ~ParserByteCode()
    {
    };

    /** \brief Copy constructor. 
    
      Implemented in Terms of Assign(const ParserByteCode &a_ByteCode)
    */
    ParserByteCode(const ParserByteCode &a_ByteCode)
    {
      Assign(a_ByteCode);
    }

    /** \brief Assignement operator.
    
      Implemented in Terms of Assign(const ParserByteCode &a_ByteCode)
    */
    ParserByteCode& operator=(const ParserByteCode &a_ByteCode)
    {
      Assign(a_ByteCode);
      return *this;
    }

    /** \brief Copy state of another object to this. 
    
      \throw nowthrow
    */
    void Assign(const ParserByteCode &a_ByteCode)
    {
      if (this==&a_ByteCode)    
          return;

      m_iStackPos = a_ByteCode.m_iStackPos;
      m_vBase = a_ByteCode.m_vBase;
    }

    /** \brief Add a Variable pointer to bytecode. 
        \param a_pVar Pointer to be added.
        \throw nothrow
    */
    void AddVar(TBaseData *a_pVar)
    {
      m_vBase.push_back( ++m_iStackPos );
      m_vBase.push_back( cmVAR );

      StorePtr(a_pVar);

      int iSize = GetValSize()-GetPtrSize();
      assert(iSize>=0);

      // Make sure variable entries have the same size like value entries
      // (fill with zeros)
      for (int i=0; i<iSize; ++i)
        m_vBase.push_back(0);
    }

    /** \brief Add a Variable pointer to bytecode. 

        Value entries in byte code consist of:
        <ul>
          <li>value array position of the value</li>
          <li>the operator code according to ParserToken::cmVAL</li>
          <li>the value stored in #mc_iSizeVal number of bytecode entries.</li>
        </ul>

        \param a_pVal Value to be added.
        \throw nothrow
    */
    void AddVal(TBaseData a_fVal)
    {
      m_vBase.push_back( ++m_iStackPos );
      m_vBase.push_back( cmVAL );

      for (int i=0; i<mc_iSizeVal; ++i)
      {
        m_vBase.push_back( *(reinterpret_cast<map_type*>(&a_fVal) + i) );
      }
    }

    /** \brief Add an operator identifier to bytecode. 
    
      Operator entries in byte code consist of:
      <ul>
        <li>value array position of the result</li>
        <li>the operator code according to ParserToken::ECmdCode</li>
      </ul>

      \sa  ParserToken::ECmdCode
    */
    void AddOp(ECmdCode a_Oprt)
    {
      m_vBase.push_back(--m_iStackPos);
      m_vBase.push_back(a_Oprt);
    }

    /** \brief Add an assignement operator
    
      Operator entries in byte code consist of:
      <ul>
        <li>cmASSIGN code</li>
        <li>the pointer of the destination variable</li>
      </ul>

      \sa  ParserToken::ECmdCode
    */
    void AddAssignOp(value_type *a_pVar)
    {
      m_vBase.push_back(--m_iStackPos);
      m_vBase.push_back(cmASSIGN);

      StorePtr(a_pVar);
    }

    /** \brief Add postfix operator to bytecode. 
    
      Postfix operator entries in byte code consist of:
      <ul>
        <li>value array position of the result</li>
        <li>the postfix operator code cmPOSTOP
        <li>its callback function pointer.</li>
      </ul>

      \param a_pFun Pointer to postfix operator callback function. 
      \throw nothrow
    */
    void AddPostOp(void *a_pFun)
    {
      m_vBase.push_back(m_iStackPos);
      m_vBase.push_back(cmPOSTOP);

      StorePtr(a_pFun);
    }

    /** \brief Add function to bytecode. 
        \param a_iArgc Number of arguments, negative numbers indicate multiarg functions.
        \param a_pFun Pointer to function callback.
    */
    void AddFun(void *a_pFun, int a_iArgc)
    {
      if (a_iArgc>=0)
      {
        m_iStackPos = m_iStackPos - a_iArgc + 1; 
      }
      else
      {
        m_iStackPos = m_iStackPos + a_iArgc + 1; 
      }

      m_vBase.push_back(m_iStackPos);
	    m_vBase.push_back(cmFUNC);
	    m_vBase.push_back(a_iArgc);

      StorePtr(a_pFun);
    }

    /** \brief Add Strung function entry to the parser bytecode. 
        \throw nothrow

        A string function entry consists of the stack position of the return value,
        followed by a cmSTRFUNC code, the function pointer and an index into the 
        string buffer maintained by the parser.
    */
    void AddStrFun(void *a_pFun, int a_iArgc, int a_iIdx)
    {
      // String functions do not reduce the value stack position they increase it
      // because they are not applied to values on the stack, they create them.
      m_vBase.push_back(++m_iStackPos);
	    m_vBase.push_back(cmFUNC_STR);
      m_vBase.push_back(a_iArgc);

      StorePtr(a_pFun);

      m_vBase.push_back(a_iIdx);
    }

    /** \brief Add end marker to bytecode.
      
        \throw nothrow 
    */
    void Finalize()
    {
      m_vBase.push_back(cmEND);	
      m_vBase.push_back(cmEND);	
      m_vBase.push_back(cmEND);	

      // shrink bytecode vector to fit
      storage_type(m_vBase).swap(m_vBase);
    }

    /** \brief Get Pointer to bytecode data storage. */
    const map_type* GetRawData() const
    {
      assert(m_vBase.size());
      return &m_vBase[0];
    }

    /** \brief Delete the bytecode. */
    void clear()
    {
      m_vBase.clear();
      m_iStackPos = 0;
    }

    /** \brief Return size of a value entry. 
    
      That many bytecode entries are necessary to store a value.

      \sa mc_iSizeVal
    */
    inline unsigned GetValSize() const 
    {
      return mc_iSizeVal;
    }

    /** \brief Return size of a pointer entry. 
    
      That many bytecode entries are necessary to store a pointer.

      \sa mc_iSizePtr
    */
    inline unsigned GetPtrSize() const 
    {
      return mc_iSizePtr;
    }

    /** \brief Remove a value number of entries from the bytecode. 
    
        \attention Currently I dont test if the entries are really value entries.
    */
    void RemoveValEntries(unsigned a_iNumber)
    {
      unsigned iSize = a_iNumber * mc_iSizeValEntry;   
      assert( (m_vBase.size()-iSize) >= 0);
      m_vBase.resize(m_vBase.size()-iSize);

      m_iStackPos -= (a_iNumber);
      assert(m_iStackPos>=0);
    }

    /** \brief Dump bytecode (for debugging only!). */
    void AsciiDump()
    {
      if (!m_vBase.size()) 
      {
        std::cout << "No bytecode available\n";
        return;
      }

      std::cout << "Entries:" << (int)m_vBase.size() 
                << " (ValSize:" << mc_iSizeVal 
                <<  ", PtrSize:" << mc_iSizePtr << ")\n";

      int i = 0;

      while ( m_vBase[i] != cmEND )
      {
	      std::cout << "IDX[" << m_vBase[i++] << "]\t";
	      switch (m_vBase[i])
	      {
          case cmVAL:
 	 	        std::cout << "VAL "; ++i;
		        std::cout << "[" << *( reinterpret_cast<double*>(&m_vBase[i]) ) << "]\n";
		        i += mc_iSizeVal;
		        break;

          case cmVAR:
            std::cout << "VAR "; ++i;
   		      std::cout << "[ADDR: 0x" << std::hex << m_vBase[i] << "]\n"; ++i;
            ++i;
            break;
      			
          case cmFUNC:
		        std::cout << "CALL\t"; ++i;
		        std::cout << "[Arg:" << std::dec << m_vBase[i] << "]"; ++i;
		        std::cout << "[ADDR: 0x" << std::hex << m_vBase[i] << "]\n"; ++i;
		        break;

          case cmFUNC_STR:
	  	      std::cout << "CALL STRFUNC\t"; ++i;
		        std::cout << "[Arg:" << std::dec << m_vBase[i] << "]"; ++i;
		        std::cout << "[ADDR: 0x" << m_vBase[i] << "]"; ++i;
		        std::cout << "[IDX:" << std::dec << m_vBase[i] << "]\n"; ++i;
	          break;

          case cmLT: std::cout << "LT\n"; ++i; break;
          case cmGT: std::cout << "GT\n"; ++i; break;
          case cmLE: std::cout << "LE\n"; ++i; break;
          case cmGE: std::cout << "GE\n"; ++i; break;
          case cmEQ: std::cout << "EQ\n"; ++i; break;
          case cmNEQ: std::cout << "NEQ\n"; ++i; break;
          case cmADD: std::cout << "ADD\n"; ++i; break;
          case cmAND: std::cout << "AND\n"; ++i; break;
          case cmOR:  std::cout << "OR\n";  ++i; break;
          case cmXOR: std::cout << "XOR\n"; ++i; break;
          case cmSUB: std::cout << "SUB\n"; ++i; break;
          case cmMUL: std::cout << "MUL\n"; ++i; break;
          case cmDIV: std::cout << "DIV\n"; ++i; break;
          case cmPOW: std::cout << "POW\n"; ++i; break;

          case cmASSIGN: 
            std::cout << "ASSIGN\t"; ++i; 
		        std::cout << "[ADDR: 0x" << m_vBase[i] << "]\n"; ++i;
            break; 

          default: std::cout << "(unknown code: " << m_vBase[i] << ")\n"; ++i;	break;
	      }
      }

      std::cout << "END" << std::endl;
    }
};

} // namespace mu

#endif


