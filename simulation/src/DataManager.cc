#include "DataVisualisation.hh"
#include "DataManager.hh"

#include <TTree.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>
#include <G4ProcessType.hh>
DataManager* DataManager::_instance = nullptr;

DataManager * DataManager::getInstance()
{
	if( _instance == nullptr )
	{
		_instance = new DataManager();
	}
	return _instance;
}

DataManager::DataManager() : _dataVisualisation(DataVisualisation::getInstance()) {}

DataManager::~DataManager() {}

/**
 * Create output trees
 */
void DataManager::createTrees()
{
	// Steps
	this->_treeSteps = new TTree("Steps", "Steps");
	this->_treeSteps->Branch("eventNumber"        , &this->_stepEventNumber         );
	this->_treeSteps->Branch("energyDeposition"   , &this->_stepEnergyDeposition    );
	this->_treeSteps->Branch("positionX"          , &this->_stepPositionX           );
	this->_treeSteps->Branch("positionY"          , &this->_stepPositionY           );
	this->_treeSteps->Branch("positionZ"          , &this->_stepPositionZ           );
	this->_treeSteps->Branch("positionR"          , &this->_stepPositionR           );
	this->_treeSteps->Branch("totalEnergy"        , &this->_stepTotalEnergy         );
	this->_treeSteps->Branch("kineticEnergy"      , &this->_stepKineticEnergy       );
	this->_treeSteps->Branch("pid"                , &this->_stepPid                 );
	this->_treeSteps->Branch("time"               , &this->_stepTime                );
	this->_treeSteps->Branch("trackId"            , &this->_stepTrackId             );

	// Tracks
	this->_treeTracks = new TTree("Tracks", "Tracks");
	this->_treeTracks->Branch("positionX"          , &this->_trackPositionX         );
	this->_treeTracks->Branch("positionY"          , &this->_trackPositionY         );
	this->_treeTracks->Branch("positionZ"          , &this->_trackPositionZ         );
	this->_treeTracks->Branch("trackId"            , &this->_trackTrackId           );

	this->_treeTracks->Branch( "positionX"     , &this->_trackPositionX      );
	this->_treeTracks->Branch( "positionY"     , &this->_trackPositionY      );
	this->_treeTracks->Branch( "positionZ"     , &this->_trackPositionZ      );
	this->_treeTracks->Branch( "momentumX"     , &this->_trackMomentumX      );
	this->_treeTracks->Branch( "momentumY"     , &this->_trackMomentumY      );
	this->_treeTracks->Branch( "momentumZ"     , &this->_trackMomentumZ      );
	this->_treeTracks->Branch( "trackId"       , &this->_trackTrackId        );
	this->_treeTracks->Branch( "parentId"      , &this->_trackParentId       );
	this->_treeTracks->Branch( "pid"           , &this->_trackPid            );
	this->_treeTracks->Branch( "creatorProcess", &this->_trackCreatorProcess );
	this->_treeTracks->Branch( "totalEnergy"   , &this->_trackTotalEnergy    );
	this->_treeTracks->Branch( "kineticEnergy" , &this->_trackKineticEnergy  );
}

/**
 * Set output file name
 *
 * Recreate respective ROOT output file and open it
 *
 * @param filename
 */
void DataManager::setOutputFileName( const std::string& filename )
{
	this->_outputFileName = filename;
	this->_outputFile     = new TFile( this->_outputFileName.c_str(), "RECREATE" );
	std::cout << "Output file set to \"" << filename << "\"" << std::endl;
}

/**
 * Clear DataManager structure begin of each event
 */
void DataManager::clear()
{
	// Step Tree Entries
	this->_stepPositionX->clear();
	this->_stepPositionY->clear();
	this->_stepPositionZ->clear();
	this->_stepPositionR->clear();
	this->_stepTotalEnergy->clear();
	this->_stepTime->clear();
	this->_stepKineticEnergy->clear();
	this->_stepPid->clear();
	this->_stepEnergyDeposition->clear();
	this->_stepEventNumber->clear();
	this->_stepTrackId->clear();

	this->_trackPositionX->clear();
	this->_trackPositionY->clear();
	this->_trackPositionZ->clear();
	this->_trackMomentumX->clear();
	this->_trackMomentumY->clear();
	this->_trackMomentumZ->clear();
	this->_trackTrackId->clear();
	this->_trackParentId->clear();
	this->_trackPid->clear();
	this->_trackCreatorProcess->clear();
	this->_trackTotalEnergy->clear();
	this->_trackKineticEnergy->clear();
}

/**
 * Save data on EndOfEvent
 */
void DataManager::save()
{
	// Fill tree and increase event number
	this->_treeSteps->Fill();
	this->_treeTracks->Fill();
	this->_evNr++;
}
/**
 * Finish everything
 *
 * Handle final plot work and write tree and histograms to output file
 */
void DataManager::finish()
{
	// Save Tree
	this->_outputFile->cd();

	// Save Plots
	DataVisualisation::getInstance()->saveData();

	// Close File
	this->_outputFile->Close();
}

void DataManager::addStep(const G4Step* step)
{
	// Basics
	this->_stepEventNumber     ->push_back(this->_evNr);
	this->_stepTrackId         ->push_back(step->GetTrack()->GetTrackID());
	this->_stepPositionX       ->push_back( step->GetPostStepPoint()->GetPosition().getX()      / CLHEP::mm  );
	this->_stepPositionY       ->push_back( step->GetPostStepPoint()->GetPosition().getY()      / CLHEP::mm  );
	this->_stepPositionZ       ->push_back( step->GetPostStepPoint()->GetPosition().getZ()      / CLHEP::mm  );
	this->_stepPositionR       ->push_back( step->GetPostStepPoint()->GetPosition().getR()      / CLHEP::mm  );
	this->_stepTime            ->push_back( step->GetPostStepPoint()->GetGlobalTime()           / CLHEP::ns  );
	this->_stepTotalEnergy     ->push_back( step->GetPostStepPoint()->GetTotalEnergy()          / CLHEP::MeV );
	this->_stepKineticEnergy   ->push_back( step->GetPostStepPoint()->GetKineticEnergy()        / CLHEP::MeV );
	this->_stepEnergyDeposition->push_back( step->GetTotalEnergyDeposit()                       / CLHEP::MeV );
	this->_stepPid             ->push_back( step->GetTrack()->GetDefinition()->GetPDGEncoding()              );

	// Histograms
	this->_histogramStepXY->Fill( this->_stepPositionX->back() , this->_stepPositionY->back() );
}

void DataManager::addTrack( const G4Track* track)
{
	this->_trackPositionX     ->push_back( track->GetPosition().getX()                      );
	this->_trackPositionY     ->push_back( track->GetPosition().getY()                      );
	this->_trackPositionZ     ->push_back( track->GetPosition().getZ()                      );
	this->_trackMomentumX     ->push_back( track->GetMomentum().getX()                      );
	this->_trackMomentumY     ->push_back( track->GetMomentum().getY()                      );
	this->_trackMomentumZ     ->push_back( track->GetMomentum().getZ()                      );
	this->_trackTotalEnergy   ->push_back( track->GetTotalEnergy()                          );
	this->_trackKineticEnergy ->push_back( track->GetKineticEnergy()                        );
	this->_trackTrackId       ->push_back( track->GetTrackID()                              );
	this->_trackParentId      ->push_back( track->GetParentID()                             );
	this->_trackPid           ->push_back( track->GetParticleDefinition()->GetPDGEncoding() );
	if( track->GetCreatorProcess() == nullptr )
	{
		// not sure what happens here
		this->_trackCreatorProcess->push_back("undefined");
	}
	else
	{
		this->_trackCreatorProcess->push_back(track->GetCreatorProcess()->GetProcessName() );
	}
}

void DataManager::createHistograms()
{
	this->_histogramStepXY = (TH2D*)this->_dataVisualisation->createHistogram( new TH2D("histogramStepXY", "", 300, -300, 300, 300, -300, 300));
}

/**
 * Prepare tree, histograms and initial clean of vectors
 */
void DataManager::prepare()
{
	if( not this->_prepared )
	{
		this->_outputFile->cd();
		this->createTrees();
		this->createHistograms();
	}
	this->clear();
	this->_prepared = true;
}
