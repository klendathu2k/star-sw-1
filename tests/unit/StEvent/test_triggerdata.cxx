// tests/unit/StEvent/test_triggerdata.cxx
// Unit tests for the STAR trigger data classes:
//   StBbcTriggerDetector, StZdcTriggerDetector, StVpdTriggerDetector,
//   StCtbTriggerDetector, StTriggerDetectorCollection,
//   StTriggerData2004, StTriggerData2022.
// No TApplication or STAR runtime context is required.

#include "../catch_amalgamated.hpp"

#include <type_traits>

#include "StBbcTriggerDetector.h"
#include "StZdcTriggerDetector.h"
#include "StVpdTriggerDetector.h"
#include "StCtbTriggerDetector.h"
#include "StTriggerDetectorCollection.h"
#include "StTriggerData.h"
#include "StTriggerData2004.h"
#include "StTriggerData2022.h"

// ---------------------------------------------------------------------------
// StBbcTriggerDetector
// ---------------------------------------------------------------------------

TEST_CASE("StBbcTriggerDetector default construction", "[StBbcTriggerDetector]") {
    StBbcTriggerDetector bbc;

    SECTION("numberOfPMTs is 48") {
        CHECK(bbc.numberOfPMTs() == 48u);
    }

    SECTION("numberOfRegisters is 2") {
        CHECK(bbc.numberOfRegisters() == 2u);
    }

    SECTION("numberOfPedestalData is 128") {
        CHECK(bbc.numberOfPedestalData() == 128u);
    }

    SECTION("numberOfScalars is 32") {
        CHECK(bbc.numberOfScalars() == 32u);
    }

    SECTION("year is 2002 (default run year set by constructor)") {
        CHECK(bbc.year() == 2002u);
    }

    SECTION("adc values are zero on all channels") {
        for (unsigned int i = 0; i < bbc.numberOfPMTs(); ++i) {
            CHECK(bbc.adc(i) == 0);
        }
    }

    SECTION("tdc values are zero on all channels") {
        for (unsigned int i = 0; i < bbc.numberOfPMTs(); ++i) {
            CHECK(bbc.tdc(i) == 0);
        }
    }

    SECTION("onlineTimeDifference is zero") {
        CHECK(bbc.onlineTimeDifference() == 0);
    }
}

TEST_CASE("StBbcTriggerDetector setters and getters", "[StBbcTriggerDetector]") {
    StBbcTriggerDetector bbc;

    SECTION("setAdc / adc round-trip on channel 0") {
        bbc.setAdc(0, 512);
        CHECK(bbc.adc(0) == 512);
    }

    SECTION("setAdc / adc round-trip on last channel") {
        const unsigned int last = bbc.numberOfPMTs() - 1;
        bbc.setAdc(last, 1023);
        CHECK(bbc.adc(last) == 1023);
    }

    SECTION("setTdc / tdc round-trip on channel 0") {
        bbc.setTdc(0, 100);
        CHECK(bbc.tdc(0) == 100);
    }

    SECTION("setTdc / tdc round-trip on channel 10") {
        bbc.setTdc(10, 4095);
        CHECK(bbc.tdc(10) == 4095);
    }

    SECTION("setYear / year round-trip") {
        bbc.setYear(2023);
        CHECK(bbc.year() == 2023u);
    }

    SECTION("setOnlineTimeDifference / onlineTimeDifference round-trip") {
        bbc.setOnlineTimeDifference(300);
        CHECK(bbc.onlineTimeDifference() == 300);
    }

    SECTION("independent channels do not alias each other") {
        bbc.setAdc(3, 200);
        bbc.setAdc(7, 400);
        CHECK(bbc.adc(3) == 200);
        CHECK(bbc.adc(7) == 400);
        CHECK(bbc.adc(0) == 0);
    }
}

// ---------------------------------------------------------------------------
// StZdcTriggerDetector
// ---------------------------------------------------------------------------

TEST_CASE("StZdcTriggerDetector default construction", "[StZdcTriggerDetector]") {
    StZdcTriggerDetector zdc;

    SECTION("numberOfZdcWords is 16") {
        CHECK(zdc.numberOfZdcWords() == 16u);
    }

    SECTION("adc values are zero on all channels") {
        for (unsigned int i = 0; i < zdc.numberOfZdcWords(); ++i) {
            CHECK(zdc.adc(i) == Catch::Approx(0.0f));
        }
    }

    SECTION("tdc values are zero on all channels") {
        for (unsigned int i = 0; i < zdc.numberOfZdcWords(); ++i) {
            CHECK(zdc.tdc(i) == Catch::Approx(0.0f));
        }
    }

    SECTION("combined ADC sum is zero") {
        CHECK(zdc.adcSum() == Catch::Approx(0.0f));
    }

    SECTION("east and west ADC sums are zero") {
        CHECK(zdc.adcSum(east) == Catch::Approx(0.0f));
        CHECK(zdc.adcSum(west) == Catch::Approx(0.0f));
    }

    SECTION("vertexZ is zero") {
        CHECK(zdc.vertexZ() == Catch::Approx(0.0f));
    }
}

TEST_CASE("StZdcTriggerDetector setters and getters", "[StZdcTriggerDetector]") {
    StZdcTriggerDetector zdc;

    SECTION("setAdc / adc round-trip on channel 0") {
        zdc.setAdc(0, 123.5f);
        CHECK(zdc.adc(0) == Catch::Approx(123.5f));
    }

    SECTION("setAdc / adc round-trip on channel 5") {
        zdc.setAdc(5, 999.0f);
        CHECK(zdc.adc(5) == Catch::Approx(999.0f));
    }

    SECTION("setTdc / tdc round-trip") {
        zdc.setTdc(2, 77.25f);
        CHECK(zdc.tdc(2) == Catch::Approx(77.25f));
    }

    SECTION("setAdcSum / adcSum round-trip for east") {
        zdc.setAdcSum(east, 512.0f);
        CHECK(zdc.adcSum(east) == Catch::Approx(512.0f));
    }

    SECTION("setAdcSum / adcSum round-trip for west") {
        zdc.setAdcSum(west, 256.0f);
        CHECK(zdc.adcSum(west) == Catch::Approx(256.0f));
    }

    SECTION("setAdcSum (combined) / adcSum round-trip") {
        zdc.setAdcSum(1024.0f);
        CHECK(zdc.adcSum() == Catch::Approx(1024.0f));
    }

    SECTION("setVertexZ / vertexZ round-trip") {
        zdc.setVertexZ(-15.3f);
        CHECK(zdc.vertexZ() == Catch::Approx(-15.3f));
    }

    SECTION("independent channels do not alias each other") {
        zdc.setAdc(1, 10.0f);
        zdc.setAdc(3, 30.0f);
        CHECK(zdc.adc(1) == Catch::Approx(10.0f));
        CHECK(zdc.adc(3) == Catch::Approx(30.0f));
        CHECK(zdc.adc(0) == Catch::Approx(0.0f));
    }
}

// ---------------------------------------------------------------------------
// StVpdTriggerDetector
// ---------------------------------------------------------------------------

TEST_CASE("StVpdTriggerDetector default construction", "[StVpdTriggerDetector]") {
    StVpdTriggerDetector vpd;

    SECTION("numberOfVpdCounters is 16") {
        CHECK(vpd.numberOfVpdCounters() == 16u);
    }

    SECTION("ADC is zero for east pmt 0") {
        CHECK(vpd.ADC(east, 0) == 0);
    }

    SECTION("TDC is zero for east pmt 0") {
        CHECK(vpd.TDC(east, 0) == 0);
    }

    SECTION("EarliestTDC is zero for east") {
        CHECK(vpd.EarliestTDC(east) == 0);
    }

    SECTION("EarliestTDC is zero for west") {
        CHECK(vpd.EarliestTDC(west) == 0);
    }

    SECTION("TimeDifference is zero") {
        CHECK(vpd.TimeDifference() == 0);
    }
}

TEST_CASE("StVpdTriggerDetector setters and getters", "[StVpdTriggerDetector]") {
    StVpdTriggerDetector vpd;

    SECTION("setADC / ADC round-trip east pmt 0") {
        vpd.setADC(east, 0, 777);
        CHECK(vpd.ADC(east, 0) == 777);
    }

    SECTION("setADC / ADC round-trip west pmt 3") {
        vpd.setADC(west, 3, 1000);
        CHECK(vpd.ADC(west, 3) == 1000);
    }

    SECTION("setTDC / TDC round-trip east pmt 2") {
        vpd.setTDC(east, 2, 512);
        CHECK(vpd.TDC(east, 2) == 512);
    }

    SECTION("setTDC / TDC round-trip west pmt 7") {
        vpd.setTDC(west, 7, 4000);
        CHECK(vpd.TDC(west, 7) == 4000);
    }

    SECTION("setEarliestTDC / EarliestTDC round-trip for east") {
        vpd.setEarliestTDC(east, 2048);
        CHECK(vpd.EarliestTDC(east) == 2048);
    }

    SECTION("setEarliestTDC / EarliestTDC round-trip for west") {
        vpd.setEarliestTDC(west, 3000);
        CHECK(vpd.EarliestTDC(west) == 3000);
    }

    SECTION("setTimeDifference / TimeDifference round-trip") {
        vpd.setTimeDifference(50);
        CHECK(vpd.TimeDifference() == 50);
    }

    SECTION("east and west ADC channels are independent") {
        vpd.setADC(east, 1, 111);
        vpd.setADC(west, 1, 222);
        CHECK(vpd.ADC(east, 1) == 111);
        CHECK(vpd.ADC(west, 1) == 222);
    }
}

// ---------------------------------------------------------------------------
// StCtbTriggerDetector
// ---------------------------------------------------------------------------

TEST_CASE("StCtbTriggerDetector default construction", "[StCtbTriggerDetector]") {
    StCtbTriggerDetector ctb;

    SECTION("numberOfTrays is 120") {
        CHECK(ctb.numberOfTrays() == 120u);
    }

    SECTION("numberOfSlats is 2") {
        CHECK(ctb.numberOfSlats() == 2u);
    }

    SECTION("numberOfPreSamples is 0") {
        CHECK(ctb.numberOfPreSamples() == 0);
    }

    SECTION("numberOfPostSamples is 0") {
        CHECK(ctb.numberOfPostSamples() == 0);
    }

    SECTION("mips for tray 0, slat 0, crossing 0 is zero") {
        CHECK(ctb.mips(0, 0, 0) == Catch::Approx(0.0f));
    }

    SECTION("mips sum over all trays and slats is zero") {
        CHECK(ctb.mips(0) == Catch::Approx(0.0));
    }
}

TEST_CASE("StCtbTriggerDetector setters and getters", "[StCtbTriggerDetector]") {
    StCtbTriggerDetector ctb;

    SECTION("setMips / mips round-trip tray 0, slat 0, crossing 0") {
        ctb.setMips(0, 0, 0, 3.5f);
        CHECK(ctb.mips(0, 0, 0) == Catch::Approx(3.5f));
    }

    SECTION("setMips / mips round-trip on last tray") {
        const unsigned int lastTray = ctb.numberOfTrays() - 1;
        ctb.setMips(lastTray, 0, 0, 7.0f);
        CHECK(ctb.mips(lastTray, 0, 0) == Catch::Approx(7.0f));
    }

    SECTION("setMips on tray 1 slat 1 round-trips") {
        ctb.setMips(1, 1, 0, 2.25f);
        CHECK(ctb.mips(1, 1, 0) == Catch::Approx(2.25f));
    }

    SECTION("mips sum reflects set values") {
        ctb.setMips(0, 0, 0, 1.0f);
        ctb.setMips(1, 0, 0, 2.0f);
        ctb.setMips(2, 1, 0, 3.0f);
        CHECK(ctb.mips(0) == Catch::Approx(6.0));
    }

    SECTION("setNumberOfPreSamples / numberOfPreSamples round-trip") {
        ctb.setNumberOfPreSamples(3);
        CHECK(ctb.numberOfPreSamples() == 3);
    }

    SECTION("setNumberOfPostSamples / numberOfPostSamples round-trip") {
        ctb.setNumberOfPostSamples(2);
        CHECK(ctb.numberOfPostSamples() == 2);
    }

    SECTION("independent trays do not alias") {
        ctb.setMips(5, 0, 0, 10.0f);
        ctb.setMips(10, 1, 0, 20.0f);
        CHECK(ctb.mips(5,  0, 0) == Catch::Approx(10.0f));
        CHECK(ctb.mips(10, 1, 0) == Catch::Approx(20.0f));
        CHECK(ctb.mips(0,  0, 0) == Catch::Approx(0.0f));
    }
}

// ---------------------------------------------------------------------------
// StTriggerDetectorCollection
// ---------------------------------------------------------------------------

TEST_CASE("StTriggerDetectorCollection default construction", "[StTriggerDetectorCollection]") {
    StTriggerDetectorCollection col;

    SECTION("bbc() returns a reference (not null-dereferenced)") {
        // Verify the address is non-null and the object is accessible.
        const StBbcTriggerDetector& bbc = col.bbc();
        CHECK(bbc.numberOfPMTs() == 48u);
    }

    SECTION("zdc() returns a reference with correct word count") {
        const StZdcTriggerDetector& zdc = col.zdc();
        CHECK(zdc.numberOfZdcWords() == 16u);
    }

    SECTION("vpd() returns a reference with correct counter count") {
        const StVpdTriggerDetector& vpd = col.vpd();
        CHECK(vpd.numberOfVpdCounters() == 16u);
    }

    SECTION("ctb() returns a reference with correct tray count") {
        const StCtbTriggerDetector& ctb = col.ctb();
        CHECK(ctb.numberOfTrays() == 120u);
    }
}

TEST_CASE("StTriggerDetectorCollection mutable accessor round-trips", "[StTriggerDetectorCollection]") {
    StTriggerDetectorCollection col;

    SECTION("bbc() mutable accessor permits modification") {
        col.bbc().setAdc(0, 42);
        CHECK(col.bbc().adc(0) == 42);
    }

    SECTION("zdc() mutable accessor permits modification") {
        col.zdc().setAdc(0, 88.0f);
        CHECK(col.zdc().adc(0) == Catch::Approx(88.0f));
    }

    SECTION("vpd() mutable accessor permits modification") {
        col.vpd().setADC(east, 0, 55);
        CHECK(col.vpd().ADC(east, 0) == 55);
    }

    SECTION("setBbcTriggerDetector replaces bbc data") {
        StBbcTriggerDetector newBbc;
        newBbc.setAdc(5, 999);
        col.setBbcTriggerDetector(newBbc);
        CHECK(col.bbc().adc(5) == 999);
    }

    SECTION("setZdcTriggerDetector replaces zdc data") {
        StZdcTriggerDetector newZdc;
        newZdc.setAdc(3, 777.0f);
        col.setZdcTriggerDetector(newZdc);
        CHECK(col.zdc().adc(3) == Catch::Approx(777.0f));
    }
}

// ---------------------------------------------------------------------------
// StTriggerData hierarchy — type checks
// ---------------------------------------------------------------------------

TEST_CASE("StTriggerData2022 is derived from StTriggerData", "[StTriggerData2022]") {
    STATIC_REQUIRE(std::is_base_of<StTriggerData, StTriggerData2022>::value);
}

TEST_CASE("StTriggerData2004 is derived from StTriggerData", "[StTriggerData2004]") {
    STATIC_REQUIRE(std::is_base_of<StTriggerData, StTriggerData2004>::value);
}

// ---------------------------------------------------------------------------
// StTriggerData2022 — default construction
// ---------------------------------------------------------------------------

TEST_CASE("StTriggerData2022 default construction", "[StTriggerData2022]") {
    // The default constructor (no arguments) is defined; it sets mDebug=0
    // but does NOT assign mYear (base-class initialises mYear to 0).
    StTriggerData2022 td;

    SECTION("year() returns 0 after default construction") {
        // The parameterised constructors set mYear=2022; the default ctor
        // relies on StTriggerData::StTriggerData() which zeroes mYear.
        CHECK(td.year() == 0);
    }

    // NOTE: All other accessors (token(), version(), triggerWord(), etc.)
    // dereference the internal mData pointer, which is uninitialised after
    // the default constructor.  Those calls are therefore omitted here.
    // To test them a fully-populated TriggerDataBlk2022 buffer is required.
}

// ---------------------------------------------------------------------------
// StTriggerData2004 — default construction
// ---------------------------------------------------------------------------

TEST_CASE("StTriggerData2004 default construction", "[StTriggerData2004]") {
    // The default constructor sets mYear=2004 and mData=0 (null pointer).
    StTriggerData2004 td;

    SECTION("year() returns 2004 after default construction") {
        CHECK(td.year() == 2004);
    }

    // NOTE: All accessors that dereference mData (token(), version(),
    // triggerWord(), etc.) will crash with a null-pointer dereference when
    // mData==0.  Those calls are therefore omitted.
    // To exercise them a valid TrgDataType2004 buffer is required.
}
