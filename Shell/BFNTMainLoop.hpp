/**
 * @file BFNTMainLoop.hpp
 * Defines class BFNTMainLoop.
 */

#ifndef __BFNTMainLoop__
#define __BFNTMainLoop__

#include "Lib/Portability.hpp"

#include "Forwards.hpp"

#include "Lib/SmartPtr.hpp"

#include "Kernel/MainLoop.hpp"

#include "BFNT.hpp"
#include "Options.hpp"

namespace Shell {

using namespace Lib;
using namespace Kernel;

class BFNTMainLoop : public MainLoop {
public:
  BFNTMainLoop(Problem& prb, const Options& opt);

protected:
  virtual void init();
  virtual MainLoopResult runImpl();

private:

#if !COMPILER_MSVC
  void runChild(size_t modelSize) __attribute__((noreturn));
  MainLoopResult spawnChild(size_t modelSize);

  Options _childOpts;
  /** the input transformer */
  BFNT _bfnt;
#endif
};

}

#endif // __BFNTMainLoop__