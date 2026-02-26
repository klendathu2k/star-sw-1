/*!
 * \file StarPDGEncoding.hh
 * \brief STAR-specific PDG Monte Carlo encoding constants and helper function.
 */
#ifndef __StarPDGEncodings_hh__
#define __StarPDGEncodings_hh__
/// \brief Helper function to define PDG ids for heavy ions.
/// \param z Charge of the heavy ion.
/// \param a Atomic number of the heavy ion.
/// \param l Number of lambdas in a hypernucleus.
/// \return PDG encoding integer for the specified heavy ion.
int hid( int z, int a, int l=0 );

/// \brief STAR-specific PDG encoding constants for special particles.
enum {
  kTriton          = 1000000000, ///< PDG code for the triton (³H nucleus).
  kHyperTriton     = 1000000001, ///< PDG code for the hypertriton (³ΛH).
  kAntiHyperTriton = 1000000002, ///< PDG code for the anti-hypertriton.
  kDalitz          = 1000000111, ///< PDG code for the Dalitz decay product.
  kLambda1520      =   20003122  ///< PDG code for the Λ(1520) resonance.
};

#endif
