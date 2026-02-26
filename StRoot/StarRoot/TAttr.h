// Author: Victor Perev   08/04/01

/*!
 * \file TAttr.h
 * \brief Named attribute map class for storing string, integer, and double values by key.
 */

#ifndef ROOT_TAttr
#define ROOT_TAttr


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TAttr                                                                //
// General attribute collection                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TString.h"
#include "TList.h"

/*!
 * \class TAttr
 * \brief Named attribute map: store and retrieve string, integer, and double values by key.
 * \details Inherits TList; each attribute is stored as a named TNamed-like entry.
 *          Supports signed/unsigned integer, double, and string value types.
 */
class TAttr: public TList
{
public:
            TAttr(const char *name="");
           ~TAttr();
  /// Store a string attribute. Passing ".remove" as \p val removes the key.
  void        SetAttr(const char *key,const char* val);
  /// Store a signed long integer attribute.
  void        SetAttr(const char *key,Long_t         val);
  /// Store an unsigned long integer attribute.
  void        SetAttr(const char *key,ULong_t     val);
  /// Store a double attribute.
  void        SetAttr(const char *key,double      val);
  /// Copy all attributes from \p att into this map; returns 0 on success.
  int         SetAttr(const TAttr *att);
  /// Remove the attribute with the given key.
  void        RemAttr(const char *key                )	{return SetAttr(key,".remove");}
  /// Return the signed long integer value for \p key (0 if absent).
  Long_t         IAttr(const char *key) const;
  /// Return the unsigned long integer value for \p key (0 if absent).
  ULong_t      UAttr(const char *key) const;
  /// Return the double value for \p key (0.0 if absent).
  double      DAttr(const char *key) const;
  /// Return the string value for \p key (empty string if absent).
  const char *SAttr(const char *key) const;
  /// Print all key-value pairs to stdout.
  void        PrintAttr() const;
 static void SetDebug(Int_t k = 0) {_debug = k;}
//		Data members
private:
 static Int_t _debug; 
ClassDef(TAttr,1)

};
#endif //ROOT_TAttr
