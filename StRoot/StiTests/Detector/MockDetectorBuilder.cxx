// StRoot/StiTests/Detector/MockDetectorBuilder.cxx

#include "StiTests/Detector/MockDetectorBuilder.h"

#include <cstdio>
#include <cmath>

#include "Sti/StiCylindricalShape.h"
#include "Sti/StiPlacement.h"
#include "Sti/StiDetector.h"
#include "Sti/StiIsActiveFunctor.h"

static const double kPi = 3.14159265358979323846;

MockDetectorBuilder::MockDetectorBuilder()
    : StiDetectorBuilder("MockDetector", /*active=*/true)
{}

void MockDetectorBuilder::buildDetectors(StMaker& /*source*/)
{
    buildTpc();
    buildSilicon();
}

void MockDetectorBuilder::buildTpc()
{
    using namespace MockShapes;
    StiMaterial* gas = MockMaterials::p10Gas();
    StiMaterial* mat = MockMaterials::p10Gas();

    for (int row = 0; row < kNTpcRows; ++row) {
        StiCylindricalShape* shape = makeTpcRowShape(row);
        add(shape);

        float r = kTpcRmin + row * kTpcDr;

        for (int sec = 0; sec < kNTpcSectors; ++sec) {
            float phi = (sec + 0.5f) * kTpcOpeningAngle;

            // Normalise phi to (-π, π)
            while (phi >  static_cast<float>(kPi)) phi -= 2.f * static_cast<float>(kPi);
            while (phi < -static_cast<float>(kPi)) phi += 2.f * static_cast<float>(kPi);

            StiPlacement* placement = new StiPlacement(phi, r, 0.f, 0.f);
            placement->setLayerRadius(r);
            placement->setLayerAngle(phi);
            placement->setZcenter(0.f);
            placement->setRegion(StiPlacement::kMidRapidity);

            StiIsActiveFunctor* active = new StiIsActiveFunctor();

            StiDetector* det = new StiDetector();
            char name[128];
            snprintf(name, sizeof(name), "TPC/Row_%02d/Sec_%02d", row, sec);
            det->setName(name);
            det->setShape(shape);
            det->setPlacement(placement);
            det->setGas(gas);
            det->setMaterial(mat);
            det->setIsActive(active);

            add(row, sec, det);
        }
    }
}

void MockDetectorBuilder::buildSilicon()
{
    using namespace MockShapes;
    StiMaterial* gas = MockMaterials::air();
    StiMaterial* mat = MockMaterials::silicon();

    // TPC occupies rows 0..(kNTpcRows-1); silicon starts at kNTpcRows
    const int siRowOffset = kNTpcRows;

    for (int layer = 0; layer < kNSiLayers; ++layer) {
        StiCylindricalShape* shape = makeSiLayerShape(layer);
        add(shape);

        float r = kSiRadii[layer];

        // One sector covering full azimuth (openingAngle = 2π)
        StiPlacement* placement = new StiPlacement(0.f, r, 0.f, 0.f);
        placement->setLayerRadius(r);
        placement->setLayerAngle(0.f);
        placement->setZcenter(0.f);
        placement->setRegion(StiPlacement::kMidRapidity);

        StiIsActiveFunctor* active = new StiIsActiveFunctor();

        StiDetector* det = new StiDetector();
        char name[128];
        snprintf(name, sizeof(name), "Si/Layer_%d/Sec_00", layer);
        det->setName(name);
        det->setShape(shape);
        det->setPlacement(placement);
        det->setGas(gas);
        det->setMaterial(mat);
        det->setIsActive(active);

        add(siRowOffset + layer, 0, det);
    }
}
