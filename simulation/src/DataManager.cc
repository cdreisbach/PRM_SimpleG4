#include "DataVisualisation.hh"
#include "DataManager.hh"

#include <TTree.h>
#include <TFile.h>
#include <TH1D.h>

#include <G4Step.hh>

DataManager* DataManager::_instance = nullptr;

DataManager * DataManager::getInstance()
{
	if( _instance == nullptr )
	{
		_instance = new DataManager();
	}
	return _instance;
}

DataManager::DataManager() {}

DataManager::~DataManager() {}

/**
 * Create output trees
 */
void DataManager::createTrees()
{
	this->_tree = new TTree("Events", "Events");
	this->_tree->Branch("eventNumber"        , &this->_eventNumber         );
	this->_tree->Branch("energyDeposition"   , &this->_energyDeposition    );
	this->_tree->Branch("positionX"          , &this->_positionX           );
	this->_tree->Branch("positionY"          , &this->_positionY           );
	this->_tree->Branch("positionZ"          , &this->_positionZ           );
	this->_tree->Branch("positionR"          , &this->_positionR           );
	this->_tree->Branch("totalEnergy"        , &this->_totalEnergy         );
	this->_tree->Branch("kineticEnergy"      , &this->_kineticEnergy       );
	this->_tree->Branch("pid"                , &this->_pid                 );
	this->_tree->Branch("time"               , &this->_time                );
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
	this->_positionX->clear();
	this->_positionY->clear();
	this->_positionZ->clear();
	this->_positionR->clear();
	this->_totalEnergy->clear();
	this->_time->clear();
	this->_kineticEnergy->clear();
	this->_pid->clear();
	this->_energyDeposition->clear();
	this->_eventNumber->clear();
}

/**
 * Save data on EndOfEvent
 */
void DataManager::save()
{
	// Fill tree and increase event number
	this->_tree->Fill();
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
	this->_tree->Write();

	// Save Plots
	DataVisualisation::getInstance()->saveData();

	// Close File
	this->_outputFile->Close();
}

void DataManager::addStep(const G4Step* step)
{
	// Basics
	this->_eventNumber     ->push_back(this->_evNr);
	this->_positionX       ->push_back( step->GetPostStepPoint()->GetPosition().getX()      / CLHEP::mm  );
	this->_positionY       ->push_back( step->GetPostStepPoint()->GetPosition().getY()      / CLHEP::mm  );
	this->_positionZ       ->push_back( step->GetPostStepPoint()->GetPosition().getZ()      / CLHEP::mm  );
	this->_positionR       ->push_back( step->GetPostStepPoint()->GetPosition().getR()      / CLHEP::mm  );
	this->_time            ->push_back( step->GetPostStepPoint()->GetGlobalTime()           / CLHEP::ns  );
	this->_totalEnergy     ->push_back( step->GetPostStepPoint()->GetTotalEnergy()          / CLHEP::MeV );
	this->_kineticEnergy   ->push_back( step->GetPostStepPoint()->GetKineticEnergy()        / CLHEP::MeV );
	this->_energyDeposition->push_back( step->GetTotalEnergyDeposit()                       / CLHEP::MeV );
	this->_pid             ->push_back( step->GetTrack()->GetDefinition()->GetPDGEncoding()              );
}

void DataManager::createHistograms() {}

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
