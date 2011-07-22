/**
 * @file ArithmeticIndex.hpp
 * Defines class ArithmeticIndex.
 */

#ifndef __ArithmeticIndex__
#define __ArithmeticIndex__

#include "Forwards.hpp"

#include "Lib/DHMap.hpp"

#include "Kernel/Theory.hpp"

#include "Kernel/Algebra/ArithmeticKB.hpp"

#include "Index.hpp"

namespace Indexing {

#if 0

using namespace Lib;
using namespace Kernel;
using namespace Kernel::Algebra;

class ConstraintDatabase
: public ArithmeticKB
{
public:
  ConstraintDatabase();

  void handleLiteral(Literal* lit, bool adding, Clause* premise, bool negative=false);

  //overrides ArithmeticKB::isNonEqual
  bool isNonEqual(TermList t, InterpretedType val, Clause*& premise);
  //overrides ArithmeticKB::isGreater
  bool isGreater(TermList t, InterpretedType val, Clause*& premise);
  //overrides ArithmeticKB::isLess
  bool isLess(TermList t, InterpretedType val, Clause*& premise);

  void reset()
  { _termConstraints.reset(); }

private:
  
  struct ConstraintInfo;
  
  Theory* theory;
  DHMap<TermList, ConstraintInfo*> _termConstraints;
};


class ArithmeticIndex
: public Index
{
public:
  ArithmeticIndex();

  void handleClause(Clause* c, bool adding);

  bool isNonEqual(TermList t, InterpretedType val, Clause*& premise)
  { return _db.isNonEqual(t, val, premise); }
  bool isGreater(TermList t, InterpretedType val, Clause*& premise)
  { return _db.isGreater(t, val, premise); }
  bool isLess(TermList t, InterpretedType val, Clause*& premise)
  { return _db.isLess(t, val, premise); }


private:
  ConstraintDatabase _db;
};
#endif
}

#endif // __ArithmeticIndex__
