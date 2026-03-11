#if !defined(EDITABLEPARAMETERS_H_INCLUDED_)
#define EDITABLEPARAMETERS_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// @file EditableParameters.h
/// @brief Parameters container that notifies Observers when values change via the Subject interface.
///
/// @ingroup StiUtilities

#include "Sti/Base/Parameters.h"
#include "Sti/Base/SubjectObserver.h"

/// @class EditableParameters
/// @brief Parameters container that notifies registered Observers when values change.
///
/// Inherits from Parameters for parameter storage and from Subject for the observer
/// notification mechanism used throughout the STI framework.
/// @ingroup StiUtilities
class EditableParameters : public Parameters, public Subject
{
 public:
  EditableParameters();
  EditableParameters(const string & name, const string & description);
  EditableParameters(const EditableParameters & parameter);
  virtual ~EditableParameters();
  const EditableParameters & operator=(const EditableParameters & parameter);
  virtual void setDefaults(); 
  friend ostream& operator<<(ostream& os, const EditableParameters&pars);
};

#endif  // !defined(EDITABLEPARAMETERS_H_INCLUDED_)
