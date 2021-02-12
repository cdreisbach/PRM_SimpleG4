#ifndef DATAVISUALISATION_H
#define DATAVISUALISATION_H

#include <vector>
#include <string>

class TH1;
class TGraph;
class TCanvas;
class DataManager;
class DataVisualisation
{
	public:
		static DataVisualisation* getInstance();
		TH1* makeLogBinningX( TH1 *histogram );

		TH1*     createHistogram( TH1* histogram );
		TGraph*  createGraph( const std::string& name, TGraph* graph );
		TCanvas* createCanvas( TCanvas* canvas );

		void saveData();

	private:
		static DataVisualisation* _instance;
		DataVisualisation();
		~DataVisualisation();

		void saveHistograms();
		void saveGraphs();
		void saveCanvae();

		std::vector<TH1*>     _histograms;
		std::vector<TGraph*>  _graphs;
		std::vector<TCanvas*> _canvae;
};


#endif
