/// @file StiMapUtilities.h
/// @brief Utility structures and functors for the keyed maps used in StiHitContainer and StiDetectorContainer.
///
/// @ingroup StiUtilities
#ifndef StiMapUtilities_h
#define StiMapUtilities_h

#include <string>
using std::string;
class StiDetector;
class StiHit;
class StTpcHit;
class StiTrackNode;
template<class NodeType> class StiCompositeTreeNode;

//Structure for hit map key
/// @class HitMapKey
/// @brief Two-dimensional map key encoding (refAngle, position) for StiHitContainer hit lookup.
/// @ingroup StiUtilities
struct HitMapKey {
    bool operator==(const HitMapKey&) const;
    double refangle;
    double position;
};

//Functor for ordering hit map key
/// @class MapKeyLessThan
/// @brief Ordering functor for HitMapKey with configurable tolerances.
/// @ingroup StiUtilities
struct MapKeyLessThan{
    MapKeyLessThan() : reftolerance(.01), postolerance(.01) {};
    bool operator() (const HitMapKey&, const HitMapKey&) const;
    double reftolerance;
    double postolerance;
};

// Structure for material, shape, or detector name map key
/// @class NameMapKey
/// @brief Map key encoding a string name for material, shape, or detector name maps.
/// @ingroup StiUtilities
struct NameMapKey {
    NameMapKey(const string& str){ name = str; }
    NameMapKey(){}
    bool operator==(const NameMapKey&) const;
    bool operator<(const NameMapKey&) const;
    string name;
};

//Detector sorter
/// @class StiDetectorNodePositionLessThan
/// @brief Ordering functor for StiDetector tree nodes by position.
/// @ingroup StiUtilities
struct StiDetectorNodePositionLessThan
{
    bool operator() (const StiCompositeTreeNode<StiDetector> *, const StiCompositeTreeNode<StiDetector> *) const;
};

/// @class SetHitUsed
/// @brief Functor that marks a StiTrackNode's associated hit as used.
/// @ingroup StiUtilities
struct SetHitUsed
{
    void operator() (StiTrackNode&);
};
/// @class SetHitUnused
/// @brief Functor that marks a StiTrackNode's associated hit as unused.
/// @ingroup StiUtilities
struct SetHitUnused
{
    void operator() (StiTrackNode&);
};

#endif


