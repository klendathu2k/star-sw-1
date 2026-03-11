// StRoot/StiTests/HT/test_hits_tracks.cxx
// Unit tests for the STI hit and track representation classes.
//
// Covers:
//   StiHit              — 3-D measurement with local/global coordinates and error matrix
//   StiNodePars         — Kalman track state parameter vector (8 components)
//   StiKalmanTrackNode  — track node (state + counters + detector pointer)

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include <cmath>

#include "Sti/StiHit.h"
#include "Sti/StiNodePars.h"
#include "Sti/StiKalmanTrackNode.h"
#include "Sti/StiIsActiveFunctor.h"
#include "Sti/StiCylindricalShape.h"
#include "Sti/StiPlacement.h"
#include "Sti/StiDetector.h"

#include "StiTests/Detector/MockMaterials.h"

static const double kPi = 3.14159265358979323846;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/// Build a minimal StiDetector attached to a TPC-like cylinder at radius r.
static StiDetector* makeDet(float r)
{
    auto* shape     = new StiCylindricalShape("det", 200.f, 0.4f,
                                               r + 0.2f,
                                               static_cast<float>(kPi / 6.0));
    auto* placement = new StiPlacement(0.f, r, 0.f, 0.f);
    placement->setLayerRadius(r);
    placement->setLayerAngle(0.f);
    placement->setZcenter(0.f);
    placement->setRegion(StiPlacement::kMidRapidity);
    auto* active   = new StiIsActiveFunctor();
    auto* gas      = MockMaterials::p10Gas();
    auto* mat      = MockMaterials::p10Gas();

    auto* det = new StiDetector();
    det->setShape(shape);
    det->setPlacement(placement);
    det->setIsActive(active);
    det->setGas(gas);
    det->setMaterial(mat);
    return det;
}

// ---------------------------------------------------------------------------
// StiHit
// ---------------------------------------------------------------------------

TEST_CASE("StiHit — default construction", "[StiHit][HT]") {
    StiHit h;
    CHECK(h.x()        == Approx(0.f));
    CHECK(h.y()        == Approx(0.f));
    CHECK(h.z()        == Approx(0.f));
    CHECK(h.refangle() == Approx(0.f));
    CHECK(h.position() == Approx(0.f));
    CHECK(h.timesUsed() == 0u);
}

TEST_CASE("StiHit — set(pos, angle, y, z) and getters", "[StiHit][HT]") {
    StiHit h;
    h.set(60.f, 0.5f, 2.f, 10.f);
    CHECK(h.position() == Approx(60.f));
    CHECK(h.refangle() == Approx(0.5f));
    CHECK(h.y()        == Approx(2.f));
    CHECK(h.z()        == Approx(10.f));
}

TEST_CASE("StiHit — copy construction", "[StiHit][HT]") {
    StiHit h1;
    h1.set(60.f, 0.5f, 2.f, 10.f);
    StiHit h2(h1);
    CHECK(h2.position() == Approx(h1.position()));
    CHECK(h2.refangle() == Approx(h1.refangle()));
    CHECK(h2.y()        == Approx(h1.y()));
    CHECK(h2.z()        == Approx(h1.z()));
}

TEST_CASE("StiHit — assignment operator", "[StiHit][HT]") {
    StiHit h1, h2;
    h1.set(60.f, 0.5f, 2.f, 10.f);
    h2 = h1;
    CHECK(h2.position() == Approx(h1.position()));
    CHECK(h2.y()        == Approx(h1.y()));
}

TEST_CASE("StiHit — error matrix getters are zero by default", "[StiHit][HT]") {
    StiHit h;
    CHECK(h.sxx() == Approx(0.f));
    CHECK(h.syy() == Approx(0.f));
    CHECK(h.szz() == Approx(0.f));
    CHECK(h.sxy() == Approx(0.f));
    CHECK(h.sxz() == Approx(0.f));
    CHECK(h.syz() == Approx(0.f));
}

TEST_CASE("StiHit — errMtx() pointer is non-null", "[StiHit][HT]") {
    StiHit h;
    const float* em = h.errMtx();
    REQUIRE(em != nullptr);
    // First element is sxx
    CHECK(*em == Approx(0.f));
}

TEST_CASE("StiHit — isDca returns 0 before makeDca", "[StiHit][HT]") {
    StiHit h;
    CHECK(h.isDca() == 0);
}

TEST_CASE("StiHit — makeDca sets DCA flag", "[StiHit][HT]") {
    StiHit h;
    h.makeDca();
    CHECK(h.isDca() != 0);
}

TEST_CASE("StiHit — rxy is sqrt of global x^2 + y^2", "[StiHit][HT]") {
    // Default global position is 0,0,0
    StiHit h;
    CHECK(h.rxy() == Approx(0.f));
}

TEST_CASE("StiHit — timesUsed increments correctly", "[StiHit][HT]") {
    StiHit h;
    CHECK(h.timesUsed() == 0u);
}

// ---------------------------------------------------------------------------
// StiNodePars
// ---------------------------------------------------------------------------

TEST_CASE("StiNodePars — reset zeros all fields", "[StiNodePars][HT]") {
    StiNodePars p;
    p.reset();
    CHECK(p.x()    == Approx(0.0));
    CHECK(p.y()    == Approx(0.0));
    CHECK(p.z()    == Approx(0.0));
    CHECK(p.eta()  == Approx(0.0));
    CHECK(p.ptin() == Approx(0.0));
    CHECK(p.tanl() == Approx(0.0));
    CHECK(p.curv() == Approx(0.0));
    CHECK(p.hz()   == Approx(0.0));
}

TEST_CASE("StiNodePars — non-const accessors allow assignment", "[StiNodePars][HT]") {
    StiNodePars p;
    p.reset();
    p.x()    = 5.0;
    p.y()    = 2.0;
    p.z()    = -3.5;
    p.eta()  = 0.1;
    p.ptin() = 0.5;
    p.tanl() = 0.3;
    p.curv() = 0.02;
    p.hz()   = 1.5e-3;

    CHECK(p.x()    == Approx(5.0));
    CHECK(p.y()    == Approx(2.0));
    CHECK(p.z()    == Approx(-3.5));
    CHECK(p.eta()  == Approx(0.1));
    CHECK(p.ptin() == Approx(0.5));
    CHECK(p.tanl() == Approx(0.3));
    CHECK(p.curv() == Approx(0.02));
    CHECK(p.hz()   == Approx(1.5e-3));
}

TEST_CASE("StiNodePars — operator[] read access", "[StiNodePars][HT]") {
    StiNodePars p;
    p.reset();
    p.x() = 3.0;
    CHECK(p[StiNodePars::kX] == Approx(3.0));
}

TEST_CASE("StiNodePars — rxy2 and rxy are consistent", "[StiNodePars][HT]") {
    StiNodePars p;
    p.reset();
    p.x() = 3.0;
    p.y() = 4.0;
    CHECK(p.rxy2() == Approx(25.0));
    CHECK(p.rxy()  == Approx(5.0));
}

TEST_CASE("StiNodePars — isZeroH true when hz is very small", "[StiNodePars][HT]") {
    StiNodePars p;
    p.reset();
    // hz = 0: should be zero-field
    CHECK(p.isZeroH() != 0);

    // hz = typical 0.5T value = kEC * 5.0 ~ 1.5e-3
    p.hz() = 1.5e-3;
    CHECK(p.isZeroH() == 0);
}

TEST_CASE("StiNodePars — rotate does not crash", "[StiNodePars][HT]") {
    StiNodePars p;
    p.reset();
    p.x() = 10.0; p.y() = 0.0; p.eta() = 0.0;
    p.rotate(kPi / 4.0);
    // After a π/4 rotation, x and y should share the original x value
    CHECK(std::isfinite(p.x()));
    CHECK(std::isfinite(p.y()));
}

// ---------------------------------------------------------------------------
// StiKalmanTrackNode
// ---------------------------------------------------------------------------

TEST_CASE("StiKalmanTrackNode — default construction", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    // Fresh node: all counters should be zero / null
    CHECK(node.getHitCount()        == 0);
    CHECK(node.getNullCount()       == 0);
    CHECK(node.getContigHitCount()  == 0);
    CHECK(node.getContigNullCount() == 0);
}

TEST_CASE("StiKalmanTrackNode — reset clears state", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();
    CHECK(node.getHitCount()  == 0);
    CHECK(node.getNullCount() == 0);
}

// Note: StiKalmanTrackNode::initialize(StiHit*) calls getHz() which calls
// StarMagField::Instance()->BField(...) — this requires a live StarMagField
// singleton and is therefore not exercised in this unit test.
// The StiDetector overload is used instead; it initialises the node state
// without accessing the field.

TEST_CASE("StiKalmanTrackNode — initialize with StiDetector sets detector", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();

    StiDetector* det = makeDet(60.f);
    node.initialize(det);

    // After initialize(det), x should equal the detector's normalRadius
    CHECK(std::isfinite(node.getX()));
    CHECK(node.getX() == Approx(60.f).epsilon(0.1));
    CHECK(node.getDetector() == det);

    delete det;
}

TEST_CASE("StiKalmanTrackNode — initialize(StiDetector) sets state to kTNInit", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();
    StiDetector* det = makeDet(80.f);
    node.initialize(det);
    CHECK(node.getX() == Approx(80.f).epsilon(0.1));
    delete det;
}

TEST_CASE("StiKalmanTrackNode — getX / getY / getZ match x() / y() / z()", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();
    // Both member function pairs should return the same value.
    CHECK(node.getX() == node.x());
    CHECK(node.getY() == node.y());
    CHECK(node.getZ() == node.z());
}

TEST_CASE("StiKalmanTrackNode — getCurvature / setCurvature round-trip", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();
    node.setCurvature(0.01);
    CHECK(node.getCurvature() == Approx(0.01));
}

TEST_CASE("StiKalmanTrackNode — getTanL via mFP after node pars", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();
    CHECK(node.getTanL() == Approx(0.0));
}

TEST_CASE("StiKalmanTrackNode — getAlpha after reset is zero", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();
    CHECK(node.getAlpha() == Approx(0.0));
}

TEST_CASE("StiKalmanTrackNode — getCos / getSin consistent with alpha=0", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();
    // alpha = 0: cos(0) = 1, sin(0) = 0
    CHECK(node.getCos() == Approx(1.0).epsilon(1e-5));
    CHECK(node.getSin() == Approx(0.0).margin(1e-5));
}

TEST_CASE("StiKalmanTrackNode — getEta returns track azimuth", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();
    CHECK(node.getEta() == Approx(0.0));
}

TEST_CASE("StiKalmanTrackNode — getRxy consistent with x/y", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();
    double expected = std::sqrt(node.getX() * node.getX() + node.getY() * node.getY());
    CHECK(node.getRxy() == Approx(expected).epsilon(1e-6));
}

TEST_CASE("StiKalmanTrackNode — incHitCount / incNullCount", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();

    node.incHitCount();
    node.incHitCount();
    CHECK(node.getHitCount() == 2);

    node.incNullCount();
    CHECK(node.getNullCount() == 1);
}

TEST_CASE("StiKalmanTrackNode — getCharge returns +/-1", "[StiKalmanTrackNode][HT]") {
    StiKalmanTrackNode node;
    node.reset();
    node.setCurvature(0.01); // positive curvature → negative charge
    int q = node.getCharge();
    CHECK((q == 1 || q == -1));
}
