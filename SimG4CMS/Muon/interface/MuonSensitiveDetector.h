#ifndef SimG4CMS_Muon_MuonSensitiveDetector_h
#define SimG4CMS_Muon_MuonSensitiveDetector_h

/** \class MuonSensitiveDetector
 *
 * implementation of SensitiveDetector for the muon detector;
 * a MuonSlaveSD handles the interfacing to the database;
 * numbering scheme are booked according
 * to the detector name
 * 
 * \author Arno Straessner, CERN <arno.straessner@cern.ch>
 *
 * Modification:
 * 19/05/03. P.Arce
 * Add SimTracks selection
 */

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimG4Core/Notification/interface/Observer.h"
#include "SimG4Core/Notification/interface/BeginOfEvent.h"
#include "SimG4Core/Notification/interface/EndOfEvent.h"
#include "SimG4Core/SensitiveDetector/interface/SensitiveTkDetector.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"

#include <string>

class MuonSlaveSD;
class MuonSimHitNumberingScheme;
class MuonFrameRotation;
class UpdatablePSimHit;
class MuonSubDetector;
class MuonG4Numbering;
class SimHitPrinter;
class TrackInformation;
class G4Track;
class G4ProcessTypeEnumerator;
class G4TrackToParticleID;
class SimTrackManager;

class MuonSensitiveDetector : 
public SensitiveTkDetector,
public Observer<const BeginOfEvent*>,
public Observer<const EndOfEvent*>
 {

 public:    
  MuonSensitiveDetector(const std::string&, const DDCompactView &,
			const SensitiveDetectorCatalog &, edm::ParameterSet const &,
			const SimTrackManager*);
  ~MuonSensitiveDetector() override;
  G4bool ProcessHits(G4Step *,G4TouchableHistory *) override;
  uint32_t setDetUnitId(const G4Step *) override;
  void EndOfEvent(G4HCofThisEvent*) override;

  void fillHits(edm::PSimHitContainer&, const std::string&) override;
  void clearHits() override;

  std::string type();

  const MuonSlaveSD* GetSlaveMuon() const {
    return slaveMuon; }
  
 private:
  void update(const BeginOfEvent *) override;
  void update(const ::EndOfEvent *) override;

  Local3DPoint toOrcaRef(Local3DPoint in ,G4Step *);
  Local3DPoint toOrcaUnits(const Local3DPoint&);
  Global3DPoint toOrcaUnits(const Global3DPoint&);

  TrackInformation* getOrCreateTrackInformation( const G4Track* theTrack );

 private:
  MuonSlaveSD* slaveMuon;
  MuonSimHitNumberingScheme* numbering;
  MuonSubDetector* detector;
  MuonFrameRotation* theRotation;
  MuonG4Numbering* g4numbering;

  void storeVolumeAndTrack(const G4Step *);
  bool newHit(G4Step *);
  void createHit(G4Step *);
  void updateHit(G4Step *);
  void saveHit();
  
  /**
   * Transform from local coordinates of a volume to local coordinates of a parent volume
   * one or more levels up the volume hierarchy: e.g. levelsUp = 1 for immediate parent. <BR>
   * This is done by moving from local_1 -> global -> local_2.
   */
  Local3DPoint InitialStepPositionVsParent(G4Step * currentStep, G4int levelsUp);
  Local3DPoint FinalStepPositionVsParent(G4Step * currentStep, G4int levelsUp);

  G4VPhysicalVolume * thePV;
  UpdatablePSimHit* theHit;
  uint32_t theDetUnitId; 
  unsigned int theTrackID;
 
  bool printHits;
  SimHitPrinter* thePrinter;
  Global3DPoint theGlobalEntry;

  //--- SimTracks cuts
  double STenergyPersistentCut;
  bool STallMuonsPersistent;

  G4ProcessTypeEnumerator* theG4ProcessTypeEnumerator;

  G4TrackToParticleID* myG4TrackToParticleID;
  const SimTrackManager* theManager;
};

#endif // MuonSensitiveDetector_h
