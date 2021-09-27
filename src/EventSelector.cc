#include "EventSelector.h"
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
	m_Ze1e1(0),
	m_Ze2e2(0),
	m_Ze3e3(0),
	m_useEvent(1)
{
	_description = "SLDCorrection finds semi-leptonic decays within jets and performs a correction to 4-momentum of the jet due to the missing neutrino(s)";

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
					std::string("declusteredJetPlusFakeIsolep")
				);

	registerOutputCollection(	LCIO::RECONSTRUCTEDPARTICLE,
					"outputIsolepCollection",
					"Name of output isolated lepton collection",
					m_outputIsolepCollection,
					std::string("IsolatedLeptons")
				);

	registerOutputCollection(	LCIO::MCPARTICLE,
					"physicsProcessCollection",
					"Name of Physics Process collection",
					m_physicsProcessCollection,
					std::string("physicsProcessCollection")
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

}

void EventSelector::init()
{

	streamlog_out(DEBUG) << "   init called  " << std::endl ;
	printParameters();
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
	m_Ze1e1 = 0;
	m_Ze2e2 = 0;
	m_Ze3e3 = 0;
	m_useEvent = 1;
	this->Clear();

}

void EventSelector::Clear()
{
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
	m_Ze1e1 = 0;
	m_Ze2e2 = 0;
	m_Ze3e3 = 0;
	m_useEvent = 1;
}

void EventSelector::processRunHeader()
{
	m_nRun = 0;
	m_nEvt = 0;
	++m_nRunSum;
}

void EventSelector::processEvent( EVENT::LCEvent *pLCEvent )
{
	m_nRun = pLCEvent->getRunNumber();
	m_nEvt = pLCEvent->getEventNumber();
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



		m_HHH = isDecayedTo( MCParticleCollection , 25 , 25 );
		m_HZZ = isDecayedTo( MCParticleCollection , 25 , 23 );
		m_HWW = isDecayedTo( MCParticleCollection , 25 , 24 );
		m_Hq5q5 = isDecayedTo( MCParticleCollection , 25 , 5 );
		m_Hq4q4 = isDecayedTo( MCParticleCollection , 25 , 4 );
		m_Hq3q3 = isDecayedTo( MCParticleCollection , 25 , 3 );
		m_Hq2q2 = isDecayedTo( MCParticleCollection , 25 , 2 );
		m_Hq1q1 = isDecayedTo( MCParticleCollection , 25 , 1 );
		m_Hgg = isDecayedTo( MCParticleCollection , 25 , 21 );
		m_He1e1 = isDecayedTo( MCParticleCollection , 25 , 11 );
		m_He2e2 = isDecayedTo( MCParticleCollection , 25 , 13 );
		m_He3e3 = isDecayedTo( MCParticleCollection , 25 , 15 );
		m_Hn1n1 = isDecayedTo( MCParticleCollection , 25 , 12 );
		m_Hn2n2 = isDecayedTo( MCParticleCollection , 25 , 14 );
		m_Hn3n3 = isDecayedTo( MCParticleCollection , 25 , 16 );
		m_Haa = isDecayedTo( MCParticleCollection , 25 , 22 );
		int totalHDecays = m_HHH + m_HZZ + m_HWW + m_Hq5q5 + m_Hq4q4 + m_Hq3q3 + m_Hq2q2 + m_Hq1q1 + m_Hgg + m_He1e1 + m_He2e2 + m_He3e3 + m_Hn1n1 + m_Hn2n2 + m_Hn3n3 + m_Haa;
		if ( totalHDecays == 0 ) m_Hother = 1;
		m_Ze1e1 = isDecayedTo( MCParticleCollection , 23 , 11 );
		m_Ze2e2 = isDecayedTo( MCParticleCollection , 23 , 13 );
		m_Ze3e3 = isDecayedTo( MCParticleCollection , 23 , 15 );

		askedDecayMode = ( ( m_HHH && m_includHHH ) || ( m_HZZ && m_includHZZ ) || ( m_HWW && m_includHWW ) || ( m_Hq5q5 && m_includHbb ) || ( m_Hq4q4 && m_includHcc ) || ( m_Hq3q3 && m_includHss ) || ( m_Hq2q2 && m_includHuu ) || ( m_Hq1q1 && m_includHdd ) || ( m_Hgg && m_includHgg ) || ( m_He1e1 && m_includHee ) || ( m_He2e2 && m_includHmumu ) || ( m_He3e3 && m_includHtautau ) || ( m_Hn1n1 && m_includHnu1nu1 ) || ( m_Hn2n2 && m_includHnu2nu2 ) || ( m_Hn3n3 && m_includHnu3nu3 ) || ( m_Haa && m_includHgammagamma ) || ( m_Hother && m_includHother ) );
		m_useEvent = ( askedDecayMode && trueNJets && trueNIsoLeps ? 1 : 0 );


		m_PhysProcCol->parameters().setValue( "Hdd" , ( int )m_Hq1q1 );
		m_PhysProcCol->parameters().setValue( "Huu" , ( int )m_Hq2q2 );
		m_PhysProcCol->parameters().setValue( "Hss" , ( int )m_Hq3q3 );
		m_PhysProcCol->parameters().setValue( "Hcc" , ( int )m_Hq4q4 );
		m_PhysProcCol->parameters().setValue( "Hbb" , ( int )m_Hq5q5 );
		m_PhysProcCol->parameters().setValue( "Hgg" , ( int )m_Hgg );
		m_PhysProcCol->parameters().setValue( "Hee" , ( int )m_He1e1 );
		m_PhysProcCol->parameters().setValue( "Hmumu" , ( int )m_He2e2 );
		m_PhysProcCol->parameters().setValue( "Htautau" , ( int )m_He3e3 );
		m_PhysProcCol->parameters().setValue( "Hnu1nu1" , ( int )m_Hn1n1 );
		m_PhysProcCol->parameters().setValue( "Hnu2nu2" , ( int )m_Hn2n2 );
		m_PhysProcCol->parameters().setValue( "Hnu3nu3" , ( int )m_Hn3n3 );
		m_PhysProcCol->parameters().setValue( "Hgammagamma" , ( int )m_Haa );
		m_PhysProcCol->parameters().setValue( "HWW" , ( int )m_HWW );
		m_PhysProcCol->parameters().setValue( "HZZ" , ( int )m_HZZ );
		m_PhysProcCol->parameters().setValue( "HHH" , ( int )m_HHH );
		m_PhysProcCol->parameters().setValue( "Hother" , ( int )m_Hother );
		m_PhysProcCol->parameters().setValue( "Zee" , ( int )m_Ze1e1 );
		m_PhysProcCol->parameters().setValue( "Zmumu" , ( int )m_Ze2e2 );
		m_PhysProcCol->parameters().setValue( "Ztautau" , ( int )m_Ze3e3 );
		m_PhysProcCol->parameters().setValue( "useEvent" , ( int )m_useEvent );

		pLCEvent->addCollection( m_PhysProcCol , m_physicsProcessCollection );
		pLCEvent->addCollection( m_IsolepCol , m_outputIsolepCollection );
		pLCEvent->addCollection( m_NewPFOsCol , m_outputPfoCollection );
	}

	catch( DataNotAvailableException &e )
        {
        	streamlog_out(MESSAGE) << "	Input collection not found in event " << m_nEvt << std::endl;
        }
}

int EventSelector::isDecayedTo( const EVENT::LCCollection *MCParticleCollection , int parentPDG , int daughtersPDG )
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
				if ( parentPDG == 25 )
				{
					if ( parent->getPDG() == d1PDG )
					{
						for ( unsigned int i_d2 = 0 ; i_d2 < parent->getDaughters().size() ; ++i_d2 )
						{
							const EVENT::MCParticle *daughter2 = parent->getDaughters()[ i_d2 ];
							if ( daughter2 != daughter1 && daughter2->getPDG() == d2PDG ) isDecaydToDaughter = 1;
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
}
