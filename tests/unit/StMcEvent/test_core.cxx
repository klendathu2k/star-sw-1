// tests/unit/StMcEvent/test_core.cxx
// Structural unit tests for the StMcEvent core classes:
//   StMcHit (via StMcTpcHit), StMcVertex, StMcTrack, StMcEvent.
//
// No ROOT I/O, TApplication, or StChain is required.
// All tests instantiate objects directly and exercise set/get round-trips.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "StMcEvent/StMcTpcHit.hh"   // concrete StMcHit subclass
#include "StMcEvent/StMcVertex.hh"
#include "StMcEvent/StMcTrack.hh"
#include "StMcEvent/StMcEvent.hh"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/// Build a StThreeVectorF from three floats.
static StThreeVectorF vec3(float x, float y, float z)
{
    return StThreeVectorF(x, y, z);
}

// ===========================================================================
// TEST_CASE: StMcHit base class
//
// StMcTpcHit is a concrete subclass of StMcHit with a public default ctor.
// All tested methods are declared on the StMcHit base and exercised via the
// subclass instance.
// ===========================================================================

TEST_CASE("StMcHit base class default construction", "[StMcHit]") {
    StMcTpcHit h;

    SECTION("position is zero") {
        const StThreeVectorF& pos = h.position();
        CHECK(pos.x() == Catch::Approx(0.f));
        CHECK(pos.y() == Catch::Approx(0.f));
        CHECK(pos.z() == Catch::Approx(0.f));
    }

    SECTION("localMomentum is zero") {
        const StThreeVectorF& p = h.localMomentum();
        CHECK(p.x() == Catch::Approx(0.f));
        CHECK(p.y() == Catch::Approx(0.f));
        CHECK(p.z() == Catch::Approx(0.f));
    }

    SECTION("dE is zero") {
        CHECK(h.dE() == Catch::Approx(0.f));
    }

    SECTION("dS (step length) is zero") {
        CHECK(h.dS() == Catch::Approx(0.f));
    }

    SECTION("tof is zero") {
        CHECK(h.tof() == Catch::Approx(0.f));
    }

    SECTION("key (step number) is zero") {
        CHECK(h.key() == 0L);
    }

    SECTION("volumeId is zero") {
        CHECK(h.volumeId() == 0L);
    }

    SECTION("parentTrack is nullptr") {
        CHECK(h.parentTrack() == nullptr);
    }

    SECTION("kMatched flag is not set") {
        CHECK(h.TestBit(StMcHit::kMatched) == false);
    }
}

TEST_CASE("StMcHit base class setPosition / position round-trip", "[StMcHit]") {
    StMcTpcHit h;
    const StThreeVectorF kPos = vec3(12.3f, -4.5f, 220.1f);
    h.setPosition(kPos);

    SECTION("x component") {
        CHECK(h.position().x() == Catch::Approx(kPos.x()));
    }
    SECTION("y component") {
        CHECK(h.position().y() == Catch::Approx(kPos.y()));
    }
    SECTION("z component") {
        CHECK(h.position().z() == Catch::Approx(kPos.z()));
    }
}

TEST_CASE("StMcHit base class setLocalMomentum / localMomentum round-trip", "[StMcHit]") {
    StMcTpcHit h;
    const StThreeVectorF kMom = vec3(0.1f, 0.2f, 1.5f);
    h.setLocalMomentum(kMom);

    SECTION("px") {
        CHECK(h.localMomentum().x() == Catch::Approx(kMom.x()));
    }
    SECTION("py") {
        CHECK(h.localMomentum().y() == Catch::Approx(kMom.y()));
    }
    SECTION("pz") {
        CHECK(h.localMomentum().z() == Catch::Approx(kMom.z()));
    }
}

TEST_CASE("StMcHit base class dE/dx setters round-trip", "[StMcHit]") {
    StMcTpcHit h;

    SECTION("setdE / dE") {
        const float kDE = 3.5e-6f;
        h.setdE(kDE);
        CHECK(h.dE() == Catch::Approx(kDE));
    }

    SECTION("setdS / dS (step length)") {
        const float kDS = 0.42f;
        h.setdS(kDS);
        CHECK(h.dS() == Catch::Approx(kDS));
    }
}

TEST_CASE("StMcHit base class setKey / key round-trip", "[StMcHit]") {
    StMcTpcHit h;
    const long kStepNumber = 1042L;
    h.setKey(kStepNumber);

    SECTION("key returns stored step number") {
        CHECK(h.key() == kStepNumber);
    }
}

TEST_CASE("StMcHit base class setParentTrack / parentTrack round-trip", "[StMcHit]") {
    StMcTpcHit h;
    StMcTrack trk;
    h.setParentTrack(&trk);

    SECTION("parentTrack is no longer nullptr") {
        CHECK(h.parentTrack() != nullptr);
    }
    SECTION("parentTrack points to the assigned track") {
        CHECK(h.parentTrack() == &trk);
    }
}

TEST_CASE("StMcHit base class kMatched bit flag", "[StMcHit]") {
    StMcTpcHit h;

    SECTION("kMatched not set initially") {
        CHECK(h.TestBit(StMcHit::kMatched) == false);
    }

    SECTION("kMatched set after SetBit") {
        h.SetBit(StMcHit::kMatched);
        CHECK(h.TestBit(StMcHit::kMatched) == true);
    }

    SECTION("kMatched cleared after ResetBit") {
        h.SetBit(StMcHit::kMatched);
        REQUIRE(h.TestBit(StMcHit::kMatched) == true);
        h.ResetBit(StMcHit::kMatched);
        CHECK(h.TestBit(StMcHit::kMatched) == false);
    }

    SECTION("kMatched flag does not affect dE") {
        const float kDE = 1.23e-5f;
        h.setdE(kDE);
        h.SetBit(StMcHit::kMatched);
        CHECK(h.dE() == Catch::Approx(kDE));
    }
}

TEST_CASE("StMcHit base class tof and volumeId setters", "[StMcHit]") {
    StMcTpcHit h;

    SECTION("setTof / tof round-trip") {
        const float kTof = 12.7f;
        h.setTof(kTof);
        CHECK(h.tof() == Catch::Approx(kTof));
    }

    SECTION("setVolumeId / volumeId round-trip") {
        const long kVolId = 102405L;
        h.setVolumeId(kVolId);
        CHECK(h.volumeId() == kVolId);
    }
}

// ===========================================================================
// TEST_CASE: StMcVertex
// ===========================================================================

TEST_CASE("StMcVertex default construction", "[StMcVertex]") {
    StMcVertex v;

    SECTION("position is at origin") {
        CHECK(v.position().x() == Catch::Approx(0.f));
        CHECK(v.position().y() == Catch::Approx(0.f));
        CHECK(v.position().z() == Catch::Approx(0.f));
    }

    SECTION("numberOfDaughters is zero") {
        CHECK(v.numberOfDaughters() == 0U);
    }

    SECTION("parent is nullptr") {
        CHECK(v.parent() == nullptr);
    }

    SECTION("tof is zero") {
        CHECK(v.tof() == Catch::Approx(0.f));
    }

    SECTION("key is zero") {
        CHECK(v.key() == 0L);
    }

    SECTION("geantProcess is zero") {
        CHECK(v.geantProcess() == 0L);
    }

    SECTION("daughters collection is empty") {
        CHECK(v.daughters().empty());
    }
}

TEST_CASE("StMcVertex setPosition / position round-trip", "[StMcVertex]") {
    StMcVertex v;
    const StThreeVectorF kPos = vec3(-0.05f, 0.03f, 1.2f);
    v.setPosition(kPos);

    SECTION("x component") {
        CHECK(v.position().x() == Catch::Approx(kPos.x()));
    }
    SECTION("y component") {
        CHECK(v.position().y() == Catch::Approx(kPos.y()));
    }
    SECTION("z component") {
        CHECK(v.position().z() == Catch::Approx(kPos.z()));
    }
}

TEST_CASE("StMcVertex setTof / tof round-trip", "[StMcVertex]") {
    StMcVertex v;
    const float kTof = 5.5f;
    v.setTof(kTof);

    SECTION("tof returns stored value") {
        CHECK(v.tof() == Catch::Approx(kTof));
    }
}

TEST_CASE("StMcVertex setGeantProcess / geantProcess round-trip", "[StMcVertex]") {
    StMcVertex v;
    const int kProcess = 13; // GEANT decay process
    v.setGeantProcess(kProcess);

    SECTION("geantProcess returns stored value") {
        CHECK(v.geantProcess() == static_cast<long>(kProcess));
    }
}

TEST_CASE("StMcVertex setGeantVolume / geantVolume round-trip", "[StMcVertex]") {
    StMcVertex v;
    v.setGeantVolume("TPCE");

    SECTION("geantVolume returns stored string") {
        CHECK(v.geantVolume() == TString("TPCE"));
    }
}

TEST_CASE("StMcVertex addDaughter / numberOfDaughters", "[StMcVertex]") {
    StMcVertex v;
    StMcTrack t1, t2, t3;

    v.addDaughter(&t1);

    SECTION("one daughter after first add") {
        CHECK(v.numberOfDaughters() == 1U);
    }

    SECTION("two daughters after second add") {
        v.addDaughter(&t2);
        CHECK(v.numberOfDaughters() == 2U);
    }

    SECTION("three daughters after three adds") {
        v.addDaughter(&t2);
        v.addDaughter(&t3);
        CHECK(v.numberOfDaughters() == 3U);
    }
}

TEST_CASE("StMcVertex daughter accessor by index", "[StMcVertex]") {
    StMcVertex v;
    StMcTrack t1, t2;
    v.addDaughter(&t1);
    v.addDaughter(&t2);

    SECTION("daughter(0) returns first added track") {
        CHECK(v.daughter(0) == &t1);
    }
    SECTION("daughter(1) returns second added track") {
        CHECK(v.daughter(1) == &t2);
    }
    SECTION("daughter out-of-range returns null") {
        CHECK(v.daughter(99) == nullptr);
    }
}

TEST_CASE("StMcVertex start-vertex track count via daughters", "[StMcVertex]") {
    // Simulate a primary vertex with several tracks starting from it.
    StMcVertex startV;
    StMcTrack t1, t2, t3;
    startV.addDaughter(&t1);
    startV.addDaughter(&t2);
    startV.addDaughter(&t3);

    SECTION("numberOfDaughters reflects tracks starting at this vertex") {
        CHECK(startV.numberOfDaughters() == 3U);
    }
}

TEST_CASE("StMcVertex stop-vertex represented as parent of daughters", "[StMcVertex]") {
    // A stop vertex for track T is the vertex T stops at; T can be the parent
    // of that vertex's daughters.  We test parent() / setParent() here.
    StMcVertex stopV;
    StMcTrack parentTrack;
    stopV.setParent(&parentTrack);

    SECTION("parent is no longer nullptr") {
        CHECK(stopV.parent() != nullptr);
    }
    SECTION("parent points to the assigned track") {
        CHECK(stopV.parent() == &parentTrack);
    }
}

TEST_CASE("StMcVertex removeDaughter", "[StMcVertex]") {
    StMcVertex v;
    StMcTrack t1, t2;
    v.addDaughter(&t1);
    v.addDaughter(&t2);
    REQUIRE(v.numberOfDaughters() == 2U);

    v.removeDaughter(&t1);

    SECTION("numberOfDaughters decremented") {
        CHECK(v.numberOfDaughters() == 1U);
    }
    SECTION("remaining daughter is t2") {
        CHECK(v.daughter(0) == &t2);
    }
}

// ===========================================================================
// TEST_CASE: StMcTrack
// ===========================================================================

TEST_CASE("StMcTrack default construction", "[StMcTrack]") {
    StMcTrack t;

    SECTION("fourMomentum px is zero") {
        CHECK(t.fourMomentum().px() == Catch::Approx(0.f));
    }
    SECTION("fourMomentum py is zero") {
        CHECK(t.fourMomentum().py() == Catch::Approx(0.f));
    }
    SECTION("fourMomentum pz is zero") {
        CHECK(t.fourMomentum().pz() == Catch::Approx(0.f));
    }
    SECTION("fourMomentum energy is zero") {
        CHECK(t.fourMomentum().e() == Catch::Approx(0.f));
    }
    SECTION("pT is zero") {
        CHECK(t.pt() == Catch::Approx(0.f));
    }
    SECTION("pdgId is zero") {
        CHECK(t.pdgId() == 0L);
    }
    SECTION("geantId is zero") {
        CHECK(t.geantId() == 0L);
    }
    SECTION("key is zero") {
        CHECK(t.key() == 0L);
    }
    SECTION("parent is nullptr") {
        CHECK(t.parent() == nullptr);
    }
    SECTION("startVertex is nullptr") {
        CHECK(t.startVertex() == nullptr);
    }
    SECTION("stopVertex is nullptr") {
        CHECK(t.stopVertex() == nullptr);
    }
    SECTION("isShower is zero (false)") {
        CHECK(t.isShower() == 0);
    }
    SECTION("IsPrimary is false") {
        CHECK(t.IsPrimary() == false);
    }
}

TEST_CASE("StMcTrack setFourMomentum / fourMomentum round-trip", "[StMcTrack]") {
    StMcTrack t;
    // pion at 500 MeV/c: m_pi = 0.13957 GeV
    const float kPx    =  0.3f;
    const float kPy    = -0.2f;
    const float kPz    =  0.8f;
    const float kMass  =  0.13957f;
    const float kE     = std::sqrt(kPx*kPx + kPy*kPy + kPz*kPz + kMass*kMass);
    const StLorentzVectorF p4(kPx, kPy, kPz, kE);
    t.setFourMomentum(p4);

    SECTION("px round-trip") {
        CHECK(t.fourMomentum().px() == Catch::Approx(kPx));
    }
    SECTION("py round-trip") {
        CHECK(t.fourMomentum().py() == Catch::Approx(kPy));
    }
    SECTION("pz round-trip") {
        CHECK(t.fourMomentum().pz() == Catch::Approx(kPz));
    }
    SECTION("energy round-trip") {
        CHECK(t.fourMomentum().e() == Catch::Approx(kE));
    }
    SECTION("pT computed correctly") {
        const float expectedPt = std::sqrt(kPx*kPx + kPy*kPy);
        CHECK(t.pt() == Catch::Approx(expectedPt).epsilon(1e-5));
    }
    SECTION("momentum 3-vector x") {
        CHECK(t.momentum().x() == Catch::Approx(kPx));
    }
}

TEST_CASE("StMcTrack pdgId and geantId setters", "[StMcTrack]") {
    StMcTrack t;

    SECTION("setPdgId / pdgId round-trip (pion+)") {
        t.setPdgId(211L);
        CHECK(t.pdgId() == 211L);
    }
    SECTION("setPdgId / pdgId round-trip (electron)") {
        t.setPdgId(11L);
        CHECK(t.pdgId() == 11L);
    }
    SECTION("setGeantId / geantId round-trip") {
        t.setGeantId(8L); // GEANT pi+
        CHECK(t.geantId() == 8L);
    }
    SECTION("setKey / key round-trip") {
        t.setKey(42L);
        CHECK(t.key() == 42L);
    }
}

TEST_CASE("StMcTrack parent pointer", "[StMcTrack]") {
    StMcTrack parent;
    StMcTrack daughter;
    daughter.setParent(&parent);

    SECTION("parent is not nullptr after setParent") {
        CHECK(daughter.parent() != nullptr);
    }
    SECTION("parent points to the assigned track") {
        CHECK(daughter.parent() == &parent);
    }
    SECTION("primary flag unset by default") {
        CHECK(daughter.IsPrimary() == false);
    }
}

TEST_CASE("StMcTrack setPrimary flag", "[StMcTrack]") {
    StMcTrack t;
    t.setPrimary(kTRUE);

    SECTION("IsPrimary returns true after setPrimary(kTRUE)") {
        CHECK(t.IsPrimary() == true);
    }
    SECTION("IsPrimary returns false after setPrimary(kFALSE)") {
        t.setPrimary(kFALSE);
        CHECK(t.IsPrimary() == false);
    }
}

TEST_CASE("StMcTrack shower flag", "[StMcTrack]") {
    StMcTrack t;

    SECTION("isShower zero initially") {
        CHECK(t.isShower() == 0);
    }
    SECTION("setShower(1) stores non-zero") {
        t.setShower(1);
        CHECK(t.isShower() != 0);
    }
    SECTION("setShower(0) stores zero") {
        t.setShower(1);
        t.setShower(0);
        CHECK(t.isShower() == 0);
    }
}

TEST_CASE("StMcTrack start and stop vertex pointers", "[StMcTrack]") {
    StMcTrack t;
    StMcVertex sv, ev;
    t.setStartVertex(&sv);
    t.setStopVertex(&ev);

    SECTION("startVertex set correctly") {
        CHECK(t.startVertex() == &sv);
    }
    SECTION("stopVertex set correctly") {
        CHECK(t.stopVertex() == &ev);
    }
}

TEST_CASE("StMcTrack hit vector access — empty on fresh object", "[StMcTrack]") {
    const StMcTrack t;

    SECTION("tpcHits is empty") {
        CHECK(t.tpcHits().empty());
    }
    SECTION("svtHits is empty") {
        CHECK(t.svtHits().empty());
    }
    SECTION("ssdHits is empty") {
        CHECK(t.ssdHits().empty());
    }
    SECTION("ftpcHits is empty") {
        CHECK(t.ftpcHits().empty());
    }
    SECTION("richHits is empty") {
        CHECK(t.richHits().empty());
    }
    SECTION("ctbHits is empty") {
        CHECK(t.ctbHits().empty());
    }
    SECTION("bemcHits is empty") {
        CHECK(t.bemcHits().empty());
    }
    SECTION("tofHits is empty") {
        CHECK(t.tofHits().empty());
    }
    SECTION("mtdHits is empty") {
        CHECK(t.mtdHits().empty());
    }
    SECTION("pxlHits is empty") {
        CHECK(t.pxlHits().empty());
    }
    SECTION("istHits is empty") {
        CHECK(t.istHits().empty());
    }
    SECTION("fgtHits is empty") {
        CHECK(t.fgtHits().empty());
    }
    SECTION("etrHits is empty") {
        CHECK(t.etrHits().empty());
    }
    SECTION("eemcHits is empty") {
        CHECK(t.eemcHits().empty());
    }
    SECTION("eprsHits is empty") {
        CHECK(t.eprsHits().empty());
    }
    SECTION("bprsHits is empty") {
        CHECK(t.bprsHits().empty());
    }
    SECTION("bsmdeHits is empty") {
        CHECK(t.bsmdeHits().empty());
    }
    SECTION("bsmdpHits is empty") {
        CHECK(t.bsmdpHits().empty());
    }
    SECTION("intermediateVertices is empty") {
        CHECK(t.intermediateVertices().empty());
    }
}

TEST_CASE("StMcTrack addTpcHit increments tpcHits", "[StMcTrack]") {
    StMcTrack t;
    StMcTpcHit h1, h2;
    t.addTpcHit(&h1);
    t.addTpcHit(&h2);

    SECTION("tpcHits size is 2") {
        CHECK(t.tpcHits().size() == 2U);
    }
    SECTION("first hit pointer matches") {
        CHECK(t.tpcHits()[0] == &h1);
    }
    SECTION("second hit pointer matches") {
        CHECK(t.tpcHits()[1] == &h2);
    }
}

TEST_CASE("StMcTrack eventGenLabel setter", "[StMcTrack]") {
    StMcTrack t;
    const long kLabel = 7L;
    t.setEventGenLabel(kLabel);

    SECTION("eventGenLabel returns stored value") {
        CHECK(t.eventGenLabel() == kLabel);
    }
}

// ===========================================================================
// TEST_CASE: StMcEvent scalars
// ===========================================================================

TEST_CASE("StMcEvent default construction", "[StMcEvent]") {
    StMcEvent ev;

    SECTION("eventNumber is zero") {
        CHECK(ev.eventNumber() == 0UL);
    }
    SECTION("runNumber is zero") {
        CHECK(ev.runNumber() == 0UL);
    }
    SECTION("type is zero") {
        CHECK(ev.type() == 0UL);
    }
    SECTION("numberOfPrimaryTracks is zero") {
        CHECK(ev.numberOfPrimaryTracks() == 0UL);
    }
    SECTION("impactParameter is zero") {
        CHECK(ev.impactParameter() == Catch::Approx(0.f));
    }
    SECTION("phiReactionPlane is zero") {
        CHECK(ev.phiReactionPlane() == Catch::Approx(0.f));
    }
    SECTION("triggerTimeOffset is zero") {
        CHECK(ev.triggerTimeOffset() == Catch::Approx(0.f));
    }
    SECTION("nBinary is zero") {
        CHECK(ev.nBinary() == 0UL);
    }
    SECTION("nWoundedEast is zero") {
        CHECK(ev.nWoundedEast() == 0UL);
    }
    SECTION("nWoundedWest is zero") {
        CHECK(ev.nWoundedWest() == 0UL);
    }
    SECTION("nJets is zero") {
        CHECK(ev.nJets() == 0UL);
    }
    SECTION("zWest is zero") {
        CHECK(ev.zWest() == 0UL);
    }
    SECTION("zEast is zero") {
        CHECK(ev.zEast() == 0UL);
    }
    SECTION("primaryVertex is nullptr") {
        CHECK(ev.primaryVertex() == nullptr);
    }
    SECTION("vertices collection is empty") {
        CHECK(ev.vertices().empty());
    }
    SECTION("tracks collection is empty") {
        CHECK(ev.tracks().empty());
    }
}

TEST_CASE("StMcEvent hit collections are nullptr after default construction", "[StMcEvent]") {
    // The default ctor calls initToZero() only; makeColls() is not invoked.
    // All pointer-based hit collections are therefore null.
    StMcEvent ev;

    SECTION("tpcHitCollection is nullptr") {
        CHECK(ev.tpcHitCollection() == nullptr);
    }
    SECTION("svtHitCollection is nullptr") {
        CHECK(ev.svtHitCollection() == nullptr);
    }
    SECTION("ssdHitCollection is nullptr") {
        CHECK(ev.ssdHitCollection() == nullptr);
    }
    SECTION("ftpcHitCollection is nullptr") {
        CHECK(ev.ftpcHitCollection() == nullptr);
    }
    SECTION("richHitCollection is nullptr") {
        CHECK(ev.richHitCollection() == nullptr);
    }
    SECTION("ctbHitCollection is nullptr") {
        CHECK(ev.ctbHitCollection() == nullptr);
    }
    SECTION("tofHitCollection is nullptr") {
        CHECK(ev.tofHitCollection() == nullptr);
    }
    SECTION("btofHitCollection is nullptr") {
        CHECK(ev.btofHitCollection() == nullptr);
    }
    SECTION("mtdHitCollection is nullptr") {
        CHECK(ev.mtdHitCollection() == nullptr);
    }
    SECTION("pxlHitCollection is nullptr") {
        CHECK(ev.pxlHitCollection() == nullptr);
    }
    SECTION("istHitCollection is nullptr") {
        CHECK(ev.istHitCollection() == nullptr);
    }
    SECTION("fgtHitCollection is nullptr") {
        CHECK(ev.fgtHitCollection() == nullptr);
    }
    SECTION("etrHitCollection is nullptr") {
        CHECK(ev.etrHitCollection() == nullptr);
    }
}

TEST_CASE("StMcEvent setEventNumber / eventNumber round-trip", "[StMcEvent]") {
    StMcEvent ev;
    const unsigned long kEvNum = 100042UL;
    ev.setEventNumber(kEvNum);

    SECTION("eventNumber returns stored value") {
        CHECK(ev.eventNumber() == kEvNum);
    }
}

TEST_CASE("StMcEvent setRunNumber / runNumber round-trip", "[StMcEvent]") {
    StMcEvent ev;
    const unsigned long kRunNum = 22150001UL;
    ev.setRunNumber(kRunNum);

    SECTION("runNumber returns stored value") {
        CHECK(ev.runNumber() == kRunNum);
    }
}

TEST_CASE("StMcEvent setType / type round-trip", "[StMcEvent]") {
    StMcEvent ev;
    const unsigned long kType = 3UL;
    ev.setType(kType);

    SECTION("type returns stored value") {
        CHECK(ev.type() == kType);
    }
}

TEST_CASE("StMcEvent setNumberOfPrimaryTracks / numberOfPrimaryTracks round-trip", "[StMcEvent]") {
    StMcEvent ev;
    const unsigned long kNPrim = 57UL;
    ev.setNumberOfPrimaryTracks(kNPrim);

    SECTION("numberOfPrimaryTracks returns stored value") {
        CHECK(ev.numberOfPrimaryTracks() == kNPrim);
    }
}

TEST_CASE("StMcEvent setImpactParameter / impactParameter round-trip", "[StMcEvent]") {
    StMcEvent ev;
    const float kB = 7.3f;
    ev.setImpactParameter(kB);

    SECTION("impactParameter returns stored value") {
        CHECK(ev.impactParameter() == Catch::Approx(kB));
    }
}

TEST_CASE("StMcEvent setPhiReactionPlane / phiReactionPlane round-trip", "[StMcEvent]") {
    StMcEvent ev;
    const float kPhi = 1.5708f; // pi/2
    ev.setPhiReactionPlane(kPhi);

    SECTION("phiReactionPlane returns stored value") {
        CHECK(ev.phiReactionPlane() == Catch::Approx(kPhi));
    }
}

TEST_CASE("StMcEvent setTriggerTimeOffset / triggerTimeOffset round-trip", "[StMcEvent]") {
    StMcEvent ev;
    const float kOffset = -12.5f;
    ev.setTriggerTimeOffset(kOffset);

    SECTION("triggerTimeOffset returns stored value") {
        CHECK(ev.triggerTimeOffset() == Catch::Approx(kOffset));
    }
}

TEST_CASE("StMcEvent heavy-ion scalars round-trip", "[StMcEvent]") {
    StMcEvent ev;
    ev.setZWest(79UL);
    ev.setNWest(197UL);
    ev.setZEast(79UL);
    ev.setNEast(197UL);
    ev.setNBinary(800UL);
    ev.setNWoundedEast(185UL);
    ev.setNWoundedWest(183UL);
    ev.setNJets(4UL);

    SECTION("zWest") { CHECK(ev.zWest() == 79UL); }
    SECTION("nWest") { CHECK(ev.nWest() == 197UL); }
    SECTION("zEast") { CHECK(ev.zEast() == 79UL); }
    SECTION("nEast") { CHECK(ev.nEast() == 197UL); }
    SECTION("nBinary") { CHECK(ev.nBinary() == 800UL); }
    SECTION("nWoundedEast") { CHECK(ev.nWoundedEast() == 185UL); }
    SECTION("nWoundedWest") { CHECK(ev.nWoundedWest() == 183UL); }
    SECTION("nJets") { CHECK(ev.nJets() == 4UL); }
}

TEST_CASE("StMcEvent setPrimaryVertex / primaryVertex round-trip", "[StMcEvent]") {
    StMcEvent ev;
    StMcVertex pv;
    // Note: ownership is not transferred; the vertex lives on the stack.
    ev.setPrimaryVertex(&pv);

    SECTION("primaryVertex is not nullptr") {
        CHECK(ev.primaryVertex() != nullptr);
    }
    SECTION("primaryVertex points to the assigned vertex") {
        CHECK(ev.primaryVertex() == &pv);
    }
}

TEST_CASE("StMcEvent vertices and tracks containers", "[StMcEvent]") {
    StMcEvent ev;

    SECTION("vertices is initially empty") {
        CHECK(ev.vertices().empty());
    }
    SECTION("tracks is initially empty") {
        CHECK(ev.tracks().empty());
    }

    SECTION("vertices accepts push_back") {
        auto* v = new StMcVertex();
        ev.vertices().push_back(v);
        CHECK(ev.vertices().size() == 1U);
    }

    SECTION("tracks accepts push_back") {
        auto* t = new StMcTrack();
        ev.tracks().push_back(t);
        CHECK(ev.tracks().size() == 1U);
    }
}

TEST_CASE("StMcEvent eventGeneratorEventLabel round-trip", "[StMcEvent]") {
    StMcEvent ev;
    const unsigned long kLabel = 999UL;
    ev.setEventGeneratorEventLabel(kLabel);

    SECTION("eventGeneratorEventLabel returns stored value") {
        CHECK(ev.eventGeneratorEventLabel() == kLabel);
    }
}

TEST_CASE("StMcEvent equality operator", "[StMcEvent]") {
    StMcEvent a, b;
    a.setEventNumber(7UL);
    b.setEventNumber(7UL);

    SECTION("events with same number compare equal") {
        CHECK(a == b);
    }
    SECTION("events with different numbers compare not equal") {
        b.setEventNumber(8UL);
        CHECK(a != b);
    }
}
