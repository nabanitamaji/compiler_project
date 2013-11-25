#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/InlineCost.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Transforms/IPO/InlinerPass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/StringRef.h"

using namespace llvm;

namespace {
	class MyInliner:public Inliner {
		private:
			InlineCost alwaysInline(CallSite CS);
		public:			
			static char ID;
			InlineCostAnalysis *ICA;
			MyInliner():Inliner(ID),ICA(0){} 	
			virtual InlineCost getInlineCost(CallSite cs);
			virtual bool runOnSCC(CallGraphSCC &scc);
			virtual void getAnalysisUsage(AnalysisUsage &AN) const; 
	};


	InlineCost MyInliner::alwaysInline(CallSite CS) {
		Function *Callee = CS.getCalledFunction();
		if(Callee && !Callee->isDeclaration() &&
			       // Callee->getAttributes()
			       ICA->isInlineViable(*Callee)) {
			errs()<<"Function that is being inlined "<<Callee->getName()<<"\n";
			return InlineCost::getAlways();
		}
		return InlineCost::getNever();
	}

	InlineCost MyInliner::getInlineCost(CallSite cs) {
		return alwaysInline(cs);
		/**
		 * This function is the one written in always inline function
		 */
		

       // 	Function *callee = cs.getCalledFunction();

       // 	//CallAnalyzer's Analyze call actuall determines to the major extent
       // 	// We will not do inlinig.
       // 	
       // 	//Do not inline if the callee does not exists
       // 	if(!callee) {
       // 		return InlineCost::getNever();
       // 	}
       //         errs()<<"Always inlining"<<"\n";
       // 	return InlineCost::getAlways();
       //        
       // 	//Do inline if the inline keyword is present
       // 	if(callee->hasFnAttribute(Attribute::AlwaysInline)) {
       // 		if(ICA->isInlineViable(*callee)) {
       // 			return InlineCost::getAlways();
       // 		}
       // 		return InlineCost::getNever();
       // 	}

       // 	//Do not Inline if recursive
       // 	bool isRecursive= false;
       // 	Function *caller = cs.getCaller();
       // 	for(Value::use_iterator  U = caller->use_begin(), E = caller->use_end();U != E ; ++U) {
       // 		CallSite site(cast<Value>(*U)) ;
       // 		if(!site)
       // 			continue;
       // 		Instruction *inst = site.getInstruction();
       // 		if(inst->getParent()->getParent() == caller) {
       // 			isRecursive = true;
       // 			break;
       // 		}
       // 	}

       // 	if(isRecursive) {
       // 		return InlineCost::getNever();	
       // 	}

       // 	//Get the information from the call Analyzer
       // 	
       // 	return InlineCost::getAlways();
       // 		       
       // 	/*
       // 	StringRef f("f1");
       // 	errs()<<"Caller  function " <<callee->getName()<<"\n";
       // 	if(callee->getName() == f) {
       // 	//return ICA->getInlineCost(cs,getInlineThreshold(cs));
       // 		return InlineCost::getAlways();
       // 	}
       // 	return InlineCost::getNever();

       //         */
		
	}

	bool MyInliner::runOnSCC(CallGraphSCC &scc) {
		ICA = &getAnalysis<InlineCostAnalysis>();
		return Inliner::runOnSCC(scc);
	}

	void MyInliner::getAnalysisUsage(AnalysisUsage &AU) const {
		AU.addRequired<InlineCostAnalysis>() ;
		Inliner::getAnalysisUsage(AU);
	}



}

char MyInliner::ID = 0;
static RegisterPass<MyInliner> X("ml","This is a custom inliner",false,false);
