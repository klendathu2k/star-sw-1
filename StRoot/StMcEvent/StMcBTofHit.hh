//                                                                           
/// @file StMcBTofHit.hh
/// @brief Monte Carlo hit in the Barrel Time-of-Flight (BTof) detector.
#ifndef StMcBTofHit_hh
#define StMcBTofHit_hh

#include "StObject.h"
#include "Stiostream.h"
#include "StThreeVectorF.hh"

//#include "StMemoryPool.hh"

class StMcTrack;

/// @brief Monte Carlo hit in the Barrel Time-of-Flight (BTof) detector.
///
/// Stores tray/module/cell geometry identifiers, energy deposit, Geant path
/// length, exact and simulated time-of-flight, collected charge, and parent track.
class StMcBTofHit : public StObject {
public:
    /// @brief Default constructor.
    StMcBTofHit();
    /// @brief Constructor with geometry and physics quantities (tray, module, cell, dE, pathLength, tof_exact, tof_measured, charge).
    StMcBTofHit(int, int, int, float, float, float, float, float); //!tray, module, cell, dE, pathLength, tof_exact, tof_measured, charge
    /// @brief Constructor with geometry, physics, hit position, and parent track.
    StMcBTofHit(int, int, int, float, float, float, float, float, StThreeVectorF&, StMcTrack*);// above + position + partentTrack
    /// @brief Destructor.
    virtual ~StMcBTofHit();
    
    /// @brief Equality comparison.
    int operator==(const StMcBTofHit&) const;
    /// @brief Inequality comparison.
    int operator!=(const StMcBTofHit&) const;
    /// @brief Accumulates energy deposit and path length from a hit in the same cell.
    void operator+=(const StMcBTofHit&);

    /// @brief Returns true if both hits occupy the same tray/module/cell.
    bool sameCell(const StMcBTofHit&) const;

  // "Get" Methods
    /// @brief Returns the BTof tray number.
    virtual int                            tray() const;
    /// @brief Returns the module number within the tray.
    virtual int                          module() const;
    /// @brief Returns the cell number within the module.
    virtual int                            cell() const;
    /// @brief Returns the energy deposited (GeV).
    virtual float                            dE() const;
    /// @brief Returns the Geant track path length through active material (cm).
    virtual float                    pathLength() const;
    /// @brief Returns the exact Geant time of flight (ns).
    virtual float                          time() const; //! time of flight geant
    /// @brief Returns the simulated (smeared) time of flight (ns).
    virtual float                           tof() const; //! tof simulated
    /// @brief Returns the collected charge.
    virtual float                        charge() const; //! collected charge
    /// @brief Returns the hit position in global coordinates.
    virtual StThreeVectorF             position() const;
    /// @brief Returns a pointer to the parent MC track.
    virtual StMcTrack*              parentTrack() const;	
    /// @brief Returns the parent MC track identifier.
    virtual int                   parentTrackId() const;	

  // "Set" Methods

    /// @brief Sets the BTof tray number.
    virtual void setTray(int);
    /// @brief Sets the module number within the tray.
    virtual void setModule(int);
    /// @brief Sets the cell number within the module.
    virtual void setCell(int);
    /// @brief Sets the energy deposited (GeV).
    virtual void setdE(float);
    /// @brief Sets the Geant track path length through active material (cm).
    virtual void setPathLength(float);
    /// @brief Sets the exact Geant time of flight (ns).
    virtual void setTime(float);
    /// @brief Sets the simulated (smeared) time of flight (ns).
    virtual void setTof(float);
    /// @brief Sets the collected charge.
    virtual void setCharge(float);
    /// @brief Sets the hit position in global coordinates.
    virtual void setPosition(StThreeVectorF&);
    /// @brief Sets the pointer to the parent MC track.
    virtual void setParentTrack(StMcTrack*);
    /// @brief Sets the parent MC track identifier.
    virtual void setParentTrackId(int);
    
  //  void* operator new(size_t)     { return mPool.alloc(); }
  //  void  operator delete(void* p) { mPool.free(p); }
    
protected:
    int                  mTray;         ///< BTof tray number.
    int                  mModule;       ///< Module number within the tray.
    int                  mCell;         ///< Cell number within the module.
    float                mdE;           ///< Energy deposited (GeV).
    float                mPathLength;   ///< Track path length through active material (cm).
    float                mTime;         ///< Exact Geant time of flight (ns).
    float                mTof;          ///< Simulated (smeared) time of flight (ns).
    float                mCharge;       ///< Collected charge.
    StThreeVectorF       mPosition;     ///< Hit position in global coordinates.
    StMcTrack*           mParentTrack;  ///< Pointer to the parent MC track.
    int			 mParentTrackId; ///< Parent MC track identifier.

  //private:
  //  static StMemoryPool mPool; 
    ClassDef(StMcBTofHit,1)
};

ostream&  operator<<(ostream& os, const StMcBTofHit&);

inline int  StMcBTofHit::tray() const {return mTray; }
inline int  StMcBTofHit::module() const {return mModule; }
inline int  StMcBTofHit::cell() const {return mCell; }
inline float StMcBTofHit::dE()  const {return mdE; }
inline float StMcBTofHit::pathLength()  const {return mPathLength; }
inline float StMcBTofHit::time()  const {return mTime; }
inline float StMcBTofHit::tof() const {return mTof; }
inline float StMcBTofHit::charge() const {return mCharge; }
inline StThreeVectorF StMcBTofHit::position() const {return mPosition; }
inline StMcTrack* StMcBTofHit::parentTrack() const {return mParentTrack; }
inline int StMcBTofHit::parentTrackId() const {return mParentTrackId; }

#endif

