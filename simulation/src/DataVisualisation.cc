#include "DataVisualisation.hh"
#include "DataManager.hh"

#include <TFile.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TCanvas.h>


DataVisualisation* DataVisualisation::_instance = nullptr;

DataVisualisation::DataVisualisation() {}

DataVisualisation::~DataVisualisation() {}

DataVisualisation* DataVisualisation::getInstance()
{
	if( _instance == nullptr )
	{
		_instance = new DataVisualisation();
	}
	return _instance;
}

void DataVisualisation::saveHistograms()
{
	DataManager::getInstance()->getOutputFile()->cd();
	for ( const auto &histogram : this->_histograms )
	{
		histogram->Write();
	}
}

void DataVisualisation::saveGraphs()
{
	DataManager::getInstance()->getOutputFile()->cd();
	for( const auto& graph : this->_graphs )
	{
		graph->Write();
	}
}

void DataVisualisation::saveCanvae()
{
	DataManager::getInstance()->getOutputFile()->cd();
	for( const auto& canvas : this->_canvae )
	{
		canvas->Write();
	}
}

TH1 *DataVisualisation::createHistogram( TH1 *histogram )
{
	// Check for existing histogram
	bool existing = false;
	for( const auto& myHistogram : this->_histograms )
	{
		if( myHistogram->GetName() == histogram->GetName() )
		{
			existing = true;
			std::cout << "WARNING: Histogram with name " << histogram->GetName() << " already existing!" << std::endl;
		}
	}
	if( !existing )
	{
		this->_histograms.push_back(histogram);
	}
	return histogram;
}

void DataVisualisation::saveData()
{
	// Save graphs
	this->saveGraphs();

	// Save histograms
	this->saveHistograms();

	// Save canvae
	this->saveCanvae();
}


TH1* DataVisualisation::makeLogBinningX( TH1* histogram )
{
	TAxis* axis = histogram->GetXaxis();
	int bins    = axis->GetNbins();

	Axis_t  from     = axis->GetXmin();
	Axis_t  to       = axis->GetXmax();
	Axis_t  width    = (to - from) / bins;
	Axis_t* new_bins = new Axis_t[bins + 1];

	for (int i = 0; i <= bins; i++)
	{
		new_bins[i] = pow(10, from + i * width);
	}
	axis->Set(bins, new_bins);
	delete[] new_bins;
	return histogram;
}

TGraph *DataVisualisation::createGraph(const std::string& name, TGraph *graph)
{
	// Set name
	graph->SetName(name.c_str());
	graph->SetTitle("");

	// Check for existing graph
	bool existing = false;
	for( const auto& myGraph : this->_graphs )
	{
		if( myGraph->GetName() == graph->GetName() )
		{
			existing = true;
			std::cout << "WARNING: Histogram with name " << graph->GetName() << " already existing!" << std::endl;
		}
	}
	if( !existing )
	{
		this->_graphs.push_back(graph);
	}
	return graph;
}

TCanvas* DataVisualisation::createCanvas( TCanvas *canvas )
{
	// Check for existing graph
	bool existing = false;
	for( const auto& myCanvas : this->_canvae )
	{
		if( myCanvas->GetName() == canvas->GetName() )
		{
			existing = true;
			std::cout << "WARNING: Canvas with name " << canvas->GetName() << " already existing!" << std::endl;
		}
	}
	if( !existing )
	{
		this->_canvae.push_back(canvas);
	}
	return canvas;
}
