/**
 * @file LingelingInterfacing.hpp
 * Defines class LingelingInterface
 */
#ifndef __LingelingInterfacing__
#define __LingelingInterfacing__

#include "Forwards.hpp"

#include "Lib/Array.hpp"
#include "Lib/ArrayMap.hpp"
#include "Lib/DArray.hpp"
#include "Lib/Deque.hpp"
#include "Lib/Exception.hpp"
#include "Lib/ScopedPtr.hpp"
#include "Lib/Stack.hpp"

#include "SATSolver.hpp"
#include "SATLiteral.hpp"
#include "SATClause.hpp"

#include <csignal>

extern "C"{
	#include "lglib.h"
}
namespace SAT{

	using namespace Lib;
	using namespace Shell;

class LingelingInterfacing : public SATSolver
{
public: 
	//constructor for the instantiation of Lingeling
	LingelingInterfacing(const Options& opts, bool generateProofs=false);
	~LingelingInterfacing();

	/**
	* The add clause is the incremental way for the lingeling sat solver. It is used in order to add new clause
	* to the current problem
	**/
	virtual void addClauses(SATClauseIterator clauseIterator, bool onlyPropagate=false);
	
	/**
	* return the current status of the problem
	*/
	virtual Status getStatus(){
		CALL("LingelingInterfacing::getStatus()");
		return _status;
	};

	/**
	* In case the status of the problem is SATISFIABLE, then return the assigned value for var
	*/
	virtual VarAssignment getAssignment(unsigned var);

	/**
	* Try to find another assignment which is likely to be different from the current one. 
	* 
	* as a precondition, the solver must have SATISFIABLE status
	*/
	virtual void randomizeAssignment();
	
	/**
	* In case the solver has status SATISFIABLE and the assignment of @c var was done at level 1, 
	* return 1.  
	* 
	*/
	virtual bool isZeroImplied(unsigned var){}

	/**
	* collect all the zero-implied variables 
	* should be used only for SATISFIABLE or UNKNOWN
	*/
	virtual void collectZeroImplied(SATLiteralStack& acc){}

	/**
   	* Return a valid clause that contains the zero-implied literal
   	* and possibly the assumptions that implied it. Return 0 if @c var
   	* was an assumption itself.
   	* If called on a proof producing solver, the clause will have
   	* a proper proof history.
   	*/
	virtual SATClause* getZeroImpliedCertificate(unsigned var){}

	/**
	* in the original solver this function took care of increasing the memory allocated for the
	* variable representation, clauses and so on.
	*/
	virtual void ensureVarCnt(unsigned newVarCnt){}

	/**
	*Adds an assumption to the solver. 
	* If conflictingCountLimit == 0 => do only unit propagation 
	* if conflictingCountLimit == UINT_MAX => do full satisfiability check
	* if the value is in between, then simply set that as the upper bound on conflictCountLimit
	*/
	virtual void addAssumption(SATLiteral literal, unsigned conflictCountLimit);

	/**
	* Retracts all the assumption made until now.
	*/
	virtual void retractAllAssumptions();

	/**
	* check if there is at least one assumption made until now
	*/
	virtual bool hasAssumptions() const;

	/**
	* get the refutation
	*/
	virtual SATClause* getRefutation();


	void testLingeling();


private: 
	virtual void addClausesToLingeling(SATClauseIterator iterator);
	void setSolverStatus(unsigned status);
	/**
	* Status of the solver 
	*/
	Status _status;
	/**
	* flag which enables proof generation
	*/
	bool _generateProofs;
	bool _hasAssumptions;
	//scoped pointer to the incremental lingleling 
	LGL * _solver;
};

}//end SAT namespace

 #endif /*LingelingInterface*/