// tests/unit/StTofSimMaker/test_StTofSimMaker.cxx
//
// Catch2 v3 unit tests for the StTofSimMaker early TOF (TOFp) simulation maker
// and its closely coupled utility classes StTofSimParam and StTofCalibration.
//
// Design constraints
// ------------------
// * No TApplication — ROOT global state is never initialised.
// * StTofSimMaker::Init() / Make() / Finish() touch the STAR database and
//   ROOT I/O; those paths are exercised only as [integration] stubs below.
// * StTofSimParam and StTofCalibration own no ROOT objects and can be
//   constructed freely.
// * StTofSimMaker is a concrete StMaker subclass; it can be stack-allocated
//   in unit scope because its ctor touches no ROOT singletons.

#include "../catch_amalgamated.hpp"

// ROOT/STAR types required before the STAR headers
#include "Rtypes.h"

// Class under test
#include "StTofSimMaker/StTofSimMaker.h"

// Coupled utility classes (tested independently where possible)
#include "StTofUtil/StTofSimParam.h"
#include "StTofUtil/StTofCalibration.h"

// ---------------------------------------------------------------------------
// Approximate comparison helper (avoids <cmath> dependency on Approx)
// ---------------------------------------------------------------------------
namespace {
    // Returns true when |a - b| <= rel * |b|  (or |a - b| <= abs_tol for ~0).
    inline bool nearlyEqual(float a, float b,
                            float rel = 1e-5f, float abs_tol = 1e-15f)
    {
        float diff = (a > b) ? (a - b) : (b - a);
        float mag  = (b > 0.f ? b : -b);
        return diff <= (mag * rel > abs_tol ? mag * rel : abs_tol);
    }
} // anonymous namespace

// ===========================================================================
// StTofSimMaker — construction and naming
// ===========================================================================

TEST_CASE("StTofSimMaker default construction", "[StTofSimMaker]") {

    StTofSimMaker maker;

    SECTION("default name is 'TofSim'") {
        REQUIRE(std::string(maker.GetName()) == "TofSim");
    }

    SECTION("GetName returns a non-null pointer") {
        REQUIRE(maker.GetName() != nullptr);
    }
}

TEST_CASE("StTofSimMaker custom-name construction", "[StTofSimMaker]") {

    StTofSimMaker maker("MyTofSim");

    SECTION("custom name is stored and returned by GetName") {
        REQUIRE(std::string(maker.GetName()) == "MyTofSim");
    }
}

TEST_CASE("StTofSimMaker empty-string name construction", "[StTofSimMaker]") {
    // Edge case: construct with an empty string — should not crash.
    StTofSimMaker maker("");

    SECTION("empty name is stored without crash") {
        REQUIRE(maker.GetName() != nullptr);
        REQUIRE(std::string(maker.GetName()) == "");
    }
}

// ---------------------------------------------------------------------------
// GetCVS() — should always return a valid, non-empty string
// ---------------------------------------------------------------------------
TEST_CASE("StTofSimMaker GetCVS returns non-empty string", "[StTofSimMaker]") {

    StTofSimMaker maker;

    SECTION("GetCVS() is not null") {
        REQUIRE(maker.GetCVS() != nullptr);
    }

    SECTION("GetCVS() is non-empty") {
        REQUIRE(std::string(maker.GetCVS()).size() > 0);
    }

    SECTION("GetCVS() contains CVS Id tag text") {
        // The static string is built from the CVS $Id$ expansion; it must
        // contain at least the base filename.
        std::string cvs(maker.GetCVS());
        REQUIRE(cvs.find("StTofSimMaker") != std::string::npos);
    }
}

// ---------------------------------------------------------------------------
// Multiple instances — names are independent
// ---------------------------------------------------------------------------
TEST_CASE("StTofSimMaker independent instances have independent names",
          "[StTofSimMaker]")
{
    StTofSimMaker a("SimA");
    StTofSimMaker b("SimB");

    REQUIRE(std::string(a.GetName()) == "SimA");
    REQUIRE(std::string(b.GetName()) == "SimB");
    REQUIRE(std::string(a.GetName()) != std::string(b.GetName()));
}

// ===========================================================================
// StTofSimParam — default parameter values
//
// All values come from the default constructor in StTofSimParam.cxx; the
// init() method is a no-op stub (XDF loading is commented out), so the
// default-ctor values ARE the operational defaults.
// ===========================================================================

TEST_CASE("StTofSimParam default construction", "[StTofSimParam]") {

    StTofSimParam p;

    // -----------------------------------------------------------------------
    // TOFp scintillator / PMT parameters
    // -----------------------------------------------------------------------
    SECTION("adc_overflow is 1024") {
        REQUIRE(nearlyEqual(p.adc_overflow(), 1024.0f));
    }

    SECTION("tdc_overflow is 2048") {
        REQUIRE(nearlyEqual(p.tdc_overflow(), 2048.0f));
    }

    SECTION("attlen (attenuation length) is 110 cm") {
        REQUIRE(nearlyEqual(p.attlen(), 110.0f));
    }

    SECTION("cath_eff (cathode efficiency) is 0.2") {
        REQUIRE(nearlyEqual(p.cath_eff(), 0.2f));
    }

    SECTION("cath_surf (cathode surface ratio) is 0.61") {
        REQUIRE(nearlyEqual(p.cath_surf(), 0.61f));
    }

    SECTION("delay (light propagation delay) is 80.2e-12 s") {
        REQUIRE(nearlyEqual(p.delay(), 80.2e-12f, 1e-4f));
    }

    SECTION("elec_noise fraction is 0 (disabled)") {
        REQUIRE(nearlyEqual(p.elec_noise(), 0.0f, 1e-5f, 1e-30f));
    }

    SECTION("gate_t0 is 3e-9 s") {
        REQUIRE(nearlyEqual(p.gate_t0(), 3.0e-9f, 1e-4f));
    }

    SECTION("gate_width is 200e-9 s") {
        REQUIRE(nearlyEqual(p.gate_width(), 200.0e-9f, 1e-4f));
    }

    SECTION("geo_from_geant flag is 1") {
        REQUIRE(nearlyEqual(p.geo_from_geant(), 1.0f));
    }

    SECTION("GeV_2_n_photons conversion is 1e7") {
        REQUIRE(nearlyEqual(p.GeV_2_n_photons(), 1.0e7f, 1e-5f));
    }

    SECTION("nphe_to_adc conversion is 0.2") {
        REQUIRE(nearlyEqual(p.nphe_to_adc(), 0.2f));
    }

    SECTION("phys_noise fraction is 0 (disabled)") {
        REQUIRE(nearlyEqual(p.phys_noise(), 0.0f, 1e-5f, 1e-30f));
    }

    SECTION("position_tolerance is 0.3 cm") {
        REQUIRE(nearlyEqual(p.position_tolerance(), 0.3f));
    }

    SECTION("slat_para is 0 (exponential model, not table)") {
        REQUIRE(nearlyEqual(p.slat_para(), 0.0f, 1e-5f, 1e-30f));
    }

    SECTION("start_res (start-detector resolution) is 50e-12 s") {
        REQUIRE(nearlyEqual(p.start_res(), 50.0e-12f, 1e-4f));
    }

    SECTION("surf_loss (surface loss factor) is 0.42") {
        REQUIRE(nearlyEqual(p.surf_loss(), 0.42f));
    }

    SECTION("time_res (slat assembly time resolution) is 16e-12 s") {
        REQUIRE(nearlyEqual(p.time_res(), 16.0e-12f, 1e-4f));
    }
}

// ---------------------------------------------------------------------------
// MRPC-TOF slow-simulation parameters
// ---------------------------------------------------------------------------
TEST_CASE("StTofSimParam MRPC slow-sim default parameters", "[StTofSimParam]") {

    StTofSimParam p;

    SECTION("ngap is 6") {
        REQUIRE(p.ngap() == 6);
    }

    SECTION("nmaxclus is 20") {
        REQUIRE(p.nmaxclus() == 20);
    }

    SECTION("din (inner glass thickness) is 0.54 mm") {
        REQUIRE(nearlyEqual(p.din(), 0.54f));
    }

    SECTION("dout (outer glass thickness) is 1.10 mm") {
        REQUIRE(nearlyEqual(p.dout(), 1.10f));
    }

    SECTION("dg (gas gap thickness) is 0.22 mm") {
        REQUIRE(nearlyEqual(p.dg(), 0.22f));
    }

    SECTION("er (permittivity) is 5.0") {
        REQUIRE(nearlyEqual(p.er(), 5.0f));
    }

    SECTION("vd_mean (mean drift velocity) is 2.28e-4 mm/ps") {
        REQUIRE(nearlyEqual(p.vd_mean(), 2.28e-4f, 1e-4f));
    }

    SECTION("nmeane (mean primary-cluster electron count) is 2.42") {
        REQUIRE(nearlyEqual(p.nmeane(), 2.42f));
    }

    SECTION("nmaxe (saturation electron count) is 1.6e7") {
        REQUIRE(nearlyEqual(p.nmaxe(), 1.6e7f, 1e-4f));
    }

    SECTION("dt (time step for slow sim) is 25 ps") {
        REQUIRE(nearlyEqual(p.dt(), 25.0f));
    }

    SECTION("ndt (max time-step bins) is 600") {
        REQUIRE(p.ndt() == 600);
    }

    SECTION("toffset (slow-sim start time) is 6000 ps") {
        REQUIRE(nearlyEqual(p.toffset(), 6000.0f));
    }

    SECTION("adc_thre (ADC threshold) is 12.8 fC") {
        REQUIRE(nearlyEqual(p.adc_thre(), 12.8f));
    }

    SECTION("dy_xtalk (x-talk boundary) is 1.0 mm") {
        REQUIRE(nearlyEqual(p.dy_xtalk(), 1.0f));
    }

    SECTION("timeres_fee (FEE time resolution) is 45e-12 s") {
        REQUIRE(nearlyEqual(p.timeres_fee(), 45.0e-12f, 1e-4f));
    }
}

// ---------------------------------------------------------------------------
// Fast-simulation parameters (MRPC-TOF + VPD)
// ---------------------------------------------------------------------------
TEST_CASE("StTofSimParam fast-sim default parameters", "[StTofSimParam]") {

    StTofSimParam p;

    SECTION("timeres_tof (TOF stop resolution) is 85e-12 s") {
        REQUIRE(nearlyEqual(p.timeres_tof(), 85.0e-12f, 1e-4f));
    }

    SECTION("thre_tof (TOF dE threshold) is 1e-7 GeV") {
        REQUIRE(nearlyEqual(p.thre_tof(), 1.0e-7f, 1e-4f));
    }

    SECTION("timeres_vpd (VPD resolution per channel) is 140e-12 s") {
        REQUIRE(nearlyEqual(p.timeres_vpd(), 140.0e-12f, 1e-4f));
    }

    SECTION("thre_vpd (VPD dE threshold) is 1e-7 GeV") {
        REQUIRE(nearlyEqual(p.thre_vpd(), 1.0e-7f, 1e-4f));
    }
}

// ---------------------------------------------------------------------------
// Simulator-switch defaults
// ---------------------------------------------------------------------------
TEST_CASE("StTofSimParam simulator switch defaults", "[StTofSimParam]") {

    StTofSimParam p;

    SECTION("simuTofp is false by default (legacy TOFp off)") {
        REQUIRE(p.simuTofp() == false);
    }

    SECTION("simuVpd is true by default") {
        REQUIRE(p.simuVpd() == true);
    }

    SECTION("simuTofFast is true by default") {
        REQUIRE(p.simuTofFast() == true);
    }

    SECTION("simuTofSlow is false by default") {
        REQUIRE(p.simuTofSlow() == false);
    }
}

// ---------------------------------------------------------------------------
// Simulator-switch setters round-trip
// ---------------------------------------------------------------------------
TEST_CASE("StTofSimParam simulator switch setters", "[StTofSimParam]") {

    StTofSimParam p;

    SECTION("setSimuTofp toggles simuTofp") {
        p.setSimuTofp(true);
        REQUIRE(p.simuTofp() == true);
        p.setSimuTofp(false);
        REQUIRE(p.simuTofp() == false);
    }

    SECTION("setSimuVpd toggles simuVpd") {
        p.setSimuVpd(false);
        REQUIRE(p.simuVpd() == false);
        p.setSimuVpd(true);
        REQUIRE(p.simuVpd() == true);
    }

    SECTION("setSimuTofFast toggles simuTofFast") {
        p.setSimuTofFast(false);
        REQUIRE(p.simuTofFast() == false);
        p.setSimuTofFast(true);
        REQUIRE(p.simuTofFast() == true);
    }

    SECTION("setSimuTofSlow toggles simuTofSlow") {
        p.setSimuTofSlow(true);
        REQUIRE(p.simuTofSlow() == true);
        p.setSimuTofSlow(false);
        REQUIRE(p.simuTofSlow() == false);
    }
}

// ---------------------------------------------------------------------------
// Derived / computed getters
// ---------------------------------------------------------------------------
TEST_CASE("StTofSimParam derived getters are consistent", "[StTofSimParam]") {

    StTofSimParam p;

    SECTION("alpha() == alpha_dg / d_gap — positive and finite") {
        // alpha = m_alpha_dg / m_d_gap = 31.944 / 0.22 ≈ 145.2
        float alpha = p.alpha();
        REQUIRE(alpha > 0.0f);
        REQUIRE(alpha < 1.0e6f);
        REQUIRE(nearlyEqual(alpha, 31.944f / 0.22f, 1e-4f));
    }

    SECTION("ka() is a positive capacitive voltage fraction") {
        // ka = er * dg / (n_gap*er*dg + (n_gap-1)*d_in + 2*d_out)  ∈ (0,1)
        float ka = p.ka();
        REQUIRE(ka > 0.0f);
        REQUIRE(ka < 1.0f);
    }

    SECTION("nclus(beta=0.99) returns positive cluster count") {
        float clus = p.nclus(0.99f);
        REQUIRE(clus > 0.0f);
    }

    SECTION("nclus clamps beta >= 1 to just below 1") {
        // Should not crash or produce NaN/Inf
        float clus_high = p.nclus(1.5f);
        float clus_edge = p.nclus(0.999999f);
        REQUIRE(nearlyEqual(clus_high, clus_edge));
    }

    SECTION("nclus clamps negative beta to 0") {
        float clus_neg  = p.nclus(-1.0f);
        float clus_zero = p.nclus(0.0f);
        REQUIRE(nearlyEqual(clus_neg, clus_zero));
    }
}

// ---------------------------------------------------------------------------
// VPD efficiency table defaults
// ---------------------------------------------------------------------------
TEST_CASE("StTofSimParam VPD efficiency table defaults", "[StTofSimParam]") {

    StTofSimParam p;

    SECTION("first 38 VPD tubes have 95% efficiency") {
        // East side: itray=121, tubes 1-19; West side: itray=122, tubes 1-19
        for (int itray = 121; itray <= 122; ++itray) {
            for (int itube = 1; itube <= 19; ++itube) {
                REQUIRE(nearlyEqual(p.eff_vpd(itray, itube), 0.95f));
            }
        }
    }

    SECTION("tubes beyond index 38 (last 12 entries) have 0% efficiency") {
        // Slots 39-50 are initialised to 0 in the ctor.
        // The mapping is eff_vpd[(itray-121)*19 + itube-1], so indices 38-49
        // correspond to itray=122 (West), tubes 20-31 — beyond real hardware.
        // We exercise the public accessor with out-of-hardware-range indices
        // only if the array bounds allow it; use direct arithmetic indices
        // encoded in the accessor.
        // itray=123 itube=1 -> index = (123-121)*19+(1-1) = 38
        // itray=123 itube=12 -> index = 38+11 = 49
        for (int itube = 1; itube <= 12; ++itube) {
            REQUIRE(nearlyEqual(p.eff_vpd(123, itube), 0.0f, 1e-5f, 1e-30f));
        }
    }
}

// ---------------------------------------------------------------------------
// MRPC-TOF efficiency table defaults
// ---------------------------------------------------------------------------
TEST_CASE("StTofSimParam MRPC-TOF efficiency table defaults", "[StTofSimParam]") {

    StTofSimParam p;

    SECTION("representative MRPC channels have 95% efficiency") {
        // Spot-check a few corners: tray 1 module 1 cell 1, tray 120 module 32 cell 6
        REQUIRE(nearlyEqual(p.eff_tof(1,   1, 1), 0.95f));
        REQUIRE(nearlyEqual(p.eff_tof(1,   1, 6), 0.95f));
        REQUIRE(nearlyEqual(p.eff_tof(60, 16, 3), 0.95f));
        REQUIRE(nearlyEqual(p.eff_tof(120, 32, 6), 0.95f));
    }
}

// ===========================================================================
// StTofCalibration — construction and calibration defaults
//
// The default constructor pre-fills TOFP_MAX_SLATS (41) StructSlatCalib
// entries with the hard-coded default values from StTofCalibration.cxx.
// ===========================================================================

TEST_CASE("StTofCalibration default construction", "[StTofCalibration]") {

    StTofCalibration cal;

    SECTION("numberOfEntries equals TOFP_MAX_SLATS = 41") {
        REQUIRE(cal.numberOfEntries() == 41);
    }
}

TEST_CASE("StTofCalibration default per-slat TDC values", "[StTofCalibration]") {

    StTofCalibration cal;

    SECTION("offset_tdc is 0 for all slats") {
        for (int i = 0; i < cal.numberOfEntries(); ++i) {
            REQUIRE(nearlyEqual(cal.slat(i).offset_tdc, 0.0f, 1e-5f, 1e-30f));
        }
    }

    SECTION("cc_tdc (TDC conversion constant) is 50e-12 s for all slats") {
        for (int i = 0; i < cal.numberOfEntries(); ++i) {
            REQUIRE(nearlyEqual(cal.slat(i).cc_tdc, 50.0e-12f, 1e-4f));
        }
    }

    SECTION("ods_tdc (TDC offset dispersion) is 0 for all slats") {
        for (int i = 0; i < cal.numberOfEntries(); ++i) {
            REQUIRE(nearlyEqual(cal.slat(i).ods_tdc, 0.0f, 1e-5f, 1e-30f));
        }
    }
}

TEST_CASE("StTofCalibration default per-slat ADC values", "[StTofCalibration]") {

    StTofCalibration cal;

    SECTION("offset_adc is 0 for all slats") {
        for (int i = 0; i < cal.numberOfEntries(); ++i) {
            REQUIRE(nearlyEqual(cal.slat(i).offset_adc, 0.0f, 1e-5f, 1e-30f));
        }
    }

    SECTION("cc_adc (ADC conversion ≈ nphe_to_adc) is 2.4e-2 for all slats") {
        for (int i = 0; i < cal.numberOfEntries(); ++i) {
            REQUIRE(nearlyEqual(cal.slat(i).cc_adc, 2.4e-2f, 1e-4f));
        }
    }

    SECTION("ods_adc (ADC offset dispersion) is 0 for all slats") {
        for (int i = 0; i < cal.numberOfEntries(); ++i) {
            REQUIRE(nearlyEqual(cal.slat(i).ods_adc, 0.0f, 1e-5f, 1e-30f));
        }
    }
}

// ---------------------------------------------------------------------------
// Consistency between StTofSimParam and StTofCalibration defaults
// ---------------------------------------------------------------------------
TEST_CASE("StTofSimParam and StTofCalibration cc_adc / nphe_to_adc consistency",
          "[StTofSimParam][StTofCalibration]")
{
    // StTofCalibration::cc_adc (2.4e-2) is documented as "typically
    // nphe_to_adc"; StTofSimParam::nphe_to_adc is also 0.2 — these are
    // separate parameters but both describe photo-electron → ADC count
    // conversion.  Neither should be zero.
    StTofSimParam    sp;
    StTofCalibration cal;

    SECTION("nphe_to_adc (SimParam) is positive") {
        REQUIRE(sp.nphe_to_adc() > 0.0f);
    }

    SECTION("cc_adc (Calibration) is positive") {
        REQUIRE(cal.slat(0).cc_adc > 0.0f);
    }
}

// ===========================================================================
// Hard-coded digitisation saturation limits
//
// StTofSimMaker::detectorResponse() clips:
//   tdc > 2048  → tdc = 2048
//   adc > 1024  → adc = 1024
// These limits also match StTofSimParam::tdc_overflow and adc_overflow.
// ===========================================================================

TEST_CASE("Digitisation saturation limits are self-consistent", "[StTofSimMaker]") {

    StTofSimParam sp;

    SECTION("adc_overflow matches TDC saturation limit used in detectorResponse") {
        // detectorResponse clips adc at 1024; adc_overflow must equal 1024.
        REQUIRE(nearlyEqual(sp.adc_overflow(), 1024.0f));
    }

    SECTION("tdc_overflow matches TDC saturation limit used in detectorResponse") {
        // detectorResponse clips tdc at 2048; tdc_overflow must equal 2048.
        REQUIRE(nearlyEqual(sp.tdc_overflow(), 2048.0f));
    }

    SECTION("adc_overflow is strictly less than tdc_overflow") {
        // ADC is 10-bit (0..1024) and TDC is 11-bit (0..2048).
        REQUIRE(sp.adc_overflow() < sp.tdc_overflow());
    }
}

// ===========================================================================
// Integration-test stubs  [integration]
//
// The tests below document the full Init → InitRun → Make → FinishRun →
// Finish lifecycle.  They are tagged [integration] and excluded from the
// default test run because they require:
//   • A ROOT TApplication (for TH1F, TFile)
//   • A STAR chain with a "geant" dataset (G2T tables)
//   • A StTofGeometry STAR-dBase connection (InitRun)
//
// To run: ./test_StTofSimMaker [integration]
// ===========================================================================

TEST_CASE("StTofSimMaker Init sets up calibration and simulation params",
          "[StTofSimMaker][integration]")
{
    // Requires: TApplication, StChain, dBase connectivity
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StTofSimMaker InitRun initialises geometry for a given run number",
          "[StTofSimMaker][integration]")
{
    // Requires: STAR dBase with run-geometry tables accessible via StMaker
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StTofSimMaker Make processes G2T TOFp hit table",
          "[StTofSimMaker][integration]")
{
    // Requires: geant dataset with g2t_tof_hit rows and a valid StEvent
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StTofSimMaker Make handles missing G2T hit table gracefully",
          "[StTofSimMaker][integration]")
{
    // When geantData exists but g2t_tof_hit is absent, Make() should log a
    // warning and continue without crashing.
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StTofSimMaker Make handles missing StEvent gracefully",
          "[StTofSimMaker][integration]")
{
    // When StEvent is not present in the input chain, Make() returns kStWarn.
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StTofSimMaker FinishRun cleans up geometry object",
          "[StTofSimMaker][integration]")
{
    // After FinishRun the internal mGeomDb pointer should be null; a second
    // call to FinishRun must not double-delete.
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StTofSimMaker Finish writes tofsim.root when m_Mode is set",
          "[StTofSimMaker][integration]")
{
    // Requires a live TApplication and writable working directory.
    // After calling Finish() with m_Mode != 0 the file tofsim.root must exist
    // and contain the nine expected histograms (dE, dS, nphe, time, tt, tt1,
    // length, adc, tdc).
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StTofSimMaker detectorResponse builds correct ADC/TDC for single hit",
          "[StTofSimMaker][integration]")
{
    // Requires: initialised mGeomDb + mCalibDb + mSimDb (i.e. after Init and
    // InitRun).  Feed a synthetic g2t_ctf_hit_st and verify:
    //   • returned StTofMCSlat has slatIndex in [0, TOFP_MAX_SLATS)
    //   • adc  <= 1024
    //   • tdc  <= 2048
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StTofSimMaker VPD hit table is processed without crash",
          "[StTofSimMaker][integration]")
{
    // Make() reads g2t_vpd_hit via geantIter; verify hit count is logged and
    // the VPD section does not corrupt the TOFp SlatCollection.
    SUCCEED("integration stub — not exercised in standalone mode");
}
