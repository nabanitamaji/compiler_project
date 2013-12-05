#include "llvm/Pass.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

namespace {

    class loopdetector : public ModulePass {
    public:
  	static char ID;
	loopdetector(ID):ModulePass(ID){};
	virtual bool runOnModule(Module &M);
	virtual void getAnalysisUsage(AnalysisUsage &AU) const{ 
	        AU.addRequired<LoopInfo>();
	}
    }; 

 
    bool loopdetector::runOnModule(Module &M) {
	ofstream denyfile;
	denyfile.open("denyfile.txt");
    	for (Module::iterator I = M.begin(), E = M.end(); I != E; ++I){
		Function *F=&(*I);
		LoopInfo &LI = getAnalysis<LoopInfo>(*F) ;
   		for (Function::iterator BB = F->begin(), BBE = F->end(); BB != BBE; ++BB){
			if (LI.getLoopFor(BB)){
				denyfile<<F->getName();
				break;
			}
			if(F->hasFnAttribute(Attribute::NoInline)){
				denyfile<<F->getName();
				break;
			}
		}
 	}	
	denyfile.close();
   	return false;
    }

}

char loopdetector::ID= 0;
static RegisterPass<loopdetector> Y("ld","detect loops" ,false,false);
