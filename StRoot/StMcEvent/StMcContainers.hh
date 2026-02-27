/***************************************************************************
 *
 * $Id: StMcContainers.hh,v 2.17 2013/03/25 23:25:35 perev Exp $
 *
 * Author: Manuel Calderon de la Barca Sanchez, Oct 1999
 ***************************************************************************
 *
 * Description: Containers for StMcEvent objects
 *
 ***************************************************************************
 *
 * $Log: StMcContainers.hh,v $
 * Revision 2.17  2013/03/25 23:25:35  perev
 * Mustafa.Pxl corrs
 *
 * Revision 2.16  2012/03/22 00:32:17  perev
 * Etr hit added
 *
 * Revision 2.15  2011/10/11 01:09:39  perev
 * Mtd added
 *
 * Revision 2.14  2009/07/24 19:08:06  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.13  2007/10/16 19:49:13  fisyak
 * rename Hft => Pxl, remove Hpd, Igt and Fst
 *
 * Revision 2.12  2006/09/25 14:20:43  fisyak
 * Add Hpd Hits
 *
 * Revision 2.11  2005/09/28 21:30:14  fisyak
 * Persistent StMcEvent
 *
 * Revision 2.10  2005/07/07 18:20:48  calderon
 * Added support for IGT detector.
 *
 * Revision 2.9  2005/04/18 20:11:32  calderon
 * Addition of Fgt and Fst files.  Modified other files to accomodate changes.
 *
 * Revision 2.8  2004/09/14 05:00:29  calderon
 * Added support for Ist, Ssd and changes to Pixel, from "El Kai".
 *
 * Revision 2.7  2003/08/20 18:50:21  calderon
 * Addition of Tof classes and Pixel classes.  Modified track, event, and
 * container code to reflect this.
 * Fix bug in StMcVertex and in clearing of some hit collections.
 *
 * Revision 2.6  2003/02/19 03:16:05  calderon
 * Introduction of Ctb Hit Class and Ctb Hit Collection class, modified
 * StMcTrack, and StMcEvent accordingly.  Clearing of hits in StMcSvtWaferHitCollection.
 *
 * Revision 2.6  2003/02/18 00:00:00  gans
 * Introduction of the CTB classes.  Modified several classes
 * accordingly.
 *
 * $Log: StMcContainers.hh,v $
 * Revision 2.17  2013/03/25 23:25:35  perev
 * Mustafa.Pxl corrs
 *
 * Revision 2.16  2012/03/22 00:32:17  perev
 * Etr hit added
 *
 * Revision 2.15  2011/10/11 01:09:39  perev
 * Mtd added
 *
 * Revision 2.14  2009/07/24 19:08:06  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.13  2007/10/16 19:49:13  fisyak
 * rename Hft => Pxl, remove Hpd, Igt and Fst
 *
 * Revision 2.12  2006/09/25 14:20:43  fisyak
 * Add Hpd Hits
 *
 * Revision 2.11  2005/09/28 21:30:14  fisyak
 * Persistent StMcEvent
 *
 * Revision 2.10  2005/07/07 18:20:48  calderon
 * Added support for IGT detector.
 *
 * Revision 2.9  2005/04/18 20:11:32  calderon
 * Addition of Fgt and Fst files.  Modified other files to accomodate changes.
 *
 * Revision 2.8  2004/09/14 05:00:29  calderon
 * Added support for Ist, Ssd and changes to Pixel, from "El Kai".
 *
 * Revision 2.7  2003/08/20 18:50:21  calderon
 * Addition of Tof classes and Pixel classes.  Modified track, event, and
 * container code to reflect this.
 * Fix bug in StMcVertex and in clearing of some hit collections.
 *
 * Revision 2.6  2003/02/19 03:16:05  calderon
 * Introduction of Ctb Hit Class and Ctb Hit Collection class, modified
 * StMcTrack, and StMcEvent accordingly.  Clearing of hits in StMcSvtWaferHitCollection.
 *
 * Revision 2.5  2000/06/06 02:58:40  calderon
 * Introduction of Calorimeter classes.  Modified several classes
 * accordingly.
 *
 * Revision 2.4  2000/03/06 18:05:21  calderon
 * 1) Modified SVT Hits storage scheme from layer-ladder-wafer to
 * barrel-ladder-wafer.
 * 2) Added Rich Hit class and collection, and links to them in other
 * classes.
 *
 * Revision 2.3  2000/01/18 20:52:31  calderon
 * Works with CC5
 *
 * Revision 2.2  1999/12/14 07:04:49  calderon
 * Numbering scheme as per SVT request.
 *
 * Revision 2.1  1999/11/19 19:06:31  calderon
 * Recommit after redoing the files.
 *
 * Revision 2.0  1999/11/17 02:00:58  calderon
 * Completely revised for new StEvent
 *
 **************************************************************************/
/// @file StMcContainers.hh
/// @brief Typedef declarations for all StMcEvent vector containers and iterators.
#ifndef StMcContainers_hh
#define StMcContainers_hh

#include <vector>

#ifndef ST_NO_NAMESPACES
using std::vector;
#endif


class StMcHit;
class StMcCalorimeterHit;
class StMcVertex;
class StMcSvtHit;
class StMcSsdHit;
class StMcTpcHit;
class StMcFtpcHit;
class StMcRichHit;
class StMcCtbHit;
class StMcPxlHit;
class StMcIstHit;
class StMcFgtHit;
class StMcEtrHit;
class StMcTofHit;
class StMcBTofHit;
class StMcMtdHit;
class StMcTrack;
   
// Owners
typedef vector<StMcHit*>     StSPtrVecMcHit;              ///< Owning vector of StMcHit pointers
typedef vector<StMcVertex*>  StSPtrVecMcVertex;            ///< Owning vector of StMcVertex pointers
typedef vector<StMcCalorimeterHit*>     StSPtrVecMcCalorimeterHit;   ///< Owning vector of StMcCalorimeterHit pointers
typedef vector<StMcSvtHit*>  StSPtrVecMcSvtHit;            ///< Owning vector of StMcSvtHit pointers
typedef vector<StMcSsdHit*>  StSPtrVecMcSsdHit;            ///< Owning vector of StMcSsdHit pointers
typedef vector<StMcTpcHit*>  StSPtrVecMcTpcHit;            ///< Owning vector of StMcTpcHit pointers
typedef vector<StMcFtpcHit*> StSPtrVecMcFtpcHit;           ///< Owning vector of StMcFtpcHit pointers
typedef vector<StMcRichHit*> StSPtrVecMcRichHit;           ///< Owning vector of StMcRichHit pointers
typedef vector<StMcCtbHit*>  StSPtrVecMcCtbHit;            ///< Owning vector of StMcCtbHit pointers
typedef vector<StMcPxlHit*> StSPtrVecMcPxlHit;             ///< Owning vector of StMcPxlHit pointers
typedef vector<StMcIstHit*>  StSPtrVecMcIstHit;            ///< Owning vector of StMcIstHit pointers
typedef vector<StMcFgtHit*>  StSPtrVecMcFgtHit;            ///< Owning vector of StMcFgtHit pointers
typedef vector<StMcEtrHit*>  StSPtrVecMcEtrHit;            ///< Owning vector of StMcEtrHit pointers
typedef vector<StMcTofHit*>  StSPtrVecMcTofHit;            ///< Owning vector of StMcTofHit pointers
typedef vector<StMcBTofHit*>  StSPtrVecMcBTofHit;          ///< Owning vector of StMcBTofHit pointers
typedef vector<StMcMtdHit*>  StSPtrVecMcMtdHit;            ///< Owning vector of StMcMtdHit pointers
typedef vector<StMcTrack*>   StSPtrVecMcTrack;             ///< Owning vector of StMcTrack pointers
// Not owners
typedef vector<StMcVertex*>  StPtrVecMcVertex;              ///< Non-owning vector of StMcVertex pointers
typedef vector<StMcCalorimeterHit*>     StPtrVecMcCalorimeterHit;    ///< Non-owning vector of StMcCalorimeterHit pointers
typedef vector<StMcHit*>     StPtrVecMcHit;                ///< Non-owning vector of StMcHit pointers
typedef vector<StMcSvtHit*>  StPtrVecMcSvtHit;             ///< Non-owning vector of StMcSvtHit pointers
typedef vector<StMcSsdHit*>  StPtrVecMcSsdHit;             ///< Non-owning vector of StMcSsdHit pointers
typedef vector<StMcTpcHit*>  StPtrVecMcTpcHit;             ///< Non-owning vector of StMcTpcHit pointers
typedef vector<StMcFtpcHit*> StPtrVecMcFtpcHit;            ///< Non-owning vector of StMcFtpcHit pointers
typedef vector<StMcRichHit*> StPtrVecMcRichHit;            ///< Non-owning vector of StMcRichHit pointers
typedef vector<StMcCtbHit*>  StPtrVecMcCtbHit;             ///< Non-owning vector of StMcCtbHit pointers
typedef vector<StMcPxlHit*> StPtrVecMcPxlHit;              ///< Non-owning vector of StMcPxlHit pointers
typedef vector<StMcIstHit*>  StPtrVecMcIstHit;             ///< Non-owning vector of StMcIstHit pointers
typedef vector<StMcFgtHit*>  StPtrVecMcFgtHit;             ///< Non-owning vector of StMcFgtHit pointers
typedef vector<StMcEtrHit*>  StPtrVecMcEtrHit;             ///< Non-owning vector of StMcEtrHit pointers
typedef vector<StMcTofHit*>  StPtrVecMcTofHit;             ///< Non-owning vector of StMcTofHit pointers
typedef vector<StMcBTofHit*>  StPtrVecMcBTofHit;           ///< Non-owning vector of StMcBTofHit pointers
typedef vector<StMcMtdHit*>  StPtrVecMcMtdHit;             ///< Non-owning vector of StMcMtdHit pointers
typedef vector<StMcTrack*>   StPtrVecMcTrack;              ///< Non-owning vector of StMcTrack pointers
//Iterators
typedef StPtrVecMcVertex::iterator  StMcVertexIterator;             ///< Mutable iterator over StMcVertex pointers
typedef StPtrVecMcCalorimeterHit::iterator  StMcCalorimeterHitIterator;  ///< Mutable iterator over StMcCalorimeterHit pointers
typedef StPtrVecMcHit::iterator     StMcHitIterator;                ///< Mutable iterator over StMcHit pointers
typedef StPtrVecMcSvtHit::iterator  StMcSvtHitIterator;             ///< Mutable iterator over StMcSvtHit pointers
typedef StPtrVecMcSsdHit::iterator  StMcSsdHitIterator;             ///< Mutable iterator over StMcSsdHit pointers
typedef StPtrVecMcTpcHit::iterator  StMcTpcHitIterator;             ///< Mutable iterator over StMcTpcHit pointers
typedef StPtrVecMcFtpcHit::iterator StMcFtpcHitIterator;            ///< Mutable iterator over StMcFtpcHit pointers
typedef StPtrVecMcRichHit::iterator StMcRichHitIterator;            ///< Mutable iterator over StMcRichHit pointers
typedef StPtrVecMcCtbHit::iterator  StMcCtbHitIterator;             ///< Mutable iterator over StMcCtbHit pointers
typedef StPtrVecMcPxlHit::iterator StMcPxlHitIterator;              ///< Mutable iterator over StMcPxlHit pointers
typedef StPtrVecMcIstHit::iterator  StMcIstHitIterator;             ///< Mutable iterator over StMcIstHit pointers
typedef StPtrVecMcFgtHit::iterator  StMcFgtHitIterator;             ///< Mutable iterator over StMcFgtHit pointers
typedef StPtrVecMcEtrHit::iterator  StMcEtrHitIterator;             ///< Mutable iterator over StMcEtrHit pointers
typedef StPtrVecMcTofHit::iterator  StMcTofHitIterator;             ///< Mutable iterator over StMcTofHit pointers
typedef StPtrVecMcBTofHit::iterator  StMcBTofHitIterator;           ///< Mutable iterator over StMcBTofHit pointers
typedef StPtrVecMcMtdHit::iterator  StMcMtdHitIterator;             ///< Mutable iterator over StMcMtdHit pointers
typedef StPtrVecMcTrack::iterator   StMcTrackIterator;              ///< Mutable iterator over StMcTrack pointers
//Const Iterators
typedef StPtrVecMcVertex::const_iterator  StMcVertexConstIterator;           ///< Const iterator over StMcVertex pointers
typedef StPtrVecMcCalorimeterHit::const_iterator  StMcCalorimeterHitConstIterator;  ///< Const iterator over StMcCalorimeterHit pointers
typedef StPtrVecMcHit::const_iterator     StMcSvtConstIterator;              ///< Const iterator over StMcHit pointers (legacy name)
typedef StPtrVecMcSvtHit::const_iterator  StMcSvtHitConstIterator;           ///< Const iterator over StMcSvtHit pointers
typedef StPtrVecMcSsdHit::const_iterator  StMcSsdHitConstIterator;           ///< Const iterator over StMcSsdHit pointers
typedef StPtrVecMcTpcHit::const_iterator  StMcTpcHitConstIterator;           ///< Const iterator over StMcTpcHit pointers
typedef StPtrVecMcFtpcHit::const_iterator StMcFtpcHitConstIterator;          ///< Const iterator over StMcFtpcHit pointers
typedef StPtrVecMcRichHit::const_iterator StMcRichHitConstIterator;          ///< Const iterator over StMcRichHit pointers
typedef StPtrVecMcCtbHit::const_iterator  StMcCtbHitConstIterator;           ///< Const iterator over StMcCtbHit pointers
typedef StPtrVecMcPxlHit::const_iterator StMcPxlHitConstIterator;            ///< Const iterator over StMcPxlHit pointers
typedef StPtrVecMcIstHit::const_iterator  StMcIstHitConstIterator;           ///< Const iterator over StMcIstHit pointers
typedef StPtrVecMcFgtHit::const_iterator  StMcFgtHitConstIterator;           ///< Const iterator over StMcFgtHit pointers
typedef StPtrVecMcEtrHit::const_iterator  StMcEtrHitConstIterator;           ///< Const iterator over StMcEtrHit pointers
typedef StPtrVecMcTofHit::const_iterator  StMcTofHitConstIterator;           ///< Const iterator over StMcTofHit pointers
typedef StPtrVecMcBTofHit::const_iterator  StMcBTofHitConstIterator;         ///< Const iterator over StMcBTofHit pointers
typedef StPtrVecMcMtdHit::const_iterator  StMcMtdHitConstIterator;           ///< Const iterator over StMcMtdHit pointers
typedef StPtrVecMcTrack::const_iterator   StMcTrackConstIterator;            ///< Const iterator over StMcTrack pointers
//     ClassDef(StMcContainers,0)
#endif //StMcContainers
