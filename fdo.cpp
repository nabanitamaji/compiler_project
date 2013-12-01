#include "llvm/Pass.h"
#include <stdio.h> 
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/CallSite.h"
#include "llvm/IR/InstrTypes.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace llvm;

namespace {
	struct fdo :public ModulePass {
		static char ID ;
		int fcntr;
		int **callgraph;
		const char** function_name_list;
		int *function_size_list;
		fdo():ModulePass(ID){};
		bool traverseFunction(Function *F);
		virtual bool runOnModule(Module &M); 
		void CG_initiate();
		int get_index(const char*);
		void CG_add(const char*,const char*);
		void CG_Dump();
	};                                                                                               
 
}

bool fdo::runOnModule(Module &M)
{
	int i;
	fcntr=0;
	for (Module::iterator I = M.begin(), E = M.end(); I != E; ++I){
		Function *F=&*I;
		if(F->getName()!="gen_profile" && F->getName()!="profile_dump" && F->size() > 0)
			fcntr++;
	}
	function_name_list=(const char **)malloc(fcntr*sizeof(char *));
	function_size_list=(int *)malloc(fcntr*sizeof(int));
	i=0;
	for (Module::iterator I = M.begin(), E = M.end(); I != E; ++I,++i)	
	{
		Function *F=&*I;
		if(F->getName()=="gen_profile" || F->getName()=="profile_dump" || F->size() <= 0){
			--i;
			continue;
		}
		function_name_list[i]=F->getName().data();
		function_size_list[i]=F->size();
	}
	CG_initiate();
	for (Module::iterator I = M.begin(), E = M.end(); I != E; ++I)	
	{
		Function *F=&*I;
		traverseFunction(F);
	}
	CG_Dump();
	return false;			
}

void fdo::CG_initiate()
{
	int i,j;
	
	callgraph=(int **)malloc(fcntr*sizeof(int *));
	for(i=0;i<fcntr;i++){
		callgraph[i]=(int *)malloc(fcntr*sizeof(int));
		for(j=0;j<fcntr;j++)
			callgraph[i][j]=0;
	}
	
/*	for(i=0;i<fcntr;i++)
		errs()<<" "<<i<<":"<<function_name_list[i]<<":"<<function_size_list[i];
	for(i=0;i<fcntr;i++)
	{
		errs()<<"\n";
		for(j=0;j<fcntr;j++)
			errs()<<" "<<callgraph[i][j];
	}
		
	errs()<<"\n------------------\n";	
*/		
}

int fdo::get_index(const char* fname)
{
	int i,pos=-1;
	for(i=0;i<fcntr;i++)
	{
		if(strcmp(function_name_list[i],fname)==0){
			pos=i;
			break;
		}
	}
	return pos;
}

void fdo::CG_add(const char* from, const char *to)
{
	int row, col;
	row=get_index(from);
	col=get_index(to);
	if(row==-1 || col == -1)
		return;
	callgraph[row][col]+=1;
/*	for(i=0;i<fcntr;i++)
		errs()<<" "<<i<<":"<<function_name_list[i]<<":"<<function_size_list[i];
	for(i=0;i<fcntr;i++)
	{
		errs()<<"\n";
		for(j=0;j<fcntr;j++)
			errs()<<" "<<callgraph[i][j];
	}
		
	errs()<<"\n------------------\n";	*/
}

void fdo::CG_Dump()
{
	const string fileName = "static_callgraph_state";
        ofstream file;
	file.open(fileName.c_str());
	if(file.is_open()) {
		for(int i  = 0 ; i < fcntr; ++i ) {   
			file<<function_name_list[i]<<"\t";
		}       	
		file<<"\n";
		file<<"\n";
		for(int i  = 0 ; i < fcntr; ++i ) {
                           file<<function_size_list[i] <<"\t";
		}       	
		file<<"\n";
		file<<"\n";
		for(int i  = 0 ; i < fcntr; ++i ) {
			for(int j = 0 ; j< fcntr; ++j) {
                           file<<callgraph[i][j]<<"\t";
			}
			file<<"\n";
		}       	
		file<<"\n";
		file.close();
	}
}

bool fdo::traverseFunction(Function *F) {
	if (F->getName()=="gen_profile" || F->getName()=="profile_dump"|| F->size()<=0) return false;
		const char * from=F->getName().data();
	const char * to;
	Module *M =F->getParent();
	
	for (Function::iterator BB = F->begin(), BBE = F->end(); BB != BBE; ++BB){
      		for (BasicBlock::iterator II = BB->begin(), IE = BB->end() ; II!= IE ; ++II){
        		CallSite CS(cast<Value>(II));
        		if (CS) {
				 Function *Fn = CS.getCalledFunction();
          			 if(Fn && Fn->getName().size()!=0)
					to=CS.getCalledFunction()->getName().data();
				 else 
					continue;
				CG_add(from,to);	
				Constant * Init_to = ConstantDataArray::getString (II->getContext(),to);
				Constant * Init_from = ConstantDataArray::getString (II->getContext(),from);
				Value * To=new GlobalVariable (*M,Init_to->getType(),true,GlobalValue::InternalLinkage,Init_to,"callee");
				Value * From=new GlobalVariable (*M,Init_from->getType(),true,GlobalValue::InternalLinkage,Init_from,"caller");
				Constant* profile = M->getOrInsertFunction("gen_profile",Type::getVoidTy(M->getContext()),PointerType::get(IntegerType::get(M->getContext(), 8), 0),PointerType::get(IntegerType::get(M->getContext(), 8), 0),NULL);		
				Function *CallFn=cast<Function>(profile);	
				vector<Value*> Args;
				Args.push_back(CastInst::CreatePointerCast(From,PointerType::get(IntegerType::get(M->getContext(), 8), 0) ,"fromptr",II));
				Args.push_back(CastInst::CreatePointerCast(To,PointerType::get(IntegerType::get(M->getContext(), 8), 0) ,"toptr",II));
				Instruction *callinst=CallInst::Create(CallFn,Args);
				callinst->insertBefore(II);
        		}
      		}
	}
	
	return false;
}



char fdo::ID = 0;
static RegisterPass<fdo> X("gen","This is a profile generator",false,false);
