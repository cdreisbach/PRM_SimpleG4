#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "PhysListEmStandard.hh"
#include "PhysListEmStandardNR.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysicsSS.hh"

#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"

#include "G4DecayPhysics.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4HadronDElasticPhysics.hh"
#include "G4HadronHElasticPhysics.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4IonPhysics.hh"

#include "G4LossTableManager.hh"
#include "G4EmConfigurator.hh"
#include "G4UnitsTable.hh"

#include "G4ProcessManager.hh"

#include "StepMax.hh"

#include "G4IonFluctuations.hh"
#include "G4UniversalFluctuation.hh"

#include "G4BraggIonGasModel.hh"
#include "G4BetheBlochIonGasModel.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList(),
                             fStepMaxProcess( nullptr )
{
	fHelIsRegisted  = false;
	fBicIsRegisted  = false;
	fBiciIsRegisted = false;

	// protected member of the base class
	verboseLevel = 1;

	fMessenger = new PhysicsListMessenger( this );

	// EM physics
	fEmName        = G4String( "emstandard_opt0" );
	fEmPhysicsList = new G4EmStandardPhysics( verboseLevel );

	// Decay physics and all particles
	fDecPhysicsList = new G4DecayPhysics( verboseLevel );
}

PhysicsList::~PhysicsList()
{
	delete fMessenger;
	delete fEmPhysicsList;
	delete fDecPhysicsList;
	for ( auto &fHadronPhy : fHadronPhys )
	{
		delete fHadronPhy;
	}
}

void PhysicsList::ConstructParticle()
{
	fDecPhysicsList->ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
	// transportation
	AddTransportation();

	// electromagnetic physics list
	fEmPhysicsList->ConstructProcess();

	// decay physics list
	fDecPhysicsList->ConstructProcess();

	// hadronic physics lists
	for ( auto &fHadronPhy : fHadronPhys )
	{
		fHadronPhy->ConstructProcess();
	}

	// step limitation (as a full process)
	AddStepMax();
}

void PhysicsList::AddPhysicsList( const G4String &name )
{
	if ( verboseLevel > 1 )
	{
		G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
	}

	if ( name == fEmName )
	{
		return;
	}

	if ( name == "local" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new PhysListEmStandard( name );
	}
	else if ( name == "emstandard_opt0" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics( verboseLevel );
	}
	else if ( name == "emstandard_opt1" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option1( verboseLevel );
	}
	else if ( name == "emstandard_opt2" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option2( verboseLevel );
	}
	else if ( name == "emstandard_opt3" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option3( verboseLevel );
	}
	else if ( name == "emstandard_opt4" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option4( verboseLevel );
	}
	else if ( name == "ionGasModels" )
	{
		AddPhysicsList( "emstandard_opt0" );
		fEmName = name;
		AddIonGasModels();
	}
	else if ( name == "standardNR" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new PhysListEmStandardNR( name );
	}
	else if ( name == "emlivermore" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmLivermorePhysics( verboseLevel );
	}
	else if ( name == "empenelope" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmPenelopePhysics( verboseLevel );
	}
	else if ( name == "emlowenergy" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmLowEPPhysics( verboseLevel );
	}
	else if ( name == "emstandardSS" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysicsSS( verboseLevel );
	}
	else if ( name == "emstandardWVI" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysicsWVI( verboseLevel );
	}
	else if ( name == "emstandardGS" )
	{
		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysicsGS( verboseLevel );
	}
	else if ( name == "elastic" && !fHelIsRegisted )
	{
		fHadronPhys.push_back( new G4HadronElasticPhysics( verboseLevel ));
		fHelIsRegisted = true;
	}
	else if ( name == "DElastic" && !fHelIsRegisted )
	{
		fHadronPhys.push_back( new G4HadronDElasticPhysics( verboseLevel ));
		fHelIsRegisted = true;
	}
	else if ( name == "HElastic" && !fHelIsRegisted )
	{
		fHadronPhys.push_back( new G4HadronHElasticPhysics( verboseLevel ));
		fHelIsRegisted = true;
	}
	else if ( name == "binary" && !fBicIsRegisted )
	{
		fHadronPhys.push_back( new G4HadronInelasticQBBC( verboseLevel ));
		fBicIsRegisted = true;
	}
	else if ( name == "binary_ion" && !fBiciIsRegisted )
	{
		fHadronPhys.push_back( new G4IonPhysics( verboseLevel ));
		fBiciIsRegisted = true;
	}
	else
	{
		G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
		       << " is not defined"
		       << G4endl;
	}
}

void PhysicsList::AddStepMax()
{
	// Step limitation seen as a process
	fStepMaxProcess = new StepMax();

	auto particleIterator = GetParticleIterator();
	particleIterator->reset();
	while( ( *particleIterator )() )
	{
		G4ParticleDefinition *particle = particleIterator->value();
		G4ProcessManager     *pmanager = particle->GetProcessManager();

		if ( fStepMaxProcess->IsApplicable( *particle ) && pmanager )
		{
			pmanager->AddDiscreteProcess( fStepMaxProcess );
		}
	}
}

void PhysicsList::AddIonGasModels()
{
	G4EmConfigurator *em_config = G4LossTableManager::Instance()->EmConfigurator();
	auto particleIterator = GetParticleIterator();
	particleIterator->reset();
	while (( *particleIterator )())
	{
		G4ParticleDefinition *particle = particleIterator->value();
		G4String partname = particle->GetParticleName();
		if ( partname == "alpha" || partname == "He3" || partname == "GenericIon" )
		{
			G4BraggIonGasModel      *mod1 = new G4BraggIonGasModel();
			G4BetheBlochIonGasModel *mod2 = new G4BetheBlochIonGasModel();
			G4double eth = 2. * MeV * particle->GetPDGMass() / proton_mass_c2;
			em_config->SetExtraEmModel( partname, "ionIoni", mod1, "", 0.0, eth             , new G4IonFluctuations()      );
			em_config->SetExtraEmModel( partname, "ionIoni", mod2, "", eth, 100 * CLHEP::TeV, new G4UniversalFluctuation() );
		}
	}
}
