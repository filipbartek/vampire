/**
 * @file SimpleCongruenceClosure.hpp
 * Defines class SimpleCongruenceClosure.
 */

#ifndef __SimpleCongruenceClosure__
#define __SimpleCongruenceClosure__

#include "Forwards.hpp"

#include "DecisionProcedure.hpp"

namespace DP {

using namespace Lib;
using namespace Kernel;



class SimpleCongruenceClosure : public DecisionProcedure
{
public:
  SimpleCongruenceClosure();

  virtual void addLiterals(LiteralIterator lits);

  virtual Status getStatus();
  virtual void getUnsatisfiableSubset(LiteralStack& res);

private:
  /**
   * Constant pair
   *
   * Is used for the shallow representation of the currifyed terms.
   */
  typedef pair<unsigned,unsigned> CPair;
  /** Equality between two constants */
  struct CEq
  {
    CEq() : c1(0), c2(0) {}
    CEq(unsigned c1, unsigned c2, Literal* lit)
     : c1(c1), c2(c2), foOrigin(true), foPremise(lit) {}
    CEq(unsigned c1, unsigned c2)
     : c1(c1), c2(c2), foOrigin(false) {}

    bool isInvalid() const { ASS_EQ(c1==0, c2==0); return c1==0; }
    string toString() const;
    string toString(SimpleCongruenceClosure& parent) const;

    unsigned c1;
    unsigned c2;
    /**
     * If true, equality comes from @c foPremise, if false,
     * it follows from a congruence between pairs represented by
     * @c c1 and @c c2.
     */
    bool foOrigin;
    /** if 0, the equality is the true!=false equality to help represent non-equality literals */
    Literal* foPremise;

  };

  unsigned getMaxConst() const { return _cInfos.size()-1; }
  unsigned getFreshConst();
  unsigned getSignatureConst(unsigned symbol, bool funct);
  unsigned getPairName(CPair p);


  struct FOConversionWorker;
  CEq convertFOEquality(Literal* equality);
  unsigned convertFONonEquality(Literal* lit);
  unsigned convertFO(TermList trm);


  unsigned deref(unsigned c) const {
    CALL("SimpleCongruenceClosure::deref");
    unsigned repr = _cInfos[c].reprConst;
    unsigned res = (repr==0) ? c : repr;
    ASS_REP2(_cInfos[res].reprConst==0, _cInfos[res].reprConst, c);
    return res;
  }
  CPair deref(CPair p) const { return CPair(deref(p.first), deref(p.second)); }
  CPair deref(CEq p) const { return CPair(deref(p.c1), deref(p.c2)); }

  unsigned getClassSize(unsigned c) const {
    return _cInfos[c].classList.size();
  }

  void addPendingEquality(CEq eq);
  void makeProofRepresentant(unsigned c);
  void propagate();

  unsigned getProofDepth(unsigned c);
  void collectUnifyingPath(unsigned c1, unsigned c2, Stack<unsigned>& path);


  static const unsigned NO_SIG_SYMBOL;
  struct ConstInfo
  {
    void init() {
      sigSymbol = NO_SIG_SYMBOL;
      term.makeEmpty();
      lit = 0;
      namedPair = CPair(0,0);
      reprConst = 0;
      proofPredecessor = 0;
    }


    /** If NO_SIG_SYMBOL, the constant doesn't represent a non-constant signature symbol */
    unsigned sigSymbol;
    bool sigSymIsFunct;
    /** If isEmpty, the constant doesn't represent a term */
    TermList term;
    /** If non-zero, constant stands for a non-equality atom */
    Literal* lit;
    /** (0,0) means the constant doesn't name a pair */
    CPair namedPair;


    /** 0 means the symbol is its own representant */
    unsigned reprConst;

    /**
     * A parent in an union-find structure without path-compression
     *
     * (used for proof construction)
     */
    unsigned proofPredecessor;
    /**
     * Premise that justified union with proofPredecessor
     */
    CEq predecessorPremise;


    /** If reprConst==0, contains other constants whose representative
     * this constant is */
    Stack<unsigned> classList;
    /** If reprConst==0, contains list of pair names in whose pairs this
     * constant appears as a representative of one of the arguments */
    Stack<unsigned> useList;
  };

  /**
   * Information on constants used in the algorithm
   *
   * The element at index 0 is ignored, as number 0 is used in place
   * of constants in special cases, such as to indicate that there is
   * no constant.
   */
  DArray<ConstInfo> _cInfos;

  /** Positive literals are made equivalent to this constant */
  unsigned _posLitConst;
  /** Negative literals are made equivalent to this constant */
  unsigned _negLitConst;

  /**
   * Map from signature symbols to the local constant numbers.
   * (if the bool is true, symbol si function, otherwise a predicate).
   */
  DHMap<pair<unsigned,bool>,unsigned> _sigConsts;

  /** Names of constant pairs */
  DHMap<CPair,unsigned> _pairNames;

  /** Constants corresponding to terms */
  DHMap<TermList,unsigned> _termNames;

  /**
   * Equality that caused unsatisfiability; if CEq::isInvalid(), there isn't such.
   */
  CEq _unsatEq;

  Stack<CEq> _pendingEqualities;

  Stack<CEq> _negEqualities;

};

}

#endif // __SimpleCongruenceClosure__
