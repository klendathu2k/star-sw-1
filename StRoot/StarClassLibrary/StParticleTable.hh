/***************************************************************************
 *
 * $Id: StParticleTable.hh,v 1.5 2005/09/22 20:09:20 fisyak Exp $
 *
 * Author: Thomas Ullrich, May 99 (based on Geant4 code, see below) 
 ***************************************************************************
 *
 * The design of the StParticleDefinition class and all concrete
 * classes derived from it is largely based on the design of the 
 * G4ParticleDefinition class from Geant4 (RD44).
 * Although the code is in large parts different (modified or rewritten)
 * and adapted to the STAR framework the basic idea stays the same.
 *
 ***************************************************************************
 *
 * $Log: StParticleTable.hh,v $
 * Revision 1.5  2005/09/22 20:09:20  fisyak
 * Make StLorentzVector persistent
 *
 * Revision 1.4  2003/09/02 17:59:35  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 1.3  2000/01/04 14:57:56  ullrich
 * Added friend declaration to avoid warning messages
 * under Linux.
 *
 * Revision 1.2  1999/12/21 15:14:26  ullrich
 * Modified to cope with new compiler version on Sun (CC5.0).
 *
 * Revision 1.1  1999/05/14 18:50:00  ullrich
 * Initial Revision
 *
 **************************************************************************/
/*!
 * \file StParticleTable.hh
 * \brief Singleton registry mapping names and PDG codes to particle definitions.
 * \author Thomas Ullrich, May 1999 (based on Geant4 G4ParticleTable)
 */
#ifndef StParticleTable_hh
#define StParticleTable_hh
#ifdef __ROOT__
#include "Rtypes.h"
#endif

#include <Stiostream.h>
#include <string>
#include <map>
#include <vector>
class StParticleDefinition;
#if !defined(ST_NO_NAMESPACES)
using std::vector;
using std::map;
using std::string;
#endif

#ifdef ST_NO_TEMPLATE_DEF_ARGS
typedef vector<StParticleDefinition*, allocator<StParticleDefinition*> > StVecPtrParticleDefinition;
#else
typedef vector<StParticleDefinition*> StVecPtrParticleDefinition;
#endif

/*!
 * \class StParticleTable
 * \brief Singleton registry of all known particle species.
 *
 * \details Maintains three parallel lookup structures keyed by particle name,
 * PDG encoding, and Geant3 ID respectively.  All concrete particle singletons
 * register themselves here at program start.  The single instance is obtained
 * via instance() or particleTable().
 *
 * \sa StParticleDefinition
 */
class StParticleTable {
public:
    virtual ~StParticleTable();
    
    /// Return the singleton StParticleTable instance (synonym for instance()).
    static StParticleTable* particleTable();
    /// Return the singleton StParticleTable instance.
    static StParticleTable* instance();
    
    /// Return the number of registered particles (synonym for size()).
    unsigned int entries() const;
    /// Return the number of registered particles.
    unsigned int size() const;
    
    /// Return \c true if a particle with the given \a name is registered.
    bool contains(const string &) const;
    /// Return \c true if a particle with the given PDG \a encoding is registered.
    bool contains(int) const;
    /// Return \c true if a particle with the given Geant3 \a id is registered.
    bool containsGeantId(int) const;
    
    /// Find a particle by name; returns \c nullptr if not found.
    StParticleDefinition* findParticle(const string&)  const;
    /// Find a particle by PDG encoding; returns \c nullptr if not found.
    StParticleDefinition* findParticle(int)  const;
    /// Find a particle by Geant3 ID; returns \c nullptr if not found.
    StParticleDefinition* findParticleByGeantId(int) const;
    
    /// Register a new particle definition.
    void insert(StParticleDefinition*);
    /// Remove a particle definition from the table.
    void erase(StParticleDefinition*);
    
    /// Dump a human-readable summary of all registered particles to \a os.
    void dump(ostream& = cout);

    /// Return a vector of pointers to all registered particle definitions.
    StVecPtrParticleDefinition allParticles() const;
    
    friend class nobody;

private:
    StParticleTable();
    StParticleTable(const StParticleTable &right);
    
    static StParticleTable *mParticleTable;

#ifdef ST_NO_TEMPLATE_DEF_ARGS
    // as soon as Sun CC4.2 is gone this goes as well
    typedef map<int, int, less<int>,
	allocator< pair<const int, int> > >
    mGeantPdgMapType;
    typedef map<int, StParticleDefinition*, less<int>,
	allocator< pair<const int,StParticleDefinition*> > >
    mPdgMapType;
    typedef map<string,	StParticleDefinition*, less<string>,
	allocator< pair<const string,StParticleDefinition*> > >
    mNameMapType;
#else
    typedef map<int, int>                      mGeantPdgMapType;
    typedef map<int, StParticleDefinition*>    mPdgMapType;
    typedef map<string, StParticleDefinition*> mNameMapType;
#endif
    
    mGeantPdgMapType   mGeantPdgMap;     // Geant3 IDs only
    mPdgMapType        mPdgMap;          // PDG IDs only
    mNameMapType       mNameMap;         // complete list
#ifdef __ROOT__
  ClassDef(StParticleTable,1)
#endif
};
#endif






