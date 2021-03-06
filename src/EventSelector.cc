#include "EventSelector.h"
#include <iostream>
#include <EVENT/LCCollection.h>
#include "EVENT/LCCollection.h"
#include "IMPL/LCCollectionVec.h"
#include "EVENT/MCParticle.h"
#include "EVENT/Vertex.h"
#include "EVENT/ReconstructedParticle.h"
#include <IMPL/ReconstructedParticleImpl.h>
#include "IMPL/ParticleIDImpl.h"
#include "UTIL/PIDHandler.h"
#include "marlin/VerbosityLevels.h"
#include "TFile.h"
#include "TH1I.h"
#include "TTree.h"
using namespace lcio ;
using namespace marlin ;

EventSelector aEventSelector;

EventSelector::EventSelector():
	Processor("EventSelector"),
	m_nRun(0),
	m_nEvt(0),
	m_nRunSum(0),
	m_nEvtSum(0),
	m_Hq1q1(0),
	m_Hq2q2(0),
	m_Hq3q3(0),
	m_Hq4q4(0),
	m_Hq5q5(0),
	m_Hgg(0),
	m_He1e1(0),
	m_He2e2(0),
	m_He3e3(0),
	m_Hn1n1(0),
	m_Hn2n2(0),
	m_Hn3n3(0),
	m_Haa(0),
	m_HWW(0),
	m_HZZ(0),
	m_HHH(0),
	m_Hother(0),
	m_totalHDecays(0),
	m_Ze1e1(0),
	m_Ze2e2(0),
	m_Ze3e3(0),
	m_useEvent(1),
	n_ZHDecays(0),
	n_ZDecays(0)
{
	_description = "EventSelector checks the decay modes of Z?H bosons and also declusters the jet particles + untags leptons from Isolated leptons in the case of > 2 IsoLeps";

	registerInputCollection(	LCIO::MCPARTICLE,
					"MCParticleCollection" ,
					"Name of the MCParticle collection"  ,
					m_mcParticleCollection,
					std::string("MCParticle")
				);

	registerInputCollection(	LCIO::RECONSTRUCTEDPARTICLE,
					"PfoCollection",
					"Name of input pfo collection",
					m_inputPfoCollection,
					std::string("PandoraPFOs")
				);

	registerInputCollection(	LCIO::RECONSTRUCTEDPARTICLE,
					"JetCollection",
					"Name of input jet collection",
					m_inputJetCollection,
					std::string("Durham_nJets")
				);

	registerInputCollection(	LCIO::RECONSTRUCTEDPARTICLE,
					"isoLepCollection",
					"Name of input Isolated Lepton collection",
					m_inputIsoLepCollection,
					std::string("ISOLeptons")
				);

	registerOutputCollection(	LCIO::RECONSTRUCTEDPARTICLE,
					"outputPfoCollection",
					"Name of output pfo collection",
					m_outputPfoCollection,
					std::string("declusteredJetPlusUnTaggedIsolep")
				);

	registerOutputCollection(	LCIO::RECONSTRUCTEDPARTICLE,
					"outputIsolepCollection",
					"Name of output isolated lepton collection",
					m_outputIsolepCollection,
					std::string("IsolatedLeptons")
				);

	registerOutputCollection(	LCIO::MCPARTICLE,
					"decayModeCollection",
					"Name of Physics Process collection",
					m_ZHdecayMode,
					std::string("ZHdecayMode")
				);

	registerProcessorParameter(	"includHbb",
					"Include H->bb events",
					m_includHbb,
					bool(true)
				);

	registerProcessorParameter(	"includHcc",
					"Include H->cc events",
					m_includHcc,
					bool(true)
				);

	registerProcessorParameter(	"includHss",
					"Include H->ss events",
					m_includHss,
					bool(true)
				);

	registerProcessorParameter(	"includHuu",
					"Include H->uu events",
					m_includHuu,
					bool(true)
				);

	registerProcessorParameter(	"includHdd",
					"Include H->dd events",
					m_includHdd,
					bool(true)
				);

	registerProcessorParameter(	"includHgg",
					"Include H->gg events",
					m_includHgg,
					bool(true)
				);

	registerProcessorParameter(	"includHee",
					"Include H->e+e- events",
					m_includHee,
					bool(true)
				);

	registerProcessorParameter(	"includHmumu",
					"Include H->mu+mu- events",
					m_includHmumu,
					bool(true)
				);

	registerProcessorParameter(	"includHtautau",
					"Include H->tau+tau- events",
					m_includHtautau,
					bool(true)
				);

	registerProcessorParameter(	"includHnu1nu1",
					"Include H->nu1nu1 events",
					m_includHnu1nu1,
					bool(true)
				);

	registerProcessorParameter(	"includHnu2nu2",
					"Include H->nu2nu2 events",
					m_includHnu2nu2,
					bool(true)
				);

	registerProcessorParameter(	"includHnu3nu3",
					"Include H->nu3nu3 events",
					m_includHnu3nu3,
					bool(true)
				);

	registerProcessorParameter(	"includHgammagamma",
					"Include H->GammaGamma events",
					m_includHgammagamma,
					bool(true)
				);

	registerProcessorParameter(	"includHWW",
					"Include H->W+W- events",
					m_includHWW,
					bool(true)
				);

	registerProcessorParameter(	"includHZZ",
					"Include H->ZZ events",
					m_includHZZ,
					bool(true)
				);

	registerProcessorParameter(	"includHHH",
					"Include H->HH events",
					m_includHHH,
					bool(true)
				);

	registerProcessorParameter(	"includHother",
					"Include H->Other events",
					m_includHother,
					bool(true)
				);

	registerProcessorParameter(	"includZee",
					"Include Z->e+e- events",
					m_includZe1e1,
					bool(true)
				);

	registerProcessorParameter(	"includZmumu",
					"Include Z->mu+mu- events",
					m_includZe2e2,
					bool(true)
				);

	registerProcessorParameter(	"includZtautau",
					"Include Z->tau+tau- events",
					m_includZe3e3,
					bool(true)
				);

	registerProcessorParameter(	"nJets",
					"Number of jet should be in the event",
					m_nJets,
					int(0)
				);

	registerProcessorParameter(	"nIsoLeps",
					"Number of Isolated Leptons should be in the event",
					m_nIsoLeps,
					int(0)
				);

	registerProcessorParameter(	"diLepInvMass",
					"Invariant mass of di-lepton system in Isolated Leptons [GeV]",
					m_diLepInvMass,
					double(91.2)
				);

	registerProcessorParameter(	"fillRootTree",
					"Fill root tree to check processor performance",
					m_fillRootTree,
					bool(true)
				);

	registerProcessorParameter(	"RootFile",
	                                "Name of the output root file",
					m_rootFile,
					std::string("Output.root")
				);

}

void EventSelector::init()
{

	streamlog_out(DEBUG) << "   init called  " << std::endl ;
	printParameters();
	if ( m_fillRootTree )
	{
		streamlog_out(DEBUG) << "	Creating root file/tree/histograms" << std::endl ;
		m_pTFile = new TFile(m_rootFile.c_str(), "recreate");
		m_pTTree = new TTree("SLDCorrection", "SLDCorrection");
		m_pTTree->SetDirectory(m_pTFile);
		m_pTTree->Branch("event", &m_nEvt, "event/I");
		m_pTTree->Branch("ZHdd", &m_Hq1q1, "ZHdd/I");
		m_pTTree->Branch("ZHuu", &m_Hq2q2, "ZHuu/I");
		m_pTTree->Branch("ZHss", &m_Hq3q3, "ZHss/I");
		m_pTTree->Branch("ZHcc", &m_Hq4q4, "ZHcc/I");
		m_pTTree->Branch("ZHbb", &m_Hq5q5, "ZHbb/I");
		m_pTTree->Branch("ZHgg", &m_Hgg, "ZHgg/I");
		m_pTTree->Branch("ZHee", &m_He1e1, "ZHee/I");
		m_pTTree->Branch("ZHmumu", &m_He2e2, "ZHmumu/I");
		m_pTTree->Branch("ZHtautaud", &m_He3e3, "ZHtautau/I");
		m_pTTree->Branch("ZHnu1nu1", &m_Hn1n1, "ZHnu1nu1/I");
		m_pTTree->Branch("ZHnu2nu2", &m_Hn2n2, "ZHnu2nu2/I");
		m_pTTree->Branch("ZHnu3nu3", &m_Hn3n3, "ZHnu3nu3/I");
		m_pTTree->Branch("ZHGammaGamma", &m_Haa, "ZHGammaGamma/I");
		m_pTTree->Branch("ZHWW", &m_HWW, "ZHWW/I");
		m_pTTree->Branch("ZHZZ", &m_HZZ, "ZHZZ/I");
		m_pTTree->Branch("ZHHH", &m_HHH, "ZHHH/I");
		m_pTTree->Branch("ZHother", &m_Hother, "ZHother/I");
		m_pTTree->Branch("totalZHDecays", &m_totalHDecays, "totalZHDecays/I");
		m_pTTree->Branch("Zee", &m_Ze1e1, "Zee/I");
		m_pTTree->Branch("Zmumu", &m_Ze2e2, "Zmumu/I");
		m_pTTree->Branch("Ztautaud", &m_Ze3e3, "Ztautau/I");
		m_pTTree->Branch("useEvent", &m_useEvent, "useEvent/I");
		h_ZHDecayMode = new TH1I( "ZHDecayMode" , "; Decay Mode" , 17 , -0.5 , 16.5 ); n_ZHDecays = 0;
		h_ZHDecayMode->GetXaxis()->SetBinLabel(1,"HH");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(2,"ZZ");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(3,"W^{+}W^{-}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(4,"b#bar{b}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(5,"c#bar{c}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(6,"s#bar{s}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(7,"u#bar{u}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(8,"d#bar{d}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(9,"gg");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(10,"e^{+}e^{-}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(11,"#mu^{+}#mu^{-}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(12,"#tau^{+}#tau^{-}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(13,"#nu_{e}#bar{#nu}_{e}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(14,"#nu_{#mu}#bar{#nu}_{#mu}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(15,"#nu_{#tau}#bar{#nu}_{#tau}");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(16,"#gamma#gamma");
		h_ZHDecayMode->GetXaxis()->SetBinLabel(17,"other");
		h_ZDecayMode = new TH1I( "ZDecayMode" , "; Decay Mode" , 3 , -0.5 , 2.5 ); n_ZDecays = 0;
		h_ZDecayMode->GetXaxis()->SetBinLabel(1,"Z#rightarrow e^{+}e^{-}");
		h_ZDecayMode->GetXaxis()->SetBinLabel(2,"Z#rightarrow #mu^{+}#mu^{-}");
		h_ZDecayMode->GetXaxis()->SetBinLabel(3,"Z#rightarrow #tau^{+}#tau^{-}");
		streamlog_out(DEBUG) << "	Created root file/tree/histograms" << std::endl ;
	}
	m_Hq1q1 = 0;
	m_Hq2q2 = 0;
	m_Hq3q3 = 0;
	m_Hq4q4 = 0;
	m_Hq5q5 = 0;
	m_Hgg = 0;
	m_He1e1 = 0;
	m_He2e2 = 0;
	m_He3e3 = 0;
	m_Hn1n1 = 0;
	m_Hn2n2 = 0;
	m_Hn3n3 = 0;
	m_Haa = 0;
	m_HWW = 0;
	m_HZZ = 0;
	m_HHH = 0;
	m_Hother = 0;
	m_totalHDecays = 0;
	m_Ze1e1 = 0;
	m_Ze2e2 = 0;
	m_Ze3e3 = 0;
	m_useEvent = 1;
	this->Clear();
	streamlog_out(DEBUG) << "   init finished successfully" << std::endl ;
}

void EventSelector::Clear()
{
	streamlog_out(DEBUG) << "   clear called" << std::endl ;
	m_Hq1q1 = 0;
	m_Hq2q2 = 0;
	m_Hq3q3 = 0;
	m_Hq4q4 = 0;
	m_Hq5q5 = 0;
	m_Hgg = 0;
	m_He1e1 = 0;
	m_He2e2 = 0;
	m_He3e3 = 0;
	m_Hn1n1 = 0;
	m_Hn2n2 = 0;
	m_Hn3n3 = 0;
	m_Haa = 0;
	m_HWW = 0;
	m_HZZ = 0;
	m_HHH = 0;
	m_Hother = 0;
	m_totalHDecays = 0;
	m_Ze1e1 = 0;
	m_Ze2e2 = 0;
	m_Ze3e3 = 0;
	m_useEvent = 1;
	streamlog_out(DEBUG) << "   clear finished successfully" << std::endl ;
}

void EventSelector::processRunHeader()
{
	streamlog_out(DEBUG) << "   processRunHeader called" << std::endl ;
	m_nRun = 0;
	m_nEvt = 0;
	++m_nRunSum;
	streamlog_out(DEBUG) << "   processRunHeader finished successfully" << std::endl ;
}

void EventSelector::processEvent( EVENT::LCEvent *pLCEvent )
{
/*
	streamlog_out(DEBUG) << "	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	processEvent Called	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl ;
	m_nRun = pLCEvent->getRunNumber();
	m_nEvt = pLCEvent->getEventNumber();
	streamlog_out(MESSAGE) << "" << std::endl;
	streamlog_out(MESSAGE) << "	////////////////////////////////////////////////////////////////////////////" << std::endl;
	streamlog_out(MESSAGE) << "	////////////////////	Processing event 	" << m_nEvt << "	////////////////////" << std::endl;
	streamlog_out(MESSAGE) << "	////////////////////////////////////////////////////////////////////////////" << std::endl;
	this->Clear();
	bool trueNJets = false;
	bool trueNIsoLeps = false;
	bool askedDecayMode = false;

	const EVENT::LCCollection *MCParticleCollection{};
	const EVENT::LCCollection *JetCollection{};
	const EVENT::LCCollection *IsoleptonCollection{};
	LCCollectionVec *m_IsolepCol = new LCCollectionVec( LCIO::RECONSTRUCTEDPARTICLE );
	LCCollectionVec *m_NewPFOsCol = new LCCollectionVec( LCIO::RECONSTRUCTEDPARTICLE );
	LCCollectionVec *m_PhysProcCol = new LCCollectionVec( LCIO::MCPARTICLE );

	try
	{
		MCParticleCollection = pLCEvent->getCollection( m_mcParticleCollection );
		JetCollection = pLCEvent->getCollection( m_inputJetCollection );
		IsoleptonCollection = pLCEvent->getCollection( m_inputIsoLepCollection );

		int nJets = JetCollection->getNumberOfElements();
		streamlog_out( DEBUG4 ) << "	" << nJets << " jets in event, looking for " << m_nJets << " jets" << std::endl;
		if ( nJets != m_nJets )
		{
			trueNJets = false;
			streamlog_out( DEBUG3 ) << "	Number of jets in the event mismatches the asked number of jets, --------EVENT REJECTED--------" << std::endl;
		}
		else
		{
			trueNJets = true;
			streamlog_out( DEBUG3 ) << "	Number of jets in the event matches the asked number of jets, --------EVENT ACCEPTED--------" << std::endl;
		}
		for ( int i_jet = 0 ; i_jet < nJets ; ++i_jet )
		{
			ReconstructedParticle* jet = dynamic_cast<ReconstructedParticle*>( JetCollection->getElementAt( i_jet ) );
			for (unsigned int i_pfo = 0 ; i_pfo < jet->getParticles().size() ; ++i_pfo )
			{
				m_NewPFOsCol->addElement( jet->getParticles()[ i_pfo ] );
			}
		}

		int nIsoLeps = IsoleptonCollection->getNumberOfElements();
		streamlog_out( DEBUG4 ) << "	" << nIsoLeps << " issolated leptons in event, looking for " << m_nIsoLeps << " isolated leptons" << std::endl;
		if ( nIsoLeps < m_nIsoLeps )
		{
			trueNIsoLeps = false;
			streamlog_out( DEBUG3 ) << "	Number of isolated leptons in the event is less than the asked number of isolated leptons, --------EVENT REJECTED--------" << std::endl;
		}
		else if ( nIsoLeps == m_nIsoLeps )
		{
			trueNIsoLeps = true;
			streamlog_out( DEBUG3 ) << "	Number of isolated leptons in the event matches the asked number of isolated leptons, --------EVENT ACCEPTED--------" << std::endl;
		}
		else
		{
			std::vector<double> massDiff{};
			std::vector<int> lep1Index{};
			std::vector<int> lep2Index{};
			for ( int i_lep1 = 0 ; i_lep1 < nIsoLeps - 1 ; ++i_lep1 )
			{
				ReconstructedParticle* lepton1 = dynamic_cast<ReconstructedParticle*>( IsoleptonCollection->getElementAt( i_lep1 ) );
				TLorentzVector lep1FourMomentum = TLorentzVector( lepton1->getMomentum()[ 0 ] , lepton1->getMomentum()[ 1 ] , lepton1->getMomentum()[ 2 ] , lepton1->getEnergy() );
				for ( int i_lep2 = i_lep1 + 1 ; i_lep2 < nIsoLeps ; ++i_lep2 )
				{
					ReconstructedParticle* lepton2 = dynamic_cast<ReconstructedParticle*>( IsoleptonCollection->getElementAt( i_lep2 ) );
					TLorentzVector lep2FourMomentum = TLorentzVector( lepton2->getMomentum()[ 0 ] , lepton2->getMomentum()[ 1 ] , lepton2->getMomentum()[ 2 ] , lepton2->getEnergy() );
					double diLepInvMass = ( lep1FourMomentum + lep2FourMomentum ).M();
					massDiff.push_back( fabs( diLepInvMass - m_diLepInvMass ) );
					lep1Index.push_back( i_lep1 );
					lep2Index.push_back( i_lep2 );
				}
			}
			double smallestMassDiff = m_diLepInvMass;
			int iLepton1 = -1;
			int iLepton2 = -1;
			for ( unsigned int i_pair = 0 ; i_pair < massDiff.size() ; ++i_pair )
			{
				if ( massDiff[ i_pair ] < smallestMassDiff )
				{
					smallestMassDiff = massDiff[ i_pair ];
					iLepton1 = lep1Index[ i_pair ];
					iLepton2 = lep2Index[ i_pair ];
				}
			}
			for ( int i_lep = 0 ; i_lep < nIsoLeps ; ++i_lep )
			{
				ReconstructedParticleImpl* isoLepton = new ReconstructedParticleImpl;
				ReconstructedParticle* lepton = dynamic_cast<ReconstructedParticle*>( IsoleptonCollection->getElementAt( i_lep ) );
				isoLepton->setType( lepton->getType() );
				isoLepton->setMomentum( lepton->getMomentum() );
				isoLepton->setEnergy( lepton->getEnergy() );
				isoLepton->setCovMatrix( lepton->getCovMatrix() );
				isoLepton->setMass( lepton->getMass() );
				isoLepton->setCharge( lepton->getCharge() );
				isoLepton->setReferencePoint( lepton->getReferencePoint() );
				for ( unsigned int j = 0 ; j < lepton->getParticleIDs().size() ; ++j )
				{
					ParticleIDImpl* inPID = dynamic_cast<ParticleIDImpl*>( lepton->getParticleIDs()[ j ] );
				        ParticleIDImpl* outPID = new ParticleIDImpl;
				        outPID->setType( inPID->getType() );
				        outPID->setPDG( inPID->getPDG() );
				        outPID->setLikelihood( inPID->getLikelihood() );
				        outPID->setAlgorithmType( inPID->getAlgorithmType() ) ;
				        for ( unsigned int k = 0 ; k < inPID->getParameters().size() ; ++k) outPID->addParameter( inPID->getParameters()[ k ] ) ;
				        isoLepton->addParticleID( outPID );
				}
				isoLepton->setParticleIDUsed( lepton->getParticleIDUsed() );
				isoLepton->setGoodnessOfPID( lepton->getGoodnessOfPID() );
				for ( unsigned int j = 0 ; j < lepton->getParticles().size() ; ++j )
				{
					isoLepton->addParticle( lepton->getParticles()[ j ] );
				}
				for ( unsigned int j = 0 ; j < lepton->getClusters().size() ; ++j )
				{
					isoLepton->addCluster( lepton->getClusters()[ j ] );
				}
				for ( unsigned int j = 0 ; j < lepton->getTracks().size() ; ++j )
				{
					isoLepton->addTrack( lepton->getTracks()[ j ] );
				}
				isoLepton->setStartVertex( lepton->getStartVertex() );

				if ( i_lep == iLepton1 || i_lep == iLepton2 )
				{
					m_IsolepCol->addElement( isoLepton );
				}
				else
				{
					m_NewPFOsCol->addElement( isoLepton );
				}
			}
			if ( iLepton1 != -1 && iLepton2 != -1 )
			{
				trueNIsoLeps = true;
				streamlog_out( DEBUG3 ) << "	Two leptons remained as isolated in the event, rest added to PFOsminusIsoLep to be clustered in jets, --------EVENT ACCEPTED--------" << std::endl;
			}


		}


		int daughter1index = -1;
		int daughter2index = -1;
		m_HHH = isZHDecayedTo( MCParticleCollection , 25 , 25 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 25 , daughter1index , daughter2index );
		m_HZZ = isZHDecayedTo( MCParticleCollection , 25 , 23 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 23 , daughter1index , daughter2index );
		m_HWW = isZHDecayedTo( MCParticleCollection , 25 , 24 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 24 , daughter1index , daughter2index );
		m_Hq5q5 = isZHDecayedTo( MCParticleCollection , 25 , 5 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 5 , daughter1index , daughter2index );
		m_Hq4q4 = isZHDecayedTo( MCParticleCollection , 25 , 4 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 4 , daughter1index , daughter2index );
		m_Hq3q3 = isZHDecayedTo( MCParticleCollection , 25 , 3 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 3 , daughter1index , daughter2index );
		m_Hq2q2 = isZHDecayedTo( MCParticleCollection , 25 , 2 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 2 , daughter1index , daughter2index );
		m_Hq1q1 = isZHDecayedTo( MCParticleCollection , 25 , 1 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 1 , daughter1index , daughter2index );
		m_Hgg = isZHDecayedTo( MCParticleCollection , 25 , 21 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 21 , daughter1index , daughter2index );
		m_He1e1 = isZHDecayedTo( MCParticleCollection , 25 , 11 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 11 , daughter1index , daughter2index );
		m_He2e2 = isZHDecayedTo( MCParticleCollection , 25 , 13 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 13 , daughter1index , daughter2index );
		m_He3e3 = isZHDecayedTo( MCParticleCollection , 25 , 15 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 15 , daughter1index , daughter2index );
		m_Hn1n1 = isZHDecayedTo( MCParticleCollection , 25 , 12 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 12 , daughter1index , daughter2index );
		m_Hn2n2 = isZHDecayedTo( MCParticleCollection , 25 , 14 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 14 , daughter1index , daughter2index );
		m_Hn3n3 = isZHDecayedTo( MCParticleCollection , 25 , 16 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 16 , daughter1index , daughter2index );
		m_Haa = isZHDecayedTo( MCParticleCollection , 25 , 22 , daughter1index , daughter2index ) || isZHDecayedTo( MCParticleCollection , 23 , 22 , daughter1index , daughter2index );
		m_totalHDecays = m_HHH + m_HZZ + m_HWW + m_Hq5q5 + m_Hq4q4 + m_Hq3q3 + m_Hq2q2 + m_Hq1q1 + m_Hgg + m_He1e1 + m_He2e2 + m_He3e3 + m_Hn1n1 + m_Hn2n2 + m_Hn3n3 + m_Haa;
		if ( m_totalHDecays == 0 ) m_Hother = 1;
		m_Ze1e1 = isZDecayedTo( MCParticleCollection , 23 , 11 , daughter1index , daughter2index );
		m_Ze2e2 = isZDecayedTo( MCParticleCollection , 23 , 13 , daughter1index , daughter2index );
		m_Ze3e3 = isZDecayedTo( MCParticleCollection , 23 , 15 , daughter1index , daughter2index );

		askedDecayMode = ( ( m_HHH && m_includHHH ) || ( m_HZZ && m_includHZZ ) || ( m_HWW && m_includHWW ) || ( m_Hq5q5 && m_includHbb ) || ( m_Hq4q4 && m_includHcc ) || ( m_Hq3q3 && m_includHss ) || ( m_Hq2q2 && m_includHuu ) || ( m_Hq1q1 && m_includHdd ) || ( m_Hgg && m_includHgg ) || ( m_He1e1 && m_includHee ) || ( m_He2e2 && m_includHmumu ) || ( m_He3e3 && m_includHtautau ) || ( m_Hn1n1 && m_includHnu1nu1 ) || ( m_Hn2n2 && m_includHnu2nu2 ) || ( m_Hn3n3 && m_includHnu3nu3 ) || ( m_Haa && m_includHgammagamma ) || ( m_Hother && m_includHother ) );
		m_useEvent = ( askedDecayMode && trueNJets && trueNIsoLeps ? 1 : 0 );


		m_PhysProcCol->parameters().setValue( "ZHdd" , ( int )m_Hq1q1 );
		m_PhysProcCol->parameters().setValue( "ZHuu" , ( int )m_Hq2q2 );
		m_PhysProcCol->parameters().setValue( "ZHss" , ( int )m_Hq3q3 );
		m_PhysProcCol->parameters().setValue( "ZHcc" , ( int )m_Hq4q4 );
		m_PhysProcCol->parameters().setValue( "ZHbb" , ( int )m_Hq5q5 );
		m_PhysProcCol->parameters().setValue( "ZHgg" , ( int )m_Hgg );
		m_PhysProcCol->parameters().setValue( "ZHee" , ( int )m_He1e1 );
		m_PhysProcCol->parameters().setValue( "ZHmumu" , ( int )m_He2e2 );
		m_PhysProcCol->parameters().setValue( "ZHtautau" , ( int )m_He3e3 );
		m_PhysProcCol->parameters().setValue( "ZHnu1nu1" , ( int )m_Hn1n1 );
		m_PhysProcCol->parameters().setValue( "ZHnu2nu2" , ( int )m_Hn2n2 );
		m_PhysProcCol->parameters().setValue( "ZHnu3nu3" , ( int )m_Hn3n3 );
		m_PhysProcCol->parameters().setValue( "ZHgammagamma" , ( int )m_Haa );
		m_PhysProcCol->parameters().setValue( "ZHWW" , ( int )m_HWW );
		m_PhysProcCol->parameters().setValue( "ZHZZ" , ( int )m_HZZ );
		m_PhysProcCol->parameters().setValue( "ZHHH" , ( int )m_HHH );
		m_PhysProcCol->parameters().setValue( "ZHother" , ( int )m_Hother );
		m_PhysProcCol->parameters().setValue( "totalHDecays" , ( int )m_totalHDecays );
		m_PhysProcCol->parameters().setValue( "Zee" , ( int )m_Ze1e1 );
		m_PhysProcCol->parameters().setValue( "Zmumu" , ( int )m_Ze2e2 );
		m_PhysProcCol->parameters().setValue( "Ztautau" , ( int )m_Ze3e3 );
		m_PhysProcCol->parameters().setValue( "useEvent" , ( int )m_useEvent );

		pLCEvent->addCollection( m_PhysProcCol , m_ZHdecayMode );
		pLCEvent->addCollection( m_IsolepCol , m_outputIsolepCollection );
		pLCEvent->addCollection( m_NewPFOsCol , m_outputPfoCollection );
		if ( m_fillRootTree )
		{
			h_ZHDecayMode->Fill( 0.0 , (double)m_HHH ); n_ZHDecays += m_HHH;
			h_ZHDecayMode->Fill( 1.0 , (double)m_HZZ ); n_ZHDecays += m_HZZ;
			h_ZHDecayMode->Fill( 2.0 , (double)m_HWW ); n_ZHDecays += m_HWW;
			h_ZHDecayMode->Fill( 3.0 , (double)m_Hq5q5 ); n_ZHDecays += m_Hq5q5;
			h_ZHDecayMode->Fill( 4.0 , (double)m_Hq4q4 ); n_ZHDecays += m_Hq4q4;
			h_ZHDecayMode->Fill( 5.0 , (double)m_Hq3q3 ); n_ZHDecays += m_Hq3q3;
			h_ZHDecayMode->Fill( 6.0 , (double)m_Hq2q2 ); n_ZHDecays += m_Hq2q2;
			h_ZHDecayMode->Fill( 7.0 , (double)m_Hq1q1 ); n_ZHDecays += m_Hq1q1;
			h_ZHDecayMode->Fill( 8.0 , (double)m_Hgg ); n_ZHDecays += m_Hgg;
			h_ZHDecayMode->Fill( 9.0 , (double)m_He1e1 ); n_ZHDecays += m_He1e1;
			h_ZHDecayMode->Fill( 10.0 , (double)m_He2e2 ); n_ZHDecays += m_He2e2;
			h_ZHDecayMode->Fill( 11.0 , (double)m_He3e3 ); n_ZHDecays += m_He3e3;
			h_ZHDecayMode->Fill( 12.0 , (double)m_Hn1n1 ); n_ZHDecays += m_Hn1n1;
			h_ZHDecayMode->Fill( 13.0 , (double)m_Hn2n2 ); n_ZHDecays += m_Hn2n2;
			h_ZHDecayMode->Fill( 14.0 , (double)m_Hn3n3 ); n_ZHDecays += m_Hn3n3;
			h_ZHDecayMode->Fill( 15.0 , (double)m_Haa ); n_ZHDecays += m_Haa;
			h_ZHDecayMode->Fill( 16.0 , (double)m_Hother ); n_ZHDecays += m_Hother;
			h_ZDecayMode->Fill( 0.0 , (double)m_Ze1e1 ); n_ZDecays += m_Ze1e1;
			h_ZDecayMode->Fill( 1.0 , (double)m_Ze2e2 ); n_ZDecays += m_Ze2e2;
			h_ZDecayMode->Fill( 2.0 , (double)m_Ze3e3 ); n_ZDecays += m_Ze3e3;
			m_pTTree->Fill();
		}
	}

	catch( DataNotAvailableException &e )
        {
        	streamlog_out(MESSAGE) << "	Input collection not found in event " << m_nEvt << std::endl;
        }
*/
}

int EventSelector::isZHDecayedTo( const EVENT::LCCollection *MCParticleCollection , int parentPDG , int daughtersPDG , int &daughter1index , int &daughter2index )
{
	int elementFrom = 8;
	int elementTo = 20;
	int isDecaydToDaughter = 0;
	int d1PDG = abs( daughtersPDG );
	int d2PDG = ( abs( daughtersPDG ) < 20 ? -1 * d1PDG : d1PDG );
	for ( int i_d1 = elementFrom ; i_d1 < elementTo ; ++i_d1 )
	{
		const EVENT::MCParticle *daughter1 = dynamic_cast<EVENT::MCParticle*>( MCParticleCollection->getElementAt( i_d1 ) );
		if ( daughter1->getPDG() == d1PDG )
		{
			for ( unsigned int i_parent = 0 ; i_parent < daughter1->getParents().size() ; ++i_parent )
			{
				const EVENT::MCParticle *parent = daughter1->getParents()[ i_parent ];
				if ( parent->getPDG() == parentPDG )
				{
					for ( unsigned int i_d2 = 0 ; i_d2 < parent->getDaughters().size() ; ++i_d2 )
					{
						const EVENT::MCParticle *daughter2 = parent->getDaughters()[ i_d2 ];
						if ( daughter2 != daughter1 && daughter2->getPDG() == d2PDG )
						{
							isDecaydToDaughter = 1;
							daughter1index = i_d1;
							for ( int i_d = elementFrom ; i_d < elementTo ; ++i_d )
							{
								const EVENT::MCParticle *testMCP = dynamic_cast<EVENT::MCParticle*>( MCParticleCollection->getElementAt( i_d ) );
								if ( testMCP == daughter2 ) daughter2index = i_d;
							}
							return isDecaydToDaughter;
						}
					}
				}


				else if ( parent->getPDG() != 25 )
				{
					for ( unsigned int i_d2 = 0 ; i_d2 < parent->getDaughters().size() ; ++i_d2 )
					{
						const EVENT::MCParticle *daughter2 = parent->getDaughters()[ i_d2 ];
						if ( daughter2 != daughter1 && daughter2->getPDG() == d2PDG ) isDecaydToDaughter = 1;
					}
				}
			}
		}
	}
	return isDecaydToDaughter;
}

int EventSelector::isZDecayedTo( const EVENT::LCCollection *MCParticleCollection , int parentPDG , int daughtersPDG , int daughter1index , int daughter2index )
{
	int elementFrom = 8;
	int elementTo = 20;
	int isDecaydToDaughter = 0;
	int d1PDG = abs( daughtersPDG );
	int d2PDG = ( abs( daughtersPDG ) < 20 ? -1 * d1PDG : d1PDG );
	for ( int i_d1 = elementFrom ; i_d1 < elementTo ; ++i_d1 )
	{
		const EVENT::MCParticle *daughter1 = dynamic_cast<EVENT::MCParticle*>( MCParticleCollection->getElementAt( i_d1 ) );
		if ( daughter1->getPDG() == d1PDG && i_d1 != daughter1index && i_d1 != daughter2index )
		{
			if ( daughter1->getParents().size() == 2 && abs( ( daughter1->getParents()[ 0 ] )->getPDG() ) == 11 && ( daughter1->getParents()[ 0 ] )->getPDG() == -1 * ( daughter1->getParents()[ 1 ] )->getPDG() )
			{
				const EVENT::MCParticle *parent = daughter1->getParents()[ 0 ];
				for ( unsigned int i_d2 = 0 ; i_d2 < parent->getDaughters().size() ; ++i_d2 )
				{
					const EVENT::MCParticle *daughter2 = parent->getDaughters()[ i_d2 ];
					if ( daughter2 != daughter1 && daughter2->getPDG() == d2PDG )
					{
						isDecaydToDaughter = 1;
						return isDecaydToDaughter;
					}
				}
			}
			else if ( daughter1->getParents().size() == 1 )
			{
				const EVENT::MCParticle *parent = daughter1->getParents()[ 0 ];
				if ( parent->getPDG() == parentPDG )
				{
					for ( unsigned int i_d2 = 0 ; i_d2 < parent->getDaughters().size() ; ++i_d2 )
					{
						const EVENT::MCParticle *daughter2 = parent->getDaughters()[ i_d2 ];
						if ( daughter2 != daughter1 && daughter2->getPDG() == d2PDG )
						{
							isDecaydToDaughter = 1;
							return isDecaydToDaughter;
						}
					}
				}
			}
		}
	}
	return isDecaydToDaughter;
}

void EventSelector::check( EVENT::LCEvent *pLCEvent )
{
	const EVENT::LCCollection *inJetCollection{};
	const EVENT::LCCollection *inIsoleptonCollection{};
	const EVENT::LCCollection *outPFOCollection{};
	const EVENT::LCCollection *outIsoleptonCollection{};
	try
	{
		inJetCollection = pLCEvent->getCollection( m_inputJetCollection );
		inIsoleptonCollection = pLCEvent->getCollection( m_inputIsoLepCollection );
		outPFOCollection = pLCEvent->getCollection( m_outputPfoCollection );
		outIsoleptonCollection = pLCEvent->getCollection( m_outputIsolepCollection );
		int nJets = inJetCollection->getNumberOfElements();
		int nInPFOs = 0;
		for ( int i_jet = 0 ; i_jet < nJets ; ++i_jet )
		{
			ReconstructedParticle* jet = dynamic_cast<ReconstructedParticle*>( inJetCollection->getElementAt( i_jet ) );
			nInPFOs += jet->getParticles().size();
		}
		int nInIsoLep = inIsoleptonCollection->getNumberOfElements();
		int nOutPFOs = outPFOCollection->getNumberOfElements();
		int nOutIsoLep = outIsoleptonCollection->getNumberOfElements();
		streamlog_out( DEBUG4 ) << "	" << nJets << " jets with " << nInPFOs << " PFOs and " << nInIsoLep << " Isolated Leptons converted to " << nOutPFOs << " PFOs and " << nOutIsoLep << " Isolated Leptons" << std::endl;
	}
	catch( DataNotAvailableException &e )
        {
          streamlog_out( WARNING ) << "	Input/Output collections not found in event: " << m_nEvt << std::endl;
        }
}

void EventSelector::end()
{
	if ( m_fillRootTree )
	{
		m_pTFile->cd();
		m_pTTree->Write();
		h_ZHDecayMode->Scale( 100.0 / n_ZHDecays );
		h_ZHDecayMode->Write();
		h_ZDecayMode->Scale( 100.0 / n_ZDecays );
		h_ZDecayMode->Write();
		m_pTFile->Close();
		delete m_pTFile;
	}
}
