// StRoot/StiTests/TF/test_track_finding.cxx
// Unit tests for the STI Kalman track finder.
//
// Only the parts that do not require a live StiToolkit, StiHitContainer,
// or event data are tested here.  The methods findTracks(), extendSeeds(),
// extendTracks(), and extendTracksToVertices() are integration-level and are
// excluded (see StRoot/StiTests/README.md, Coverage Notes).

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "Sti/StiKalmanTrackFinder.h"

// ---------------------------------------------------------------------------
// StiKalmanTrackFinder — lightweight construction and state queries
// ---------------------------------------------------------------------------

TEST_CASE("StiKalmanTrackFinder — construction with null toolkit", "[StiKalmanTrackFinder][TF]") {
    // The default constructor takes no toolkit pointer.
    StiKalmanTrackFinder finder;
    CHECK(true); // construction must not crash
}

TEST_CASE("StiKalmanTrackFinder — getNTracks returns 0 on fresh object", "[StiKalmanTrackFinder][TF]") {
    StiKalmanTrackFinder finder;
    CHECK(finder.getNTracks() == 0);
}

TEST_CASE("StiKalmanTrackFinder — getNPrims returns 0 on fresh object", "[StiKalmanTrackFinder][TF]") {
    StiKalmanTrackFinder finder;
    CHECK(finder.getNPrims() == 0);
}

TEST_CASE("StiKalmanTrackFinder — unset does not crash", "[StiKalmanTrackFinder][TF]") {
    StiKalmanTrackFinder finder;
    finder.unset();
    CHECK(true);
}

TEST_CASE("StiKalmanTrackFinder — static debug flags round-trip", "[StiKalmanTrackFinder][TF]") {
    StiKalmanTrackFinder::setDebug(2);
    CHECK(StiKalmanTrackFinder::debug() == 2);
    StiKalmanTrackFinder::setDebug(0); // restore default
}

TEST_CASE("StiKalmanTrackFinder — addSeedFinder accepts a derived finder", "[StiKalmanTrackFinder][TF]") {
    // Use another StiKalmanTrackFinder as a trivial seed finder.
    StiKalmanTrackFinder finder;
    StiKalmanTrackFinder seedFinder;
    finder.addSeedFinder(&seedFinder);
    // If we get here without crash the method works.
    CHECK(true);
}
