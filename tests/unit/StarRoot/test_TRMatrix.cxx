// tests/unit/StarRoot/test_TRMatrix.cxx
// Unit tests for TRArray, TRMatrix, TRVector, TRSymMatrix, and TRDiagMatrix.

#include "../catch_amalgamated.hpp"

#include "StarRoot/TRArray.h"
#include "StarRoot/TRMatrix.h"
#include "StarRoot/TRVector.h"
#include "StarRoot/TRSymMatrix.h"
#include "StarRoot/TRDiagMatrix.h"

#include <cmath>

static const double kEps = 1.0e-12;

// ---------------------------------------------------------------------------
// TRArray
// ---------------------------------------------------------------------------
TEST_CASE("TRArray default construction", "[TRArray]") {
    TRArray a(4);
    REQUIRE(a.GetSize() == 4);
    for (int i = 0; i < 4; ++i)
        CHECK(a[i] == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("TRArray construction from C array", "[TRArray]") {
    const double vals[] = {1.0, 2.0, 3.0};
    TRArray a(3, vals);
    CHECK(a[0] == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(a[1] == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(a[2] == Catch::Approx(3.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// TRMatrix
// ---------------------------------------------------------------------------
TEST_CASE("TRMatrix default construction", "[TRMatrix]") {
    TRMatrix m(2, 3);
    CHECK(m.GetNrows() == 2);
    CHECK(m.GetNcols() == 3);
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            CHECK(m(i, j) == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("TRMatrix construction from C array", "[TRMatrix]") {
    const double data[] = {1.0, 2.0,
                           3.0, 4.0,
                           5.0, 6.0};
    TRMatrix m(3, 2, data);
    CHECK(m(0, 0) == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(m(0, 1) == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(m(1, 0) == Catch::Approx(3.0).epsilon(1e-12));
    CHECK(m(2, 1) == Catch::Approx(6.0).epsilon(1e-12));
}

TEST_CASE("TRMatrix element assignment", "[TRMatrix]") {
    TRMatrix m(2, 2);
    m(0, 0) = 7.5;
    m(1, 1) = -3.0;
    CHECK(m(0, 0) == Catch::Approx(7.5).epsilon(1e-12));
    CHECK(m(1, 1) == Catch::Approx(-3.0).epsilon(1e-12));
    CHECK(m(0, 1) == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("TRMatrix matrix type", "[TRMatrix]") {
    TRMatrix m(3, 4);
    CHECK(m.GetMatrixType() == kRectangular);
}

TEST_CASE("TRMatrix scalar multiplication", "[TRMatrix]") {
    const double data[] = {1.0, 2.0, 3.0, 4.0};
    TRMatrix m(2, 2, data);
    TRMatrix r = m * 2.0;
    CHECK(r(0, 0) == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(r(1, 1) == Catch::Approx(8.0).epsilon(1e-12));
}

TEST_CASE("TRMatrix unit matrix", "[TRMatrix]") {
    TRMatrix m(kUnit, 3);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            CHECK(m(i, j) == Catch::Approx(i == j ? 1.0 : 0.0).margin(kEps));
}

TEST_CASE("TRMatrix transposed construction", "[TRMatrix]") {
    const double data[] = {1.0, 2.0,
                           3.0, 4.0};
    TRMatrix m(2, 2, data);
    TRMatrix t(m, kTransposed);
    CHECK(t(0, 0) == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(t(0, 1) == Catch::Approx(3.0).epsilon(1e-12));  // was m(1,0)
    CHECK(t(1, 0) == Catch::Approx(2.0).epsilon(1e-12));  // was m(0,1)
    CHECK(t(1, 1) == Catch::Approx(4.0).epsilon(1e-12));
}

TEST_CASE("TRMatrix kAxBT (A * B^T) product", "[TRMatrix]") {
    // A = [[1,0],[0,2]], B = [[1,0],[0,3]] => A*B^T = [[1,0],[0,6]]
    const double dA[] = {1.0, 0.0, 0.0, 2.0};
    const double dB[] = {1.0, 0.0, 0.0, 3.0};
    TRMatrix A(2, 2, dA);
    TRMatrix B(2, 2, dB);
    TRMatrix R(A, kAxBT, B);
    CHECK(R(0, 0) == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(R(1, 1) == Catch::Approx(6.0).epsilon(1e-12));
    CHECK(R(0, 1) == Catch::Approx(0.0).margin(kEps));
}

// ---------------------------------------------------------------------------
// TRVector
// ---------------------------------------------------------------------------
TEST_CASE("TRVector default construction", "[TRVector]") {
    TRVector v(3);
    CHECK(v.GetNrows() == 3);
    CHECK(v.GetMatrixType() == kVector);
    for (int i = 0; i < 3; ++i)
        CHECK(v(i) == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("TRVector construction from C array", "[TRVector]") {
    const double data[] = {1.0, 2.0, 3.0};
    TRVector v(3, data);
    CHECK(v(0) == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(v(1) == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(v(2) == Catch::Approx(3.0).epsilon(1e-12));
}

TEST_CASE("TRVector scalar multiply and add", "[TRVector]") {
    const double data[] = {2.0, 4.0, 6.0};
    TRVector v(3, data);
    TRVector r = v * 0.5;
    CHECK(r(0) == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(r(1) == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(r(2) == Catch::Approx(3.0).epsilon(1e-12));
}

TEST_CASE("TRVector cross product", "[TRVector]") {
    const double dataA[] = {1.0, 0.0, 0.0};
    const double dataB[] = {0.0, 1.0, 0.0};
    TRVector a(3, dataA);
    TRVector b(3, dataB);
    TRVector c = a.Cross(b);
    // x-hat cross y-hat = z-hat
    CHECK(c(0) == Catch::Approx(0.0).margin(kEps));
    CHECK(c(1) == Catch::Approx(0.0).margin(kEps));
    CHECK(c(2) == Catch::Approx(1.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// TRSymMatrix
// ---------------------------------------------------------------------------
TEST_CASE("TRSymMatrix default construction", "[TRSymMatrix]") {
    TRSymMatrix s(3);
    CHECK(s.GetNrows() == 3);
    CHECK(s.GetNcols() == 3);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            CHECK(s(i, j) == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("TRSymMatrix symmetry", "[TRSymMatrix]") {
    // packed lower-triangular: element order is (0,0),(1,0),(1,1),(2,0),(2,1),(2,2)
    const double vals[] = {4.0,
                           2.0, 9.0,
                           1.0, 3.0, 16.0};
    TRSymMatrix s(3, vals);
    // off-diagonals must be symmetric
    CHECK(s(1, 0) == Catch::Approx(s(0, 1)).epsilon(1e-12));
    CHECK(s(2, 0) == Catch::Approx(s(0, 2)).epsilon(1e-12));
    CHECK(s(2, 1) == Catch::Approx(s(1, 2)).epsilon(1e-12));
    CHECK(s(0, 0) == Catch::Approx(4.0).epsilon(1e-12));
    CHECK(s(1, 1) == Catch::Approx(9.0).epsilon(1e-12));
    CHECK(s(2, 2) == Catch::Approx(16.0).epsilon(1e-12));
}

TEST_CASE("TRSymMatrix inversion of diagonal matrix", "[TRSymMatrix]") {
    // Diagonal 2x2: [[4,0],[0,9]] => inverse [[0.25,0],[0,~0.111]]
    const double vals[] = {4.0,
                           0.0, 9.0};
    TRSymMatrix s(2, vals);
    s.Inverse();
    CHECK(s(0, 0) == Catch::Approx(0.25).epsilon(1e-10));
    CHECK(s(1, 1) == Catch::Approx(1.0 / 9.0).epsilon(1e-10));
    CHECK(s(0, 1) == Catch::Approx(0.0).margin(1e-10));
}

// ---------------------------------------------------------------------------
// TRDiagMatrix
// ---------------------------------------------------------------------------
TEST_CASE("TRDiagMatrix construction and element access", "[TRDiagMatrix]") {
    const double diag[] = {2.0, 5.0, 8.0};
    TRDiagMatrix d(3, diag);
    CHECK(d.GetNrows() == 3);
    CHECK(d.GetMatrixType() == kDiagonalMatrix);
    CHECK(d(0) == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(d(1) == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(d(2) == Catch::Approx(8.0).epsilon(1e-12));
}
