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
	m_Ze3e3(0)
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
					"Include H->NueNue events",
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

	LCCollection *MCParticleCollection{};
	LCCollection *JetCollection{};
	LCCollection *IsoleptonCollection{};
}

void EventSelector::check( EVENT::LCEvent *pLCEvent )
{
}

void EventSelector::end()
{
}
