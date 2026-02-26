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

// ---------------------------------------------------------------------------
// TRArray: additional methods
// ---------------------------------------------------------------------------
TEST_CASE("TRArray Set resizes and zeros the array", "[TRArray]") {
    TRArray a(3);
    a.Set(5);
    CHECK(a.GetSize() == 5);
    for (int i = 0; i < 5; ++i)
        CHECK(a[i] == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("TRArray Set with values initialises from C array", "[TRArray]") {
    const double src[] = {1.0, 2.0, 3.0};
    TRArray a(3);
    a.Set(3, src);
    CHECK(a[0] == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(a[1] == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(a[2] == Catch::Approx(3.0).epsilon(1e-12));
}

TEST_CASE("TRArray GetArray returns internal pointer", "[TRArray]") {
    const double vals[] = {4.0, 5.0, 6.0};
    TRArray a(3, vals);
    const double *p = a.GetArray();
    REQUIRE(p != nullptr);
    CHECK(p[0] == Catch::Approx(4.0).epsilon(1e-12));
    CHECK(p[2] == Catch::Approx(6.0).epsilon(1e-12));
}

TEST_CASE("TRArray IsValid and SetValid", "[TRArray]") {
    TRArray a(3);
    CHECK(a.IsValid() == kTRUE);
    a.SetValid(kFALSE);
    CHECK(a.IsValid() == kFALSE);
    a.SetValid(kTRUE);
    CHECK(a.IsValid() == kTRUE);
}

TEST_CASE("TRArray Mag2 and Mag", "[TRArray]") {
    const double vals[] = {3.0, 4.0};  // mag = 5
    TRArray a(2, vals);
    CHECK(a.Mag2() == Catch::Approx(25.0).epsilon(1e-12));
    CHECK(a.Mag()  == Catch::Approx(5.0).epsilon(1e-12));
}

TEST_CASE("TRArray AdoptA takes ownership of external array", "[TRArray]") {
    double *buf = new double[3]{7.0, 8.0, 9.0};
    TRArray a(0);
    a.AdoptA(3, buf);
    CHECK(a.GetSize() == 3);
    CHECK(a[0] == Catch::Approx(7.0).epsilon(1e-12));
    CHECK(a[2] == Catch::Approx(9.0).epsilon(1e-12));
    // TRArray does not free buf when fIsNotOwn; caller responsible
    // (no delete here: TRArray::~TRArray sets fArray=0 when fIsNotOwn)
}

TEST_CASE("TRArray Verify returns true for matching arrays", "[TRArray]") {
    const double vals[] = {1.0, 2.0, 3.0};
    TRArray a(3, vals);
    TRArray b(3, vals);
    CHECK(a.Verify(b) == kTRUE);
}

TEST_CASE("TRArray operator= assignment", "[TRArray]") {
    const double vals[] = {1.0, 2.0, 3.0};
    TRArray a(3, vals);
    TRArray b(3);
    b = a;
    CHECK(b[0] == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(b[1] == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(b[2] == Catch::Approx(3.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// TRMatrix: additional methods
// ---------------------------------------------------------------------------
TEST_CASE("TRMatrix NI and NJ accessors", "[TRMatrix]") {
    TRMatrix m(3, 4);
    CHECK(m.NI() == 3);
    CHECK(m.NJ() == 4);
}

TEST_CASE("TRMatrix SetMatrix resizes and resets", "[TRMatrix]") {
    TRMatrix m(2, 2);
    m(0,0) = 99.0;
    m.SetMatrix(3, 3);
    CHECK(m.GetNrows() == 3);
    CHECK(m.GetNcols() == 3);
}

TEST_CASE("TRMatrix AddRow appends a row", "[TRMatrix]") {
    TRMatrix m(1, 3);
    m(0, 0) = 1.0; m(0, 1) = 2.0; m(0, 2) = 3.0;
    const double row[] = {4.0, 5.0, 6.0};
    m.AddRow(row);
    CHECK(m.GetNrows() == 2);
    CHECK(m(1, 0) == Catch::Approx(4.0).epsilon(1e-12));
    CHECK(m(1, 2) == Catch::Approx(6.0).epsilon(1e-12));
}

TEST_CASE("TRMatrix GetRow returns pointer to row data", "[TRMatrix]") {
    const double data[] = {1.0, 2.0, 3.0, 4.0};
    TRMatrix m(2, 2, data);
    const double *r0 = m.GetRow(0);
    CHECK(r0[0] == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(r0[1] == Catch::Approx(2.0).epsilon(1e-12));
}

TEST_CASE("TRMatrix Add computes kAxB (matrix multiplication)", "[TRMatrix]") {
    // A = [[1,2],[3,4]], B = [[5,6],[7,8]] => A*B = [[19,22],[43,50]]
    const double dA[] = {1.0, 2.0, 3.0, 4.0};
    const double dB[] = {5.0, 6.0, 7.0, 8.0};
    TRMatrix A(2, 2, dA);
    TRMatrix B(2, 2, dB);
    TRMatrix R(2, 2);
    R.Add(A, kAxB, B);
    CHECK(R(0, 0) == Catch::Approx(19.0).epsilon(1e-12));
    CHECK(R(0, 1) == Catch::Approx(22.0).epsilon(1e-12));
    CHECK(R(1, 0) == Catch::Approx(43.0).epsilon(1e-12));
    CHECK(R(1, 1) == Catch::Approx(50.0).epsilon(1e-12));
}

TEST_CASE("TRMatrix Substruct computes kAxB subtraction", "[TRMatrix]") {
    // R = A*B - C*D where A=B=I(2x2), C=D=I => A*B - C*D = 0
    const double dI[] = {1.0, 0.0, 0.0, 1.0};
    TRMatrix I(2, 2, dI);
    TRMatrix R(2, 2, dI);
    R.Substruct(I, kAxB, I);
    CHECK(R(0, 0) == Catch::Approx(0.0).margin(kEps));
    CHECK(R(1, 1) == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("TRMatrix Print does not crash", "[TRMatrix]") {
    TRMatrix m(2, 2);
    CHECK_NOTHROW(m.Print());
}

// ---------------------------------------------------------------------------
// TRVector: Unit
// ---------------------------------------------------------------------------
TEST_CASE("TRVector Unit returns normalised vector", "[TRVector]") {
    const double data[] = {3.0, 0.0, 4.0};  // mag = 5
    TRVector v(3, data);
    TRVector u = v.Unit();
    CHECK(u(0) == Catch::Approx(0.6).epsilon(1e-12));
    CHECK(u(1) == Catch::Approx(0.0).margin(kEps));
    CHECK(u(2) == Catch::Approx(0.8).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// TRSymMatrix: Product, SpmInv, AddRow
// ---------------------------------------------------------------------------
TEST_CASE("TRSymMatrix Product computes A^T S A", "[TRSymMatrix]") {
    // S = [[4,0],[0,9]], v = [1,1]  =>  v^T S v = 4+9 = 13
    const double svals[] = {4.0, 0.0, 9.0};
    TRSymMatrix S(2, svals);
    const double vdata[] = {1.0, 1.0};
    TRVector v(2, vdata);
    double result = S.Product(v);
    CHECK(result == Catch::Approx(13.0).epsilon(1e-10));
}

TEST_CASE("TRSymMatrix SpmInv inverts a 2x2 diagonal", "[TRSymMatrix]") {
    // S = [[4,0],[0,9]] => inverse = [[0.25,0],[0,~0.111]]
    const double svals[] = {4.0, 0.0, 9.0};
    TRSymMatrix S(2, svals);
    TRSymMatrix inv(2);
    inv.SpmInv(S);
    CHECK(inv(0, 0) == Catch::Approx(0.25).epsilon(1e-10));
    CHECK(inv(1, 1) == Catch::Approx(1.0 / 9.0).epsilon(1e-10));
    CHECK(inv(0, 1) == Catch::Approx(0.0).margin(1e-10));
}

TEST_CASE("TRSymMatrix AddRow appends to packed storage", "[TRSymMatrix]") {
    TRSymMatrix s(1);
    s(0, 0) = 4.0;
    const double row[] = {1.0, 9.0};  // new row: s(1,0)=1, s(1,1)=9
    s.AddRow(row);
    CHECK(s.GetNrows() == 2);
    CHECK(s(1, 0) == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(s(1, 1) == Catch::Approx(9.0).epsilon(1e-12));
}

TEST_CASE("TRSymMatrix TrsInv inverts a 1x1 triangular system", "[TRSymMatrix]") {
    // 1x1: g = {2.0}  => gi should be {0.5}
    double g[]  = {2.0};
    double gi[] = {0.0};
    TRSymMatrix::TrsInv(g, gi, 1);
    CHECK(gi[0] == Catch::Approx(0.5).epsilon(1e-12));
}

TEST_CASE("TRSymMatrix TrInv inverts a 1x1 triangular system", "[TRSymMatrix]") {
    double g[]  = {3.0};
    double gi[] = {0.0};
    TRSymMatrix::TrInv(g, gi, 1);
    CHECK(gi[0] == Catch::Approx(1.0 / 3.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// TRDiagMatrix: Product, AddRow
// ---------------------------------------------------------------------------
TEST_CASE("TRDiagMatrix Product computes v^T D v", "[TRDiagMatrix]") {
    // D = diag(2,3), v = (1,2) => v^T D v = 1*2*1 + 2*3*2 = 2+12 = 14
    const double diag[] = {2.0, 3.0};
    TRDiagMatrix d(2, diag);
    const double vdata[] = {1.0, 2.0};
    TRVector v(2, vdata);
    double result = d.Product(v, kAxSxAT);
    CHECK(result == Catch::Approx(14.0).epsilon(1e-10));
}

TEST_CASE("TRDiagMatrix AddRow appends diagonal element", "[TRDiagMatrix]") {
    const double diag[] = {4.0, 9.0};
    TRDiagMatrix d(2, diag);
    d.AddRow(16.0);  // add diagonal element only
    CHECK(d.GetNrows() == 3);
    CHECK(d(2) == Catch::Approx(16.0).epsilon(1e-12));
}

TEST_CASE("TRSymMatrix TrchLU Cholesky factorisation of 1x1", "[TRSymMatrix]") {
    // 1x1: g = {4.0} => Cholesky: L = {2.0}
    double g[]  = {4.0};
    double gi[] = {0.0};
    int fail = TRSymMatrix::TrchLU(g, gi, 1);
    CHECK(fail == 0);
    CHECK(gi[0] == Catch::Approx(2.0).epsilon(1e-12));
}

TEST_CASE("TRSymMatrix TrsmUL applies triangular solve on 1x1", "[TRSymMatrix]") {
    // 1x1: TrsmUL(g, gi, 1) where gi = {2.0} from TrchLU above => gi^{-1} = {0.5}
    double g[]  = {2.0};   // Cholesky factor
    double gi[] = {2.0};
    TRSymMatrix::TrsmUL(g, gi, 1);
    CHECK(gi[0] == Catch::Approx(0.5).epsilon(1e-10));
}

TEST_CASE("TRSymMatrix spminv inverts a 2x2 diagonal via raw interface", "[TRSymMatrix]") {
    // Packed lower-triangular: [[4,0],[0,9]] => {4,0,9}
    Double_t v[] = {4.0, 0.0, 9.0};
    Double_t b[] = {0.0, 0.0};
    Int_t nrank  = 0;
    Double_t diag[2] = {0.0, 0.0};
    Bool_t   flag[2] = {kFALSE, kFALSE};
    TRSymMatrix::spminv(v, b, 2, nrank, diag, flag);
    CHECK(nrank == 2);
    CHECK(v[0] == Catch::Approx(0.25).epsilon(1e-10));
    CHECK(v[2] == Catch::Approx(1.0 / 9.0).epsilon(1e-10));
}
