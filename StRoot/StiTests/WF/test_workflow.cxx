// StRoot/StiTests/WF/test_workflow.cxx
// Unit tests for the STI workflow / steering maker (StiMaker).
//
// StiMaker inherits StMaker.  Init(), Make(), and Finish() require the full
// STAR reconstruction chain and cannot be invoked without one.  Only the
// lightweight aspects — construction, name, Clear(), and toolkit accessor —
// are tested here.
//
// See StRoot/StiTests/README.md Coverage Notes for a detailed explanation of
// why the WF category has low coverage (~29%).

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "StiMaker/StiMaker.h"

TEST_CASE("StiMaker — construction with default name", "[StiMaker][WF]") {
    StiMaker maker;
    CHECK(std::string(maker.GetName()) == "Sti");
}

TEST_CASE("StiMaker — construction with custom name", "[StiMaker][WF]") {
    StiMaker maker("MySti");
    CHECK(std::string(maker.GetName()) == "MySti");
}

TEST_CASE("StiMaker — getToolkit returns null before Init", "[StiMaker][WF]") {
    StiMaker maker;
    // Toolkit is not created until Init() is called.
    StiToolkit* tk = maker.getToolkit();
    CHECK(tk == nullptr);
}

TEST_CASE("StiMaker — Clear does not crash before Init", "[StiMaker][WF]") {
    StiMaker maker;
    maker.Clear();
    CHECK(true);
}
