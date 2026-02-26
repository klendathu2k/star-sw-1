/// @file StPidParticleDefinition.h
/// @brief Enumeration of particle species used in STAR PID algorithms.
#ifndef __StPidParticleDefinition_h__
#define __StPidParticleDefinition_h__

/// @brief Particle species identifiers for PID algorithms.
enum StPidParticle {
    kPidElectron ,  ///< Electron / positron
    kPidProton   ,  ///< Proton / anti-proton
    kPidKaon     ,  ///< Charged kaon
    kPidPion     ,  ///< Charged pion
    kPidMuon     ,  ///< Muon / anti-muon
    kPidDeuteron ,  ///< Deuteron
    kPidTriton   ,  ///< Triton
    kPidHe3      ,  ///< Helium-3 nucleus
    kPidAlpha    ,  ///< Alpha particle (He-4)
    KPidParticles   ///< Total number of particle species (sentinel)
};
#endif /* __StPidParticleDefinition_h__ */
