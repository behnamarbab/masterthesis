// snippet of wafl-llvm-pass.so.cc
#include <math.h>
// ...
bool WAFLCoverage::runOnModule(Module &M) {
  // ...
  GlobalVariable *WAFLMapPtr =
    new GlobalVariable(M, PointerType::get(Int32Ty, 0), false,
    GlobalValue::ExternalLinkage, 0, "__wafl_area_ptr");
    
  // ...
  for (auto &F : M) {
    for (auto &BB : F) {  
      /* Count the instructions */
      CountAllVisitor CAV;
      CAV.visit(BB);

      // ...
      LoadInst *ERUPtr = IRB.CreateLoad(WAFLMapPtr);
      MapPtr->setMetadata(M.getMDKindID("nosanitize"),
        MDNode::get(C, None));
      
      Value* EdgeId = IRB.CreateXor(PrevLocCasted, CurLoc);
      Value *ERUPtrIdx =
          IRB.CreateGEP(ERUPtr, EdgeId);

      /* Setup the counter for storage */
      u32 log_count = (u32) log2(CAV.Count+1);
      Value *CNT = IRB.getInt32(log_count);
    
      LoadInst *ERULoad = IRB.CreateLoad(ERUPtrIdx);
      Value *ERUIncr = IRB.CreateAdd(ERULoad, CNT);

      IRB.CreateStore(ERUIncr, ERUPtrIdx)
        ->setMetadata(M.getMDKindID("nosanitize"),
        MDNode::get(C, None));

      inst_blocks++;
    }
  }
}