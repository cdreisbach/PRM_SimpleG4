#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <iostream>
#include <vector>

class TTree;
class TFile;
class TH2D;
class G4Step;
class G4Track;
class DataVisualisation;
class DataManager
{
	public:
		static DataManager* getInstance();
		~DataManager();

		void createTrees();
		void addStep( const G4Step* step );
		void addTrack( const G4Track* track );
		void setOutputFileName( const std::string& outputFileName );
		void clear();
		void finish();
		void save();
		void prepare();

		std::string getOutputFileName() const { return this->_outputFileName; }

		TFile* getOutputFile() {return this->_outputFile; }

		unsigned int getTotalNumberOfEvents() const { return this->_evNr + 1; }

	private:
		static DataManager* _instance;
		DataManager();
		void createHistograms();
		bool _prepared = false;
		unsigned int _evNr = 0;

		DataVisualisation* _dataVisualisation;

		TTree*      _treeSteps;
		TTree*      _treeTracks;
		TFile*      _outputFile;
		std::string _outputFileName;

		// Step tree entries
		std::vector<double>* _stepEventNumber      = nullptr;
		std::vector<double>* _stepTrackId          = nullptr;
		std::vector<double>* _stepEnergyDeposition = nullptr;
		std::vector<double>* _stepPositionX        = nullptr;
		std::vector<double>* _stepPositionY        = nullptr;
		std::vector<double>* _stepPositionZ        = nullptr;
		std::vector<double>* _stepPositionR        = nullptr;
		std::vector<double>* _stepTotalEnergy      = nullptr;
		std::vector<double>* _stepKineticEnergy    = nullptr;
		std::vector<double>* _stepPid              = nullptr;
		std::vector<double>* _stepTime             = nullptr;

		// Tracks
		std::vector<double>*      _trackPositionX        = nullptr;
		std::vector<double>*      _trackPositionY        = nullptr;
		std::vector<double>*      _trackPositionZ        = nullptr;
		std::vector<double>*      _trackMomentumX        = nullptr;
		std::vector<double>*      _trackMomentumY        = nullptr;
		std::vector<double>*      _trackMomentumZ        = nullptr;
		std::vector<double>*      _trackTotalEnergy      = nullptr;
		std::vector<double>*      _trackKineticEnergy    = nullptr;
		std::vector<double>*      _trackTrackId          = nullptr;
		std::vector<double>*      _trackParentId         = nullptr;
		std::vector<double>*      _trackPid              = nullptr;
		std::vector<std::string>* _trackCreatorProcess   = nullptr;

		TH2D* _histogramStepXY;
};

#endif
