// Author: Victor Perev   08/04/01

/*!
 * \file TUnixTime.h
 * \brief UNIX timestamp class with local/GMT conversion and ROOT TDatime interoperability.
 */

#ifndef ROOT_TUnixTime
#define ROOT_TUnixTime


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TUnixTime                                                            //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TString.h"
class TDatime;
struct tm;

/*!
 * \class TUnixTime
 * \brief UNIX timestamp with conversions between local and GMT, and ROOT TDatime format.
 * \details Stores a POSIX seconds-since-epoch value (UTC) in \c fUTime and provides
 *          conversion methods to/from STAR-style YYYYMMDD / HHMMSS integers, TDatime,
 *          and human-readable strings in both local and GMT time zones.
 */
class TUnixTime
{
public:
   /// Construct from a raw UNIX epoch value (default 0 = 1970-01-01 00:00:00 UTC).
   TUnixTime(ULong_t utime = 0);
   /// Construct from STAR-style date/time integers and a timezone flag (gmt=1 for UTC).
   TUnixTime(Int_t date,Int_t time,int gmt);
   /// Construct from a ROOT TDatime and a timezone flag.
   TUnixTime(const TDatime &tdt,int gmt);
   virtual ~TUnixTime(){;}
   ULong_t operator()() const		{ return fUTime;}
   TUnixTime &operator =(ULong_t ut) 	{ fUTime =ut ; return *this;}
   TUnixTime &operator+=(Int_t sec) 	{ fUTime+=sec; return *this;}
   ULong_t GetUTime()			{ return fUTime;}
   /// Return local-time date and time as STAR integers (YYYYMMDD, HHMMSS).
   void   GetLTime(Int_t &idate, Int_t &itime);
   /// Return UTC date and time as STAR integers (YYYYMMDD, HHMMSS).
   void   GetGTime(Int_t &idate, Int_t &itime);
   /// Return a human-readable local-time string.
   TString GetLString();
   /// Return a human-readable UTC string.
   TString GetGString();

   void SetUTime(ULong_t utime){ fUTime=utime;}
   /// Set the timestamp from local-time STAR integers (YYYYMMDD, HHMMSS).
   void SetLTime(Int_t idate, Int_t itime);
   /// Set the timestamp from UTC STAR integers (YYYYMMDD, HHMMSS).
   void SetGTime(Int_t idate, Int_t itime);
   /// Set the timestamp from a local-time TDatime.
   void SetLTime(const TDatime &loc);
   /// Set the timestamp from a UTC TDatime.
   void SetGTime(const TDatime &gmt);
   /// Convert a TDatime to a UNIX epoch value; \p gmt=1 if TDatime is UTC.
   static ULong_t Convert(const TDatime &dt,int gmt);
private:
   void SetGTime(const struct tm *gt);
   void SetLTime(const struct tm *gt);

//		Data members
   ULong_t fUTime; 
   
   ClassDef(TUnixTime,2)

};
#endif //ROOT_TUnixTime
