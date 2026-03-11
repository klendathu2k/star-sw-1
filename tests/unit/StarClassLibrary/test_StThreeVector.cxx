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
    // eta = -ln( tan(theta/2) ); as theta -> 0, eta -> +inf.
    // The implementation returns the sentinel 1e20 when tan(theta/2) <= 0.
    SECTION("along z-axis (theta=0) gives sentinel 1e20") {
        StThreeVectorD v(0.0, 0.0, 1.0);
        CHECK(v.pseudoRapidity() == Catch::Approx(1e20).epsilon(1e-10));
    }

    SECTION("in transverse plane (theta=pi/2) gives eta=0") {
        StThreeVectorD v(1.0, 0.0, 0.0);
        CHECK(v.pseudoRapidity() == Catch::Approx(0.0).margin(kEps));
    }
}

TEST_CASE("StThreeVectorD pseudoRapidity at fixed theta values", "[StThreeVectorD]") {
    // eta = -ln( tan(theta/2) )
    // Vectors are chosen so that theta is exact: v = (sin(theta), 0, cos(theta)).
    SECTION("theta = 45 deg (pi/4): eta = ln(1 + sqrt(2)) ~ 0.8814") {
        // tan(pi/8) = sqrt(2) - 1  =>  eta = ln(1 + sqrt(2))
        StThreeVectorD v(1.0, 0.0, 1.0);  // theta = atan2(1,1) = pi/4
        const double expected = std::log(1.0 + std::sqrt(2.0));
        CHECK(v.pseudoRapidity() == Catch::Approx(expected).epsilon(1e-10));
    }

    SECTION("theta = 90 deg (pi/2): eta = 0") {
        StThreeVectorD v(1.0, 0.0, 0.0);
        CHECK(v.pseudoRapidity() == Catch::Approx(0.0).margin(kEps));
    }

    SECTION("theta = 135 deg (3*pi/4): eta = -ln(1 + sqrt(2)) ~ -0.8814") {
        // tan(3*pi/8) = 1 + sqrt(2)  =>  eta = -ln(1 + sqrt(2))
        StThreeVectorD v(1.0, 0.0, -1.0);  // theta = atan2(1,-1) = 3*pi/4
        const double expected = -std::log(1.0 + std::sqrt(2.0));
        CHECK(v.pseudoRapidity() == Catch::Approx(expected).epsilon(1e-10));
    }
}

// ---------------------------------------------------------------------------
// Individual component setters
// ---------------------------------------------------------------------------
TEST_CASE("StThreeVectorD setX/setY/setZ", "[StThreeVectorD]") {
    StThreeVectorD v;
    v.setX(1.5);
    v.setY(2.5);
    v.setZ(3.5);
    CHECK(v.x() == Catch::Approx(1.5).epsilon(1e-12));
    CHECK(v.y() == Catch::Approx(2.5).epsilon(1e-12));
    CHECK(v.z() == Catch::Approx(3.5).epsilon(1e-12));
}

TEST_CASE("StThreeVectorD set(x,y,z) sets all components", "[StThreeVectorD]") {
    StThreeVectorD v;
    v.set(4.0, 5.0, 6.0);
    CHECK(v.x() == Catch::Approx(4.0).epsilon(1e-12));
    CHECK(v.y() == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(v.z() == Catch::Approx(6.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// Spherical coordinate setters
// ---------------------------------------------------------------------------
TEST_CASE("StThreeVectorD setMagnitude preserves direction", "[StThreeVectorD]") {
    StThreeVectorD v(3.0, 0.0, 4.0);  // mag = 5
    v.setMagnitude(10.0);
    CHECK(v.mag() == Catch::Approx(10.0).epsilon(1e-12));
    CHECK(v.x() / v.z() == Catch::Approx(3.0 / 4.0).epsilon(1e-10));
}

TEST_CASE("StThreeVectorD setPhi preserves mag and theta", "[StThreeVectorD]") {
    StThreeVectorD v(1.0, 0.0, 1.0);
    double mag0   = v.mag();
    double theta0 = v.theta();
    v.setPhi(M_PI / 2.0);
    CHECK(v.mag()   == Catch::Approx(mag0).epsilon(1e-10));
    CHECK(v.theta() == Catch::Approx(theta0).epsilon(1e-10));
    CHECK(v.phi()   == Catch::Approx(M_PI / 2.0).epsilon(1e-10));
}

TEST_CASE("StThreeVectorD setTheta preserves mag and phi", "[StThreeVectorD]") {
    StThreeVectorD v(1.0, 0.0, 0.0);  // phi=0, theta=pi/2, mag=1
    double mag0 = v.mag();
    double phi0 = v.phi();
    v.setTheta(M_PI / 4.0);
    CHECK(v.mag()   == Catch::Approx(mag0).epsilon(1e-10));
    CHECK(v.phi()   == Catch::Approx(phi0).margin(1e-10));
    CHECK(v.theta() == Catch::Approx(M_PI / 4.0).epsilon(1e-10));
}

// ---------------------------------------------------------------------------
// Scalar getters
// ---------------------------------------------------------------------------
TEST_CASE("StThreeVectorD mag2", "[StThreeVectorD]") {
    StThreeVectorD v(1.0, 2.0, 2.0);  // mag = 3
    CHECK(v.mag2() == Catch::Approx(9.0).epsilon(1e-12));
}

TEST_CASE("StThreeVectorD perp and perp2", "[StThreeVectorD]") {
    StThreeVectorD v(3.0, 4.0, 100.0);  // transverse = 5
    CHECK(v.perp()  == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(v.perp2() == Catch::Approx(25.0).epsilon(1e-12));
}

TEST_CASE("StThreeVectorD cosTheta", "[StThreeVectorD]") {
    StThreeVectorD along_z(0.0, 0.0, 5.0);
    CHECK(along_z.cosTheta() == Catch::Approx(1.0).epsilon(1e-12));

    StThreeVectorD in_xy(1.0, 0.0, 0.0);
    CHECK(in_xy.cosTheta() == Catch::Approx(0.0).margin(kEps));
}

// ---------------------------------------------------------------------------
// Validity
// ---------------------------------------------------------------------------
TEST_CASE("StThreeVectorD valid and bad", "[StThreeVectorD]") {
    StThreeVectorD v(1.0, 2.0, 3.0);
    CHECK(v.valid() == 1);
    CHECK(v.bad()   == 0);

    // Component larger than default world radius (1e5) is invalid
    StThreeVectorD huge(2.0e5, 0.0, 0.0);
    CHECK(huge.valid() == 0);
    CHECK(huge.bad()   != 0);
}

// ---------------------------------------------------------------------------
// angle, orthogonal, massHypothesis, pseudoProduct
// ---------------------------------------------------------------------------
TEST_CASE("StThreeVectorD angle between two vectors", "[StThreeVectorD]") {
    StThreeVectorD x(1.0, 0.0, 0.0);
    StThreeVectorD y(0.0, 1.0, 0.0);
    CHECK(x.angle(y) == Catch::Approx(M_PI / 2.0).epsilon(1e-12));
    CHECK(x.angle(x) == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("StThreeVectorD orthogonal is perpendicular to original", "[StThreeVectorD]") {
    StThreeVectorD v(1.0, 2.0, 3.0);
    StThreeVectorD o = v.orthogonal();
    CHECK(v.dot(o) == Catch::Approx(0.0).margin(kEps));
    CHECK(o.mag() > 0.0);
}

TEST_CASE("StThreeVectorD massHypothesis", "[StThreeVectorD]") {
    // (0,0,3) with mass 4 → E = sqrt(9+16) = 5
    StThreeVectorD v(0.0, 0.0, 3.0);
    CHECK(v.massHypothesis(4.0) == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(v.massHypothesis(0.0) == Catch::Approx(3.0).epsilon(1e-12));
}

TEST_CASE("StThreeVectorD pseudoProduct scales components independently", "[StThreeVectorD]") {
    StThreeVectorD v(2.0, 3.0, 4.0);
    StThreeVectorD r = v.pseudoProduct(2.0, 3.0, 0.5);
    CHECK(r.x() == Catch::Approx(4.0).epsilon(1e-12));
    CHECK(r.y() == Catch::Approx(9.0).epsilon(1e-12));
    CHECK(r.z() == Catch::Approx(2.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// xyz() pointer access
// ---------------------------------------------------------------------------
TEST_CASE("StThreeVectorD xyz pointer accesses components", "[StThreeVectorD]") {
    StThreeVectorD v(1.0, 2.0, 3.0);
    const double *p = v.xyz();
    CHECK(p[0] == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(p[1] == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(p[2] == Catch::Approx(3.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// Rotations
// ---------------------------------------------------------------------------
TEST_CASE("StThreeVectorD rotateZ by pi/2 maps x-hat to y-hat", "[StThreeVectorD]") {
    StThreeVectorD v(1.0, 0.0, 0.0);
    v.rotateZ(M_PI / 2.0);
    CHECK(v.x() == Catch::Approx(0.0).margin(kEps));
    CHECK(v.y() == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(v.z() == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("StThreeVectorD rotateX by pi/2 maps y-hat to z-hat", "[StThreeVectorD]") {
    StThreeVectorD v(0.0, 1.0, 0.0);
    v.rotateX(M_PI / 2.0);
    CHECK(v.x() == Catch::Approx(0.0).margin(kEps));
    CHECK(v.y() == Catch::Approx(0.0).margin(kEps));
    CHECK(v.z() == Catch::Approx(1.0).epsilon(1e-12));
}

TEST_CASE("StThreeVectorD rotateY by pi/2 maps z-hat to x-hat", "[StThreeVectorD]") {
    StThreeVectorD v(0.0, 0.0, 1.0);
    v.rotateY(M_PI / 2.0);
    CHECK(v.x() == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(v.y() == Catch::Approx(0.0).margin(kEps));
    CHECK(v.z() == Catch::Approx(0.0).margin(kEps));
}

// ---------------------------------------------------------------------------
// Additional arithmetic operators
// ---------------------------------------------------------------------------
TEST_CASE("StThreeVectorD operator+=", "[StThreeVectorD]") {
    StThreeVectorD v(1.0, 2.0, 3.0);
    StThreeVectorD w(4.0, 5.0, 6.0);
    v += w;
    CHECK(v.x() == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(v.y() == Catch::Approx(7.0).epsilon(1e-12));
    CHECK(v.z() == Catch::Approx(9.0).epsilon(1e-12));
}

TEST_CASE("StThreeVectorD operator-=", "[StThreeVectorD]") {
    StThreeVectorD v(5.0, 7.0, 9.0);
    StThreeVectorD w(1.0, 2.0, 3.0);
    v -= w;
    CHECK(v.x() == Catch::Approx(4.0).epsilon(1e-12));
    CHECK(v.y() == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(v.z() == Catch::Approx(6.0).epsilon(1e-12));
}

TEST_CASE("StThreeVectorD operator/=", "[StThreeVectorD]") {
    StThreeVectorD v(2.0, 4.0, 6.0);
    v /= 2.0;
    CHECK(v.x() == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(v.y() == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(v.z() == Catch::Approx(3.0).epsilon(1e-12));
}

TEST_CASE("StThreeVectorD operator[] element access", "[StThreeVectorD]") {
    StThreeVectorD v(1.5, 2.5, 3.5);
    CHECK(v[0] == Catch::Approx(1.5).epsilon(1e-12));
    CHECK(v[1] == Catch::Approx(2.5).epsilon(1e-12));
    CHECK(v[2] == Catch::Approx(3.5).epsilon(1e-12));
}

TEST_CASE("StThreeVectorD operator= copy assignment", "[StThreeVectorD]") {
    StThreeVectorD v(1.0, 2.0, 3.0);
    StThreeVectorD w;
    w = v;
    CHECK(w.x() == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(w.y() == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(w.z() == Catch::Approx(3.0).epsilon(1e-12));
}
