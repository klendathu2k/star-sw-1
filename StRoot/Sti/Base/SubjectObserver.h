//SubjectObserver.h
//M.L. Miller (Yale Software)
//11/01

#ifndef SubjectObserver_HH
#define SubjectObserver_HH

/// @file SubjectObserver.h
/// @brief Observer pattern base classes used for parameter change notification in STI.
///
/// @ingroup StiUtilities

#include "Stiostream.h"
#include <vector>
using std::vector;

#include <algorithm>
using std::for_each;

class Subject;
class Observer;

/// @class Subject
/// @brief Observable base class that maintains a list of Observers and notifies them on changes.
///
/// Used for parameter change notification throughout the STI framework.
/// @ingroup StiUtilities
class Subject
{
public:
    ///For convenience
    typedef vector<Observer*> ObserverVec;
    
    //ctr-dstr
    Subject();
    virtual ~Subject();
    
    //gets/sets
    virtual void attach(Observer*);
    virtual void detach(Observer*);
    
    //Action
    virtual void notify();
    
protected:
    ObserverVec mObservers;
};

/// @class Observer
/// @brief Abstract observer that receives change notifications from a Subject.
///
/// Implement getNewState() to respond to subject changes. Used with EditableParameters
/// to react when STI tunable parameters are modified.
/// @ingroup StiUtilities
class Observer
{
public:
    
    Observer();
    Observer(Subject * subject);
    virtual ~Observer();
    virtual void getNewState()=0;
    virtual void changed(Subject* changedSubject);
    virtual void forgetSubject(Subject* theObsoleteSubject);
    
protected:  
    
    Subject* mSubject;
};

/*
  inline void Observer::changed(Subject* changedSubject)
  {
  if (changedSubject!=mSubject) 
  cout <<"Observer::changed(Subject*) - ERROR - changedSubject!=mSubject"<<endl;
  else 
  getNewState();
  }*/

inline void Observer::forgetSubject(Subject* obsolete)
{
  if (obsolete==mSubject) 
      mSubject=0;
  else 
      cout <<"Observer::forgetSubject(Subject*) - ERROR - obsolete!=mSubject"<<endl;
}

#endif
