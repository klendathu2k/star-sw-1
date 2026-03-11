#ifndef StiRootSimpleTrackFilter_H_INCLUDED
#define StiRootSimpleTrackFilter_H_INCLUDED

/// @file EditableFilter.h
/// @brief Abstract filter with runtime-editable parameters derived from EditableParameters.
///
/// @ingroup StiUtilities

/*! Pure virtual class defining an editable filter. */

#include "Sti/Base/Filter.h"
#include "Sti/Base/EditableParameters.h"

/// @class EditableFilter
/// @brief Filter with runtime-editable parameters inherited from EditableParameters.
///
/// Combines the Filter predicate interface with EditableParameters so that filter
/// criteria can be adjusted at runtime or via a GUI.
/// @ingroup StiUtilities
template <class Filtered>
class EditableFilter : public Filter<Filtered>, public EditableParameters
{
 public:
  
  EditableFilter();
  EditableFilter(const string & name, const string & description);
  virtual ~EditableFilter();
};

template <class Filtered>
EditableFilter<Filtered>::EditableFilter()
  :  Filter<Filtered>(),
     EditableParameters()
{}

template <class Filtered>
EditableFilter<Filtered>::EditableFilter(const string & name, const string & description)
  :  Filter<Filtered>(),
     EditableParameters(name,description)
{}

template <class Filtered>
EditableFilter<Filtered>::~EditableFilter()
{}

#endif
