#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/InlineCost.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Transforms/IPO/InlinerPass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/InstIterator.h"

#include "freq.h"

using namespace llvm;
using namespace arijit;

namespace {
	class MyInliner:public Inliner {
		private:
			InlineCostAnalysis *ICA;
			InlineCost alwaysInline(CallSite CS);
		        Freq freq;
		public:			
			static char ID;
			//MyInliner():Inliner(ID),ICA(0),TTI(0),TD(0){} 	
			MyInliner():Inliner(ID),ICA(0){} 	
			virtual InlineCost getInlineCost(CallSite cs);
			virtual bool runOnSCC(CallGraphSCC &scc);
			virtual void getAnalysisUsage(AnalysisUsage &AN) const; 

		private:
			int getInstrCount(Function *func);
			InlineCost filterInline(CallSite cs);
	};



	InlineCost MyInliner::alwaysInline(CallSite CS) {
		Function *Callee = CS.getCalledFunction();
		if(Callee && !Callee->isDeclaration() &&
			         Callee->getAttributes().hasAttribute(
			        	AttributeSet::FunctionIndex,
			        	Attribute::AlwaysInline)  &&
			       // Callee->getAttributes() &&
			        ICA->isInlineViable(*Callee)) {
			errs()<<"Function that is being inlined "<<Callee->getName()<<"\n";
			return InlineCost::getAlways();
		}
		return InlineCost::getNever();
	}

	int MyInliner::getInstrCount(Function *func) {
		int count = 0;
		for(inst_iterator itr = inst_begin(func) ; itr != inst_end(func); ++itr) {
			count++;
		}
		return count;
	}

	InlineCost MyInliner::filterInline(CallSite cs) {
		Function *caller=  cs.getCaller();
		Function *callee = cs.getCalledFunction();
		
		//Never Inline if the Callee does not exists
		if(!callee) {
			return InlineCost::getNever();			
		}
		//Never Inline if the construct is not proper for inling
		if(!ICA->isInlineViable(*callee)) {
			return InlineCost::getNever();
		}
                if(callee->hasFnAttribute(Attribute::NoInline) || callee->mayBeOverridden()|| cs.isNoInline()) {
			return InlineCost::getNever();
		}

		//Now the job of the CallAnalyzer should be implemented
	}


        InlineCost MyInliner::getInlineCost(CallSite cs) {
        
		InlineCost filterCost = filterInline(cs);
		if(filterCost.isNever()) {
		 	return InlineCost::getNever();
		}

		const char *callerName=  cs.getCaller()->getName().data();
                const char *calleeName=  cs.getCalledFunction()->getName().data();
		int frq = freq.getFreq(callerName,calleeName);
		int size = cs.getCalledFunction()->size();
		int instrCount = getInstrCount(cs.getCalledFunction());
		errs() <<" Instruction count  : "<<instrCount<<"\n";
		
		if(frq > 30  && instrCount < 50 ) {
			errs()<<"Function being called more than 30 : "<<callerName<<"  :  "<<calleeName<<" Freq : "<<frq<<" Size : "<<size<<"\n";
			return InlineCost::getAlways();
		}

		return InlineCost::getNever();
	}

	bool MyInliner::runOnSCC(CallGraphSCC &scc) {
		ICA = &getAnalysis<InlineCostAnalysis>();
		//const TargetTransformInfo &TTI = getAnalysis<TargetTransformInfo>();
		//const DataLayout *TD = getAnalysisIfAvailable<DataLayout>();
		//CallAnalyzer CA (TD,*TTI,callee,225);
		return Inliner::runOnSCC(scc);
	}

	void MyInliner::getAnalysisUsage(AnalysisUsage &AU) const {
		AU.addRequired<InlineCostAnalysis>() ;
		AU.addRequired<TargetTransformInfo>();
		Inliner::getAnalysisUsage(AU);
	}

}

char MyInliner::ID = 0;
static RegisterPass<MyInliner> X("ml","This is a custom inliner",false,false);
