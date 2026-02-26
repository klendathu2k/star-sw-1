// tests/unit/StChain/test_StChain.cxx
// Unit tests for StChain — the top-level maker that manages the processing
// pipeline.
//
// StChain inherits from StMaker (which inherits from TDataSet / TObject).
// Full event-loop tests require a TApplication and are integration tests;
// the structural tests below work without one.

#include "../catch_amalgamated.hpp"
#include "StChain.h"

TEST_CASE("StChain construction", "[StChain]") {
    StChain chain("testChain");

    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(chain.GetName()) == "testChain");
    }
}

TEST_CASE("StChain default construction", "[StChain]") {
    StChain chain;

    SECTION("default name is non-null") {
        REQUIRE(chain.GetName() != nullptr);
    }

    SECTION("default name is 'bfcChain'") {
        // The default argument in the StChain ctor is "bfcChain".
        CHECK(std::string(chain.GetName()) == "bfcChain");
    }
}

// ---------------------------------------------------------------------------
// Integration-test placeholders
// These would require TApplication + a full STAR environment:
//   TEST_CASE("StChain Init/Make/Finish event loop", "[StChain][integration]")
//   TEST_CASE("StChain maker registration", "[StChain][integration]")
// ---------------------------------------------------------------------------
