// tests/unit/StarRoot/test_TPolinom.cxx
// Unit tests for TPolinom (polynomial with error propagation).

#include "../catch_amalgamated.hpp"
#include "StarRoot/TPolinom.h"

#include <cmath>

static const double kEps = 1.0e-10;

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------
TEST_CASE("TPolinom constant polynomial", "[TPolinom]") {
    TPolinom p(5.0);
    CHECK(p.Eval(0.0) == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(p.Eval(3.0) == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(p.Eval(-2.0) == Catch::Approx(5.0).epsilon(1e-12));
}

TEST_CASE("TPolinom linear polynomial", "[TPolinom]") {
    // p(x) = 3 + 2x
    TPolinom p(3.0, 2.0);
    CHECK(p.Eval(0.0)  == Catch::Approx(3.0).epsilon(1e-12));
    CHECK(p.Eval(1.0)  == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(p.Eval(-1.0) == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(p.Eval(2.0)  == Catch::Approx(7.0).epsilon(1e-12));
}

TEST_CASE("TPolinom quadratic polynomial", "[TPolinom]") {
    // p(x) = 1 + 0x + 1x^2  i.e. x^2 + 1
    TPolinom p(1.0, 0.0, 1.0);
    CHECK(p.Eval(0.0)  == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(p.Eval(1.0)  == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(p.Eval(2.0)  == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(p.Eval(-3.0) == Catch::Approx(10.0).epsilon(1e-12));
}

TEST_CASE("TPolinom construction from coefficient array", "[TPolinom]") {
    // p(x) = 2 + 3x + 4x^2
    const double coefs[] = {2.0, 3.0, 4.0};
    TPolinom p(2, coefs);
    CHECK(p.Eval(0.0) == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(p.Eval(1.0) == Catch::Approx(9.0).epsilon(1e-12));
    CHECK(p.Eval(2.0) == Catch::Approx(24.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// Static Eval
// ---------------------------------------------------------------------------
TEST_CASE("TPolinom static Eval", "[TPolinom]") {
    double coefs[] = {1.0, 0.0, 1.0};  // x^2 + 1
    CHECK(TPolinom::Eval(3.0, 2, coefs) == Catch::Approx(10.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// Derivative
// ---------------------------------------------------------------------------
TEST_CASE("TPolinom derivative of constant is zero", "[TPolinom]") {
    TPolinom p(7.0);
    CHECK(p.Deriv(0.0) == Catch::Approx(0.0).margin(kEps));
    CHECK(p.Deriv(5.0) == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("TPolinom derivative of linear is constant", "[TPolinom]") {
    // p(x) = 3 + 2x => p'(x) = 2
    TPolinom p(3.0, 2.0);
    CHECK(p.Deriv(0.0)  == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(p.Deriv(10.0) == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(p.Deriv(-5.0) == Catch::Approx(2.0).epsilon(1e-12));
}

TEST_CASE("TPolinom derivative of quadratic", "[TPolinom]") {
    // p(x) = 1 + 0x + 3x^2 => p'(x) = 6x
    TPolinom p(1.0, 0.0, 3.0);
    CHECK(p.Deriv(0.0) == Catch::Approx(0.0).margin(kEps));
    CHECK(p.Deriv(1.0) == Catch::Approx(6.0).epsilon(1e-12));
    CHECK(p.Deriv(2.0) == Catch::Approx(12.0).epsilon(1e-12));
    CHECK(p.Deriv(-2.0) == Catch::Approx(-12.0).epsilon(1e-12));
}

TEST_CASE("TPolinom derivative of cubic", "[TPolinom]") {
    // p(x) = x^3 => coefs [0,0,0,1], p'(x) = 3x^2
    const double coefs[] = {0.0, 0.0, 0.0, 1.0};
    TPolinom p(3, coefs);
    CHECK(p.Eval(2.0)   == Catch::Approx(8.0).epsilon(1e-12));
    CHECK(p.Deriv(2.0)  == Catch::Approx(12.0).epsilon(1e-12));
    CHECK(p.Deriv(-1.0) == Catch::Approx(3.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// Copy construction and assignment
// ---------------------------------------------------------------------------
TEST_CASE("TPolinom copy construction", "[TPolinom]") {
    TPolinom original(1.0, 2.0, 3.0);
    TPolinom copy(original);
    CHECK(copy.Eval(1.0) == Catch::Approx(original.Eval(1.0)).epsilon(1e-12));
    CHECK(copy.Eval(-2.0) == Catch::Approx(original.Eval(-2.0)).epsilon(1e-12));
}

TEST_CASE("TPolinom assignment", "[TPolinom]") {
    TPolinom a(4.0, 1.0);
    TPolinom b(0.0);
    b = a;
    CHECK(b.Eval(3.0) == Catch::Approx(a.Eval(3.0)).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// SetCoeff
// ---------------------------------------------------------------------------
TEST_CASE("TPolinom SetCoeff modifies individual coefficient", "[TPolinom]") {
    TPolinom p(0.0, 0.0, 0.0);  // zero quadratic
    p.SetCoeff(2, 5.0);         // now 5x^2
    CHECK(p.Eval(2.0) == Catch::Approx(20.0).epsilon(1e-12));
}
