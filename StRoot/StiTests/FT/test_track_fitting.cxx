// StRoot/StiTests/FT/test_track_fitting.cxx
// Unit tests for the STI Kalman track fitter and its helpers.
//
// Covers:
//   StiKalmanTrackFitter   — construction and debug flags
//   QaFit                  — quality-accumulator helper (construction, add, finish)
//   StiMcsErrs             — multiple-coulomb-scattering error struct
//   StiTrackNodeHelper     — node propagation helper (construction, setDir)
//
// Note: StiKalmanTrackFitter::fit(track, direction) requires a fully assembled
// StiKalmanTrack with initialized nodes and a live StiDetectorContainer.
// That is an integration-level test; only the lightweight API is covered here.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "Sti/StiKalmanTrackFitter.h"
#include "Sti/StiTrackNodeHelper.h"

// ---------------------------------------------------------------------------
// StiKalmanTrackFitter
// ---------------------------------------------------------------------------

TEST_CASE("StiKalmanTrackFitter — default construction", "[StiKalmanTrackFitter][FT]") {
    StiKalmanTrackFitter fitter;
    CHECK(true); // must not crash
}

TEST_CASE("StiKalmanTrackFitter — static debug flags round-trip", "[StiKalmanTrackFitter][FT]") {
    StiKalmanTrackFitter::setDebug(3);
    CHECK(StiKalmanTrackFitter::debug() == 3);
    StiKalmanTrackFitter::setDebug(0);
}

// ---------------------------------------------------------------------------
// QaFit
// ---------------------------------------------------------------------------

TEST_CASE("QaFit — default construction", "[QaFit][FT]") {
    QaFit qa;
    CHECK(qa.getTally() == 0);
}

TEST_CASE("QaFit — reset zeroes tally", "[QaFit][FT]") {
    QaFit qa;
    qa.add(1.0);
    qa.reset();
    CHECK(qa.getTally() == 0);
}

TEST_CASE("QaFit — add increments tally", "[QaFit][FT]") {
    QaFit qa;
    qa.add(1.0);
    qa.add(2.0);
    qa.add(3.0);
    CHECK(qa.getTally() == 3);
}

TEST_CASE("QaFit — finish does not crash", "[QaFit][FT]") {
    QaFit qa;
    qa.add(1.0);
    qa.add(2.0);
    qa.finish();
    CHECK(true);
}

TEST_CASE("QaFit — getAccu returns finite value after entries", "[QaFit][FT]") {
    QaFit qa;
    qa.add(1.0);
    qa.add(2.0);
    qa.finish();
    double v = qa.getAccu(0);
    CHECK(std::isfinite(v));
}

TEST_CASE("QaFit — getMaxi returns finite value after entries", "[QaFit][FT]") {
    QaFit qa;
    qa.add(5.0);
    qa.finish();
    double m = qa.getMaxi(0);
    CHECK(std::isfinite(m));
}

// ---------------------------------------------------------------------------
// StiMcsErrs
// ---------------------------------------------------------------------------

TEST_CASE("StiMcsErrs — construction initialises to zero via reset", "[StiMcsErrs][FT]") {
    StiMcsErrs errs;
    errs.reset();
    CHECK(errs._cEE     == Approx(0.0));
    CHECK(errs._cPP     == Approx(0.0));
    CHECK(errs._cTP     == Approx(0.0));
    CHECK(errs._cTT     == Approx(0.0));
    CHECK(errs._ptinCorr == Approx(0.0));
}

// ---------------------------------------------------------------------------
// StiTrackNodeHelper
// ---------------------------------------------------------------------------

TEST_CASE("StiTrackNodeHelper — default construction", "[StiTrackNodeHelper][FT]") {
    StiTrackNodeHelper helper;
    CHECK(true); // must not crash
}

TEST_CASE("StiTrackNodeHelper — setDir does not crash", "[StiTrackNodeHelper][FT]") {
    StiTrackNodeHelper helper;
    helper.setDir(+1);
    helper.setDir(-1);
    CHECK(true);
}

TEST_CASE("StiTrackNodeHelper — isCutStep is callable", "[StiTrackNodeHelper][FT]") {
    int v = StiTrackNodeHelper::isCutStep();
    (void)v;
    CHECK(true);
}
