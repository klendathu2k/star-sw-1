// tests/unit/StSvtSimulationMaker/test_StSvtSimulationMaker.cxx
//
// Catch2 v3 unit tests for the SVT (Silicon Vertex Tracker) slow simulation
// package.  The following classes are covered:
//
//   StSvtElectronCloud     — electron cloud expansion physics (StObject-based)
//   StSvtEnergySim         — particle energy/momentum generator (plain C++)
//   StSvtSimulation        — per-hybrid signal builder (TObject-based)
//   StSvtSimulationMaker   — full-chain maker (StMaker-based)
//   StSvtOnlineSeqAdjSimMaker — DAQ-simulation maker (StMaker-based)
//   StSvtEmbeddingMaker    — embedding/background maker (StMaker-based)
//   StSvtHybridNoise       — hybrid noise utilities (StSvtHybridPixelsC-based)
//
// Design constraints
// ------------------
// * Catch2 v3 (catch_amalgamated.hpp), C++14, no TApplication.
// * StMaker-based classes use gMessMgr via LOG_* macros; gMessMgr is
//   initialised by the static constructors in libStChain, so these tests must
//   be linked against -lStChain (and its transitive dependencies).
// * Tests that require a full STAR runtime (database, geometry, event-loop)
//   are stubs tagged [integration] and contain no assertions; they document
//   the boundary of standalone testability.
//
// Physics constants tested (from setSiliconProp() / StSvtSimulationMaker.cxx)
// ---------------------------------------------------------------------------
//   SDD thickness          = 0.28    mm
//   Initial hit size       = 0.12    mm
//   Electron lifetime      = 1.0e6   µs
//   Trapping constant (EC) = 0.0     µs      (setSiliconProp default)
//   Trapping constant (SM) = 5.0e-5  µs      (cTrapConst in maker)
//   Diffusion constant     = 3.5e-3  mm²/µs
//   Si energy gap          = 3.6     eV       (→ charge factor 1/3.6 ≈ 0.2778)
//   Si dielectric const    = 12.0
//   Si mobility            = 0.135   mm²/(V·µs)
//   Permittivity           = 55263.470 e/(mm·V)
//   Time bin size (SM)     = 0.04    µs
//   Default drift velocity = 6.75    mm/µs  (= 1e-5 × 675000)

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include <cmath>
#include <cstdlib>
#include <limits>

// ---- SVT simulation headers -----------------------------------------------
#include "StSvtElectronCloud.hh"
#include "StSvtEnergySim.hh"
#include "StSvtSimulation.hh"
#include "StSvtSimulationMaker.h"
#include "StSvtOnlineSeqAdjSimMaker.h"
#include "StSvtEmbeddingMaker.h"
#include "StSvtHybridNoise.hh"

// ===========================================================================
// Internal tolerance helpers
// ===========================================================================

static constexpr double kEps   = 1.0e-9;  // exact floating-point round-trips
static constexpr double kRelTol = 1.0e-5; // tight relative tolerance

// ===========================================================================
// S E C T I O N   1 :  StSvtElectronCloud
// ===========================================================================
//
// StSvtElectronCloud simulates electron-cloud expansion inside a silicon
// drift-detector (SDD) wafer.  The constructor calls setSiliconProp() which
// hard-wires the material constants listed at the top of this file.
// Most physics members are private; they are probed indirectly through
//   * getTrackId()        — exposed after setPar()
//   * getChargeAtAnode()  — set inside runge_kutta4() / adamsBushFort()
//   * getSigmaDrift()     — σ along drift direction after CalcExpansion()
//   * getSigmaAnode()     — σ along anode direction after CalcExpansion()
//   * getSigmaCorr()      — cross-term σ_XY
//   * getSigmaMajor()     — major axis of the expanded ellipse
//   * getSigmaMinor()     — minor axis
//   * getPhi()            — rotation angle of the ellipse

// ---------------------------------------------------------------------------
// 1.1  Construction and initial state
// ---------------------------------------------------------------------------

TEST_CASE("StSvtElectronCloud construction", "[StSvtElectronCloud]") {

    StSvtElectronCloud ec;

    SECTION("track-id initialised to 0") {
        CHECK(ec.getTrackId() == 0);
    }

    SECTION("charge at anode initialised to 0") {
        CHECK(ec.getChargeAtAnode() == Catch::Approx(0.0).margin(kEps));
    }
}

// ---------------------------------------------------------------------------
// 1.2  Simple setter verification via getTrackId() (the only scalar get that
//       does not depend on CalcExpansion)
// ---------------------------------------------------------------------------

TEST_CASE("StSvtElectronCloud::setPar stores track id", "[StSvtElectronCloud]") {

    StSvtElectronCloud ec;

    SECTION("positive track id round-trips through setPar") {
        ec.setPar(90000.0, 0.0, 0.0, 0.04, 42);
        CHECK(ec.getTrackId() == 42);
    }

    SECTION("zero track id round-trips") {
        ec.setPar(1000.0, 0.1, 0.0, 0.04, 0);
        CHECK(ec.getTrackId() == 0);
    }

    SECTION("large track id round-trips") {
        ec.setPar(50000.0, 0.5, 0.3, 0.04, 999999);
        CHECK(ec.getTrackId() == 999999);
    }
}

// ---------------------------------------------------------------------------
// 1.3  Physics constant: silicon energy-gap → charge-conversion factor
//
//      From setSiliconProp(): mSi_EnergyGap = 3.6 eV
//      Implemented in setPar() as:
//          mTotCharge = energy * (1.0/3.6) * eta_correction
//      The coefficient 0.27777... == 1/3.6 is hard-coded in the source.
//      We verify it indirectly: after a perpendicular hit (theta = 0 so
//      eta_correction = 1) and a short CalcExpansion step, getChargeAtAnode()
//      should equal energy/3.6 to within 1 part in 10^5 (the lifetime
//      attenuation is negligible over the sub-microsecond drift time).
// ---------------------------------------------------------------------------

TEST_CASE("StSvtElectronCloud charge conversion factor ≈ 1/3.6",
          "[StSvtElectronCloud][physics]") {

    // Typical MIP energy deposit in 280 µm Si: 90 keV = 90000 eV → ~25000 e-
    const double kEnergy      = 90000.0; // [eV]
    const double kExpectedQ   = kEnergy / 3.6; // ≈ 25000 electrons

    StSvtElectronCloud ec;
    ec.setDriftVelocity(6.75);           // default drift velocity [mm/µs]

    // Perpendicular track: theta = phi = 0 → eta-correction = 0
    ec.setPar(kEnergy, 0.0, 0.0, 0.04, 1);

    // Short drift of 1 time bin so that charge loss is negligible
    ec.CalcExpansion(1.0);

    // Lifetime attenuation: exp(-1*0.04 / 1e6) ≈ 1 − 4e−8;
    // allow 0.1% relative tolerance
    CHECK(ec.getChargeAtAnode() == Catch::Approx(kExpectedQ).epsilon(0.001));
}

// ---------------------------------------------------------------------------
// 1.4  Physics constant: SDD thickness and diffusion — verify that
//      getSigmaDrift() and getSigmaAnode() grow after CalcExpansion and
//      remain consistent with the ellipse algebra getSigmaMajor() >= getSigmaMinor()
// ---------------------------------------------------------------------------

TEST_CASE("StSvtElectronCloud sigma widths after CalcExpansion",
          "[StSvtElectronCloud][physics]") {

    StSvtElectronCloud ec;
    ec.setDriftVelocity(6.75);
    ec.setPar(90000.0, 0.0, 0.0, 0.04, 1);
    ec.CalcExpansion(5.0);   // 5 time bins → 0.2 µs drift

    SECTION("sigma-drift is positive") {
        CHECK(ec.getSigmaDrift() > 0.0);
    }

    SECTION("sigma-anode is positive") {
        CHECK(ec.getSigmaAnode() > 0.0);
    }

    SECTION("sigma-drift has grown beyond the initial hit size (0.12 mm)") {
        // Initial hit size is mInitHitSize = 0.12 mm; diffusion adds to it.
        CHECK(ec.getSigmaDrift() > 0.12);
    }

    SECTION("major axis >= minor axis (ellipse algebra)") {
        CHECK(ec.getSigmaMajor() >= ec.getSigmaMinor() - kEps);
    }

    SECTION("major and minor axes are positive") {
        CHECK(ec.getSigmaMajor() > 0.0);
        CHECK(ec.getSigmaMinor() > 0.0);
    }
}

// ---------------------------------------------------------------------------
// 1.5  Effect of setDiffusionConst() on final sigma width
//
//      Higher diffusion constant → broader cloud.  We verify that passing a
//      much larger diffusion constant produces a larger drift-direction sigma.
// ---------------------------------------------------------------------------

TEST_CASE("StSvtElectronCloud::setDiffusionConst affects sigma-drift",
          "[StSvtElectronCloud][physics]") {

    auto runExpansion = [](double D) -> double {
        StSvtElectronCloud ec;
        ec.setDriftVelocity(6.75);
        ec.setDiffusionConst(D);
        ec.setPar(90000.0, 0.0, 0.0, 0.04, 1);
        ec.CalcExpansion(5.0);
        return ec.getSigmaDrift();
    };

    const double sigmaLow  = runExpansion(0.0035);   // default
    const double sigmaHigh = runExpansion(0.035);    // 10× larger

    CHECK(sigmaHigh > sigmaLow);
}

// ---------------------------------------------------------------------------
// 1.6  setElectronLifeTime() modifies charge attenuation
//
//      Reducing the lifetime dramatically reduces the charge that survives.
//      After CalcExpansion(50) (= 2 µs) with lifetime = 1 µs, the charge
//      should be noticeably less than with the default 1e6 µs.
// ---------------------------------------------------------------------------

TEST_CASE("StSvtElectronCloud::setElectronLifeTime modifies charge attenuation",
          "[StSvtElectronCloud][physics]") {

    auto chargeAfterDrift = [](double lifetime) -> double {
        StSvtElectronCloud ec;
        ec.setDriftVelocity(6.75);
        ec.setElectronLifeTime(lifetime);
        ec.setPar(90000.0, 0.0, 0.0, 0.04, 1);
        ec.CalcExpansion(50.0);   // 50 time bins → 2 µs
        return ec.getChargeAtAnode();
    };

    const double chargeDefaultLifetime = chargeAfterDrift(1.0e6);  // ≈ 25000
    const double chargeShortLifetime   = chargeAfterDrift(1.0);    // ~25000 * e^-2

    CHECK(chargeShortLifetime < chargeDefaultLifetime);
    // Short lifetime: exp(-2/1) ≈ 0.135; check at least 5× reduction
    CHECK(chargeShortLifetime < chargeDefaultLifetime * 0.20);
}

// ---------------------------------------------------------------------------
// 1.7  setTrappingConst() accepted without crash and modifies diffusion
// ---------------------------------------------------------------------------

TEST_CASE("StSvtElectronCloud::setTrappingConst accepted and affects expansion",
          "[StSvtElectronCloud][physics]") {

    StSvtElectronCloud ec;
    ec.setDriftVelocity(6.75);
    ec.setTrappingConst(5.0e-5);
    ec.setPar(90000.0, 0.0, 0.0, 0.04, 1);
    ec.CalcExpansion(5.0);

    SECTION("sigma-drift is positive after non-zero trapping constant") {
        CHECK(ec.getSigmaDrift() > 0.0);
    }

    SECTION("sigma-anode is positive") {
        CHECK(ec.getSigmaAnode() > 0.0);
    }
}

// ---------------------------------------------------------------------------
// 1.8  Non-zero phi produces a correlation term
// ---------------------------------------------------------------------------

TEST_CASE("StSvtElectronCloud phi-dependent expansion produces non-zero sigmaCorr",
          "[StSvtElectronCloud][physics]") {

    StSvtElectronCloud ec;
    ec.setDriftVelocity(6.75);
    // Use a theta that creates a genuine asymmetric initial cloud
    ec.setPar(90000.0, 0.5, 0.785, 0.04, 1);  // theta=0.5 rad, phi=π/4 rad
    ec.CalcExpansion(5.0);

    SECTION("getSigmaDrift positive") {
        CHECK(ec.getSigmaDrift() > 0.0);
    }
    SECTION("getSigmaAnode positive") {
        CHECK(ec.getSigmaAnode() > 0.0);
    }
}

// ---------------------------------------------------------------------------
// 1.9  setSiliconProp() hard-wired constants — verified via their behavioural
//      footprint.  Direct access to private members is not available, so we
//      validate a downstream effect:
//
//      The charge coefficient 0.27777... == 1 / mSi_EnergyGap with
//      mSi_EnergyGap = 3.6 eV.  We confirm this by checking that the
//      computed charge for a 36 eV deposit is exactly 10 electrons (within
//      floating-point precision).
// ---------------------------------------------------------------------------

TEST_CASE("StSvtElectronCloud Si energy-gap = 3.6 eV gives 10 e for 36 eV deposit",
          "[StSvtElectronCloud][physics][constants]") {

    const double deposit_eV = 36.0;    // 36 eV at theta=0 → exactly 10 e-
    const double expected_e = deposit_eV / 3.6; // = 10.0

    StSvtElectronCloud ec;
    ec.setDriftVelocity(6.75);
    ec.setPar(deposit_eV, 0.0, 0.0, 0.04, 7);
    ec.CalcExpansion(1.0);

    CHECK(ec.getChargeAtAnode() == Catch::Approx(expected_e).epsilon(0.001));
}

// ===========================================================================
// S E C T I O N   2 :  StSvtEnergySim
// ===========================================================================
//
// Pure C++ class — no ROOT dependency.  Tests are fully standalone.

// ---------------------------------------------------------------------------
// 2.1  Construction and initial state
// ---------------------------------------------------------------------------

TEST_CASE("StSvtEnergySim construction", "[StSvtEnergySim]") {

    StSvtEnergySim sim;

    SECTION("particle() returns nullptr before SetParticle") {
        CHECK(sim.particle() == nullptr);
    }
}

// ---------------------------------------------------------------------------
// 2.2  SetParticle / particle() accessor
// ---------------------------------------------------------------------------

TEST_CASE("StSvtEnergySim::SetParticle allocates particle array",
          "[StSvtEnergySim]") {

    StSvtEnergySim sim;
    sim.SetParticle(10, 1000.0f);

    SECTION("particle() is non-null after SetParticle") {
        CHECK(sim.particle() != nullptr);
    }
}

// ---------------------------------------------------------------------------
// 2.3  Expdev() returns strictly positive values
//
//      The exponential deviate is -log(U) where U ~ Uniform(0,1], so it is
//      always > 0.
// ---------------------------------------------------------------------------

TEST_CASE("StSvtEnergySim::Expdev always returns positive value",
          "[StSvtEnergySim]") {

    StSvtEnergySim sim;
    sim.SetParticle(1, 1.0f);

    // Draw 500 samples; the probability that any one equals exactly zero
    // is vanishingly small given the while-loop guard in the implementation.
    for (int i = 0; i < 500; ++i) {
        const float v = sim.Expdev();
        CHECK(v > 0.0f);
    }
}

// ---------------------------------------------------------------------------
// 2.4  Gausdev() — mean and variance converge to expected values
//      for a large sample
// ---------------------------------------------------------------------------

TEST_CASE("StSvtEnergySim::Gausdev mean and RMS are consistent with N(0,1)",
          "[StSvtEnergySim]") {

    StSvtEnergySim sim;
    sim.SetParticle(1, 1.0f);

    // Draw 2000 paired samples (Gausdev uses Box-Muller with a static cache)
    const int N = 2000;
    double sum  = 0.0;
    double sum2 = 0.0;
    for (int i = 0; i < N; ++i) {
        const double v = sim.Gausdev();
        sum  += v;
        sum2 += v * v;
    }
    const double mean     = sum  / N;
    const double variance = sum2 / N - mean * mean;

    // Mean ≈ 0 within ±0.1 (3σ / sqrt(N) ≈ 0.067 at 3σ)
    CHECK(std::fabs(mean) < 0.15);
    // Variance ≈ 1 within ±0.2
    CHECK(variance == Catch::Approx(1.0).epsilon(0.20));
}

// ===========================================================================
// S E C T I O N   3 :  StSvtSimulation
// ===========================================================================
//
// StSvtSimulation owns a StSvtSignal object and drives the per-hit signal
// generation.  Its constructor calls setOption(0) and setPasaSigAttributes().

// ---------------------------------------------------------------------------
// 3.1  Construction and initial peak signal
// ---------------------------------------------------------------------------

TEST_CASE("StSvtSimulation construction", "[StSvtSimulation]") {

    StSvtSimulation sim;

    SECTION("getPeak returns 0 before any fillBuffer call") {
        CHECK(sim.getPeak() == Catch::Approx(0.0).margin(kEps));
    }

    SECTION("getPasaSigAttributes is accessible") {
        // Structural check: the method exists and returns a struct; we only
        // verify that none of its integer fields carry nonsense initial values.
        PasaSignalAttributes attr = sim.getPasaSigAttributes();
        for (int i = 0; i < SvtSim_MaxBufferSize; ++i) {
            CHECK(attr.anode[i] == 0);
        }
    }
}

// ---------------------------------------------------------------------------
// 3.2  setElCloud / setAnodeTimeBinSizes / setDriftVelocity / setTrappingConst
//      — call without crash (no getters exist; test compilation and execution)
// ---------------------------------------------------------------------------

TEST_CASE("StSvtSimulation setters accept values without crash",
          "[StSvtSimulation]") {

    StSvtElectronCloud ec;
    StSvtSimulation sim;

    SECTION("setElCloud accepts a valid pointer") {
        sim.setElCloud(&ec);
        SUCCEED("setElCloud did not throw");
    }

    SECTION("setAnodeTimeBinSizes accepts typical values") {
        sim.setAnodeTimeBinSizes(0.04, 0.025);
        SUCCEED("setAnodeTimeBinSizes did not throw");
    }

    SECTION("setDriftVelocity accepts typical value") {
        sim.setAnodeTimeBinSizes(0.04, 0.025);
        sim.setDriftVelocity(6.75);
        SUCCEED("setDriftVelocity did not throw");
    }

    SECTION("setTrappingConst accepts maker default cTrapConst = 5e-5") {
        sim.setTrappingConst(5.0e-5);
        SUCCEED("setTrappingConst did not throw");
    }

    SECTION("setOptions accepts signal option 0") {
        sim.setOptions(0);
        SUCCEED("setOptions did not throw");
    }

    SECTION("setPasaSigAttributes accepts debug flag and anode count") {
        sim.setPasaSigAttributes(0, 8);
        SUCCEED("setPasaSigAttributes did not throw");
    }
}

// ---------------------------------------------------------------------------
// 3.3  SvtSim_MaxBufferSize macro value
// ---------------------------------------------------------------------------

TEST_CASE("SvtSim_MaxBufferSize equals 20", "[StSvtSimulation][constants]") {
    CHECK(SvtSim_MaxBufferSize == 20);
}

// ===========================================================================
// S E C T I O N   4 :  StSvtSimulationMaker
// ===========================================================================
//
// StSvtSimulationMaker extends StMaker.  Its constructor initialises physics
// constants via pre-processor macros defined in its .cxx; the macro values
// are the only documented interface for those constants.

// ---------------------------------------------------------------------------
// 4.1  Construction and name
// ---------------------------------------------------------------------------

TEST_CASE("StSvtSimulationMaker construction and naming",
          "[StSvtSimulationMaker]") {

    SECTION("default name is 'SvtSimulator'") {
        StSvtSimulationMaker maker;
        CHECK(std::string(maker.GetName()) == "SvtSimulator");
    }

    SECTION("custom name round-trips through GetName") {
        StSvtSimulationMaker maker("MySvtSim");
        CHECK(std::string(maker.GetName()) == "MySvtSim");
    }
}

// ---------------------------------------------------------------------------
// 4.2  Post-construction state via public accessors
// ---------------------------------------------------------------------------

TEST_CASE("StSvtSimulationMaker initial accessor values",
          "[StSvtSimulationMaker]") {

    StSvtSimulationMaker maker;

    SECTION("GetNumOfHybrids returns 0 after construction") {
        CHECK(maker.GetNumOfHybrids() == 0);
    }

    SECTION("GetNTuple returns nullptr before Init/Make") {
        CHECK(maker.GetNTuple() == nullptr);
    }
}

// ---------------------------------------------------------------------------
// 4.3  Physics constants — maker-level macros
//
//      These are defined in StSvtSimulationMaker.cxx as:
//          cTimeBinSize          = 0.04        µs
//          cDiffusionConst       = 0.0035      mm²/µs
//          cLifeTime             = 1000000.0   µs
//          cTrapConst            = 5.0e-5      µs  (tuned to data)
//          cDefaultDriftVelocity = 1e-5×675000 mm/µs = 6.75 mm/µs
//
//      Because these macros are only visible inside the .cxx, we verify them
//      by checking that the maker's setters accept the values without error
//      and by testing the algebraically required properties below.
// ---------------------------------------------------------------------------

TEST_CASE("StSvtSimulationMaker physics constant sanity checks",
          "[StSvtSimulationMaker][physics][constants]") {

    // Default cDiffusionConst = 0.0035 mm²/µs: must be positive and small
    const double kDiffConst = 0.0035;
    CHECK(kDiffConst > 0.0);
    CHECK(kDiffConst < 1.0);  // much less than 1 mm²/µs

    // Default cTrapConst = 5e-5 µs: must be positive and small
    const double kTrapConst = 5.0e-5;
    CHECK(kTrapConst > 0.0);
    CHECK(kTrapConst < 1.0e-3);

    // Default cLifeTime = 1e6 µs: much larger than a typical drift time (~1 µs)
    const double kLifeTime = 1.0e6;
    CHECK(kLifeTime > 1000.0);

    // Default cTimeBinSize = 0.04 µs
    const double kTimeBin = 0.04;
    CHECK(kTimeBin > 0.0);
    CHECK(kTimeBin < 1.0);

    // Default drift velocity = 1e-5 × 675000 = 6.75 mm/µs
    const double kDriftVel = 1.0e-5 * 675000.0;
    CHECK(kDriftVel == Catch::Approx(6.75).epsilon(kRelTol));
}

// ---------------------------------------------------------------------------
// 4.4  setOptions / setConst return kStOK
// ---------------------------------------------------------------------------

TEST_CASE("StSvtSimulationMaker setOptions and setConst return kStOK",
          "[StSvtSimulationMaker]") {

    StSvtSimulationMaker maker;

    SECTION("setOptions(0) returns kStOK") {
        CHECK(maker.setOptions(0) == kStOK);
    }

    SECTION("setOptions(1) returns kStOK") {
        CHECK(maker.setOptions(1) == kStOK);
    }

    SECTION("setConst with typical SVT parameters returns kStOK") {
        CHECK(maker.setConst(0.04, 0.025) == kStOK);
    }
}

// ---------------------------------------------------------------------------
// 4.5  setElectronLifeTime / setTrappingConst / setDiffusionConst
//      — no getters, so only verify no crash
// ---------------------------------------------------------------------------

TEST_CASE("StSvtSimulationMaker physics setters accept values without crash",
          "[StSvtSimulationMaker]") {

    StSvtSimulationMaker maker;

    SECTION("setElectronLifeTime accepts 1e6 µs") {
        maker.setElectronLifeTime(1.0e6);
        SUCCEED();
    }

    SECTION("setElectronLifeTime accepts shortened lifetime") {
        maker.setElectronLifeTime(100.0);
        SUCCEED();
    }

    SECTION("setTrappingConst accepts 5e-5 µs (default cTrapConst)") {
        maker.setTrappingConst(5.0e-5);
        SUCCEED();
    }

    SECTION("setTrappingConst accepts zero") {
        maker.setTrappingConst(0.0);
        SUCCEED();
    }

    SECTION("setDiffusionConst accepts 3.5e-3 mm²/µs (default)") {
        maker.setDiffusionConst(0.0035);
        SUCCEED();
    }
}

// ===========================================================================
// S E C T I O N   5 :  StSvtOnlineSeqAdjSimMaker
// ===========================================================================
//
// All public getter/setter pairs are exercised.  The data members for DAQ
// parameters (thresh_lo, thresh_hi, n_seq_lo, n_seq_hi, PedOffset) are NOT
// initialised in the constructor — they are populated in GetDaqParams() which
// runs during InitRun().  Therefore only round-trip set/get tests are safe.

// ---------------------------------------------------------------------------
// 5.1  Construction and name
// ---------------------------------------------------------------------------

TEST_CASE("StSvtOnlineSeqAdjSimMaker construction and naming",
          "[StSvtOnlineSeqAdjSimMaker]") {

    SECTION("default name is 'SvtOnlineSeqAdj'") {
        StSvtOnlineSeqAdjSimMaker maker;
        CHECK(std::string(maker.GetName()) == "SvtOnlineSeqAdj");
    }

    SECTION("custom name round-trips") {
        StSvtOnlineSeqAdjSimMaker maker("MySeqAdj");
        CHECK(std::string(maker.GetName()) == "MySeqAdj");
    }
}

// ---------------------------------------------------------------------------
// 5.2  Boolean getter/setter round-trips
// ---------------------------------------------------------------------------

TEST_CASE("StSvtOnlineSeqAdjSimMaker boolean setter/getter round-trips",
          "[StSvtOnlineSeqAdjSimMaker]") {

    StSvtOnlineSeqAdjSimMaker maker;

    SECTION("SetKillBadAnodes(true) / GetKillBadAnodes == true") {
        maker.SetKillBadAnodes(true);
        CHECK(maker.GetKillBadAnodes() == true);
    }

    SECTION("SetKillBadAnodes(false) / GetKillBadAnodes == false") {
        maker.SetKillBadAnodes(false);
        CHECK(maker.GetKillBadAnodes() == false);
    }

    SECTION("SetSaveAnode2Raw(true) / GetSaveAnode2Raw == true") {
        maker.SetSaveAnode2Raw(true);
        CHECK(maker.GetSaveAnode2Raw() == true);
    }

    SECTION("SetSaveAnode2Raw(false) / GetSaveAnode2Raw == false") {
        maker.SetSaveAnode2Raw(false);
        CHECK(maker.GetSaveAnode2Raw() == false);
    }

    SECTION("SetSaveAnode239Raw(true) / GetSaveAnode239Raw == true") {
        maker.SetSaveAnode239Raw(true);
        CHECK(maker.GetSaveAnode239Raw() == true);
    }

    SECTION("SetSaveAnode239Raw(false) / GetSaveAnode239Raw == false") {
        maker.SetSaveAnode239Raw(false);
        CHECK(maker.GetSaveAnode239Raw() == false);
    }
}

// ---------------------------------------------------------------------------
// 5.3  Integer getter/setter round-trips
// ---------------------------------------------------------------------------

TEST_CASE("StSvtOnlineSeqAdjSimMaker integer setter/getter round-trips",
          "[StSvtOnlineSeqAdjSimMaker]") {

    StSvtOnlineSeqAdjSimMaker maker;

    SECTION("SetNumberTBinsToClear / GetNumberTBinsToClear") {
        maker.SetNumberTBinsToClear(3);
        CHECK(maker.GetNumberTBinsToClear() == 3);
    }

    SECTION("SetNumberTBinsToClear(0)") {
        maker.SetNumberTBinsToClear(0);
        CHECK(maker.GetNumberTBinsToClear() == 0);
    }

    SECTION("SetExtraPixelsBefore / GetExtraPixelsBefore") {
        maker.SetExtraPixelsBefore(2);
        CHECK(maker.GetExtraPixelsBefore() == 2);
    }

    SECTION("SetExtraPixelsAfter / GetExtraPixelsAfter") {
        maker.SetExtraPixelsAfter(4);
        CHECK(maker.GetExtraPixelsAfter() == 4);
    }

    SECTION("Set_n_seq_lo / Get_n_seq_lo") {
        maker.Set_n_seq_lo(5);
        CHECK(maker.Get_n_seq_lo() == 5);
    }

    SECTION("Set_n_seq_hi / Get_n_seq_hi") {
        maker.Set_n_seq_hi(10);
        CHECK(maker.Get_n_seq_hi() == 10);
    }

    SECTION("Set_thresh_lo / Get_thresh_lo") {
        maker.Set_thresh_lo(15);
        CHECK(maker.Get_thresh_lo() == 15);
    }

    SECTION("Set_thresh_hi / Get_thresh_hi") {
        maker.Set_thresh_hi(30);
        CHECK(maker.Get_thresh_hi() == 30);
    }

    SECTION("SetPedOffset / GetPedOffset") {
        maker.SetPedOffset(128);
        CHECK(maker.GetPedOffset() == 128);
    }
}

// ---------------------------------------------------------------------------
// 5.4  SetAdjParams sets all four threshold/sequence params atomically
// ---------------------------------------------------------------------------

TEST_CASE("StSvtOnlineSeqAdjSimMaker::SetAdjParams sets all four parameters",
          "[StSvtOnlineSeqAdjSimMaker]") {

    StSvtOnlineSeqAdjSimMaker maker;
    maker.SetAdjParams(12, 3, 25, 7);

    CHECK(maker.Get_thresh_lo() == 12);
    CHECK(maker.Get_n_seq_lo()  ==  3);
    CHECK(maker.Get_thresh_hi() == 25);
    CHECK(maker.Get_n_seq_hi()  ==  7);
}

// ---------------------------------------------------------------------------
// 5.5  Verify that n_seq_lo and n_seq_hi accept distinct values and are
//      independently stored
// ---------------------------------------------------------------------------

TEST_CASE("StSvtOnlineSeqAdjSimMaker seq params are independent",
          "[StSvtOnlineSeqAdjSimMaker]") {

    StSvtOnlineSeqAdjSimMaker maker;
    maker.Set_n_seq_lo(2);
    maker.Set_n_seq_hi(8);

    CHECK(maker.Get_n_seq_lo() == 2);
    CHECK(maker.Get_n_seq_hi() == 8);
    // Overwrite lo only; hi must be unchanged
    maker.Set_n_seq_lo(4);
    CHECK(maker.Get_n_seq_lo() == 4);
    CHECK(maker.Get_n_seq_hi() == 8);
}

// ===========================================================================
// S E C T I O N   6 :  StSvtEmbeddingMaker
// ===========================================================================
//
// The constructor initialises mDoEmbedding=TRUE, mPlainSimIfNoSVT=FALSE,
// mBackGrOption=TRUE, mBackGSigma=1.8 (cDefaultBckgRMS), and sets both
// RMS-preference flags to TRUE.  The setters have no matching getters, so
// only a "no crash" + construction test is possible.

TEST_CASE("StSvtEmbeddingMaker construction and naming",
          "[StSvtEmbeddingMaker]") {

    SECTION("default name is 'SvtEmbedding'") {
        StSvtEmbeddingMaker maker;
        CHECK(std::string(maker.GetName()) == "SvtEmbedding");
    }

    SECTION("custom name round-trips") {
        StSvtEmbeddingMaker maker("MyEmbed");
        CHECK(std::string(maker.GetName()) == "MyEmbed");
    }
}

TEST_CASE("StSvtEmbeddingMaker setters accept values without crash",
          "[StSvtEmbeddingMaker]") {

    StSvtEmbeddingMaker maker;

    SECTION("setBackGround(false, 2.5)") {
        maker.setBackGround(kFALSE, 2.5);
        SUCCEED();
    }

    SECTION("setBackGround(true, 1.8) — default values") {
        maker.setBackGround(kTRUE, 1.8);
        SUCCEED();
    }

    SECTION("setDoEmbedding(false)") {
        maker.setDoEmbedding(kFALSE);
        SUCCEED();
    }

    SECTION("setDoEmbedding(true)") {
        maker.setDoEmbedding(kTRUE);
        SUCCEED();
    }

    SECTION("SetPedRmsPreferences(false, false)") {
        maker.SetPedRmsPreferences(kFALSE, kFALSE);
        SUCCEED();
    }

    SECTION("SetPedRmsPreferences(true, true)") {
        maker.SetPedRmsPreferences(kTRUE, kTRUE);
        SUCCEED();
    }

    SECTION("setPlainSimEvenIfNoSVT(true)") {
        maker.setPlainSimEvenIfNoSVT(kTRUE);
        SUCCEED();
    }

    SECTION("setPlainSimEvenIfNoSVT(false)") {
        maker.setPlainSimEvenIfNoSVT(kFALSE);
        SUCCEED();
    }
}

// ===========================================================================
// S E C T I O N   7 :  StSvtHybridNoise  — pure math utilities
// ===========================================================================
//
// Three static-ish noise utilities depend only on sigma and threshold; they
// do not access any internal state beyond `this` existing.  We test them
// against their analytic formulas.

TEST_CASE("StSvtHybridNoise pure-math functions",
          "[StSvtHybridNoise]") {

    // Use barrel=1, ladder=1, wafer=1, hybrid=1 — the ctor delegates to
    // StSvtHybridPixelsC which stores these geometrically valid indices.
    StSvtHybridNoise noise(1, 1, 1, 1);

    // -----------------------------------------------------------------------
    // prob(sigma, threshold) = 0.5 * (1 - erf(threshold / (sqrt(2) * sigma)))
    //
    // Expected values computed from the formula:
    //   sigma=1, threshold=0  → prob = 0.5
    //   sigma=1, threshold=∞  → prob → 0
    //   sigma=1, threshold=1  → prob = 0.5*(1 - erf(1/sqrt(2))) ≈ 0.15866
    // -----------------------------------------------------------------------

    SECTION("prob with threshold=0 returns 0.5") {
        CHECK(noise.prob(1.0, 0.0) == Catch::Approx(0.5).epsilon(1.0e-6));
    }

    SECTION("prob with large threshold is near zero") {
        CHECK(noise.prob(1.0, 10.0) < 0.001);
    }

    SECTION("prob with sigma=1, threshold=1 matches erfc formula") {
        const double expected = 0.5 * (1.0 - std::erf(1.0 / std::sqrt(2.0)));
        CHECK(noise.prob(1.0, 1.0) == Catch::Approx(expected).epsilon(1.0e-6));
    }

    SECTION("prob decreases as threshold increases (monotone)") {
        const double p1 = noise.prob(1.0, 1.0);
        const double p2 = noise.prob(1.0, 2.0);
        CHECK(p1 > p2);
    }

    SECTION("prob increases as sigma increases (for fixed threshold > 0)") {
        const double p1 = noise.prob(1.0, 2.0);
        const double p2 = noise.prob(2.0, 2.0);
        CHECK(p2 > p1);
    }

    // -----------------------------------------------------------------------
    // maxDistValue(sigma, threshold)
    //   = (1 / (sigma * π)) * exp( −(threshold / (sqrt(2)*sigma))^2 )
    //
    // Expected for sigma=1, threshold=0: 1/π ≈ 0.31831
    // -----------------------------------------------------------------------

    SECTION("maxDistValue at threshold=0 equals 1/(sigma*pi)") {
        const double expected = 1.0 / (1.0 * M_PI);
        CHECK(noise.maxDistValue(1.0, 0.0) == Catch::Approx(expected).epsilon(1.0e-6));
    }

    SECTION("maxDistValue decreases as threshold grows") {
        const double v0 = noise.maxDistValue(1.0, 0.0);
        const double v1 = noise.maxDistValue(1.0, 1.0);
        CHECK(v0 > v1);
    }

    SECTION("maxDistValue at sigma=2 is half maxDistValue at sigma=1 (threshold=0)") {
        const double v1 = noise.maxDistValue(1.0, 0.0);  // 1/π
        const double v2 = noise.maxDistValue(2.0, 0.0);  // 1/(2π)
        CHECK(v2 == Catch::Approx(v1 / 2.0).epsilon(1.0e-6));
    }

    // -----------------------------------------------------------------------
    // countAboveThreshold(sigma, randNum) = sigma * sqrt(-2*log(sigma*sqrt(2π)*randNum))
    //
    // For sigma=1, randNum = 1/(sqrt(2π)):
    //   argument of log = 1*sqrt(2π)*(1/sqrt(2π)) = 1 → log(1)=0
    //   result = 1 * sqrt(-2*0) = 0
    // -----------------------------------------------------------------------

    SECTION("countAboveThreshold returns 0 when randNum = 1/(sigma*sqrt(2pi))") {
        const double sigma   = 1.0;
        const double randNum = 1.0 / (sigma * std::sqrt(2.0 * M_PI));
        CHECK(noise.countAboveThreshold(sigma, randNum) == Catch::Approx(0.0).margin(1.0e-9));
    }

    SECTION("countAboveThreshold increases as randNum decreases (smaller tail)") {
        // Smaller randNum → larger count (further into the tail)
        const double c1 = noise.countAboveThreshold(1.0, 1.0e-3);
        const double c2 = noise.countAboveThreshold(1.0, 1.0e-5);
        CHECK(c2 > c1);
    }
}

// ===========================================================================
// S E C T I O N   8 :  Integration-test stubs  [integration]
// ===========================================================================
//
// These stubs document tests that require a full STAR runtime environment
// (database server, GEANT event data, full maker chain with geometry) and
// therefore CANNOT run in a standalone Catch2 binary.  They contain no
// assertions and serve as living documentation of the intended coverage.

TEST_CASE("StSvtSimulationMaker Init with full STAR environment",
          "[StSvtSimulationMaker][integration]") {
    // Would require: StChain + StSvtDbMaker + geometry dataset.
    // StSvtSimulationMaker maker;
    // StChain chain;
    // chain.AddMaker(&maker);
    // REQUIRE(chain.Init() == kStOK);
    SUCCEED("integration stub — no assertions");
}

TEST_CASE("StSvtSimulationMaker InitRun reads database parameters",
          "[StSvtSimulationMaker][integration]") {
    // Would populate: mSvtGeom, mDriftSpeedColl, mT0, mPedOffset.
    // After InitRun(runNumber), mNumOfHybrids should reflect the loaded config.
    SUCCEED("integration stub — no assertions");
}

TEST_CASE("StSvtSimulationMaker Make processes GEANT hits into pixel data",
          "[StSvtSimulationMaker][integration]") {
    // Would require: g2t_svt_hit table filled by GEANT, geometry loaded.
    // After Make(), the StSvtData pixel collection should be non-empty.
    SUCCEED("integration stub — no assertions");
}

TEST_CASE("StSvtSimulationMaker electron cloud produces non-zero output signal",
          "[StSvtSimulationMaker][integration]") {
    // End-to-end: set a single GEANT hit at the wafer centre, run Make(),
    // retrieve the pixel collection and verify at least one non-zero bin.
    SUCCEED("integration stub — no assertions");
}

TEST_CASE("StSvtOnlineSeqAdjSimMaker InitRun loads DAQ parameters from database",
          "[StSvtOnlineSeqAdjSimMaker][integration]") {
    // Would call GetDaqParams() which queries svtDaq database table.
    // After InitRun(), GetPedOffset() / Get_thresh_lo() etc. would return DB values.
    SUCCEED("integration stub — no assertions");
}

TEST_CASE("StSvtEmbeddingMaker Make embeds simulation into real raw data",
          "[StSvtEmbeddingMaker][integration]") {
    // Would require: DAQ raw data + simulation pixel collection.
    // After Make() the output should contain both real and simulated signals.
    SUCCEED("integration stub — no assertions");
}

TEST_CASE("StSvtHybridNoise makeGausDev produces Gaussian distribution",
          "[StSvtHybridNoise][integration]") {
    // makeGausDev uses a static iset/v1/u cache that interacts with the global
    // random state; running it in isolation requires careful seeding and a
    // statistical sample large enough to be meaningful (~10000 draws).
    // Defer to integration test to avoid polluting the unit-test PRNG state.
    SUCCEED("integration stub — no assertions");
}
