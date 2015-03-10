/**
 * @file ClauseQueue.cpp
 * Implements class ClauseQueue of clause priority queues
 * @since 30/12/2007 Manchester
 */

#include "Debug/Tracer.hpp"

#include "Lib/Allocator.hpp"
#include "Lib/Random.hpp"
#include "Lib/Environment.hpp"
#include "MainLoopContext.hpp"

#if VDEBUG
#include "Clause.hpp"
#endif

#include "ClauseQueue.hpp"

using namespace Lib;
using namespace Kernel;

/** Temporary!!! */
ClauseQueue::~ClauseQueue ()
{
  CALL("ClauseQueue::~ClauseQueue");

  removeAll();
} // ClauseQueue::~ClauseQueue

/**
 * Bind @b v to @b t.
 * @pre @b v must previously be unbound
 */
void ClauseQueue::insert(Clause* c)
{
  CALL("ClauseQueue::insert");

  // select a random height between 0 and top
  unsigned h = 0;
  while (Random::getBit()) {
    h++;
  }
  if (h > _height) {
    if (_height < MAX_HEIGHT) {
      _height++;
    }
    h = _height;
    _left.nodes[h] = 0;
  }
  void* mem = ALLOC_KNOWN_LOCAL(sizeof(Node)+h*sizeof(Node*),"ClauseQueue::Node");
  Node* newNode = reinterpret_cast<Node*>(mem);
  newNode->clause = c;

  // left is a node with a value smaller than that of newNode and having
  // a large enough height.
  // this node is on the left of the inserted one
  Node* left = &_left;
  // lh is the height on which we search for the next node
  unsigned lh = _height;
  for (;;) {
    Node* next = left->nodes[lh];
    if (next == 0 || lessThan(c,next->clause)) {
      if (lh <= h) {
	left->nodes[lh] = newNode;
	newNode->nodes[lh] = next;
      }
      if (lh == 0) {
	return;
      }
      lh--;
      continue;
    }
    left = next;
  }
} // ClauseQueue::insert

/**
 * Remove the clause c from the queue.
 * @since 30/12/2007 Manchester
 */
bool ClauseQueue::remove(Clause* c)
{
  CALL("ClauseQueue::remove");

  unsigned h = _height;
  Node* left = &_left;

  for (;;) {
    Node* next = left->nodes[h];
    if (next && c == next->clause) {
      unsigned height = h;
      // found, first change the links going to next
      for (;;) {
	left->nodes[h] = next->nodes[h];
	if (h == 0) {
	  break;
	}
	h--;
	while (left->nodes[h] != next) {
	  left = left->nodes[h];
	}
      }
      // deallocate the node
      DEALLOC_KNOWN_LOCAL(next,
		    sizeof(Node)+height*sizeof(Node*),
                    "ClauseQueue::Node");
      while (_height > 0 && ! _left.nodes[_height]) {
	_height--;
      }
      return true;
    }

    if (next == 0 || lessThan(c,next->clause)) {
      if(h==0) {
	return false;
      }
      h--;
    }
    else {
      left = next;
    }
  }
} // ClauseQueue::remove


/**
 * Remove the leftmost clause c from the queue.
 * @since 30/12/2007 Manchester
 */
Clause* ClauseQueue::pop()
{
  CALL("ClauseQueue::pop");
  ASS(_height >= 0);
  ASS(_left.nodes[0] != 0);

  Node* node = _left.nodes[0];
  unsigned h = 0;
  _left.nodes[0] = node->nodes[0];
  while (h < _height && _left.nodes[h+1] == node) {
    h++;
    _left.nodes[h] = node->nodes[h];
  }
  // now h is the height of the node
  Clause* c = node->clause;

  // deallocate the node
  DEALLOC_KNOWN_LOCAL(node,
		sizeof(Node)+h*sizeof(Node*),
                "ClauseQueue::Node");
  while (_height > 0 && ! _left.nodes[_height]) {
    _height--;
  }
  return c;
} // ClauseQueue::pop

/**
 * Remove all clauses from the queue.
 * @since 31/12/2007 Manchester
 */
void ClauseQueue::removeAll()
{
  CALL("ClauseQueue::removeAll");

  while (_left.nodes[0]) {
    pop();
  }
} // removeAll

#if VDEBUG
void ClauseQueue::output(ostream& str) const
{
  if (_height < 0) {
    return;
  }
  for (const Node* node = _left.nodes[0]; node; node=node->nodes[0]) {
    str << node->clause->toString() << '\n';
  }
} // ClauseQueue::output
#endif
