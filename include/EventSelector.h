#ifndef EventSelector_h
#define EventSelector_h 1
#include <marlin/Processor.h>
#include <marlin/Global.h>
#include "lcio.h"
#include "EVENT/LCStrVec.h"
#include "IMPL/LCCollectionVec.h"
#include "UTIL/LCRelationNavigator.h"
#include <EVENT/MCParticle.h>
#include "EVENT/ReconstructedParticle.h"
#include <IMPL/ReconstructedParticleImpl.h>
#include "IMPL/ParticleIDImpl.h"
#include "UTIL/PIDHandler.h"
#include <string>
#include <vector>
#include <math.h>
#include <set>
#include "TLorentzVector.h"
class TFile;
class TH1F;
class TH1I;
class TTree;
using namespace lcio ;
using namespace marlin ;

class EventSelector : public Processor
{
public:
	virtual Processor *newProcessor()
	{
		return new EventSelector;
	}
	EventSelector();
	virtual ~EventSelector() = default;
	EventSelector( const EventSelector& ) = delete;
	EventSelector &operator = ( const EventSelector& ) = delete;
	virtual void init();
	virtual void Clear();
	virtual void processRunHeader();
	virtual void processEvent( EVENT::LCEvent *pLCEvent );
	int isZHDecayedTo( const EVENT::LCCollection *MCParticleCollection , int parentPDG , int daughtersPDG , int &daughter1index , int &daughter2index );
	int isZDecayedTo( const EVENT::LCCollection *MCParticleCollection , int parentPDG , int daughtersPDG , int daughter1index , int daughter2index );
 	virtual void check( EVENT::LCEvent *pLCEvent );
	virtual void end();
private:

	typedef std::vector<int>		IntVector;
	typedef std::vector<double>		DoubleVector;
	typedef std::vector<float>		FloatVector;

	std::string				m_mcParticleCollection{};
	std::string				m_inputPfoCollection{};
	std::string				m_inputJetCollection{};
	std::string				m_inputIsoLepCollection{};
	std::string				m_outputPfoCollection{};
	std::string				m_outputIsolepCollection{};
	std::string				m_ZHdecayMode{};
	std::string				m_rootFile{};

	int					m_nRun;
	int					m_nEvt;
	int					m_nRunSum;
	int					m_nEvtSum;
	bool					m_fillRootTree = true;

	bool					m_includHbb = true;
	bool					m_includHcc = true;
	bool					m_includHss = true;
	bool					m_includHuu = true;
	bool					m_includHdd = true;
	bool					m_includHgg = true;
	bool					m_includHee = true;
	bool					m_includHmumu = true;
	bool					m_includHtautau = true;
	bool					m_includHnu1nu1 = true;
	bool					m_includHnu2nu2 = true;
	bool					m_includHnu3nu3 = true;
	bool					m_includHgammagamma = true;
	bool					m_includHWW = true;
	bool					m_includHZZ = true;
	bool					m_includHHH = true;
	bool					m_includHother = true;
	bool					m_includZe1e1 = true;
	bool					m_includZe2e2 = true;
	bool					m_includZe3e3 = true;
	int					m_nJets = 0;
	int					m_nIsoLeps = 0;
	double					m_diLepInvMass = 91.2;

	int					m_Hq1q1 = 0;
	int					m_Hq2q2 = 0;
	int					m_Hq3q3 = 0;
	int					m_Hq4q4 = 0;
	int					m_Hq5q5 = 0;
	int					m_Hgg = 0;
	int					m_He1e1 = 0;
	int					m_He2e2 = 0;
	int					m_He3e3 = 0;
	int					m_Hn1n1 = 0;
	int					m_Hn2n2 = 0;
	int					m_Hn3n3 = 0;
	int					m_Haa = 0;
	int					m_HWW = 0;
	int					m_HZZ = 0;
	int					m_HHH = 0;
	int					m_Hother = 0;
	int					m_totalHDecays = 0;
	int					m_Ze1e1 = 0;
	int					m_Ze2e2 = 0;
	int					m_Ze3e3 = 0;
	int					m_useEvent = 0;

	TFile					*m_pTFile{};
	TTree					*m_pTTree{};
	TH1I					*h_ZHDecayMode{};
	int					n_ZHDecays = 0;
	TH1I					*h_ZDecayMode{};
	int					n_ZDecays = 0;
};
#endif
