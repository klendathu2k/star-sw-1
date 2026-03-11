// StRoot/StiTests/VF/test_vertex_finding.cxx
// Unit tests for the STI vertex finder interface.
//
// StiVertexFinder is abstract.  We define a minimal concrete subclass
// MockVertexFinder that implements all pure virtuals, then test the
// full public interface.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "Sti/StiVertexFinder.h"
#include "Sti/StiHit.h"
#include "Sti/Base/StiFactory.h"

// ---------------------------------------------------------------------------
// Minimal concrete vertex finder for testing
// ---------------------------------------------------------------------------

class MockVertexFinder : public StiVertexFinder {
public:
    explicit MockVertexFinder()
        : StiVertexFinder("MockVertexFinder")
        , mFitCalled(false)
    {
        using HitFactory = StiFactory<StiHit, StiHit>;
        _hitFactory = HitFactory::myInstance();
    }

    virtual int fit(StEvent* /*event*/) override
    {
        mFitCalled = true;
        // Push a dummy vertex hit
        using HitFactory = StiFactory<StiHit, StiHit>;
        StiHit* v = static_cast<HitFactory*>(_hitFactory)->getInstance();
        v->set(0.f, 0.f, 0.f, 0.f);
        _result.push_back(v);
        return 0;
    }

    virtual StiHit* getVertex(int index) override
    {
        if (index < 0 || static_cast<size_t>(index) >= _result.size())
            return nullptr;
        return _result[index];
    }

    virtual int size() const override
    {
        return static_cast<int>(_result.size());
    }

    virtual void clear() override
    {
        _result.clear();
        mFitCalled = false;
    }

    bool mFitCalled;
};

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

TEST_CASE("MockVertexFinder — construction", "[StiVertexFinder][VF]") {
    MockVertexFinder vf;
    CHECK(vf.getName() == "MockVertexFinder");
    CHECK(vf.size()    == 0);
}

TEST_CASE("StiVertexFinder — getHitFactory returns non-null", "[StiVertexFinder][VF]") {
    MockVertexFinder vf;
    Factory<StiHit>* f = vf.getHitFactory();
    REQUIRE(f != nullptr);
}

TEST_CASE("StiVertexFinder — result() is empty before fit", "[StiVertexFinder][VF]") {
    MockVertexFinder vf;
    const std::vector<StiHit*>* r = vf.result();
    REQUIRE(r != nullptr);
    CHECK(r->empty());
}

TEST_CASE("StiVertexFinder — fit populates result", "[StiVertexFinder][VF]") {
    MockVertexFinder vf;
    int status = vf.fit(nullptr);
    CHECK(status == 0);
    CHECK(vf.mFitCalled == true);
    CHECK(vf.size() == 1);
}

TEST_CASE("StiVertexFinder — getVertex returns hit after fit", "[StiVertexFinder][VF]") {
    MockVertexFinder vf;
    vf.fit(nullptr);
    StiHit* v = vf.getVertex(0);
    REQUIRE(v != nullptr);
}

TEST_CASE("StiVertexFinder — getVertex(out-of-range) returns nullptr", "[StiVertexFinder][VF]") {
    MockVertexFinder vf;
    vf.fit(nullptr);
    CHECK(vf.getVertex(99) == nullptr);
}

TEST_CASE("StiVertexFinder — clear empties results", "[StiVertexFinder][VF]") {
    MockVertexFinder vf;
    vf.fit(nullptr);
    REQUIRE(vf.size() == 1);

    vf.clear();
    CHECK(vf.size() == 0);
    CHECK(vf.mFitCalled == false);
}
