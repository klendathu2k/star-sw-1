// tests/unit/StarRoot/test_TUnixTime.cxx
// Unit tests for TUnixTime: UNIX timestamp wrapper with GMT / local
// conversion.
//
// Uses a fixed reference timestamp:
//   2000-01-01 00:00:00 UTC = 946684800 seconds since epoch.
// All tests use the GMT interface to avoid local-timezone dependence.

#include "../catch_amalgamated.hpp"
#include "StarRoot/TUnixTime.h"
#include "TDatime.h"

// Reference: 2000-01-01 00:00:00 UTC
static const ULong_t kEpoch2000 = 946684800UL;

// ROOT-format: date = YYYYMMDD, time = HHMMSS
static const Int_t kDate2000 = 20000101;
static const Int_t kTime2000 = 0;

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime default construction stores zero", "[TUnixTime]") {
    TUnixTime t;
    CHECK(t.GetUTime() == 0UL);
}

TEST_CASE("TUnixTime construction from ULong_t", "[TUnixTime]") {
    TUnixTime t(kEpoch2000);
    CHECK(t.GetUTime() == kEpoch2000);
}

// ---------------------------------------------------------------------------
// Operator()
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime operator() returns stored time", "[TUnixTime]") {
    TUnixTime t(kEpoch2000);
    CHECK(t() == kEpoch2000);
}

// ---------------------------------------------------------------------------
// Assignment and increment
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime operator= sets new value", "[TUnixTime]") {
    TUnixTime t;
    t = kEpoch2000;
    CHECK(t.GetUTime() == kEpoch2000);
}

TEST_CASE("TUnixTime operator+= increments by seconds", "[TUnixTime]") {
    TUnixTime t(kEpoch2000);
    t += 3600;  // add one hour
    CHECK(t.GetUTime() == kEpoch2000 + 3600UL);
}

// ---------------------------------------------------------------------------
// SetGTime / GetGTime round-trip
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime SetGTime/GetGTime round-trip for 2000-01-01", "[TUnixTime]") {
    TUnixTime t;
    t.SetGTime(kDate2000, kTime2000);

    Int_t gotDate = 0, gotTime = 0;
    t.GetGTime(gotDate, gotTime);

    CHECK(gotDate == kDate2000);
    CHECK(gotTime == kTime2000);
}

TEST_CASE("TUnixTime SetGTime/GetGTime round-trip for 2010-06-15 12:30:45", "[TUnixTime]") {
    TUnixTime t;
    t.SetGTime(20100615, 123045);

    Int_t gotDate = 0, gotTime = 0;
    t.GetGTime(gotDate, gotTime);

    CHECK(gotDate == 20100615);
    CHECK(gotTime == 123045);
}

// ---------------------------------------------------------------------------
// Static Convert
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime::Convert produces same result as SetGTime for 2000-01-01", "[TUnixTime]") {
    TDatime dt(kDate2000, kTime2000);
    ULong_t converted = TUnixTime::Convert(dt, /*gmt=*/1);

    TUnixTime t;
    t.SetGTime(kDate2000, kTime2000);

    CHECK(converted == t.GetUTime());
}

TEST_CASE("TUnixTime::Convert round-trips through GetGTime", "[TUnixTime]") {
    TDatime dt(20050715, 80000);  // 2005-07-15 08:00:00
    ULong_t ut = TUnixTime::Convert(dt, /*gmt=*/1);
    TUnixTime t(ut);

    Int_t gotDate = 0, gotTime = 0;
    t.GetGTime(gotDate, gotTime);

    CHECK(gotDate == 20050715);
    CHECK(gotTime == 80000);
}

// ---------------------------------------------------------------------------
// SetUTime
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime SetUTime changes stored value", "[TUnixTime]") {
    TUnixTime t(0UL);
    t.SetUTime(kEpoch2000 + 7200UL);
    CHECK(t.GetUTime() == kEpoch2000 + 7200UL);
}

// ---------------------------------------------------------------------------
// GetGString — UTC human-readable string
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime GetGString returns non-empty UTC string", "[TUnixTime]") {
    TUnixTime t(kEpoch2000);
    TString s = t.GetGString();
    CHECK(s.Length() >= 24);  // asctime format is always 26 chars including '\n'
}

TEST_CASE("TUnixTime GetGString contains expected year and month for 2000-01-01", "[TUnixTime]") {
    TUnixTime t(kEpoch2000);
    TString s = t.GetGString();
    // asctime format: "Ddd Mmm DD HH:MM:SS YYYY\n"
    CHECK(s.Contains("2000"));
    CHECK(s.Contains("Jan"));
}

// ---------------------------------------------------------------------------
// GetLString — local-time human-readable string
// (exact date/time is timezone-dependent; test only structure/non-emptiness)
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime GetLString returns non-empty string", "[TUnixTime]") {
    TUnixTime t(kEpoch2000);
    TString s = t.GetLString();
    CHECK(s.Length() >= 24);
}

// ---------------------------------------------------------------------------
// SetLTime / GetLTime round-trip
// Use midday (12:00:00) to avoid DST midnight transitions.
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime SetLTime/GetLTime round-trip at midday", "[TUnixTime]") {
    TUnixTime t;
    t.SetLTime(20100615, 120000);  // 2010-06-15 12:00:00 local
    Int_t gotDate = 0, gotTime = 0;
    t.GetLTime(gotDate, gotTime);
    CHECK(gotDate == 20100615);
    CHECK(gotTime == 120000);
}

// ---------------------------------------------------------------------------
// SetLTime(const TDatime&) overload — should produce same epoch as integer overload
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime SetLTime TDatime overload matches integer overload", "[TUnixTime]") {
    TDatime dt(20100615, 120000);

    TUnixTime t1;
    t1.SetLTime(20100615, 120000);

    TUnixTime t2;
    t2.SetLTime(dt);

    CHECK(t2.GetUTime() == t1.GetUTime());
}

// ---------------------------------------------------------------------------
// SetGTime(const TDatime&) overload — should produce same epoch as integer overload
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime SetGTime TDatime overload matches integer overload", "[TUnixTime]") {
    TDatime dt(kDate2000, kTime2000);

    TUnixTime t1;
    t1.SetGTime(kDate2000, kTime2000);

    TUnixTime t2;
    t2.SetGTime(dt);

    CHECK(t2.GetUTime() == t1.GetUTime());
}

// ---------------------------------------------------------------------------
// TUnixTime(Int_t date, Int_t time, int gmt) constructor
// ---------------------------------------------------------------------------
TEST_CASE("TUnixTime (date,time,gmt) constructor matches SetGTime for gmt=1", "[TUnixTime]") {
    TUnixTime t1(kDate2000, kTime2000, /*gmt=*/1);

    TUnixTime t2;
    t2.SetGTime(kDate2000, kTime2000);

    CHECK(t1.GetUTime() == t2.GetUTime());
}

TEST_CASE("TUnixTime (date,time,gmt) constructor matches SetLTime for gmt=0", "[TUnixTime]") {
    TUnixTime t1(20100615, 120000, /*gmt=*/0);

    TUnixTime t2;
    t2.SetLTime(20100615, 120000);

    CHECK(t1.GetUTime() == t2.GetUTime());
}
