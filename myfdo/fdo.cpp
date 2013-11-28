#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
using namespace llvm;

#include <vector>
using namespace std;


namespace {

	class DynamicCG:public FunctionPass {
		public:
			static char ID;
			DynamicCG():FunctionPass(ID){}
			virtual bool runOnFunction(Function &);
	};
	bool DynamicCG::runOnFunction (Function &func) {
		for (inst_iterator inst = inst_begin(func); inst != inst_end(func); ++inst ){
			if(isa<CallInst>(*inst))  {
				Instruction *Instruction = &*inst;
				CallInst *callInstruction = static_cast<CallInst*>(Instruction);
				Function *callee = callInstruction->getCalledFunction();
				if(callee) {
					StringRef callerName = func.getName();
					StringRef calleeName = callee->getName();
					errs()<<"Caller: "<<callerName<<"  Callee : "<<calleeName<<"\n";
					Module *module = func.getParent();
					Constant *fdoHook = module->getOrInsertFunction("fdo_writer",
							Type::getVoidTy(module->getContext()),
							PointerType::get(IntegerType::get(module->getContext(), 8), 0),
							PointerType::get(IntegerType::get(module->getContext(), 8), 0),
							NULL);
					Function *fdoHookFunction = cast<Function>(fdoHook);
					vector<Value*> argList;
					Constant *callerNameValue = ConstantDataArray::getString(module->getContext(),
							callerName,true);
					Constant *calleeNameValue = ConstantDataArray::getString(module->getContext(),
							calleeName,true);
					PointerType *ptr = PointerType::get(IntegerType::get(module->getContext(),8),0);
					argList.push_back(CastInst::CreatePointerCast(static_cast<Value*>(callerNameValue),
								ptr,"callerCast",Instruction->getParent()));
					argList.push_back(CastInst::CreatePointerCast(static_cast<Value*>(calleeNameValue),
								ptr,"calleeCast",Instruction->getParent()));
					
				}      			
			}
		}
		return false;
	}
}                                                                                   



char DynamicCG::ID=0;
RegisterPass<DynamicCG> X("dcg","This is a dynamic call graphs function",false,false);
