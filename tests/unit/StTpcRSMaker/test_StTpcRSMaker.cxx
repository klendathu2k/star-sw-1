// tests/unit/StTpcRSMaker/test_StTpcRSMaker.cxx
// Unit tests for StTpcRSMaker — structural and pure-math tests.
// Integration tests require TApplication + full STAR runtime and are
// kept as commented-out stubs tagged [integration].
//
// Note on private-method access: Gatti() and PadResponseFunc() are
// declared private in StTpcRSMaker.  The #define below is a standard
// white-box unit-test technique that promotes access to public at
// compile time only.  It does not alter the class memory layout (access
// specifiers are layout-neutral in C++) and the symbols are resolved
// from the compiled library at link time with no ODR impact on the
// data representation.

#include "../catch_amalgamated.hpp"

// Promote private static methods to public for white-box testing.
#define private public
#include "StTpcRSMaker.h"
#undef private

#include <cmath>
#include <initializer_list>

// ---------------------------------------------------------------------------
// 1. Construction
// ---------------------------------------------------------------------------
TEST_CASE("StTpcRSMaker construction and naming", "[StTpcRSMaker]") {
    StTpcRSMaker maker("testTpc");

    REQUIRE(std::string(maker.GetName()) == "testTpc");
}

// ---------------------------------------------------------------------------
// 2. EMode enum values
// ---------------------------------------------------------------------------
TEST_CASE("StTpcRSMaker EMode enum values", "[StTpcRSMaker]") {
    CHECK(static_cast<int>(StTpcRSMaker::kPAI)         == 0);
    CHECK(static_cast<int>(StTpcRSMaker::kBICHSEL)     == 1);
    CHECK(static_cast<int>(StTpcRSMaker::kGAINOAtALL)  == 2);
    CHECK(static_cast<int>(StTpcRSMaker::kdEdxCorr)    == 3);
    CHECK(static_cast<int>(StTpcRSMaker::kDistortion)  == 4);
    CHECK(static_cast<int>(StTpcRSMaker::kNoToflight)  == 5);
    CHECK(static_cast<int>(StTpcRSMaker::kHEED)        == 6);
}

// ---------------------------------------------------------------------------
// 3. Gatti function
//
// Signature: static Double_t Gatti(Double_t *x, Double_t *p)
//   p[0] = w   — pad width [cm]
//   p[1] = h   — anode-cathode gap [cm]
//   p[2] = (wire spacing, not used inside Gatti itself)
//   p[3] = K3  — single Gatti shape parameter
//
// The function returns the charge fraction induced on a pad of width w
// centred at position x[0] (cm) relative to the avalanche point at 0.
// Analytically it integrates to 1 over all pad positions.
// ---------------------------------------------------------------------------
TEST_CASE("StTpcRSMaker::Gatti symmetry", "[StTpcRSMaker]") {
    // Typical STAR inner-sector parameters.
    const Double_t w  = 0.335; // pad width [cm]
    const Double_t h  = 0.200; // anode-cathode gap [cm]
    const Double_t K3 = 0.55;
    Double_t par[4] = {w, h, 0.0, K3};

    // Gatti(x) == Gatti(-x) for all x — the distribution is even.
    for (double xval : {0.05, 0.15, 0.335, 0.670, 1.005}) {
        double xpos = xval, xneg = -xval;
        CAPTURE(xval);
        CHECK(StTpcRSMaker::Gatti(&xpos, par) ==
              Catch::Approx(StTpcRSMaker::Gatti(&xneg, par)).epsilon(1e-10));
    }
}

TEST_CASE("StTpcRSMaker::Gatti normalisation", "[StTpcRSMaker]") {
    const Double_t w  = 0.335;
    const Double_t h  = 0.200;
    const Double_t K3 = 0.55;
    Double_t par[4] = {w, h, 0.0, K3};

    // Sum Gatti over 101 pads spaced by w.  The analytic sum equals 1
    // exactly; 101 terms capture all but ~10^-30 of the total.
    double sum = 0.0;
    for (int n = -50; n <= 50; ++n) {
        double xval = n * w;
        sum += StTpcRSMaker::Gatti(&xval, par);
    }
    CHECK(sum == Catch::Approx(1.0).epsilon(0.01));
}

TEST_CASE("StTpcRSMaker::Gatti maximum at pad centre", "[StTpcRSMaker]") {
    const Double_t w  = 0.335;
    const Double_t h  = 0.200;
    const Double_t K3 = 0.55;
    Double_t par[4] = {w, h, 0.0, K3};

    // The pad centred on the avalanche (x=0) collects more charge than
    // the immediately adjacent pad (x = ±w).
    double x0 = 0.0, xw = w;
    CHECK(StTpcRSMaker::Gatti(&x0, par) > StTpcRSMaker::Gatti(&xw, par));
}

// ---------------------------------------------------------------------------
// 4. polya function  (Gamma distribution for avalanche gain statistics)
//
// Signature: static Double_t polya(Double_t *x, Double_t *par)
//   par[0] = k     — shape parameter
//   par[1] = mu    — location parameter
//   par[2] = theta — scale parameter
//
// Delegates to TMath::GammaDist(x[0], par[0], par[1], par[2]).
// With mean = k * theta = 1 the distribution is normalised on [0, ∞).
// ---------------------------------------------------------------------------
TEST_CASE("StTpcRSMaker::polya is positive for x > 0", "[StTpcRSMaker]") {
    const Double_t k     = 1.38;
    const Double_t mu    = 0.0;
    const Double_t theta = 1.0 / k; // mean = k * theta = 1
    Double_t par[3] = {k, mu, theta};

    for (double xval : {0.1, 0.5, 1.0, 2.0, 5.0}) {
        CAPTURE(xval);
        CHECK(StTpcRSMaker::polya(&xval, par) > 0.0);
    }
}

TEST_CASE("StTpcRSMaker::polya is zero at x = 0", "[StTpcRSMaker]") {
    const Double_t k     = 1.38;
    const Double_t mu    = 0.0;
    const Double_t theta = 1.0 / k;
    Double_t par[3] = {k, mu, theta};

    // TMath::GammaDist returns 0 for x <= mu; with mu = 0 the PDF(0) = 0
    // for shape k > 1.
    double xval = 0.0;
    CHECK(StTpcRSMaker::polya(&xval, par) == Catch::Approx(0.0).margin(1e-12));
}

TEST_CASE("StTpcRSMaker::polya normalisation", "[StTpcRSMaker]") {
    const Double_t k     = 1.38;
    const Double_t mu    = 0.0;
    const Double_t theta = 1.0 / k;
    Double_t par[3] = {k, mu, theta};

    // Riemann sum with step = 0.01 from 0 to 20.
    // The gamma PDF with mean 1 is essentially zero beyond x = 10,
    // so 20 is a safe upper bound.
    const double step = 0.01;
    double sum = 0.0;
    for (double xval = 0.5 * step; xval < 20.0; xval += step) {
        sum += StTpcRSMaker::polya(&xval, par) * step;
    }
    CHECK(sum == Catch::Approx(1.0).epsilon(0.01));
}

// ---------------------------------------------------------------------------
// 5. Ec function  (HEED minimal-ionisation energy spectrum)
//
// Signature: static Double_t Ec(Double_t *x, Double_t *p)
//   p[0] = W — mean energy to create one ion pair [eV]
//
// Piecewise definition:
//   x  <  W/2          → 0
//   W/2 ≤ x  <  W      → 1
//   W  ≤ x  ≤ 3.064*W  → (W/x)^4
//   x  >  3.064*W      → 0
// ---------------------------------------------------------------------------
TEST_CASE("StTpcRSMaker::Ec HEED energy spectrum", "[StTpcRSMaker]") {
    const double W = 26.2; // [eV]  (standard STAR TPC value)
    Double_t p[1] = {W};

    SECTION("returns 0 for x strictly below W/2") {
        double xval = W / 2.0 - 1.0; // 12.1 eV
        CHECK(StTpcRSMaker::Ec(&xval, p) == Catch::Approx(0.0).margin(1e-12));
    }

    SECTION("returns 1 in the plateau W/2 <= x < W") {
        double xval = W * 0.75; // 19.65 eV — midway through plateau
        CHECK(StTpcRSMaker::Ec(&xval, p) == Catch::Approx(1.0).epsilon(1e-10));
    }

    SECTION("returns 1 at x = W (power-law boundary)") {
        // (W/W)^4 = 1
        double xval = W;
        CHECK(StTpcRSMaker::Ec(&xval, p) == Catch::Approx(1.0).epsilon(1e-10));
    }

    SECTION("returns (W/x)^4 for x > W") {
        double xval = 2.0 * W;                          // 52.4 eV
        const double expected = std::pow(W / xval, 4.0); // (1/2)^4 = 1/16
        CHECK(StTpcRSMaker::Ec(&xval, p) == Catch::Approx(expected).epsilon(1e-10));
    }

    SECTION("returns 0 beyond 3.064*W") {
        double xval = 3.064 * W + 1.0;
        CHECK(StTpcRSMaker::Ec(&xval, p) == Catch::Approx(0.0).margin(1e-12));
    }
}

// ---------------------------------------------------------------------------
// 6. fei function  (exponential-integral signal shape primitive)
//
// Signature: static Double_t fei(Double_t t, Double_t t0, Double_t T)
//
// Returns 0 when t < t0 (signal has not started).
// Returns a finite positive value when t >= t0 > 0 and T > 0.
// ---------------------------------------------------------------------------
TEST_CASE("StTpcRSMaker::fei returns 0 before signal onset", "[StTpcRSMaker]") {
    // t < t0  →  signal not yet present.
    CHECK(StTpcRSMaker::fei(0.0,  1e-7, 1e-6) == Catch::Approx(0.0).margin(1e-30));
    CHECK(StTpcRSMaker::fei(-1.0, 0.0,  1e-6) == Catch::Approx(0.0).margin(1e-30));
    CHECK(StTpcRSMaker::fei(1e-8, 1e-7, 1e-6) == Catch::Approx(0.0).margin(1e-30));
}

TEST_CASE("StTpcRSMaker::fei returns finite positive value after onset",
          "[StTpcRSMaker]") {
    // t = 5e-7 > t0 = 1e-7, T = 1e-6  →  (t+t0)/T = 0.6, t0/T = 0.1
    // Both arguments to ROOT::Math::expint are in (0, 100), giving a
    // finite, positive result.
    const double val = StTpcRSMaker::fei(5e-7, 1e-7, 1e-6);
    CHECK(std::isfinite(val));
    CHECK(val > 0.0);
}

// ---------------------------------------------------------------------------
// Integration-test stubs
// These require a TApplication, a running STAR database, and InitRun().
// ---------------------------------------------------------------------------
// TEST_CASE("StTpcRSMaker::InducedCharge", "[StTpcRSMaker][integration]") {
//     // Double_t t0 = 0;
//     // double q = StTpcRSMaker::InducedCharge(s, h, ra, Va, t0);
//     // CHECK(q > 0.0);
// }
//
// TEST_CASE("StTpcRSMaker::InitRun", "[StTpcRSMaker][integration]") {
//     // TApplication app("app", nullptr, nullptr);
//     // StTpcRSMaker maker("TpcRS");
//     // REQUIRE(maker.InitRun(19084033) == kStOK);
// }
//
// TEST_CASE("StTpcRSMaker::Make", "[StTpcRSMaker][integration]") {
//     // TApplication app("app", nullptr, nullptr);
//     // StTpcRSMaker maker("TpcRS");
//     // maker.InitRun(19084033);
//     // REQUIRE(maker.Make() == kStOK);
// }
