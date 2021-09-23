#ifndef EventSelector_h
#define EventSelector_h 1
#include <marlin/Processor.h>
#include <marlin/Global.h>
#include "lcio.h"
#include "EVENT/LCStrVec.h"
#include "IMPL/LCCollectionVec.h"
#include "UTIL/LCRelationNavigator.h"
#include <EVENT/MCParticle.h>
#include <IMPL/ReconstructedParticle.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <string>
#include <vector>
#include <math.h>
#include <set>
#include "TLorentzVector.h"
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

	bool					m_includbJets = true;
	bool					m_includcJets = true;
	bool					m_includgJets = true;
	bool					m_includOthers = true;
	bool					m_includbJets = true;
	bool					m_includcJets = true;
	bool					m_includgJets = true;
	bool					m_includOthers = true;
};
#endif
