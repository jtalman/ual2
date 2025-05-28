// @(#)root/xml:$Name:  $:$Id: TXMLPlayer.cxx,v 1.7 2005/02/23 11:46:02 brun Exp $
// Author: Sergey Linev, Rene Brun  10.05.2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//________________________________________________________________________
//
// Class for xml code generation
// It should be used for generation of xml steramers, which could be used outside root
// enviroment. This means, that with help of such streamers user can read and write 
// objects from/to xml file, which later can be accepted by ROOT. 
//
// At the moment supported only classes, which are not inherited from TObject
// and which not contains any TObject members.
//
// To generate xml code:
//
// 1. ROOT library with required classes should be created.
//    In general, without such library non of user objects can be stored and
//    retrived from any ROOT file
//
// 2. Generate xml streamers by root script like: 
//
//    void generate() {
//      gSystem->Load("libRXML.so");   // load ROOT xml library
//      gSystem->Load("libuser.so");   // load user ROOT library
//
//      TList lst;
//      lst.Add(gROOT->GetClass("TUserClass1"));
//      lst.Add(gROOT->GetClass("TUserClass2"));
//      ...
//      TXMLPlayer player;             
//      player.ProduceCode(&lst, "streamers");    // create xml streamers
//    }
// 
//  3. Copy "streamers.h", "streamers.cxx", "TXmlFile.h", "TXmlFile.cxx" files
//     to user project and compile them. TXmlFile class implementation can be taken 
//     from http://www-linux.gsi.de/~linev/xmlfile.tar.gz
//
// TXMLPlayer class generates one function per class, which called class streamer.
// Name of such function for class TExample will be TExample_streamer.
//
// Following data members for streamed classes are supported:
//  - simple data types (int, double, float)
//  - array of simple types (int[5], double[5][6])
//  - dynamic array of simple types (int* with comment field // [fSize])
//  - const char*
//  - object of any nonROOT class
//  - pointer on object
//  - array of objects
//  - array of pointers on objects
//  - stl string
//  - stl vector, list, deque, set, multiset, map, multimap 
//  - allowed arguments for stl containers are: simple data types, string, object, pointer on object
//  Any other data member can not be (yet) read from xml file and write to xml file.
//
// If data member of class is private or protected, it can not be accessed via 
// member name. Two alternative way is supported. First, if for class member fValue 
// exists function GetValue(), it will be used to get value from the class, and if 
// exists SetValue(), it will be used to set apropriate data member. Names of setter
// and getter methods can be specified in comments filed like:
//
//     int  fValue;   // *OPTION={GetMethod="GetV";SetMethod="SetV"}
// 
// If getter or setter methods does not available, address to data member will be 
// calculated as predefined offeset to object start address. In that case generated code 
// should be used only on the same platform (OS + compiler), where it was generated.
//
// Generated streamers resolve inheritance tree for given class. This allows to have
// array (or vector) of object pointers on some basic class, while objects of derived 
// class(es) are used.
//
// To access data from xml files, user should use TXmlFile class, which is different from
// ROOT TXMLFile, but provides very similar functionality. For example, to read 
// object from xml file:
//
//        TXmlFile file("test.xml");             // open xml file 
//        file.ls();                             // show list of keys in file
//        TExample* ex1 = (TExample*) file.Get("ex1", TExample_streamer); // get object 
//        file.Close(); 
//
// To write object to file: 
// 
//        TXmlFile outfile("test2.xml", "recreate");    // create xml file
//        TExample* ex1 = new TExample;
//        outfile.Write(ex1, "ex1", TExample_streamer);   // write object to file
//        outfile.Close();
//  
// Complete example for generating and using of external xml streamers can be taken from
// http://www-linux.gsi.de/~linev/xmlreader.tar.gz
//
// Any bug reports and requests for additional functionality are welcome. 
//  
// Sergey Linev, S.Linev@gsi.de
//
//________________________________________________________________________

#include "TXMLPlayer.h"

#include "Riostream.h"
#include "TClass.h"
#include "TStreamerInfo.h"
#include "TStreamerElement.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TDataMember.h"
#include "TMethod.h"
#include "TDataType.h"
#include "TMethodCall.h"
#include "TFunction.h"
#include "TVirtualCollectionProxy.h"
#include "TClassEdit.h"
#include <string>
#include <vector>

const char* tab1 = "   ";
const char* tab2 = "      ";
const char* tab3 = "         ";
const char* tab4 = "            ";

const char* names_xmlfileclass = "TXmlFile";

ClassImp(TXMLPlayer);

//______________________________________________________________________________
TXMLPlayer::TXMLPlayer() : TObject() 
{
// default constructor
}

//______________________________________________________________________________
TXMLPlayer::~TXMLPlayer() 
{
// destructor of TXMLPlayer object
}

//______________________________________________________________________________
TString TXMLPlayer::GetStreamerName(TClass* cl) 
{
// returns streamer function name for given class 

  if (cl==0) return "";
  TString res = cl->GetName();
  res += "_streamer";
  return res;
}
      
//______________________________________________________________________________
Bool_t TXMLPlayer::ProduceCode(TList* cllist, const char* filename) 
{
// Produce streamers for provide class list
// TList should include list of classes, for which code should be generated.
// filename specify name of file (without extension), where streamers should be
// created. Function produces two files: header file and source file.
// For instance, if filename is "streamers", files "streamers.h" and "streamers.cxx"
// will be created.

   if ((cllist==0) || (filename==0)) return kFALSE;
   
   ofstream fh(TString(filename)+".h");
   ofstream fs(TString(filename)+".cxx");
   
   fh << "// generated header file" << endl << endl;
   fh << "#ifndef " << filename << "_h" << endl;
   fh << "#define " << filename << "_h" << endl << endl;
   
   fh << "#include \"" << names_xmlfileclass << ".h\"" << endl << endl;
   
   fs << "// generated source file" << endl << endl;
   fs << "#include \"" << filename << ".h\"" << endl << endl;
   
   // produce appropriate include for all classes
   
   TObjArray inclfiles;
   TIter iter(cllist);
   TClass* cl = 0;
   while ((cl = (TClass*) iter()) != 0) {
     if (inclfiles.FindObject(cl->GetDeclFileName())==0) {
        fs << "#include \"" << cl->GetDeclFileName() << "\"" << endl;
        inclfiles.Add(new TNamed(cl->GetDeclFileName(),""));
     }
   }
   inclfiles.Delete();
   
   fh << endl;
   fs << endl;
   
   
   // produce streamers declarations and implementations
   
   iter.Reset();
   
   while ((cl = (TClass*) iter()) != 0) {
       
      fh << "extern void* " << GetStreamerName(cl) << "(" 
         << names_xmlfileclass << " &buf, void* ptr = 0, bool checktypes = true);" << endl << endl; 
      
      ProduceStreamerSource(fs, cl, cllist);
   }

   fh << "#endif" << endl << endl;
   fs << endl << endl;
    
   return kTRUE; 
}

//______________________________________________________________________________
TString TXMLPlayer::GetMemberTypeName(TDataMember* member)
{
// returns name of simple data type for given data member

   if (member==0) return "int"; 
    
   if (member->IsBasic())
   switch (member->GetDataType()->GetType()) { 
     case kChar_t:     return "char";
     case kShort_t:    return "short";
     case kInt_t:      return "int";
     case kLong_t:     return "long";
     case kLong64_t:   return "long long";
     case kFloat_t:    return "float";
     case kDouble32_t:
     case kDouble_t:   return "double";
     case kUChar_t:    {
       char first = member->GetDataType()->GetTypeName()[0];  
       if ((first=='B') || (first=='b')) return "bool";
                                    else return "unsigned char";         
     }
     case kBool_t:     return "bool";
     case kUShort_t:   return "unsigned short";
     case kUInt_t:     return "unsigned int";
     case kULong_t:    return "unsigned long";
     case kULong64_t:  return "unsigned long long";
   }
   
   if (member->IsEnum()) return "int";
   
   return member->GetTypeName();
}

//______________________________________________________________________________
TString TXMLPlayer::GetBasicTypeName(TStreamerElement* el)
{
// return simple data types for given TStreamerElement object

   if (el->GetType() == TStreamerInfo::kCounter) return "int"; 

   switch (el->GetType() % 20) {
     case TStreamerInfo::kChar:     return "char";
     case TStreamerInfo::kShort:    return "short";
     case TStreamerInfo::kInt:      return "int";
     case TStreamerInfo::kLong:     return "long";
     case TStreamerInfo::kLong64:   return "long long";
     case TStreamerInfo::kFloat:    return "float";
     case TStreamerInfo::kDouble32:
     case TStreamerInfo::kDouble:   return "double";
     case TStreamerInfo::kUChar: {
       char first = el->GetTypeNameBasic()[0];  
       if ((first=='B') || (first=='b')) return "bool";
                                    else return "unsigned char";         
     }
     case TStreamerInfo::kBool:     return "bool";
     case TStreamerInfo::kUShort:   return "unsigned short";
     case TStreamerInfo::kUInt:     return "unsigned int";
     case TStreamerInfo::kULong:    return "unsigned long";
     case TStreamerInfo::kULong64:  return "unsigned long long";
   }
   return "int";  
}

//______________________________________________________________________________
TString TXMLPlayer::GetBasicTypeReaderMethodName(Int_t type, const char* realname) 
{
// return functions name to read simple data type from xml file 

   if (type == TStreamerInfo::kCounter) return "ReadInt"; 
    
   switch (type % 20) {
     case TStreamerInfo::kChar:     return "ReadChar";
     case TStreamerInfo::kShort:    return "ReadShort";
     case TStreamerInfo::kInt:      return "ReadInt";
     case TStreamerInfo::kLong:     return "ReadLong";
     case TStreamerInfo::kLong64:   return "ReadLong64";
     case TStreamerInfo::kFloat:    return "ReadFloat";
     case TStreamerInfo::kDouble32:
     case TStreamerInfo::kDouble:   return "ReadDouble";
     case TStreamerInfo::kUChar: {
       bool isbool = false;
       if (realname!=0) 
         isbool = (TString(realname).Index("bool",0, TString::kIgnoreCase)>=0);
       if (isbool) return "ReadBool";
              else return "ReadUChar";         
     }
     case TStreamerInfo::kBool:     return "ReadBool";
     case TStreamerInfo::kUShort:   return "ReadUShort";
     case TStreamerInfo::kUInt:     return "ReadUInt";
     case TStreamerInfo::kULong:    return "ReadULong";
     case TStreamerInfo::kULong64:  return "ReadULong64";
   }
   return "ReadValue";  
}

//______________________________________________________________________________
const char* TXMLPlayer::ElementGetter(TClass* cl, const char* membername, int specials)
{
// produce code to access member of given class. 
// Parameter specials has following meaning:
//    0 - nothing special
//    1 - cast to data type
//    2 - produce pointer on given member 
//    3 - skip casting when produce pointer by buf.P() function

   TClass* membercl = cl ? cl->GetBaseDataMember(membername) : 0;
   TDataMember* member = membercl ? membercl->GetDataMember(membername) : 0;
   TMethodCall* mgetter = member ? member->GetterMethod(0) : 0;
   
   if ((mgetter!=0) && (mgetter->GetMethod()->Property() & kIsPublic)) {
      fGetterName = "obj->";
      fGetterName += mgetter->GetMethodName();
      fGetterName += "()";
   } else 
   if ((member==0) || ((member->Property() & kIsPublic) != 0)) {
      fGetterName = "obj->";
      fGetterName += membername;
   } else {
      fGetterName = "";
      bool deref = (member->GetArrayDim()==0) && (specials!=2);
      if (deref) fGetterName += "*(";
      if (specials!=3) {
         fGetterName += "("; 
         if (member->Property() & kIsConstant) fGetterName += "const ";
         fGetterName += GetMemberTypeName(member);
         if (member->IsaPointer()) fGetterName+="*";
         fGetterName += "*) ";
      }
      fGetterName += "buf.P(obj,";
      fGetterName += member->GetOffset();
      fGetterName += ")";
      if (deref) fGetterName += ")";
      specials = 0;
   }
   
   if ((specials==1) && (member!=0)) {
      TString cast = "(";
      cast += GetMemberTypeName(member);   
      if (member->IsaPointer() || (member->GetArrayDim()>0)) cast += "*";
      cast += ") ";
      cast += fGetterName;
      fGetterName = cast;
   }
   
   if ((specials==2) && (member!=0)) {
      TString buf = "&(";
      buf += fGetterName; 
      buf += ")";
      fGetterName = buf;
   }
   
   return fGetterName.Data();
}

//______________________________________________________________________________
const char* TXMLPlayer::ElementSetter(TClass* cl, const char* membername, char* endch)
{
// Produce code to set value to given data member.
// endch should be output after value is specified.

   strcpy(endch,""); 
   
   TClass* membercl = cl ? cl->GetBaseDataMember(membername) : 0;
   TDataMember* member = membercl ? membercl->GetDataMember(membername) : 0;
   TMethodCall* msetter = member ? member->SetterMethod(cl) : 0;
   
   if ((msetter!=0) && (msetter->GetMethod()->Property() & kIsPublic)) {
      fSetterName = "obj->";
      fSetterName += msetter->GetMethodName();
      fSetterName += "(";
      strcpy(endch,")"); 
   } else 
   if ((member==0) || (member->Property() & kIsPublic) != 0) {
      fSetterName = "obj->";
      fSetterName += membername;
      fSetterName += " = ";
   } else {
      fSetterName = "";
      if (member->GetArrayDim()==0) fSetterName += "*";
      fSetterName += "((";
      if (member->Property() & kIsConstant) fSetterName += "const ";
      fSetterName += GetMemberTypeName(member);
      if (member->IsaPointer()) fSetterName += "*";
      fSetterName += "*) buf.P(obj,";
      fSetterName += member->GetOffset();
      fSetterName += ")) = ";
   }
   return fSetterName.Data();
}

//______________________________________________________________________________
void TXMLPlayer::ProduceStreamerSource(ostream& fs, TClass* cl, TList* cllist) 
{
// Produce source code of streamer function for specified class

   if (cl==0) return; 
   TStreamerInfo* info = cl->GetStreamerInfo();
   TObjArray* elements = info->GetElements();
   if (elements==0) return;

   fs << "//__________________________________________________________________________" << endl;
   fs << "void* " << GetStreamerName(cl) << "(" 
         << names_xmlfileclass << " &buf, void* ptr, bool checktypes)" << endl; 
   fs << "{" << endl;
   fs << tab1 << cl->GetName() << " *obj = (" << cl->GetName() << "*) ptr;" << endl;
   
   fs << tab1 << "if (buf.IsReading()) { " << endl;
   
   TIter iter(cllist);
   TClass* c1 = 0;
   bool firstchild = true;
   
   while ((c1 = (TClass*) iter()) != 0) {
      if (c1==cl) continue;
      if (c1->GetListOfBases()->FindObject(cl->GetName())==0) continue;
      if (firstchild) {
         fs << tab2 << "if (checktypes) {" << endl;
         fs << tab3 << "void* ";
         firstchild = false;
      } else 
         fs << tab3;
      fs << "res = " << GetStreamerName(c1) 
         << "(buf, dynamic_cast<" << c1->GetName() << "*>(obj));" << endl;
      fs << tab3 << "if (res) return dynamic_cast<" << cl->GetName() 
         << "*>(("<< c1->GetName() << " *) res);" << endl;
   }
   if (!firstchild) fs << tab2 << "}" << endl;
   
   fs << tab2 << "if (!buf.CheckClassNode(\"" << cl->GetName() << "\", " 
              << info->GetClassVersion() << ")) return 0;" << endl;
              
   fs << tab2 << "if (obj==0) obj = new " << cl->GetName() << ";" << endl;
   
   int n;
   for (n=0;n<=elements->GetLast();n++) {
       
      TStreamerElement* el = dynamic_cast<TStreamerElement*> (elements->At(n));
      if (el==0) continue; 
      
      Int_t typ = el->GetType();
      
      switch (typ) {
         // basic types
         case TStreamerInfo::kBool:              
         case TStreamerInfo::kChar:              
         case TStreamerInfo::kShort:
         case TStreamerInfo::kInt:  
         case TStreamerInfo::kLong:  
         case TStreamerInfo::kLong64:
         case TStreamerInfo::kFloat: 
         case TStreamerInfo::kDouble:
         case TStreamerInfo::kUChar:             
         case TStreamerInfo::kUShort:
         case TStreamerInfo::kUInt:  
         case TStreamerInfo::kULong: 
         case TStreamerInfo::kULong64: 
         case TStreamerInfo::kDouble32:
         case TStreamerInfo::kCounter: {
            char endch[5]; 
            fs << tab2 << ElementSetter(cl, el->GetName(), endch);
            fs << "buf." << GetBasicTypeReaderMethodName(el->GetType(), 0) 
               << "(\"" << el->GetName() << "\")" << endch << ";" << endl;
            continue;
         }
         
         // array of basic types like bool[10]
         case TStreamerInfo::kOffsetL + TStreamerInfo::kBool:    
         case TStreamerInfo::kOffsetL + TStreamerInfo::kChar:    
         case TStreamerInfo::kOffsetL + TStreamerInfo::kShort:   
         case TStreamerInfo::kOffsetL + TStreamerInfo::kInt:     
         case TStreamerInfo::kOffsetL + TStreamerInfo::kLong:    
         case TStreamerInfo::kOffsetL + TStreamerInfo::kLong64:  
         case TStreamerInfo::kOffsetL + TStreamerInfo::kFloat:   
         case TStreamerInfo::kOffsetL + TStreamerInfo::kDouble:  
         case TStreamerInfo::kOffsetL + TStreamerInfo::kUChar:   
         case TStreamerInfo::kOffsetL + TStreamerInfo::kUShort:  
         case TStreamerInfo::kOffsetL + TStreamerInfo::kUInt:    
         case TStreamerInfo::kOffsetL + TStreamerInfo::kULong:   
         case TStreamerInfo::kOffsetL + TStreamerInfo::kULong64: 
         case TStreamerInfo::kOffsetL + TStreamerInfo::kDouble32: {
            fs << tab2 << "buf.ReadArray("
                       << ElementGetter(cl, el->GetName(), (el->GetArrayDim()>1) ? 1 : 0);
            fs         << ", " << el->GetArrayLength()
                       << ", \"" << el->GetName() << "\");" << endl; 
            continue;   
         }
         
         // array of basic types like bool[n] 
         case TStreamerInfo::kOffsetP + TStreamerInfo::kBool:    
         case TStreamerInfo::kOffsetP + TStreamerInfo::kChar:    
         case TStreamerInfo::kOffsetP + TStreamerInfo::kShort:   
         case TStreamerInfo::kOffsetP + TStreamerInfo::kInt:     
         case TStreamerInfo::kOffsetP + TStreamerInfo::kLong:    
         case TStreamerInfo::kOffsetP + TStreamerInfo::kLong64:  
         case TStreamerInfo::kOffsetP + TStreamerInfo::kFloat:   
         case TStreamerInfo::kOffsetP + TStreamerInfo::kDouble:  
         case TStreamerInfo::kOffsetP + TStreamerInfo::kUChar:   
         case TStreamerInfo::kOffsetP + TStreamerInfo::kUShort:  
         case TStreamerInfo::kOffsetP + TStreamerInfo::kUInt:    
         case TStreamerInfo::kOffsetP + TStreamerInfo::kULong:   
         case TStreamerInfo::kOffsetP + TStreamerInfo::kULong64:
         case TStreamerInfo::kOffsetP + TStreamerInfo::kDouble32: {
            TStreamerBasicPointer* elp = dynamic_cast<TStreamerBasicPointer*> (el);
            if (elp==0) {
              cout << "fatal error with TStreamerBasicPointer" << endl;  
              continue;
            }
            char endch[5]; 
            
            fs << tab2 << ElementSetter(cl, el->GetName(), endch);
            fs         << "buf.ReadArray(" << ElementGetter(cl, el->GetName());
            fs         << ", " << ElementGetter(cl, elp->GetCountName());
            fs         << ", \"" << el->GetName() << "\", true)" << endch << ";" << endl; 
            continue;   
         }
         
         case TStreamerInfo::kCharStar: {
            char endch[5]; 
            fs << tab2 << ElementSetter(cl, el->GetName(), endch);
            fs         << "buf.ReadCharStar(" << ElementGetter(cl, el->GetName());
            fs         << ", \"" << el->GetName() << "\")" << endch << ";" << endl; 
            continue;   
         }
         
         case TStreamerInfo::kBase: {
            fs << tab2 << GetStreamerName(el->GetClassPointer()) 
               << "(buf, dynamic_cast<" << el->GetClassPointer()->GetName() 
               << "*>(obj), false);" << endl;
            continue;
         }
         
         // Class*   Class not derived from TObject and with comment field //->
         case TStreamerInfo::kAnyp:     
         case TStreamerInfo::kAnyp    + TStreamerInfo::kOffsetL: {
            if (el->GetArrayLength()>0) {
               fs << tab2 << "buf.ReadObjectArr(" << ElementGetter(cl, el->GetName());
               fs         << ", " << el->GetArrayLength() << ", -1"
                          << ", \"" << el->GetName() << "\", "
                          << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            } else {
              fs << tab2 << "buf.ReadObject(" << ElementGetter(cl, el->GetName());
              fs         << ", \"" << el->GetName() << "\", "
                         << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            }
            continue;
         }
         
         // Class*   Class not derived from TObject and no comment
         case TStreamerInfo::kAnyP: 
         case TStreamerInfo::kAnyP + TStreamerInfo::kOffsetL: {
            if (el->GetArrayLength()>0) {
              fs << tab2 << "for (int n=0;n<" << el->GetArrayLength() << ";n++) "
                         << "delete (" << ElementGetter(cl, el->GetName()) << ")[n];" << endl;
              fs << tab2 << "buf.ReadObjectPtrArr((void**) " << ElementGetter(cl, el->GetName(), 3);
              fs         << ", " << el->GetArrayLength() 
                         << ", \"" << el->GetName() << "\", "
                         << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            } else {
              char endch[5]; 
            
              fs << tab2 << "delete " << ElementGetter(cl, el->GetName()) << ";" << endl;
              fs << tab2 << ElementSetter(cl, el->GetName(), endch);
              fs         << "(" << el->GetClassPointer()->GetName() 
                         << "*) buf.ReadObjectPtr(\"" << el->GetName() << "\", "
                         << GetStreamerName(el->GetClassPointer()) 
                         << ")" <<endch << ";" << endl;
            }
            continue;
         }

         // Class  NOT derived from TObject
         case TStreamerInfo::kAny: {  
            fs << tab2 << "buf.ReadObject(" << ElementGetter(cl, el->GetName(), 2);
            fs         << ", \"" << el->GetName() << "\", "
                       << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            continue;
         }
       
         // Class  NOT derived from TObject, array
         case TStreamerInfo::kAny + TStreamerInfo::kOffsetL: { 
            fs << tab2 << "buf.ReadObjectArr(" << ElementGetter(cl, el->GetName());
            fs         << ", " << el->GetArrayLength() 
                       << ", sizeof(" << el->GetClassPointer()->GetName()
                       << "), \"" << el->GetName() << "\", "
                       << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            continue;
         }
         
         // container with no virtual table (stl) and no comment
         case TStreamerInfo::kSTLp:                
         case TStreamerInfo::kSTL:
         case TStreamerInfo::kSTLp + TStreamerInfo::kOffsetL:                
         case TStreamerInfo::kSTL + TStreamerInfo::kOffsetL: {
            TStreamerSTL* elstl = dynamic_cast<TStreamerSTL*> (el);
            if (elstl==0) break; // to make skip
            
            if (ProduceSTLstreamer(fs, cl, elstl, false)) continue;
            
            fs << tab2 << "// STL type = " << elstl->GetSTLtype() << endl;
            break;
         }
      }
     fs << tab2 << "buf.SkipMember(\"" << el->GetName() 
                << "\");   // sinfo type " << el->GetType() 
                << " of class " << el->GetClassPointer()->GetName()
                << " not supported" << endl;
   }
   
   fs << tab2 << "buf.EndClassNode();" << endl;
   
   fs << tab1 << "} else {" << endl;
   
   // generation of writing part of class streamer
      
   fs << tab2 << "if (obj==0) return 0;" << endl;
   
   firstchild = true;
   iter.Reset();
   while ((c1 = (TClass*) iter()) != 0) {
      if (c1==cl) continue;
      if (c1->GetListOfBases()->FindObject(cl->GetName())==0) continue;
      if (firstchild) {
        firstchild = false;
        fs << tab2 << "if (checktypes) {" << endl;  
      }
      fs << tab3 << "if (dynamic_cast<" << c1->GetName() << "*>(obj))" << endl;
      fs << tab4 << "return " << GetStreamerName(c1) << "(buf, dynamic_cast<" << c1->GetName() << "*>(obj));" << endl;
   }
   if (!firstchild) fs << tab2 << "}" << endl;
   
   fs << tab2 << "buf.StartClassNode(\"" << cl->GetName() << "\", " 
              << info->GetClassVersion() << ");" << endl;
   
   for (n=0;n<=elements->GetLast();n++) {
       
      TStreamerElement* el = dynamic_cast<TStreamerElement*> (elements->At(n));
      if (el==0) continue; 
      
      Int_t typ = el->GetType();
      
      switch (typ) {
         // write basic types
         case TStreamerInfo::kBool:              
         case TStreamerInfo::kChar:              
         case TStreamerInfo::kShort:
         case TStreamerInfo::kInt:  
         case TStreamerInfo::kLong:  
         case TStreamerInfo::kLong64:
         case TStreamerInfo::kFloat: 
         case TStreamerInfo::kDouble:
         case TStreamerInfo::kUChar:             
         case TStreamerInfo::kUShort:
         case TStreamerInfo::kUInt:  
         case TStreamerInfo::kULong: 
         case TStreamerInfo::kULong64: 
         case TStreamerInfo::kDouble32: 
         case TStreamerInfo::kCounter: {
            fs << tab2 << "buf.WriteValue(";
            if (typ==TStreamerInfo::kUChar) 
              fs <<"(unsigned char) " << ElementGetter(cl, el->GetName());
            else  
              fs << ElementGetter(cl, el->GetName());
            fs << ", \"" << el->GetName() << "\");" << endl; 
            continue;
         }
         
         // array of basic types   
         case TStreamerInfo::kOffsetL + TStreamerInfo::kBool:    
         case TStreamerInfo::kOffsetL + TStreamerInfo::kChar:    
         case TStreamerInfo::kOffsetL + TStreamerInfo::kShort:   
         case TStreamerInfo::kOffsetL + TStreamerInfo::kInt:     
         case TStreamerInfo::kOffsetL + TStreamerInfo::kLong:    
         case TStreamerInfo::kOffsetL + TStreamerInfo::kLong64:  
         case TStreamerInfo::kOffsetL + TStreamerInfo::kFloat:   
         case TStreamerInfo::kOffsetL + TStreamerInfo::kDouble:  
         case TStreamerInfo::kOffsetL + TStreamerInfo::kUChar:   
         case TStreamerInfo::kOffsetL + TStreamerInfo::kUShort:  
         case TStreamerInfo::kOffsetL + TStreamerInfo::kUInt:    
         case TStreamerInfo::kOffsetL + TStreamerInfo::kULong:   
         case TStreamerInfo::kOffsetL + TStreamerInfo::kULong64: 
         case TStreamerInfo::kOffsetL + TStreamerInfo::kDouble32: {
            fs << tab2 << "buf.WriteArray("
                       << ElementGetter(cl, el->GetName(), (el->GetArrayDim()>1) ? 1 : 0);
            fs         << ", " << el->GetArrayLength()
                       << ", \"" << el->GetName() << "\");" << endl; 
            continue;   
         }
         
         case TStreamerInfo::kOffsetP + TStreamerInfo::kBool:    
         case TStreamerInfo::kOffsetP + TStreamerInfo::kChar:    
         case TStreamerInfo::kOffsetP + TStreamerInfo::kShort:   
         case TStreamerInfo::kOffsetP + TStreamerInfo::kInt:     
         case TStreamerInfo::kOffsetP + TStreamerInfo::kLong:    
         case TStreamerInfo::kOffsetP + TStreamerInfo::kLong64:  
         case TStreamerInfo::kOffsetP + TStreamerInfo::kFloat:   
         case TStreamerInfo::kOffsetP + TStreamerInfo::kDouble:  
         case TStreamerInfo::kOffsetP + TStreamerInfo::kUChar:   
         case TStreamerInfo::kOffsetP + TStreamerInfo::kUShort:  
         case TStreamerInfo::kOffsetP + TStreamerInfo::kUInt:    
         case TStreamerInfo::kOffsetP + TStreamerInfo::kULong:   
         case TStreamerInfo::kOffsetP + TStreamerInfo::kULong64:
         case TStreamerInfo::kOffsetP + TStreamerInfo::kDouble32: {
            TStreamerBasicPointer* elp = dynamic_cast<TStreamerBasicPointer*> (el);
            if (elp==0) {
              cout << "fatal error with TStreamerBasicPointer" << endl;  
              continue;
            }
            fs << tab2 << "buf.WriteArray(" << ElementGetter(cl, el->GetName());
            fs         << ", " << ElementGetter(cl, elp->GetCountName())
                       << ", \"" << el->GetName() << "\", true);" << endl; 
            continue;   
         }
         
         case TStreamerInfo::kCharStar: {
            fs << tab2 << "buf.WriteCharStar(" << ElementGetter(cl, el->GetName())
                       << ", \"" << el->GetName() << "\");" << endl; 
            continue;   
         }
         
         case TStreamerInfo::kBase: {
            fs << tab2 << GetStreamerName(el->GetClassPointer()) 
               << "(buf, dynamic_cast<" << el->GetClassPointer()->GetName() 
               << "*>(obj), false);" << endl;
            continue;
         }
         
         // Class*   Class not derived from TObject and with comment field //->
         case TStreamerInfo::kAnyp:     
         case TStreamerInfo::kAnyp    + TStreamerInfo::kOffsetL: {
            if (el->GetArrayLength()>0) {
               fs << tab2 << "buf.WriteObjectArr(" << ElementGetter(cl, el->GetName());
               fs         << ", " << el->GetArrayLength() << ", -1"
                          << ", \"" << el->GetName() << "\", "
                          << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            } else {
               fs << tab2 << "buf.WriteObject(" << ElementGetter(cl, el->GetName());
               fs         << ", \"" << el->GetName() << "\", "
                          << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            }
            continue;
         }
         
         // Class*   Class not derived from TObject and no comment
         case TStreamerInfo::kAnyP: 
         case TStreamerInfo::kAnyP + TStreamerInfo::kOffsetL: {
            if (el->GetArrayLength()>0) {    
              fs << tab2 << "buf.WriteObjectPtrArr((void**) " << ElementGetter(cl, el->GetName(), 3);
              fs         << ", " << el->GetArrayLength() 
                         << ", \"" << el->GetName() << "\", "
                         << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            } else {
              fs << tab2 << "buf.WriteObjectPtr(" << ElementGetter(cl, el->GetName());
              fs         << ", \"" << el->GetName() << "\", "
                         << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            }
            continue;
         }

         case TStreamerInfo::kAny: {    // Class  NOT derived from TObject
            fs << tab2 << "buf.WriteObject(" << ElementGetter(cl, el->GetName(), 2);
            fs         << ", \"" << el->GetName() << "\", "
                       << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            continue;
         }
        
         case TStreamerInfo::kAny    + TStreamerInfo::kOffsetL: {
            fs << tab2 << "buf.WriteObjectArr(" << ElementGetter(cl, el->GetName());
            fs         << ", " << el->GetArrayLength() 
                       << ", sizeof(" << el->GetClassPointer()->GetName()
                       << "), \"" << el->GetName() << "\", "
                       << GetStreamerName(el->GetClassPointer()) << ");" << endl;
            continue;
         }

         // container with no virtual table (stl) and no comment
         case TStreamerInfo::kSTLp + TStreamerInfo::kOffsetL:
         case TStreamerInfo::kSTL + TStreamerInfo::kOffsetL: 
         case TStreamerInfo::kSTLp:                
         case TStreamerInfo::kSTL: {
            TStreamerSTL* elstl = dynamic_cast<TStreamerSTL*> (el);
            if (elstl==0) break; // to make skip
            
            if (ProduceSTLstreamer(fs, cl, elstl, true)) continue;
            fs << tab2 << "// STL type = " << elstl->GetSTLtype() << endl;
            break;
         }
         
      }
      fs << tab2 << "buf.MakeEmptyMember(\"" << el->GetName() 
                 << "\");   // sinfo type " << el->GetType() 
                 << " of class " << el->GetClassPointer()->GetName()
                 << " not supported" << endl;
   }
   
   fs << tab2 << "buf.EndClassNode();" << endl;
   
   fs << tab1 << "}" << endl;
   fs << tab1 << "return obj;" << endl;
   fs << "}" << endl << endl;
}

//______________________________________________________________________________
void TXMLPlayer::ReadSTLarg(ostream& fs, 
                            TString& argname, 
                            int argtyp, 
                            bool isargptr, 
                            TClass* argcl, 
                            TString& tname,
                            TString& ifcond) 
{
// Produce code to read argument of stl container from xml file

   switch(argtyp) {
     case TStreamerInfo::kBool:              
     case TStreamerInfo::kChar:              
     case TStreamerInfo::kShort:
     case TStreamerInfo::kInt:  
     case TStreamerInfo::kLong:  
     case TStreamerInfo::kLong64:
     case TStreamerInfo::kFloat: 
     case TStreamerInfo::kDouble:
     case TStreamerInfo::kUChar:             
     case TStreamerInfo::kUShort:
     case TStreamerInfo::kUInt:  
     case TStreamerInfo::kULong: 
     case TStreamerInfo::kULong64: 
     case TStreamerInfo::kDouble32:
     case TStreamerInfo::kCounter: {
        fs << tname << " " << argname << " = buf." 
           << GetBasicTypeReaderMethodName(argtyp, tname.Data()) << "(0);" << endl;
        break; 
     }

     case TStreamerInfo::kObject: {
        fs << tname << (isargptr ? " ": " *") << argname << " = " 
           << "(" << argcl->GetName() << "*)"
           << "buf.ReadObjectPtr(0, " 
           << GetStreamerName(argcl) << ");" << endl;
        if (!isargptr) {
          if (ifcond.Length()>0) ifcond+=" && ";
          ifcond += argname;
          TString buf = "*"; 
          buf += argname;  
          argname = buf;   
        }
        break; 
     }
     
     case TStreamerInfo::kSTLstring: {
        fs << "string *" << argname << " = " 
           << "buf.ReadSTLstring();" << endl;
                if (!isargptr) {
       if (ifcond.Length()>0) ifcond+=" && ";
          ifcond += argname;
          TString buf = "*"; 
          buf += argname;  
          argname = buf;   
        }
        break;
     }
     
     default:
       fs << "/* argument " << argname << " not supported */";
  }
}

//______________________________________________________________________________
void TXMLPlayer::WriteSTLarg(ostream& fs, const char* accname, int argtyp, bool isargptr, TClass* argcl)
{
// Produce code to write argument of stl container to xml file

  switch(argtyp) {
     case TStreamerInfo::kBool:
     case TStreamerInfo::kChar:
     case TStreamerInfo::kShort:
     case TStreamerInfo::kInt:  
     case TStreamerInfo::kLong:  
     case TStreamerInfo::kLong64:
     case TStreamerInfo::kFloat: 
     case TStreamerInfo::kDouble:
     case TStreamerInfo::kUChar:             
     case TStreamerInfo::kUShort:
     case TStreamerInfo::kUInt:  
     case TStreamerInfo::kULong: 
     case TStreamerInfo::kULong64: 
     case TStreamerInfo::kDouble32:
     case TStreamerInfo::kCounter: {
        fs << "buf.WriteValue(" << accname << ", 0);" << endl;
        break; 
     }
     
     case TStreamerInfo::kObject: {
        fs << "buf.WriteObjectPtr(";
        if (isargptr) fs << accname;
                 else fs << "&(" << accname << ")";
        fs << ", 0, " <<  GetStreamerName(argcl) << ");" << endl;
        break; 
     }
     
     case TStreamerInfo::kSTLstring: {
        fs << "buf.WriteSTLstring(";
        if (isargptr) fs << accname;
                 else fs << "&(" << accname << ")";
        fs << ");" << endl;
        break;         
     }
     
     default:
        fs << "/* argument not supported */" << endl;
  }
}

//______________________________________________________________________________
bool TXMLPlayer::ProduceSTLstreamer(ostream& fs, TClass* cl, TStreamerSTL* el, Bool_t isWriting)
{
// Produce code of xml streamer for data member of stl type

   if ((cl==0) || (el==0)) return false;
   
   TClass* contcl = el->GetClassPointer();  
//   fs << tab2 << "// class " << contcl->GetName() << endl;
   
   bool isstr = (el->GetSTLtype() == TStreamerElement::kSTLstring);   
   bool isptr = el->IsaPointer();
   bool isarr = (el->GetArrayLength()>0);
   bool isparent = (strcmp(el->GetName(), contcl->GetName())==0);
   
   int stltyp = -1;
   int narg = 0;
   int argtype[2];
   bool isargptr[2];
   TClass* argcl[2];
   TString argtname[2];
   
   if (isstr) stltyp = TStreamerElement::kSTLstring; else 
     if (TClassEdit::IsSTLCont(contcl->GetName())) {  
        string shortTypeName = TClassEdit::ShortType(contcl->GetName(), 
                                                     TClassEdit::kDropStlDefault);
        int nestedLoc = 0;
        vector<string> splitName;
        TClassEdit::GetSplit(shortTypeName.c_str(), splitName, nestedLoc);
        
        stltyp = TClassEdit::STLKind(splitName[0].c_str());
        switch (stltyp) {
          case TClassEdit::kVector   : narg = 1; break;
          case TClassEdit::kList     : narg = 1; break;
          case TClassEdit::kDeque    : narg = 1; break;
          case TClassEdit::kMap      : narg = 2; break;
          case TClassEdit::kMultiMap : narg = 2; break;
          case TClassEdit::kSet      : narg = 1; break;
          case TClassEdit::kMultiSet : narg = 1; break;
          default: return false;
        }
     
        for(int n=0;n<narg;n++) {
          argtype[n] = -1;
          isargptr[n] = false;
          argcl[n] = 0;
          argtname[n] = "";
          
//          if (splitName.size()<=n) return false;
       
          TString buf = splitName[n+1];
          
          argtname[n] = buf;         
          
          // nested STL containers not yet supported
          if (TClassEdit::IsSTLCont(buf.Data())) return false; 
          
          int pstar = buf.Index("*");
          
          if (pstar>0) {
             isargptr[n] = true;    
             pstar--;
             while ((pstar>0) && (buf[pstar]==' ')) pstar--;
             buf.Remove(pstar+1);
             
          } else 
            isargptr[n] = false;
            
          if (buf.Index("const ")==0) {
             buf.Remove(0,6);
             while ((buf.Length()>0) && (buf[0]==' ')) buf.Remove(0,1); 
          }
          
          TDataType *dt = (TDataType*)gROOT->GetListOfTypes()->FindObject(buf);
          if (dt) argtype[n] = dt->GetType(); else 
          if (buf=="string") argtype[n] = TStreamerInfo::kSTLstring; else {
             argcl[n] = gROOT->GetClass(buf);
             if (argcl[n]!=0) argtype[n]=TStreamerInfo::kObject;
          }
       
          if (argtype[n]<0) stltyp = -1;
       
//       fs << tab2 << "// arg " << n << "  name = /" << argtname[n] << "/  typ = " << argtype[n]
//                  << "  isptr = " << isargptr[n] << "   class = " << (argcl[n] ? argcl[n]->GetName() : "null") << endl;
     }
     
     if (stltyp<0) return false;
   }
   
   bool akaarrayaccess = (narg==1) && (argtype[0]<20);

   char tabs[30], tabs2[30];
   
   if (isWriting) {
       
     fs << tab2 << "if (buf.StartSTLnode(\"" 
                << fXmlSetup.XmlGetElementName(el) << "\")) {" << endl;
     
     fs << tab3 << contcl->GetName() << " ";
     
     TString accname;
     if (isptr) {
        if (isarr) { fs << "**cont"; accname = "(*cont)->"; }
              else { fs << "*cont"; accname = "cont->"; }
     } else 
       if (isarr) { fs << "*cont"; accname = "cont->"; }
             else { fs << "&cont"; accname = "cont."; }

     fs << " = ";
     
     if (isparent)
       fs << "*dynamic_cast<" << contcl->GetName() << "*>(obj);" << endl;
     else  
       fs << ElementGetter(cl, el->GetName()) << ";" << endl;
     
     if (isarr && el->GetArrayLength()) {
       strcpy(tabs, tab4);  
       fs << tab3 << "for(int n=0;n<" << el->GetArrayLength() << ";n++) {" << endl;
     } else strcpy(tabs, tab3);
    
     strcpy(tabs2, tabs); 
       
     if (isptr) {
       strcat(tabs2, tab1);   
       fs << tabs << "if (" << (isarr ? "*cont" : "cont") << "==0) {" << endl;
       fs << tabs2 << "buf.WriteSTLsize(0" << (isstr ? ",true);" : ");") << endl;
       fs << tabs << "} else {" << endl;
     }
      
     fs << tabs2 << "buf.WriteSTLsize(" << accname 
                 << (isstr ? "length(), true);" : "size());") << endl;
                 
     if (isstr) {
        fs << tabs2 << "buf.WriteSTLstringData(" << accname << "c_str());" << endl;
     } else {   
        if (akaarrayaccess) {
           fs << tabs2 << argtname[0] << "* arr = new " << argtname[0] 
                                      << "[" << accname << "size()];" << endl; 
           fs << tabs2 << "int k = 0;" << endl;                           
        }
              
        fs << tabs2 << contcl->GetName() << "::const_iterator iter;" << endl;
        fs << tabs2 << "for (iter = " << accname << "begin(); iter != " 
                    << accname << "end(); iter++)";
        if (akaarrayaccess) {
           fs << endl << tabs2 << tab1 << "arr[k++] = *iter;" << endl;
           fs << tabs2 << "buf.WriteArray(arr, " << accname << "size(), 0, false);" << endl;
           fs << tabs2 << "delete[] arr;" << endl;
        } else            
        if (narg==1) {
           fs << endl << tabs2 << tab1;
           WriteSTLarg(fs, "*iter", argtype[0], isargptr[0], argcl[0]);
        } else
        if (narg==2) {
           fs << " {" << endl;
           fs << tabs2 << tab1;
           WriteSTLarg(fs, "iter->first", argtype[0], isargptr[0], argcl[0]);
           fs << tabs2 << tab1;
           WriteSTLarg(fs, "iter->second", argtype[1], isargptr[1], argcl[1]);
           fs << tabs2 << "}" << endl;   
        }
     
     } // if (isstr)
    
     if (isptr) fs << tabs << "}" << endl;
     
     if (isarr && el->GetArrayLength()) {
       if (isptr) fs << tabs << "cont++;" << endl;
             else fs << tabs << "(void*) cont = (char*) cont + sizeof(" << contcl->GetName() << ");" << endl;
       fs << tab3 << "}" << endl;
     }
     
     fs << tab3 << "buf.EndSTLnode();" << endl;
     fs << tab2 << "}" << endl;
     
     
   } else {
       
       
     fs << tab2 << "if (buf.VerifySTLnode(\"" 
                << fXmlSetup.XmlGetElementName(el) << "\")) {" << endl;
     
     fs << tab3 << contcl->GetName() << " ";
     TString accname, accptr;
     if (isptr) {
        if (isarr) { fs << "**cont"; accname = "(*cont)->"; accptr = "*cont"; }
              else { fs << "*cont"; accname = "cont->"; accptr = "cont"; }
     } else 
       if (isarr) { fs << "*cont"; accname = "cont->"; }
             else { fs << "&cont"; accname = "cont."; }
             
     fs << " = ";
     
     if (isparent)
       fs << "*dynamic_cast<" << contcl->GetName() << "*>(obj);" << endl;
     else  
       fs << ElementGetter(cl, el->GetName()) << ";" << endl;
     
     if (isarr && el->GetArrayLength()) {
       strcpy(tabs, tab4);  
       fs << tab3 << "for(int n=0;n<" << el->GetArrayLength() << ";n++) {" << endl;
     } else strcpy(tabs, tab3);
    
     fs << tabs << "int size = buf.ReadSTLsize(" << (isstr ? "true);" : ");") << endl;
     
     if (isptr) {
        fs << tabs << "delete " << accptr << ";" << endl;
        fs << tabs << "if (size==0) " << accptr << " = 0;" << endl;
        fs << tabs << "        else " << accptr << " = new " << contcl->GetName() << ";" << endl;
        if (!isarr) {
          char endch[5]; 
          fs << tabs << ElementSetter(cl, el->GetName(), endch);
          fs         << "cont" << endch << ";" << endl;
        }
     } else {
        fs << tabs << accname << (isstr ? "erase();" : "clear();") << endl; 
     }
     
     if (isstr) {
        fs << tabs << "if (size>0) " << accname << "assign(buf.ReadSTLstringData(size));" << endl;  
     } else {
        if (akaarrayaccess) {
           fs << tabs << argtname[0] << "* arr = new " << argtname[0] << "[size];" << endl;
           fs << tabs << "buf.ReadArray(arr, size, 0, false);" << endl;
        }
     
        fs << tabs << "for(int k=0;k<size;k++)";
        
        if (akaarrayaccess) {
          fs << endl << tabs << tab1 << accname; 
          if ((stltyp==TClassEdit::kSet) || (stltyp==TClassEdit::kMultiSet))
            fs << "insert"; else fs << "push_back";  
          fs << "(arr[k]);" << endl;
          fs << tabs << "delete[] arr;" << endl;
        } else
        if (narg==1) {
          TString arg1("arg"), ifcond;
          fs << " {" << endl << tabs << tab1;
          ReadSTLarg(fs, arg1, argtype[0], isargptr[0], argcl[0], argtname[0], ifcond);  
          fs << tabs << tab1;
          if (ifcond.Length()>0) fs << "if (" << ifcond << ") ";
          fs << accname;
          if ((stltyp==TClassEdit::kSet) || (stltyp==TClassEdit::kMultiSet))
            fs << "insert"; else fs << "push_back";  
          fs << "(" << arg1 << ");" << endl;
          fs << tabs << "}" << endl;
        }
        else 
        if (narg==2) {
           TString arg1("arg1"), arg2("arg2"), ifcond;  
           fs << " {" << endl << tabs << tab1;
           ReadSTLarg(fs, arg1, argtype[0], isargptr[0], argcl[0], argtname[0], ifcond);  
           fs << tabs << tab1;
           ReadSTLarg(fs, arg2, argtype[1], isargptr[1], argcl[1], argtname[1], ifcond);
           fs << tabs << tab1;
           if (ifcond.Length()>0) fs << "if (" << ifcond << ") ";
           fs << accname << "insert(make_pair(" 
              << arg1 << ", " << arg2 << "));" << endl;
           fs << tabs << "}" << endl;   
        }
     }
     
     if (isarr && el->GetArrayLength()) {
       if (isptr) fs << tabs << "cont++;" << endl;
             else fs << tabs << "(void*) cont = (char*) cont + sizeof(" << contcl->GetName() << ");" << endl;
       fs << tab3 << "}" << endl;
     }
     
     fs << tab3 << "buf.EndSTLnode();" << endl;
     fs << tab2 << "}" << endl;
   } 
   return true;
}
