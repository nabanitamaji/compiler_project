#ifndef INLINER_H
#define INLINER_H

#define DEBUG_TYPE "inline"
#include "llvm/Analysis/CallGraphSCCPass.h"

namespace llvm {
  class CallSite;

/// Inliner - This class contains all of the helper code which is used to
/// perform the inlining operations that does not depend on the policy.
///
struct Inliner : public CallGraphSCCPass {
  Inliner(const void *ID);

  /// getAnalysisUsage - For this class, we declare that we require and preserve
  /// the call graph.  If the derived class implements this method, it should
  /// always explicitly call the implementation here.
  virtual void getAnalysisUsage(AnalysisUsage &Info) const;

  // Main run interface method, this implements the interface required by the
  // Pass class.
  virtual bool runOnSCC(const std::vector<CallGraphNode *> &SCC);

  // doFinalization - Remove now-dead linkonce functions at the end of
  // processing to avoid breaking the SCC traversal.
  virtual bool doFinalization(CallGraph &CG);


  /// This method returns the value specified by the -inline-threshold value,
  /// specified on the command line.  This is typically not directly needed.
  ///
  unsigned getInlineThreshold() const { return InlineThreshold; }

  /// getInlineCost - This method must be implemented by the subclass to
  /// determine the cost of inlining the specified call site.  If the cost
  /// returned is greater than the current inline threshold, the call site is
  /// not inlined.
  ///
  virtual int getInlineCost(CallSite CS) = 0;

private:
  // InlineThreshold - Cache the value here for easy access.
  unsigned InlineThreshold;
};

} // End llvm namespace

#endif
