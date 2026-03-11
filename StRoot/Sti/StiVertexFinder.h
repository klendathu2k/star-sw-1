/// @file StiVertexFinder.h
/// @brief Abstract interface for primary-vertex finding in the STI framework.
///
/// @ingroup StiVertexFinding
#ifndef StiVertexFinder_H
#define StiVertexFinder_H 1
#include "Sti/Base/Factory.h"
#include "Sti/Base/Named.h"
#include <vector>

class StEvent;
class StiHit;

/// @class StiVertexFinder
/// @brief Abstract interface for primary-vertex finders.
///
/// Concrete implementations (e.g. PPV — Pile-up Protected Vertex finder)
/// are registered with StiMaker at Init() time.  The interface exposes:
/// - `fit(StEvent*)` — runs the vertex finding algorithm; returns the number
///   of vertices found, or a negative error code.
/// - `getVertex(i)` — returns the i-th found vertex as a StiHit.
/// - `size()` — number of vertices found in the last call to `fit()`.
/// - `clear()` — resets the vertex list (called between events).
/// - `result()` — const access to the full vertex vector.
/// @ingroup StiVertexFinding
/*!
An abstract class defining  the interface to the vertex finder.
*/
/// @class StiVertexFinder
/// @brief Abstract interface for finding primary vertices within the STI framework.
///
/// Concrete subclasses use different vertex-finding strategies; found vertices are stored as StiHit objects.
/// @ingroup StiVertexFinding
class StiVertexFinder : public Named
{
public:
  StiVertexFinder(const string & name);
  virtual ~StiVertexFinder();
  /// Find the vertex(es) associated with the given event
  virtual int fit(StEvent*)=0;                     // fit the vertex
  virtual StiHit * getVertex(int index) =0;	
  virtual int  size() const =0;	
  Factory<StiHit>* getHitFactory();
  virtual void clear()=0;

const std::vector<StiHit*> *result();

protected:
  StiVertexFinder();//not implemented
  Factory<StiHit>* _hitFactory;
std::vector<StiHit*> _result;
};

inline Factory<StiHit>* StiVertexFinder::getHitFactory()
{
  return _hitFactory;
}

#endif
