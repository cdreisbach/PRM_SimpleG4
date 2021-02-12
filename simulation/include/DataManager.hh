#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <iostream>
#include <vector>

class TTree;
class TFile;
class G4Step;
class ProtonEvent;
class DataManager
{
	public:
		static DataManager* getInstance();
		~DataManager();

		void createTrees();
		void addStep( const G4Step* step );
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

		TTree*      _tree;
		TFile*      _outputFile;
		std::string _outputFileName;

		// Step tree entries
		std::vector<double>* _eventNumber      = nullptr;
		std::vector<double>* _energyDeposition = nullptr;
		std::vector<double>* _positionX        = nullptr;
		std::vector<double>* _positionY        = nullptr;
		std::vector<double>* _positionZ        = nullptr;
		std::vector<double>* _positionR        = nullptr;
		std::vector<double>* _totalEnergy      = nullptr;
		std::vector<double>* _kineticEnergy    = nullptr;
		std::vector<double>* _pid              = nullptr;
		std::vector<double>* _time             = nullptr;
};

#endif
