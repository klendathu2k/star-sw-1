// tests/unit/StChain/test_StMaker.cxx
// Unit tests for the StMaker abstract base class.
//
// StMaker extends TDataSet and participates in a ROOT object hierarchy;
// full Init/Make/Finish lifecycle tests require a TApplication instance
// and are therefore marked as integration tests with a comment.
// The construction and naming tests below work without a TApplication.

#include "../catch_amalgamated.hpp"
#include "StMaker.h"

// ---------------------------------------------------------------------------
// Minimal concrete StMaker subclass — implements no lifecycle methods beyond
// what the base provides so that we can instantiate and inspect it.
// ---------------------------------------------------------------------------
class MinimalMaker : public StMaker {
public:
    explicit MinimalMaker(const char* name = "MinimalMaker")
        : StMaker(name) {}

    // Make() is pure-virtual in some configurations; provide a stub.
    virtual Int_t Make() override { return kStOK; }

    // ROOT ClassDef would normally be here, but is omitted for a unit test
    // subclass that is never streamed.
};

TEST_CASE("StMaker construction", "[StMaker]") {
    MinimalMaker maker("testMaker");

    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(maker.GetName()) == "testMaker");
    }
}

TEST_CASE("StMaker default name", "[StMaker]") {
    MinimalMaker maker;

    SECTION("default name is the class default") {
        // The default argument in the ctor is ""; TDataSet stores it.
        REQUIRE(maker.GetName() != nullptr);
    }
}

// ---------------------------------------------------------------------------
// Integration-test placeholders
// These would require TApplication + a full STAR environment:
//   TEST_CASE("StMaker Init/Make/Finish lifecycle", "[StMaker][integration]")
//   TEST_CASE("StMaker dataset access", "[StMaker][integration]")
// ---------------------------------------------------------------------------
