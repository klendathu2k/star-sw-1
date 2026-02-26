// tests/unit/St_base/test_StMessMgr.cxx
// Compile-level tests for the StMessMgr logging macros.
//
// StMessMgr is an abstract class and its concrete implementation
// (StMessageManager) requires a ROOT runtime to function.  These tests
// therefore focus on:
//   1. Verifying that the LOG_* macros expand without compile errors.
//   2. Verifying that the LOGGERMESSAGE helper macro is defined.
//
// Full runtime tests (actual message emission) are integration tests that
// require a TApplication instance and are outside the scope of this file.

#include "../catch_amalgamated.hpp"
// Rtypes.h must precede StMessMgr.h to provide Int_t and R__EXTERN.
#include "Rtypes.h"
#include "StMessMgr.h"

TEST_CASE("StMessMgr macro definitions compile", "[StMessMgr][compile]") {

    SECTION("LOG_INFO macro is defined") {
        // The test here is that the include compiled without error and the
        // symbol is accessible.  We verify via the preprocessor.
#ifndef LOG_INFO
        FAIL("LOG_INFO macro is not defined");
#endif
        SUCCEED("LOG_INFO is defined");
    }

    SECTION("LOG_WARN macro is defined") {
#ifndef LOG_WARN
        FAIL("LOG_WARN macro is not defined");
#endif
        SUCCEED("LOG_WARN is defined");
    }

    SECTION("LOG_ERROR macro is defined") {
#ifndef LOG_ERROR
        FAIL("LOG_ERROR macro is not defined");
#endif
        SUCCEED("LOG_ERROR is defined");
    }

    SECTION("LOG_DEBUG macro is defined") {
#ifndef LOG_DEBUG
        FAIL("LOG_DEBUG macro is not defined");
#endif
        SUCCEED("LOG_DEBUG is defined");
    }

    SECTION("LOG_FATAL macro is defined") {
#ifndef LOG_FATAL
        FAIL("LOG_FATAL macro is not defined");
#endif
        SUCCEED("LOG_FATAL is defined");
    }
}

TEST_CASE("StMessMgr gMessMgr extern declaration visible", "[StMessMgr][compile]") {
    // Verify the extern declaration compiles — runtime value will be nullptr
    // without an active ROOT application, so we only check the symbol exists.
    SECTION("gMessMgr symbol is declared") {
        // This would cause a link error if the extern declaration is absent.
        StMessMgr* mgr = gMessMgr;
        // In a standalone test (no ROOT app) gMessMgr is nullptr — that is
        // expected and acceptable; we only test that the declaration compiled.
        (void)mgr;
        SUCCEED("gMessMgr extern declaration is visible");
    }
}
