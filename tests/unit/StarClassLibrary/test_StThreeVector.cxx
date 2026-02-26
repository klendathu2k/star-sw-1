// tests/unit/StarClassLibrary/test_StThreeVector.cxx
// Unit tests for StThreeVector<double> (typedef StThreeVectorD).
//
// StThreeVector is a pure header-only template; no ROOT application context
// is required for any of these tests.

#include "../catch_amalgamated.hpp"
#include "StarClassLibrary/StThreeVectorD.hh"

#include <cmath>

// Tolerance for floating-point comparisons.
static const double kEps = 1.0e-10;

TEST_CASE("StThreeVectorD default construction", "[StThreeVectorD]") {
    StThreeVectorD v;

    SECTION("x component is zero") { CHECK(v.x() == Catch::Approx(0.0).margin(kEps)); }
    SECTION("y component is zero") { CHECK(v.y() == Catch::Approx(0.0).margin(kEps)); }
    SECTION("z component is zero") { CHECK(v.z() == Catch::Approx(0.0).margin(kEps)); }
    SECTION("magnitude is zero")   { CHECK(v.mag() == Catch::Approx(0.0).margin(kEps)); }
}

TEST_CASE("StThreeVectorD construction with values", "[StThreeVectorD]") {
    StThreeVectorD v(3.0, 4.0, 0.0);

    SECTION("x accessor") { CHECK(v.x() == Catch::Approx(3.0).epsilon(1e-12)); }
    SECTION("y accessor") { CHECK(v.y() == Catch::Approx(4.0).epsilon(1e-12)); }
    SECTION("z accessor") { CHECK(v.z() == Catch::Approx(0.0).margin(kEps)); }

    SECTION("magnitude equals sqrt(x^2+y^2+z^2)") {
        double expected = std::sqrt(3.0*3.0 + 4.0*4.0);
        CHECK(v.mag() == Catch::Approx(expected).epsilon(1e-12));
    }
}

TEST_CASE("StThreeVectorD phi angle", "[StThreeVectorD]") {
    SECTION("positive x-axis: phi = 0") {
        StThreeVectorD v(1.0, 0.0, 0.0);
        CHECK(v.phi() == Catch::Approx(0.0).margin(kEps));
    }

    SECTION("positive y-axis: phi = pi/2") {
        StThreeVectorD v(0.0, 1.0, 0.0);
        CHECK(v.phi() == Catch::Approx(M_PI / 2.0).epsilon(1e-12));
    }

    SECTION("negative x-axis: phi = pi") {
        StThreeVectorD v(-1.0, 0.0, 0.0);
        CHECK(std::abs(v.phi()) == Catch::Approx(M_PI).epsilon(1e-12));
    }
}

TEST_CASE("StThreeVectorD theta angle", "[StThreeVectorD]") {
    SECTION("along z-axis: theta = 0") {
        StThreeVectorD v(0.0, 0.0, 1.0);
        CHECK(v.theta() == Catch::Approx(0.0).margin(kEps));
    }

    SECTION("in xy-plane: theta = pi/2") {
        StThreeVectorD v(1.0, 0.0, 0.0);
        CHECK(v.theta() == Catch::Approx(M_PI / 2.0).epsilon(1e-12));
    }
}

TEST_CASE("StThreeVectorD dot product", "[StThreeVectorD]") {
    StThreeVectorD a(1.0, 2.0, 3.0);
    StThreeVectorD b(4.0, 5.0, 6.0);

    SECTION("a . b == 1*4 + 2*5 + 3*6 == 32") {
        CHECK(a.dot(b) == Catch::Approx(32.0).epsilon(1e-12));
    }

    SECTION("orthogonal vectors have zero dot product") {
        StThreeVectorD x(1.0, 0.0, 0.0);
        StThreeVectorD y(0.0, 1.0, 0.0);
        CHECK(x.dot(y) == Catch::Approx(0.0).margin(kEps));
    }
}

TEST_CASE("StThreeVectorD cross product", "[StThreeVectorD]") {
    StThreeVectorD xhat(1.0, 0.0, 0.0);
    StThreeVectorD yhat(0.0, 1.0, 0.0);

    StThreeVectorD zhat = xhat.cross(yhat);

    SECTION("x cross y = z") {
        CHECK(zhat.x() == Catch::Approx(0.0).margin(kEps));
        CHECK(zhat.y() == Catch::Approx(0.0).margin(kEps));
        CHECK(zhat.z() == Catch::Approx(1.0).epsilon(1e-12));
    }
}

TEST_CASE("StThreeVectorD arithmetic operators", "[StThreeVectorD]") {
    StThreeVectorD a(1.0, 2.0, 3.0);
    StThreeVectorD b(4.0, 5.0, 6.0);

    SECTION("operator+ component-wise") {
        StThreeVectorD c = a + b;
        CHECK(c.x() == Catch::Approx(5.0).epsilon(1e-12));
        CHECK(c.y() == Catch::Approx(7.0).epsilon(1e-12));
        CHECK(c.z() == Catch::Approx(9.0).epsilon(1e-12));
    }

    SECTION("operator- component-wise") {
        StThreeVectorD c = b - a;
        CHECK(c.x() == Catch::Approx(3.0).epsilon(1e-12));
        CHECK(c.y() == Catch::Approx(3.0).epsilon(1e-12));
        CHECK(c.z() == Catch::Approx(3.0).epsilon(1e-12));
    }

    SECTION("operator*= scalar") {
        StThreeVectorD v(1.0, 2.0, 3.0);
        v *= 2.0;
        CHECK(v.x() == Catch::Approx(2.0).epsilon(1e-12));
        CHECK(v.y() == Catch::Approx(4.0).epsilon(1e-12));
        CHECK(v.z() == Catch::Approx(6.0).epsilon(1e-12));
    }
}

TEST_CASE("StThreeVectorD unit vector", "[StThreeVectorD]") {
    StThreeVectorD v(3.0, 4.0, 0.0);
    StThreeVectorD u = v.unit();

    SECTION("unit vector has magnitude 1") {
        CHECK(u.mag() == Catch::Approx(1.0).epsilon(1e-12));
    }

    SECTION("unit vector preserves direction") {
        // u should be proportional to v.
        double scale = v.mag();
        CHECK(u.x() == Catch::Approx(v.x() / scale).epsilon(1e-12));
        CHECK(u.y() == Catch::Approx(v.y() / scale).epsilon(1e-12));
    }
}

TEST_CASE("StThreeVectorD pseudoRapidity", "[StThreeVectorD]") {
    SECTION("along z-axis (theta=0) gives large positive eta") {
        // pseudoRapidity returns 1e20 when tan(theta/2) <= 0.
        StThreeVectorD v(0.0, 0.0, 1.0);
        CHECK(v.pseudoRapidity() > 10.0);
    }

    SECTION("in transverse plane (theta=pi/2) gives eta=0") {
        StThreeVectorD v(1.0, 0.0, 0.0);
        CHECK(v.pseudoRapidity() == Catch::Approx(0.0).margin(kEps));
    }
}
