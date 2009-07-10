/**
 * @file ForwardSubsumptionAndResolution.cpp
 * Implements class ForwardSubsumptionAndResolution.
 */


#include "../Lib/VirtualIterator.hpp"
#include "../Lib/DArray.hpp"
#include "../Lib/List.hpp"
#include "../Lib/Comparison.hpp"
#include "../Lib/Metaiterators.hpp"

#include "../Kernel/Term.hpp"
#include "../Kernel/Clause.hpp"
#include "../Kernel/Inference.hpp"
#include "../Kernel/Matcher.hpp"
#include "../Kernel/MLMatcher.hpp"

#include "../Indexing/Index.hpp"
#include "../Indexing/LiteralIndex.hpp"
#include "../Indexing/LiteralMiniIndex.hpp"
#include "../Indexing/IndexManager.hpp"

#include "../Saturation/SaturationAlgorithm.hpp"

#include "../Lib/Environment.hpp"
#include "../Shell/Statistics.hpp"

#include "ForwardSubsumptionAndResolution.hpp"

extern bool reporting;

namespace Inferences
{
using namespace Lib;
using namespace Kernel;
using namespace Indexing;
using namespace Saturation;

void ForwardSubsumptionAndResolution::attach(SaturationAlgorithm* salg)
{
  CALL("SLQueryForwardSubsumption::attach");
  ForwardSimplificationEngine::attach(salg);
  _unitIndex=static_cast<UnitClauseSimplifyingLiteralIndex*>(
	  _salg->getIndexManager()->request(SIMPLIFYING_UNIT_CLAUSE_SUBST_TREE) );
  _fwIndex=static_cast<FwSubsSimplifyingLiteralIndex*>(
	  _salg->getIndexManager()->request(FW_SUBSUMPTION_SUBST_TREE) );
}

void ForwardSubsumptionAndResolution::detach()
{
  CALL("SLQueryForwardSubsumption::detach");
  _unitIndex=0;
  _fwIndex=0;
  _salg->getIndexManager()->release(SIMPLIFYING_UNIT_CLAUSE_SUBST_TREE);
  _salg->getIndexManager()->release(FW_SUBSUMPTION_SUBST_TREE);
  ForwardSimplificationEngine::detach();
}


struct ClauseMatches {
private:
  //private and undefined operator= and copy constructor to avoid implicitly generated ones
  ClauseMatches(const ClauseMatches&);
  ClauseMatches& operator=(const ClauseMatches&);
public:
  ClauseMatches(Clause* cl) : _cl(cl), _zeroCnt(cl->length())
  {
    unsigned clen=_cl->length();
    _matches=static_cast<LiteralList**>(ALLOC_KNOWN(clen*sizeof(void*), "Inferences::ClauseMatches"));
    for(unsigned i=0;i<clen;i++) {
      _matches[i]=0;
    }
  }
  ~ClauseMatches()
  {
    unsigned clen=_cl->length();
    for(unsigned i=0;i<clen;i++) {
      _matches[i]->destroy();
    }
    DEALLOC_KNOWN(_matches, clen*sizeof(void*), "Inferences::ClauseMatches");
  }

  void addMatch(Literal* baseLit, Literal* instLit)
  {
    addMatch(_cl->getLiteralPosition(baseLit), instLit);
  }
  void addMatch(unsigned bpos, Literal* instLit)
  {
    if(!_matches[bpos]) {
      _zeroCnt--;
    }
    LiteralList::push(instLit,_matches[bpos]);
  }
  void fillInMatches(LiteralMiniIndex* miniIndex)
  {
    unsigned blen=_cl->length();

    for(unsigned bi=0;bi<blen;bi++) {
      LiteralMiniIndex::InstanceIterator instIt(*miniIndex, (*_cl)[bi], false);
      while(instIt.hasNext()) {
	Literal* matched=instIt.next();
	addMatch(bi, matched);
      }
    }
  }
  bool anyNonMatched() { return _zeroCnt; }

  Clause* _cl;
  unsigned _zeroCnt;
  LiteralList** _matches;

  class ZeroMatchLiteralIterator
  {
  public:
    ZeroMatchLiteralIterator(ClauseMatches* cm)
    : _lits(cm->_cl->literals()), _mlists(cm->_matches), _remaining(cm->_cl->length())
    {
      if(!cm->_zeroCnt) {
	_remaining=0;
      }
    }
    bool hasNext()
    {
      while(_remaining>0 && *_mlists) {
	_lits++; _mlists++; _remaining--;
      }
      return _remaining;
    }
    Literal* next()
    {
      _remaining--;
      _mlists++;
      return *(_lits++);
    }
  private:
    Literal** _lits;
    LiteralList** _mlists;
    unsigned _remaining;
  };
};


typedef Stack<ClauseMatches*> CMStack;

bool isSubsumed(Clause* cl, CMStack& cmStore)
{
  CALL("isSubsumed");

  CMStack::Iterator csit(cmStore);
  while(csit.hasNext()) {
    ClauseMatches* clmatches;
    clmatches=csit.next();
    Clause* mcl=clmatches->_cl;

    if(clmatches->anyNonMatched()) {
      continue;
    }

    if(MLMatcher::canBeMatched(mcl,cl,clmatches->_matches,0)) {
      return true;
    }
  }
  return false;
}

Clause* generateSubsumptionResolutionClause(Clause* cl, Literal* lit, Clause* baseClause)
{
  CALL("generateSubsumptionResolutionClause");
  int clength = cl->length();
  int newLength = clength-1;

  Inference* inf = new Inference2(Inference::SUBSUMPTION_RESOLUTION, cl, baseClause);
  Unit::InputType inpType = (Unit::InputType)
  	max(cl->inputType(), baseClause->inputType());

  Clause* res = new(newLength) Clause(newLength, inpType, inf);

  int next = 0;
  bool found=false;
  for(int i=0;i<clength;i++) {
    Literal* curr=(*cl)[i];
    //As we will apply subsumption resolution after duplicate literal
    //deletion, the same literal should never occur twice.
    ASS(curr!=lit || !found);
    if(curr!=lit || found) {
	(*res)[next++] = curr;
    } else {
      found=true;
    }
  }

  res->setAge(cl->age());
  env.statistics->forwardSubsumptionResolution++;

  return res;
}

bool checkForSubsumptionResolution(Clause* cl, ClauseMatches* cms, Literal* resLit)
{
  Clause* mcl=cms->_cl;
  unsigned mclen=mcl->length();

  ClauseMatches::ZeroMatchLiteralIterator zmli(cms);
  if(zmli.hasNext()) {
    while(zmli.hasNext()) {
      Literal* bl=zmli.next();
//      if( !resLit->couldBeInstanceOf(bl, true) ) {
      if( ! MatchingUtils::match(bl, resLit, true) ) {
	return false;
      }
    }
  } else {
    bool anyResolvable=false;
    for(unsigned i=0;i<mclen;i++) {
//      if(resLit->couldBeInstanceOf((*mcl)[i], true)) {
      if( MatchingUtils::match((*mcl)[i], resLit, true) ) {
	anyResolvable=true;
	break;
      }
    }
    if(!anyResolvable) {
      return false;
    }
  }

  return MLMatcher::canBeMatched(mcl,cl,cms->_matches,resLit);
}

void ForwardSubsumptionAndResolution::perform(Clause* cl, ForwardSimplificationPerformer* simplPerformer)
{
  CALL("ForwardSubsumptionResolution::perform");

  Clause* resolutionClause=0;

  unsigned clen=cl->length();
  if(clen==0) {
    return;
  }


  for(unsigned li=0;li<clen;li++) {
    SLQueryResultIterator rit=_unitIndex->getGeneralizations( (*cl)[li], false, false);
    while(rit.hasNext()) {
      Clause* premise=rit.next().clause;
      if(simplPerformer->willPerform(premise)) {
	simplPerformer->perform(premise, 0);
	env.statistics->forwardSubsumed++;
	if(!simplPerformer->clauseKept()) {
	  return;
	}
      }
    }
  }

  LiteralMiniIndex miniIndex(cl);
  Clause::requestAux();

  static CMStack cmStore(64);
  ASS(cmStore.isEmpty());

  for(unsigned li=0;li<clen;li++) {
    SLQueryResultIterator rit=_fwIndex->getGeneralizations( (*cl)[li], false, false);
    while(rit.hasNext()) {
      SLQueryResult res=rit.next();
      Clause* mcl=res.clause;
      if(mcl->hasAux()) {
	//we've already checked this clause
	continue;
      }
      unsigned mlen=mcl->length();
      ASS_G(mlen,1);

      ClauseMatches* cms=new ClauseMatches(mcl);
      mcl->setAux(cms);
      cmStore.push(cms);
//      cms->addMatch(res.literal, (*cl)[li]);
//      cms->fillInMatches(&miniIndex, res.literal, (*cl)[li]);
      cms->fillInMatches(&miniIndex);

      if(cms->anyNonMatched()) {
        continue;
      }

      if(MLMatcher::canBeMatched(mcl,cl,cms->_matches,0) && simplPerformer->willPerform(mcl)) {
	simplPerformer->perform(mcl, 0);
	env.statistics->forwardSubsumed++;
	if(!simplPerformer->clauseKept()) {
	  goto fin;
	}
      }
    }
  }
  if(!_subsumptionResolution) {
    goto fin;
  }

  for(unsigned li=0;li<clen;li++) {
    Literal* resLit=(*cl)[li];
    SLQueryResultIterator rit=_unitIndex->getGeneralizations( resLit, true, false);
    while(rit.hasNext()) {
      Clause* mcl=rit.next().clause;
      if(simplPerformer->willPerform(mcl)) {
	resolutionClause=generateSubsumptionResolutionClause(cl,resLit,mcl);
	simplPerformer->perform(mcl, resolutionClause);
	if(!simplPerformer->clauseKept()) {
	  goto fin;
	}
      }
    }
  }

  {
    CMStack::Iterator csit(cmStore);
    while(csit.hasNext()) {
      ClauseMatches* cms=csit.next();
      for(unsigned li=0;li<clen;li++) {
	Literal* resLit=(*cl)[li];
	if(checkForSubsumptionResolution(cl, cms, resLit) && simplPerformer->willPerform(cms->_cl)) {
	  resolutionClause=generateSubsumptionResolutionClause(cl,resLit,cms->_cl);
	  simplPerformer->perform(cms->_cl, resolutionClause);
	  if(!simplPerformer->clauseKept()) {
	    goto fin;
	  }
	}
      }
    }
  }

  for(unsigned li=0;li<clen;li++) {
    Literal* resLit=(*cl)[li];	//resolved literal
    Set<Clause*> matchedClauses;
    SLQueryResultIterator rit=_fwIndex->getGeneralizations( resLit, true, false);
    while(rit.hasNext()) {
      SLQueryResult res=rit.next();
      Clause* mcl=res.clause;

      if(mcl->hasAux()) {
	//we have already examined this clause
	continue;
      }

      ClauseMatches* cms=new ClauseMatches(mcl);
      res.clause->setAux(cms);
      cmStore.push(cms);
      cms->fillInMatches(&miniIndex);

      if(checkForSubsumptionResolution(cl, cms, resLit) && simplPerformer->willPerform(cms->_cl)) {
	resolutionClause=generateSubsumptionResolutionClause(cl,resLit,cms->_cl);
	simplPerformer->perform(cms->_cl, resolutionClause);
	if(!simplPerformer->clauseKept()) {
	  goto fin;
	}
      }
    }
  }


fin:
  Clause::releaseAux();
  while(cmStore.isNonEmpty()) {
    delete cmStore.pop();
  }
}

}
