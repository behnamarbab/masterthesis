// snippet of wafl-llvm-pass.so.cc
#include <math.h>
// ..

bool WAFLCoverage::runOnModule(Module &M) {
  // ...
  GlobalVariable *WAFLMapPtr =
      new GlobalVariable(M, PointerType::get(Int8Ty, 0), false,
      GlobalValue::ExternalLinkage, 0, "__wafl_area_ptr");
  // ..

  for (auto &F : M) {
    for (auto &BB : F) {
      
      // ...

      LoadInst *IcntPtr = IRB.CreateLoad(WAFLMapPtr);
      MapPtr->setMetadata(M.getMDKindID("nosanitize"),
        MDNode::get(C, None));
      
      Value* EdgeId = IRB.CreateXor(PrevLocCasted, CurLoc);
      Value *IcntPtrIdx =
          IRB.CreateGEP(IcntPtr, EdgeId);

      /* Count the instructions */
      CountAllVisitor CAV;
      CAV.visit(BB);

      /* Setup the counter for storage */
      u8 log_count = (u8) log2(CAV.Count);
      Value *CNT = IRB.getInt8(log_count);
    
      LoadInst *IcntLoad = IRB.CreateLoad(IcntPtrIdx);
      Value *IcntIncr = IRB.CreateAdd(IcntLoad, CNT);

      IRB.CreateStore(IcntIncr, IcntPtrIdx)
        ->setMetadata(M.getMDKindID("nosanitize"),
        MDNode::get(C, None));

      inst_blocks++;
    }
  }
}