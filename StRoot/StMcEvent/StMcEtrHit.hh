/***************************************************************************
 *
 ***************************************************************************/
/// @file StMcEtrHit.hh
/// @brief Monte Carlo hit class for the End-cap Tagger Ring (ETR).
#ifndef StMcEtrHit_hh
#define StMcEtrHit_hh

#include "StMcHit.hh"
class g2t_etr_hit_st;

/// @brief Monte Carlo hit class for the End-cap Tagger Ring (ETR).
///
/// Inherits position, momentum, and track information from StMcHit.
/// The volume ID encodes the layer and sector indices.
class StMcEtrHit : public StMcHit {
public:
  /// @brief Default constructor.
  StMcEtrHit() {}
  /// @brief Constructor from a GEANT g2t_etr_hit table row.
  StMcEtrHit(g2t_etr_hit_st* pt); 
  /// @brief Destructor.
  ~StMcEtrHit() {}
  
  /// @brief Returns the layer index decoded from the volume ID.
  int layer() const;
  /// @brief Returns the sector index decoded from the volume ID.
  int sector() const;
  /// @brief Prints hit information to standard output (ROOT menu entry).
  void Print(Option_t *option="") const; // *MENU* 
    
private:
    ClassDef(StMcEtrHit,1)
};
ostream&  operator<<(ostream& os, const StMcEtrHit&);
#endif
