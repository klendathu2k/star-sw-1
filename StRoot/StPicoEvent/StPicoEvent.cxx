#include <algorithm>
#include <limits>

#include "StEvent/StBTofHeader.h"
#include "StEvent/StEventTypes.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"

#include "StPicoEvent/StPicoUtilities.h"
#include "StPicoEvent/StPicoEvent.h"

ClassImp(StPicoEvent)

//_________________
StPicoEvent::StPicoEvent():
  mRunId(0), mEventId(0), mFillId(0), mBField(0), mTime(0),
  mPrimaryVertex{ -999., -999., -999.}, mPrimaryVertexError{ -999., -999., -999},
  mRanking(-999), mNBEMCMatch(0), mNBTOFMatch(0),
  mTriggerIds{},
  mRefMultFtpcEast(0), mRefMultFtpcWest(0),
  mRefMultNeg(0), mRefMultPos(0),
  mRefMult2NegEast(0), mRefMult2PosEast(0), mRefMult2NegWest(0), mRefMult2PosWest(0),
  mRefMult3NegEast(0), mRefMult3PosEast(0), mRefMult3NegWest(0), mRefMult3PosWest(0),
  mRefMult4NegEast(0), mRefMult4PosEast(0), mRefMult4NegWest(0), mRefMult4PosWest(0),
  mRefMultHalfNegEast(0), mRefMultHalfPosEast(0), mRefMultHalfNegWest(0), mRefMultHalfPosWest(0),
  mGRefMult(0), mNumberOfGlobalTracks(0), mbTofTrayMultiplicity(0), mNHitsHFT{},
  mNVpdHitsEast(0), mNVpdHitsWest(0), mNTofT0(0), mVzVpd(-999.),
  mZDCx(0), mBBCx(0), mBackgroundRate(0), mBbcBlueBackgroundRate(0), mBbcYellowBackgroundRate(0),
  mBbcEastRate(0), mBbcWestRate(0), mZdcEastRate(0), mZdcWestRate(0),
  mZdcSumAdcEast(0), mZdcSumAdcWest(0),
  mZdcSmdEastHorizontal{}, mZdcSmdEastVertical{}, mZdcSmdWestHorizontal{}, mZdcSmdWestVertical{},
  mBbcAdcEast{}, mBbcAdcWest{},
  mHighTowerThreshold{}, mJetPatchThreshold{} {
    /* empty */
}

//_________________
StPicoEvent::StPicoEvent(StMuDst const& muDst) : StPicoEvent() {

  StMuEvent* ev = muDst.event() ;

  mRunId = ev->runNumber();
  mEventId = ev->eventNumber();
  mFillId = ev->runInfo().beamFillNumber(blue);
  mBField = ev->magneticField();

  mTime=ev->eventInfo().time();

  mPrimaryVertex = ev->primaryVertexPosition();
  mPrimaryVertexError = ev->primaryVertexErrors();

  if (StMuPrimaryVertex* pv = muDst.primaryVertex()) {
    mRanking = pv->ranking();
    mNBEMCMatch = pv->nBEMCMatch();
    mNBTOFMatch = pv->nBTOFMatch();
  }

  mTriggerIds = ev->triggerIdCollection().nominal().triggerIds();

  mRefMultFtpcEast = (UShort_t)(ev->refMultFtpcEast());
  mRefMultFtpcWest = (UShort_t)(ev->refMultFtpcWest());
  mRefMultNeg = (UShort_t)(ev->refMultNeg());
  mRefMultPos = (UShort_t)(ev->refMultPos());

  {
    using namespace StPicoUtilities;
    auto custom_refMult = StPicoUtilities::calculateRefMult(muDst);
    mRefMult2NegEast = custom_refMult[RefMult2NegEast];
    mRefMult2PosEast = custom_refMult[RefMult2PosEast];
    mRefMult2NegWest = custom_refMult[RefMult2NegWest];
    mRefMult2PosWest = custom_refMult[RefMult2PosWest];
    mRefMult3NegEast = custom_refMult[RefMult3NegEast];
    mRefMult3PosEast = custom_refMult[RefMult3PosEast];
    mRefMult3NegWest = custom_refMult[RefMult3NegWest];
    mRefMult3PosWest = custom_refMult[RefMult3PosWest];
    mRefMult4NegEast = custom_refMult[RefMult4NegEast];
    mRefMult4PosEast = custom_refMult[RefMult4PosEast];
    mRefMult4NegWest = custom_refMult[RefMult4NegWest];
    mRefMult4PosWest = custom_refMult[RefMult4PosWest];
    mRefMultHalfNegEast = custom_refMult[RefMultHalfNegEast];
    mRefMultHalfPosEast = custom_refMult[RefMultHalfPosEast];
    mRefMultHalfNegWest = custom_refMult[RefMultHalfNegWest];
    mRefMultHalfPosWest = custom_refMult[RefMultHalfPosWest];
  }

  mGRefMult = (UShort_t)ev->grefmult();
  mNumberOfGlobalTracks = muDst.numberOfGlobalTracks();
  mbTofTrayMultiplicity = ev->btofTrayMultiplicity() ;
  mNHitsHFT[0] = (UShort_t)ev->numberOfPxlInnerHits();
  mNHitsHFT[1] = (UShort_t)ev->numberOfPxlOuterHits();
  mNHitsHFT[2] = (UShort_t)ev->numberOfIstHits();
  mNHitsHFT[3] = (UShort_t)ev->numberOfSsdHits();

  if (StBTofHeader* header = muDst.btofHeader()) {
    mNVpdHitsEast = (UChar_t)(header->numberOfVpdHits(east));
    mNVpdHitsWest = (UChar_t)(header->numberOfVpdHits(west));
    mNTofT0 = (UShort_t)(header->nTzero());
    mVzVpd = header->vpdVz();
  }

  mZDCx = (UInt_t)ev->runInfo().zdcCoincidenceRate();
  mBBCx = (UInt_t)ev->runInfo().bbcCoincidenceRate();
  mBackgroundRate = ev->runInfo().backgroundRate();
  mBbcBlueBackgroundRate = ev->runInfo().bbcBlueBackgroundRate();
  mBbcYellowBackgroundRate = ev->runInfo().bbcYellowBackgroundRate();
  mBbcEastRate = ev->runInfo().bbcEastRate();
  mBbcWestRate = ev->runInfo().bbcWestRate();
  mZdcEastRate = ev->runInfo().zdcEastRate();
  mZdcWestRate = ev->runInfo().zdcWestRate();

  StZdcTriggerDetector& ZDC = ev->zdcTriggerDetector();
  mZdcSumAdcEast = (UShort_t)ZDC.adcSum(east);
  mZdcSumAdcWest = (UShort_t)ZDC.adcSum(west);
  for (int strip = 1; strip < 9; ++strip) {
    if (ZDC.zdcSmd(east, 1, strip))
      mZdcSmdEastHorizontal[strip - 1] = (UShort_t)ZDC.zdcSmd(east, 1, strip);
    if (ZDC.zdcSmd(east, 0, strip))
      mZdcSmdEastVertical[strip - 1] = (UShort_t)ZDC.zdcSmd(east, 0, strip);
    if (ZDC.zdcSmd(west, 1, strip))
      mZdcSmdWestHorizontal[strip - 1] = (UShort_t)ZDC.zdcSmd(west, 1, strip);
    if (ZDC.zdcSmd(west, 0, strip))
      mZdcSmdWestVertical[strip - 1] = (UShort_t)ZDC.zdcSmd(west, 0, strip);
  }

  StBbcTriggerDetector bbc = ev->bbcTriggerDetector() ;
  for (UInt_t i = 0; i < bbc.numberOfPMTs(); ++i) {
    UInt_t const eastWest = (i < 24) ? 0 : 1 ; // East:0-23, West:24-47
    UInt_t const pmtId    = i % 24 ;         // pmtId:0-23

    if (eastWest == 0) mBbcAdcEast[pmtId] = bbc.adc(i) ;
    else               mBbcAdcWest[pmtId] = bbc.adc(i) ;
  }
}

//_________________
StPicoEvent::StPicoEvent(const StPicoEvent &event) {
  mRunId = event.mRunId;
  mEventId = event.mEventId;
  mFillId = event.mFillId;
  mBField = event.mBField;
  mTime = event.mTime;
  mPrimaryVertex = event.mPrimaryVertex;
  mPrimaryVertexError = event.mPrimaryVertexError;
  mRanking = event.mRanking;
  mNBEMCMatch = event.mNBEMCMatch;
  mNBTOFMatch = event.mNBTOFMatch;

  mTriggerIds = event.mTriggerIds;

  mRefMultFtpcEast = event.mRefMultFtpcEast;
  mRefMultFtpcWest = event.mRefMultFtpcWest;
  mRefMultNeg = event.mRefMultNeg;
  mRefMultPos = event.mRefMultPos;
  mRefMult2NegEast = event.mRefMult2NegEast;
  mRefMult2PosEast = event.mRefMult2PosEast;
  mRefMult2NegWest = event.mRefMult2NegWest;
  mRefMult2PosWest = event.mRefMult2PosWest;
  mRefMult3NegEast = event.mRefMult3NegEast;
  mRefMult3PosEast = event.mRefMult3PosEast;
  mRefMult3NegWest = event.mRefMult3NegWest;
  mRefMult3PosWest = event.mRefMult3PosWest;
  mRefMult4NegEast = event.mRefMult4NegEast;
  mRefMult4PosEast = event.mRefMult4PosEast;
  mRefMult4NegWest = event.mRefMult4NegWest;
  mRefMult4PosWest = event.mRefMult4PosWest;
  mRefMultHalfNegEast = event.mRefMultHalfNegEast;
  mRefMultHalfPosEast = event.mRefMultHalfPosEast;
  mRefMultHalfNegWest = event.mRefMultHalfNegWest;
  mRefMultHalfPosWest = event.mRefMultHalfPosWest;

  mGRefMult = event.mGRefMult;
  mNumberOfGlobalTracks = event.mNumberOfGlobalTracks;
  mbTofTrayMultiplicity = event.mbTofTrayMultiplicity;
  for(int iIter=0; iIter<4; iIter++) {
    mNHitsHFT[iIter] = event.mNHitsHFT[iIter];
  }

  mNVpdHitsEast = event.mNVpdHitsEast;
  mNVpdHitsWest = event.mNVpdHitsWest;
  mNTofT0 = event.mNTofT0;
  mVzVpd = event.mVzVpd;

  mZDCx = event.mZDCx;
  mBBCx = event.mBBCx;
  mBackgroundRate = event.mBackgroundRate;
  mBbcBlueBackgroundRate = event.mBbcBlueBackgroundRate;
  mBbcYellowBackgroundRate = event.mBbcYellowBackgroundRate;
  mBbcEastRate = event.mBbcEastRate;
  mBbcWestRate = event.mBbcWestRate;
  mZdcEastRate = event.mZdcEastRate;
  mZdcWestRate = event.mZdcWestRate;

  mZdcSumAdcEast = event.mZdcSumAdcEast;
  mZdcSumAdcWest = event.mZdcSumAdcWest;
  for(int iIter=0; iIter<8; iIter++) {
    mZdcSmdEastHorizontal[iIter] = event.mZdcSmdEastHorizontal[iIter];
    mZdcSmdEastVertical[iIter] = event.mZdcSmdEastVertical[iIter];
    mZdcSmdWestHorizontal[iIter] = event.mZdcSmdWestHorizontal[iIter];
    mZdcSmdWestVertical[iIter] = event.mZdcSmdWestVertical[iIter];
  }

  for(int iIter=0; iIter<24; iIter++) {
    mBbcAdcEast[iIter] = event.mBbcAdcEast[iIter];
    mBbcAdcWest[iIter] = event.mBbcAdcWest[iIter];
  }

  for(int iIter=0; iIter<4; iIter++) {
    mHighTowerThreshold[iIter] = event.mHighTowerThreshold[iIter];
    mJetPatchThreshold[iIter] = event.mJetPatchThreshold[iIter];
  }
}

//_________________
StPicoEvent::~StPicoEvent() { 
  /* empty */
}

//_________________
int StPicoEvent::year() const {
  return mRunId / 1000000 - 1 + 2000;
}

//_________________
int StPicoEvent::day() const {
  return (mRunId % 1000000) / 1000;
}

//_________________
bool StPicoEvent::isTrigger(unsigned int id) const {
  return std::find(mTriggerIds.begin(), mTriggerIds.end(), id) != mTriggerIds.end();
}