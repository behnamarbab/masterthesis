// snippet of wafl-llvm-pass.so.cc
#include <math.h>
// ...
bool WAFLCoverage::runOnModule(Module &M) {
  // ...
  GlobalVariable *WAFLMapPtr =
    new GlobalVariable(M, PointerType::get(Int32Ty, 0), false,
    GlobalValue::ExternalLinkage, 0, "__wafl_area_ptr");
    
  llvm::FunctionType *ORCIncrement =
    llvm::FunctionType::get(builder.getVoidTy(), ORC_args, false);
  llvm::Function *ORC_Increment =
    llvm::Function::Create(ORCIncrement, 
      llvm::Function::ExternalLinkage, "addORC", &M);
  // ...
  for (auto &F : M) {
    for (auto &BB : F) {  
      /* Count the instructions */
      CountAllVisitor CAV;
      CAV.visit(BB);

      // ...
      LoadInst *ORCPtr = IRB.CreateLoad(WAFLMapPtr);
      MapPtr->setMetadata(M.getMDKindID("nosanitize"),
        MDNode::get(C, None));
      
      Value* EdgeId = IRB.CreateXor(PrevLocCasted, CurLoc);
      Value *ORCPtrIdx =
          IRB.CreateGEP(ORCPtr, EdgeId);

      /* Setup the counter for storage */
      u32 log_count = (u32) log2(CAV.Count+1);
      Value *CNT = IRB.getInt32(log_count);
    
      LoadInst *ORCLoad = IRB.CreateLoad(ORCPtrIdx);
      Value *ORCIncr = IRB.CreateAdd(ORCLoad, CNT);

      IRB.CreateStore(ORCIncr, ORCPtrIdx)
        ->setMetadata(M.getMDKindID("nosanitize"),
        MDNode::get(C, None));

      IRB.CreateCall(ORC_Increment, ArrayRef<Value*>({ CNT }));

      inst_blocks++;
    }
  }
}