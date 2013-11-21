#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Module.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/InlineCost.h"
#include "llvm/CallingConv.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Transforms/IPO/InlinerPass.h"




using namespace llvm;


namespace {
	class MyInliner:public Inliner {
		public:			
			static char ID;
			InlineCostAnalyzer *ICA;
			MyInliner():Inliner(ID),ICA(0){} 	
			virtual bool runOnSCC(CallGraphSCC &scc);
	};
}

bool MyInliner::runOnSCC(CallGraphSCC &scc) {
	return false;
}


char MyInliner::ID = 0;
static RegisterPass<MyInliner> X("ml","This is a custom inliner",false,false);
