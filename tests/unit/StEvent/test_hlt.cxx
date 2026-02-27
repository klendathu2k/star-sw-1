// tests/unit/StEvent/test_hlt.cxx
// Unit tests for the HLT (High Level Trigger) data classes stored in StEvent.
//
// Covers default construction and accessor/mutator round-trips for:
//   StHltBEmcTowerHit, StHltBTofHit, StHltVpdHit,
//   StHltTrack, StHltTrackNode,
//   StHltHighPt, StHltDiElectron, StHltHeavyFragment,
//   StHltEvent
//
// No TApplication or STAR runtime is required.

#include "../catch_amalgamated.hpp"

#include "StHltBEmcTowerHit.h"
#include "StHltBTofHit.h"
#include "StHltVpdHit.h"
#include "StHltTrack.h"
#include "StHltTrackNode.h"
#include "StHltHighPt.h"
#include "StHltDiElectron.h"
#include "StHltHeavyFragment.h"
#include "StHltEvent.h"

// ---------------------------------------------------------------------------
// StHltBEmcTowerHit
// ---------------------------------------------------------------------------

TEST_CASE("StHltBEmcTowerHit default construction", "[StHltBEmcTowerHit]") {
    const StHltBEmcTowerHit hit;

    SECTION("adc is zero")      { CHECK(hit.adc()    == 0); }
    SECTION("energy is zero")   { CHECK(hit.energy() == Catch::Approx(0.0f)); }
    SECTION("phi is zero")      { CHECK(hit.phi()    == Catch::Approx(0.0f)); }
    SECTION("eta is zero")      { CHECK(hit.eta()    == Catch::Approx(0.0f)); }
    SECTION("z is zero")        { CHECK(hit.z()      == Catch::Approx(0.0f)); }
    SECTION("softId is zero")   { CHECK(hit.softId() == Catch::Approx(0.0f)); }
    SECTION("daqId is zero")    { CHECK(hit.daqId()  == Catch::Approx(0.0f)); }
    SECTION("trackNode is null") { CHECK(hit.trackNode() == nullptr); }
}

TEST_CASE("StHltBEmcTowerHit setters and getters", "[StHltBEmcTowerHit]") {
    StHltBEmcTowerHit hit;

    SECTION("setAdc / adc") {
        hit.setAdc(512);
        CHECK(hit.adc() == 512);
    }
    SECTION("setEnergy / energy") {
        hit.setEnergy(2.5f);
        CHECK(hit.energy() == Catch::Approx(2.5f));
    }
    SECTION("setPhi / phi") {
        hit.setPhi(1.23f);
        CHECK(hit.phi() == Catch::Approx(1.23f));
    }
    SECTION("setEta / eta") {
        hit.setEta(-0.75f);
        CHECK(hit.eta() == Catch::Approx(-0.75f));
    }
    SECTION("setZ / z") {
        hit.setZ(45.0f);
        CHECK(hit.z() == Catch::Approx(45.0f));
    }
    SECTION("setSoftId / softId") {
        hit.setSoftId(1234);
        CHECK(hit.softId() == Catch::Approx(1234.0f));
    }
    SECTION("setDaqId / daqId") {
        hit.setDaqId(42);
        CHECK(hit.daqId() == Catch::Approx(42.0f));
    }
}

// ---------------------------------------------------------------------------
// StHltBTofHit
// ---------------------------------------------------------------------------

TEST_CASE("StHltBTofHit default construction", "[StHltBTofHit]") {
    const StHltBTofHit hit;

    SECTION("trayId is zero")       { CHECK(hit.trayId()      == 0); }
    SECTION("channel is zero")      { CHECK(hit.channel()     == 0); }
    SECTION("tdc is zero")          { CHECK(hit.tdc()         == Catch::Approx(0.0f)); }
    SECTION("tot is zero")          { CHECK(hit.tot()         == Catch::Approx(0.0f)); }
    SECTION("tof is zero")          { CHECK(hit.tof()         == Catch::Approx(0.0f)); }
    SECTION("triggerTime is zero")  { CHECK(hit.triggerTime() == Catch::Approx(0.0f)); }
    SECTION("module derived from channel 0") { CHECK(hit.module() == 0); }
    SECTION("cell derived from channel 0")   { CHECK(hit.cell()   == 0); }
    SECTION("trackNode is null")    { CHECK(hit.trackNode()   == nullptr); }
}

TEST_CASE("StHltBTofHit channel encodes module and cell", "[StHltBTofHit]") {
    StHltBTofHit hit;

    // channel = module * 6 + cell
    SECTION("module 3 cell 4 => channel 22") {
        hit.setChannel(22);   // 3*6 + 4
        CHECK(hit.module() == 3);
        CHECK(hit.cell()   == 4);
    }
    SECTION("module 0 cell 5 => channel 5") {
        hit.setChannel(5);
        CHECK(hit.module() == 0);
        CHECK(hit.cell()   == 5);
    }
}

TEST_CASE("StHltBTofHit setters and getters", "[StHltBTofHit]") {
    StHltBTofHit hit;

    SECTION("setTrayId / trayId") {
        hit.setTrayId(15);
        CHECK(hit.trayId() == 15);
    }
    SECTION("setTdc / tdc") {
        hit.setTdc(12.5f);
        CHECK(hit.tdc() == Catch::Approx(12.5f));
    }
    SECTION("setTot / tot") {
        hit.setTot(3.1f);
        CHECK(hit.tot() == Catch::Approx(3.1f));
    }
    SECTION("setTof / tof") {
        hit.setTof(8.4f);
        CHECK(hit.tof() == Catch::Approx(8.4f));
    }
    SECTION("setTriggerTime / triggerTime") {
        hit.setTriggerTime(100.0f);
        CHECK(hit.triggerTime() == Catch::Approx(100.0f));
    }
}

// ---------------------------------------------------------------------------
// StHltVpdHit
// ---------------------------------------------------------------------------

TEST_CASE("StHltVpdHit default construction", "[StHltVpdHit]") {
    const StHltVpdHit hit;

    SECTION("direction is east (0)") { CHECK(hit.direction() == east); }
    SECTION("channel is zero")       { CHECK(hit.channel()   == 0); }
    SECTION("tdc is zero")           { CHECK(hit.tdc()       == Catch::Approx(0.0f)); }
    SECTION("tot is zero")           { CHECK(hit.tot()       == Catch::Approx(0.0f)); }
    SECTION("tof is zero")           { CHECK(hit.tof()       == Catch::Approx(0.0f)); }
    SECTION("triggerTime is zero")   { CHECK(hit.triggerTime() == Catch::Approx(0.0f)); }
    SECTION("module derived from channel 0") { CHECK(hit.module() == 0); }
    SECTION("cell derived from channel 0")   { CHECK(hit.cell()   == 0); }
}

TEST_CASE("StHltVpdHit channel encodes module and cell", "[StHltVpdHit]") {
    StHltVpdHit hit;

    SECTION("channel 13 => module 2, cell 1") {
        hit.setChannel(13);   // 2*6 + 1
        CHECK(hit.module() == 2);
        CHECK(hit.cell()   == 1);
    }
}

TEST_CASE("StHltVpdHit setters and getters", "[StHltVpdHit]") {
    StHltVpdHit hit;

    SECTION("setDirection / direction") {
        hit.setDirection(west);
        CHECK(hit.direction() == west);
    }
    SECTION("setChannel / channel") {
        hit.setChannel(7);
        CHECK(hit.channel() == 7);
    }
    SECTION("setTdc / tdc") {
        hit.setTdc(55.0f);
        CHECK(hit.tdc() == Catch::Approx(55.0f));
    }
    SECTION("setTot / tot") {
        hit.setTot(2.0f);
        CHECK(hit.tot() == Catch::Approx(2.0f));
    }
    SECTION("setTof / tof") {
        hit.setTof(9.9f);
        CHECK(hit.tof() == Catch::Approx(9.9f));
    }
    SECTION("setTriggerTime / triggerTime") {
        hit.setTriggerTime(200.0f);
        CHECK(hit.triggerTime() == Catch::Approx(200.0f));
    }
}

// ---------------------------------------------------------------------------
// StHltTrack
// ---------------------------------------------------------------------------

TEST_CASE("StHltTrack default construction", "[StHltTrack]") {
    const StHltTrack trk;

    SECTION("id is zero")          { CHECK(trk.id()           == 0); }
    SECTION("charge q is zero")    { CHECK(trk.q()            == 0); }
    SECTION("chi2[0] is zero")     { CHECK(trk.chi2(0)        == Catch::Approx(0.0f)); }
    SECTION("chi2[1] is zero")     { CHECK(trk.chi2(1)        == Catch::Approx(0.0f)); }
    SECTION("chi2 out-of-range")   { CHECK(trk.chi2(2)        == Catch::Approx(0.0f)); }
    SECTION("dedx is zero")        { CHECK(trk.dedx()         == Catch::Approx(0.0f)); }
    SECTION("pt is zero")          { CHECK(trk.pt()           == Catch::Approx(0.0f)); }
    SECTION("phi0 is zero")        { CHECK(trk.phi0()         == Catch::Approx(0.0f)); }
    SECTION("psi is zero")         { CHECK(trk.psi()          == Catch::Approx(0.0f)); }
    SECTION("r0 is zero")          { CHECK(trk.r0()           == Catch::Approx(0.0f)); }
    SECTION("tanl is zero")        { CHECK(trk.tanl()         == Catch::Approx(0.0f)); }
    SECTION("z0 is zero")          { CHECK(trk.z0()           == Catch::Approx(0.0f)); }
    SECTION("length is zero")      { CHECK(trk.length()       == Catch::Approx(0.0f)); }
    SECTION("nHits is zero")       { CHECK(trk.nHits()        == 0u); }
    SECTION("ndedx is zero")       { CHECK(trk.ndedx()        == 0u); }
    SECTION("innerMostRow is zero") { CHECK(trk.innerMostRow() == 0); }
    SECTION("outerMostRow is zero") { CHECK(trk.outerMostRow() == 0); }
    SECTION("dpt is zero")         { CHECK(trk.dpt()          == Catch::Approx(0.0f)); }
    SECTION("dpsi is zero")        { CHECK(trk.dpsi()         == Catch::Approx(0.0f)); }
    SECTION("dz0 is zero")         { CHECK(trk.dz0()          == Catch::Approx(0.0f)); }
    SECTION("dtanl is zero")       { CHECK(trk.dtanl()        == Catch::Approx(0.0f)); }
    SECTION("trackNode is null")   { CHECK(trk.trackNode()    == nullptr); }
}

TEST_CASE("StHltTrack setters and getters", "[StHltTrack]") {
    StHltTrack trk;

    SECTION("setId / id") {
        trk.setId(99);
        CHECK(trk.id() == 99);
    }
    SECTION("setQ / q") {
        trk.setQ(1);
        CHECK(trk.q() == 1);
    }
    SECTION("setPt / pt") {
        trk.setPt(3.5f);
        CHECK(trk.pt() == Catch::Approx(3.5f));
    }
    SECTION("setChi2(0) / chi2(0)") {
        trk.setChi2(0, 1.2f);
        CHECK(trk.chi2(0) == Catch::Approx(1.2f));
    }
    SECTION("setChi2(1) / chi2(1)") {
        trk.setChi2(1, 0.8f);
        CHECK(trk.chi2(1) == Catch::Approx(0.8f));
    }
    SECTION("setDedx / dedx") {
        trk.setDedx(4.2e-6f);
        CHECK(trk.dedx() == Catch::Approx(4.2e-6f));
    }
    SECTION("setPsi / psi") {
        trk.setPsi(0.5f);
        CHECK(trk.psi() == Catch::Approx(0.5f));
    }
    SECTION("setTanl / tanl") {
        trk.setTanl(0.3f);
        CHECK(trk.tanl() == Catch::Approx(0.3f));
    }
    SECTION("setZ0 / z0") {
        trk.setZ0(12.0f);
        CHECK(trk.z0() == Catch::Approx(12.0f));
    }
    SECTION("setLength / length") {
        trk.setLength(250.0f);
        CHECK(trk.length() == Catch::Approx(250.0f));
    }
}

// ---------------------------------------------------------------------------
// StHltTrackNode
// ---------------------------------------------------------------------------

TEST_CASE("StHltTrackNode default construction", "[StHltTrackNode]") {
    const StHltTrackNode node;

    SECTION("globalTrack is null")   { CHECK(node.globalTrack()   == nullptr); }
    SECTION("primaryTrack is null")  { CHECK(node.primaryTrack()  == nullptr); }
    SECTION("bTofHit is null")       { CHECK(node.bTofHit()       == nullptr); }
    SECTION("bEmcTowerHit is null")  { CHECK(node.bEmcTowerHit()  == nullptr); }

    SECTION("globalTrackSN is -1")   { CHECK(node.globalTrackSN()  == -1); }
    SECTION("primaryTrackSN is -1")  { CHECK(node.primaryTrackSN() == -1); }
    SECTION("tofHitSN is -1")        { CHECK(node.tofHitSN()       == -1); }
    SECTION("emcTowerSN is -1")      { CHECK(node.emcTowerSN()     == -1); }

    SECTION("bEmcMatchPhiDiff is zero") { CHECK(node.bEmcMatchPhiDiff() == Catch::Approx(0.0)); }
    SECTION("bEmcMatchZEdge is zero")   { CHECK(node.bEmcMatchZEdge()   == Catch::Approx(0.0)); }
    SECTION("bTofProjChannel is zero")  { CHECK(node.bTofProjChannel()  == Catch::Approx(0.0f)); }
    SECTION("bTofCellLocalY is zero")   { CHECK(node.bTofCellLocalY()   == Catch::Approx(0.0f)); }
    SECTION("bTofCellLocalZ is zero")   { CHECK(node.bTofCellLocalZ()   == Catch::Approx(0.0f)); }
    SECTION("bTofPathLength is zero")   { CHECK(node.bTofPathLength()   == Catch::Approx(0.0f)); }
    SECTION("beta is zero")             { CHECK(node.beta()             == Catch::Approx(0.0f)); }
    SECTION("tof is zero")              { CHECK(node.tof()              == Catch::Approx(0.0f)); }
}

TEST_CASE("StHltTrackNode setters and getters", "[StHltTrackNode]") {
    StHltTrackNode node;

    SECTION("setGlobalTrackSN / globalTrackSN") {
        node.setGlobalTrackSN(10);
        CHECK(node.globalTrackSN() == 10);
    }
    SECTION("setPrimaryTrackSN / primaryTrackSN") {
        node.setPrimaryTrackSN(20);
        CHECK(node.primaryTrackSN() == 20);
    }
    SECTION("setTofHitSN / tofHitSN") {
        node.setTofHitSN(5);
        CHECK(node.tofHitSN() == 5);
    }
    SECTION("setEmcTowerSN / emcTowerSN") {
        node.setEmcTowerSN(3);
        CHECK(node.emcTowerSN() == 3);
    }
    SECTION("setBEmcMatchPhiDiff / bEmcMatchPhiDiff") {
        node.setBEmcMatchPhiDiff(0.12);
        CHECK(node.bEmcMatchPhiDiff() == Catch::Approx(0.12));
    }
    SECTION("setBEmcMatchZEdge / bEmcMatchZEdge") {
        node.setBEmcMatchZEdge(2.5);
        CHECK(node.bEmcMatchZEdge() == Catch::Approx(2.5));
    }
    SECTION("setBTofPathLength / bTofPathLength") {
        node.setBTofPathLength(180.0f);
        CHECK(node.bTofPathLength() == Catch::Approx(180.0f));
    }
    SECTION("setBeta / beta") {
        node.setBeta(0.95f);
        CHECK(node.beta() == Catch::Approx(0.95f));
    }
    SECTION("setTof / tof") {
        node.setTof(14.3f);
        CHECK(node.tof() == Catch::Approx(14.3f));
    }
}

// ---------------------------------------------------------------------------
// StHltHighPt
// ---------------------------------------------------------------------------

TEST_CASE("StHltHighPt default construction", "[StHltHighPt]") {
    const StHltHighPt hp;

    SECTION("globalTrackSN is -1")  { CHECK(hp.globalTrackSN()  == -1); }
    SECTION("primaryTrackSN is -1") { CHECK(hp.primaryTrackSN() == -1); }
    SECTION("tofHitSN is -1")       { CHECK(hp.tofHitSN()       == -1); }
    SECTION("emcTowerSN is -1")     { CHECK(hp.emcTowerSN()     == -1); }

    SECTION("bEmcMatchPhiDiff is zero") { CHECK(hp.bEmcMatchPhiDiff() == Catch::Approx(0.0)); }
    SECTION("bEmcMatchZEdge is zero")   { CHECK(hp.bEmcMatchZEdge()   == Catch::Approx(0.0)); }
    SECTION("bTofProjChannel is zero")  { CHECK(hp.bTofProjChannel()  == Catch::Approx(0.0f)); }
    SECTION("bTofCellLocalY is zero")   { CHECK(hp.bTofCellLocalY()   == Catch::Approx(0.0f)); }
    SECTION("bTofCellLocalZ is zero")   { CHECK(hp.bTofCellLocalZ()   == Catch::Approx(0.0f)); }
    SECTION("bTofPathLength is zero")   { CHECK(hp.bTofPathLength()   == Catch::Approx(0.0f)); }
    SECTION("beta is zero")             { CHECK(hp.beta()             == Catch::Approx(0.0f)); }
    SECTION("tof is zero")              { CHECK(hp.tof()              == Catch::Approx(0.0f)); }

    // Embedded track and hit objects should be zero-initialised.
    SECTION("primaryTrack pt is zero")  { CHECK(hp.primaryTrack().pt()   == Catch::Approx(0.0f)); }
    SECTION("globalTrack pt is zero")   { CHECK(hp.globalTrack().pt()    == Catch::Approx(0.0f)); }
    SECTION("bTofHit tdc is zero")      { CHECK(hp.bTofHit().tdc()       == Catch::Approx(0.0f)); }
    SECTION("bEmcTowerHit energy zero") { CHECK(hp.bEmcTowerHit().energy() == Catch::Approx(0.0f)); }
}

TEST_CASE("StHltHighPt setters and getters", "[StHltHighPt]") {
    StHltHighPt hp;

    SECTION("setBeta / beta") {
        hp.setBeta(0.88f);
        CHECK(hp.beta() == Catch::Approx(0.88f));
    }
    SECTION("setTof / tof") {
        hp.setTof(18.0f);
        CHECK(hp.tof() == Catch::Approx(18.0f));
    }
    SECTION("setGlobalTrackSN / globalTrackSN") {
        hp.setGlobalTrackSN(7);
        CHECK(hp.globalTrackSN() == 7);
    }
    SECTION("setBEmcMatchPhiDiff / bEmcMatchPhiDiff") {
        hp.setBEmcMatchPhiDiff(0.05);
        CHECK(hp.bEmcMatchPhiDiff() == Catch::Approx(0.05));
    }
    SECTION("setBTofCellLocalY / bTofCellLocalY") {
        hp.setBTofCellLocalY(1.5f);
        CHECK(hp.bTofCellLocalY() == Catch::Approx(1.5f));
    }
    SECTION("setPrimaryTrack propagates pt") {
        StHltTrack trk;
        trk.setPt(5.0f);
        hp.setPrimaryTrack(trk);
        CHECK(hp.primaryTrack().pt() == Catch::Approx(5.0f));
    }
    SECTION("setBEmcTowerHit propagates energy") {
        StHltBEmcTowerHit emcHit;
        emcHit.setEnergy(3.2f);
        hp.setBEmcTowerHit(emcHit);
        CHECK(hp.bEmcTowerHit().energy() == Catch::Approx(3.2f));
    }
}

// ---------------------------------------------------------------------------
// StHltDiElectron
// ---------------------------------------------------------------------------

TEST_CASE("StHltDiElectron default construction", "[StHltDiElectron]") {
    const StHltDiElectron de;

    SECTION("invariantMass is zero") { CHECK(de.invariantMass() == Catch::Approx(0.0f)); }
    SECTION("pt is zero")            { CHECK(de.pt()            == Catch::Approx(0.0f)); }
    SECTION("psi is zero")           { CHECK(de.psi()           == Catch::Approx(0.0f)); }
    SECTION("tanl is zero")          { CHECK(de.tanl()          == Catch::Approx(0.0f)); }

    SECTION("daughter1 selectionBit is -1")    { CHECK(de.daughter1SelectionBit()    == -1); }
    SECTION("daughter1 globalTrackSN is -1")   { CHECK(de.daughter1globalTrackSN()   == -1); }
    SECTION("daughter1 primaryTrackSN is -1")  { CHECK(de.daughter1primaryTrackSN()  == -1); }
    SECTION("daughter1 tofHitSN is -1")        { CHECK(de.daughter1tofHitSN()        == -1); }
    SECTION("daughter1 emcTowerSN is -1")      { CHECK(de.daughter1emcTowerSN()      == -1); }

    SECTION("daughter2 selectionBit is -1")    { CHECK(de.daughter2SelectionBit()    == -1); }
    SECTION("daughter2 globalTrackSN is -1")   { CHECK(de.daughter2globalTrackSN()   == -1); }
    SECTION("daughter2 primaryTrackSN is -1")  { CHECK(de.daughter2primaryTrackSN()  == -1); }
    SECTION("daughter2 tofHitSN is -1")        { CHECK(de.daughter2tofHitSN()        == -1); }
    SECTION("daughter2 emcTowerSN is -1")      { CHECK(de.daughter2emcTowerSN()      == -1); }

    SECTION("daughter1 bEmcMatchPhiDiff is zero") {
        CHECK(de.daughter1bEmcMatchPhiDiff() == Catch::Approx(0.0));
    }
    SECTION("daughter2 bEmcMatchPhiDiff is zero") {
        CHECK(de.daughter2bEmcMatchPhiDiff() == Catch::Approx(0.0));
    }

    SECTION("daughter1 primary track pt is zero") {
        CHECK(de.daughter1primaryTrack().pt() == Catch::Approx(0.0f));
    }
    SECTION("daughter2 primary track pt is zero") {
        CHECK(de.daughter2primaryTrack().pt() == Catch::Approx(0.0f));
    }
}

TEST_CASE("StHltDiElectron setters and getters", "[StHltDiElectron]") {
    StHltDiElectron de;

    SECTION("setInvariantMass / invariantMass") {
        de.setInvariantMass(3.1f);
        CHECK(de.invariantMass() == Catch::Approx(3.1f));
    }
    SECTION("setPt / pt") {
        de.setPt(2.4f);
        CHECK(de.pt() == Catch::Approx(2.4f));
    }
    SECTION("setPsi / psi") {
        de.setPsi(1.0f);
        CHECK(de.psi() == Catch::Approx(1.0f));
    }
    SECTION("setTanl / tanl") {
        de.setTanl(0.6f);
        CHECK(de.tanl() == Catch::Approx(0.6f));
    }
    SECTION("setDaughter1SelectionBit / daughter1SelectionBit") {
        de.setDaughter1SelectionBit(3);
        CHECK(de.daughter1SelectionBit() == 3);
    }
    SECTION("setDaughter2SelectionBit / daughter2SelectionBit") {
        de.setDaughter2SelectionBit(5);
        CHECK(de.daughter2SelectionBit() == 5);
    }
    SECTION("setDaughter1PrimaryTrack propagates pt") {
        StHltTrack trk;
        trk.setPt(4.5f);
        de.setDaughter1PrimaryTrack(trk);
        CHECK(de.daughter1primaryTrack().pt() == Catch::Approx(4.5f));
    }
    SECTION("setDaughter2PrimaryTrack propagates pt") {
        StHltTrack trk;
        trk.setPt(3.2f);
        de.setDaughter2PrimaryTrack(trk);
        CHECK(de.daughter2primaryTrack().pt() == Catch::Approx(3.2f));
    }
    SECTION("setDaughter1Beta / daughter1beta") {
        de.setDaughter1Beta(0.97f);
        CHECK(de.daughter1beta() == Catch::Approx(0.97f));
    }
    SECTION("setDaughter2Beta / daughter2beta") {
        de.setDaughter2Beta(0.93f);
        CHECK(de.daughter2beta() == Catch::Approx(0.93f));
    }
    SECTION("setDaughter1BEmcMatchPhiDiff / daughter1bEmcMatchPhiDiff") {
        de.setDaughter1BEmcMatchPhiDiff(0.03);
        CHECK(de.daughter1bEmcMatchPhiDiff() == Catch::Approx(0.03));
    }
    SECTION("setDaughter2BEmcMatchZEdge / daughter2bEmcMatchZEdge") {
        de.setDaughter2BEmcMatchZEdge(1.8);
        CHECK(de.daughter2bEmcMatchZEdge() == Catch::Approx(1.8));
    }
}

// ---------------------------------------------------------------------------
// StHltHeavyFragment
// ---------------------------------------------------------------------------

TEST_CASE("StHltHeavyFragment default construction", "[StHltHeavyFragment]") {
    const StHltHeavyFragment hf;

    SECTION("globalTrackSN is -1")  { CHECK(hf.globalTrackSN()  == -1); }
    SECTION("primaryTrackSN is -1") { CHECK(hf.primaryTrackSN() == -1); }
    SECTION("tofHitSN is -1")       { CHECK(hf.tofHitSN()       == -1); }
    SECTION("emcTowerSN is -1")     { CHECK(hf.emcTowerSN()     == -1); }

    SECTION("bEmcMatchPhiDiff is zero") { CHECK(hf.bEmcMatchPhiDiff() == Catch::Approx(0.0)); }
    SECTION("bEmcMatchZEdge is zero")   { CHECK(hf.bEmcMatchZEdge()   == Catch::Approx(0.0)); }
    SECTION("bTofProjChannel is zero")  { CHECK(hf.bTofProjChannel()  == Catch::Approx(0.0f)); }
    SECTION("bTofCellLocalY is zero")   { CHECK(hf.bTofCellLocalY()   == Catch::Approx(0.0f)); }
    SECTION("bTofCellLocalZ is zero")   { CHECK(hf.bTofCellLocalZ()   == Catch::Approx(0.0f)); }
    SECTION("bTofPathLength is zero")   { CHECK(hf.bTofPathLength()   == Catch::Approx(0.0f)); }
    SECTION("beta is zero")             { CHECK(hf.beta()             == Catch::Approx(0.0f)); }
    SECTION("tof is zero")              { CHECK(hf.tof()              == Catch::Approx(0.0f)); }

    SECTION("primaryTrack pt is zero")    { CHECK(hf.primaryTrack().pt()      == Catch::Approx(0.0f)); }
    SECTION("globalTrack pt is zero")     { CHECK(hf.globalTrack().pt()       == Catch::Approx(0.0f)); }
    SECTION("bTofHit tdc is zero")        { CHECK(hf.bTofHit().tdc()          == Catch::Approx(0.0f)); }
    SECTION("bEmcTowerHit energy zero")   { CHECK(hf.bEmcTowerHit().energy()  == Catch::Approx(0.0f)); }
}

TEST_CASE("StHltHeavyFragment setters and getters", "[StHltHeavyFragment]") {
    StHltHeavyFragment hf;

    SECTION("setBeta / beta") {
        hf.setBeta(0.4f);
        CHECK(hf.beta() == Catch::Approx(0.4f));
    }
    SECTION("setTof / tof") {
        hf.setTof(22.0f);
        CHECK(hf.tof() == Catch::Approx(22.0f));
    }
    SECTION("setGlobalTrackSN / globalTrackSN") {
        hf.setGlobalTrackSN(11);
        CHECK(hf.globalTrackSN() == 11);
    }
    SECTION("setBTofPathLength / bTofPathLength") {
        hf.setBTofPathLength(210.0f);
        CHECK(hf.bTofPathLength() == Catch::Approx(210.0f));
    }
    SECTION("setGlobalTrack propagates dedx") {
        StHltTrack trk;
        trk.setDedx(5.0e-6f);
        hf.setGlobalTrack(trk);
        CHECK(hf.globalTrack().dedx() == Catch::Approx(5.0e-6f));
    }
    SECTION("setBTofHit propagates tdc") {
        StHltBTofHit tofHit;
        tofHit.setTdc(30.0f);
        hf.setBTofHit(tofHit);
        CHECK(hf.bTofHit().tdc() == Catch::Approx(30.0f));
    }
}

// ---------------------------------------------------------------------------
// StHltEvent
// ---------------------------------------------------------------------------

TEST_CASE("StHltEvent default construction", "[StHltEvent]") {
    const StHltEvent evt;

    SECTION("version is zero")              { CHECK(evt.version()              == 0u); }
    SECTION("triggerReasonBitOred is zero") { CHECK(evt.triggerReasonBitOred() == 0u); }
    SECTION("vpdVertexZ is zero")           { CHECK(evt.vpdVertexZ()           == Catch::Approx(0.0f)); }
    SECTION("t0 is zero")                   { CHECK(evt.t0()                   == Catch::Approx(0.0f)); }
    SECTION("innerSecGain is zero")         { CHECK(evt.innerSecGain()         == Catch::Approx(0.0f)); }
    SECTION("outerSecGain is zero")         { CHECK(evt.outerSecGain()         == Catch::Approx(0.0f)); }

    SECTION("globalTrack collection empty")    { CHECK(evt.globalTrack().size()    == 0u); }
    SECTION("primaryTrack collection empty")   { CHECK(evt.primaryTrack().size()   == 0u); }
    SECTION("trackNode collection empty")      { CHECK(evt.trackNode().size()      == 0u); }
    SECTION("bEmcTowerHits collection empty")  { CHECK(evt.bEmcTowerHits().size()  == 0u); }
    SECTION("bTofHit collection empty")        { CHECK(evt.bTofHit().size()        == 0u); }
    SECTION("vpdHit collection empty")         { CHECK(evt.vpdHit().size()         == 0u); }
    SECTION("highPt collection empty")         { CHECK(evt.highPt().size()         == 0u); }
    SECTION("heavyFragment collection empty")  { CHECK(evt.heavyFragment().size()  == 0u); }
    SECTION("diElectron collection empty")     { CHECK(evt.diElectron().size()     == 0u); }
    SECTION("triggerReason collection empty")  { CHECK(evt.triggerReason().size()  == 0u); }
}

TEST_CASE("StHltEvent scalar setters and getters", "[StHltEvent]") {
    StHltEvent evt;

    SECTION("setVersion / version") {
        evt.setVersion(2u);
        CHECK(evt.version() == 2u);
    }
    SECTION("setTriggerReasonBitOred / triggerReasonBitOred") {
        evt.setTriggerReasonBitOred(0xF);
        CHECK(evt.triggerReasonBitOred() == 0xFu);
    }
    SECTION("setVpdVertexZ / vpdVertexZ") {
        evt.setVpdVertexZ(5.5f);
        CHECK(evt.vpdVertexZ() == Catch::Approx(5.5f));
    }
    SECTION("setT0 / t0") {
        evt.setT0(1.1f);
        CHECK(evt.t0() == Catch::Approx(1.1f));
    }
    SECTION("setInnerSecGain / innerSecGain") {
        evt.setInnerSecGain(0.9f);
        CHECK(evt.innerSecGain() == Catch::Approx(0.9f));
    }
    SECTION("setOuterSecGain / outerSecGain") {
        evt.setOuterSecGain(1.05f);
        CHECK(evt.outerSecGain() == Catch::Approx(1.05f));
    }
}

TEST_CASE("StHltEvent addHighPt grows highPt collection", "[StHltEvent]") {
    StHltEvent evt;
    REQUIRE(evt.highPt().size() == 0u);

    const StHltHighPt hp1;
    evt.addHighPt(&hp1);
    CHECK(evt.highPt().size() == 1u);

    const StHltHighPt hp2;
    evt.addHighPt(&hp2);
    CHECK(evt.highPt().size() == 2u);
}

TEST_CASE("StHltEvent addDiElectron grows diElectron collection", "[StHltEvent]") {
    StHltEvent evt;
    REQUIRE(evt.diElectron().size() == 0u);

    const StHltDiElectron de;
    evt.addDiElectron(&de);
    CHECK(evt.diElectron().size() == 1u);
}

TEST_CASE("StHltEvent addHeavyFragment grows heavyFragment collection", "[StHltEvent]") {
    StHltEvent evt;
    REQUIRE(evt.heavyFragment().size() == 0u);

    const StHltHeavyFragment hf;
    evt.addHeavyFragment(&hf);
    CHECK(evt.heavyFragment().size() == 1u);
}

TEST_CASE("StHltEvent addTrackNode grows trackNode collection", "[StHltEvent]") {
    StHltEvent evt;
    REQUIRE(evt.trackNode().size() == 0u);

    const StHltTrackNode node;
    evt.addTrackNode(&node);
    CHECK(evt.trackNode().size() == 1u);
}

TEST_CASE("StHltEvent addVpdHit grows vpdHit collection", "[StHltEvent]") {
    StHltEvent evt;
    REQUIRE(evt.vpdHit().size() == 0u);

    const StHltVpdHit vhit;
    evt.addVpdHit(&vhit);
    CHECK(evt.vpdHit().size() == 1u);
}

TEST_CASE("StHltEvent addBEmcTowerHit grows bEmcTowerHits collection", "[StHltEvent]") {
    StHltEvent evt;
    REQUIRE(evt.bEmcTowerHits().size() == 0u);

    const StHltBEmcTowerHit emcHit;
    evt.addBEmcTowerHit(&emcHit);
    CHECK(evt.bEmcTowerHits().size() == 1u);
}

TEST_CASE("StHltEvent addBTofHit grows bTofHit collection", "[StHltEvent]") {
    StHltEvent evt;
    REQUIRE(evt.bTofHit().size() == 0u);

    const StHltBTofHit tofHit;
    evt.addBTofHit(&tofHit);
    CHECK(evt.bTofHit().size() == 1u);
}
