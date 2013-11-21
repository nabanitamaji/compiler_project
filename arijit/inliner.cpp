#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Module.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/InlineCost.h"
#include "llvm/CallingConv.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Transforms/IPO/InlinerPass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
	class MyInliner:public Inliner {
		public:			
			static char ID;
			InlineCostAnalyzer *ICA;
			MyInliner():Inliner(ID),ICA(0){} 	
			virtual InlineCost getInlineCost(CallSite CS);
			virtual bool runOnSCC(CallGraphSCC &scc);
			virtual void getAnalysisUsage(AnalysisUsage &AN);
	};


	InlineCost MyInliner::getInlineCost(CallSite cs) {
		errs()<<"Inline Cost Threshold is : "<<getInlineThreshold(cs)<<"\n";
		CallInst *callInst = reinterpret_cast<CallInst*>(cs.getInstruction());
		Function *callee = cs.getCalledFunction();
		errs()<<"Caller  function " <<callee->getName()<<"\n";
		return ICA->getInlineCost(cs,getInlineThreshold(cs));
	}

	bool MyInliner::runOnSCC(CallGraphSCC &scc) {
		ICA = &getAnalysis<InlineCostAnalyzer>();
		return Inliner::runOnSCC(scc);
		//  return false;
		//
	}

	void MyInliner::getAnalysisUsage(AnalysisUsage &AU) {
		AU.addRequired<InlineCostAnalyzer>() ;
		Inliner::getAnalysisUsage(AU);
	}

}

char MyInliner::ID = 0;
static RegisterPass<MyInliner> X("ml","This is a custom inliner",false,false);
