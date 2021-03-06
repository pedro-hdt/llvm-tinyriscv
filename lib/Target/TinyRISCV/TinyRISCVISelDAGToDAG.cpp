//===-- TinyRISCVISelDAGToDAG.cpp - A dag to dag inst selector for TinyRISCV ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the TinyRISCV target.
//
//===----------------------------------------------------------------------===//

#include "TinyRISCV.h"
#include "TinyRISCVTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#include "TinyRISCVInstrInfo.h"

using namespace llvm;

/// TinyRISCVDAGToDAGISel - TinyRISCV specific code to select TinyRISCV machine
/// instructions for SelectionDAG operations.
///
namespace {
class TinyRISCVDAGToDAGISel : public SelectionDAGISel {
  const TinyRISCVSubtarget &Subtarget;

public:
  explicit TinyRISCVDAGToDAGISel(TinyRISCVTargetMachine &TM, CodeGenOpt::Level OptLevel)
      : SelectionDAGISel(TM, OptLevel), Subtarget(*TM.getSubtargetImpl()) {}

  SDNode *Select(SDNode *N) override;

  bool SelectAddr(SDValue Addr, SDValue &Base, SDValue &Offset);

  virtual const char *getPassName() const override {
    return "TinyRISCV DAG->DAG Pattern Instruction Selection";
  }

private:
// TODO add needed helper methods for special selection
//  SDNode *SelectMoveImmediate(SDNode *N);
//  SDNode *SelectConditionalBranch(SDNode *N);

// Include the pieces autogenerated from the target description.
#include "TinyRISCVGenDAGISel.inc"
};
} // end anonymous namespace

bool TinyRISCVDAGToDAGISel::SelectAddr(SDValue Addr, SDValue &Base, SDValue &Offset) {
  if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    EVT PtrVT = getTargetLowering()->getPointerTy(CurDAG->getDataLayout());
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), PtrVT);
    Offset = CurDAG->getTargetConstant(0, Addr, MVT::i32);
    return true;
  }
  if (Addr.getOpcode() == ISD::TargetExternalSymbol ||
      Addr.getOpcode() == ISD::TargetGlobalAddress ||
      Addr.getOpcode() == ISD::TargetGlobalTLSAddress) {
    return false; // direct calls.
  }

  Base = Addr;
  Offset = CurDAG->getTargetConstant(0, Addr, MVT::i32);
  return true;
}


SDNode *TinyRISCVDAGToDAGISel::Select(SDNode *N) {
  switch (N->getOpcode()) {
  // TODO add all needed cases where the default tablegen patterns are not enough
  }

  return SelectCode(N);
}

/// createTinyRISCVISelDag - This pass converts a legalized DAG into a
/// TinyRISCV-specific DAG, ready for instruction scheduling.
///
FunctionPass *llvm::createTinyRISCVISelDag(TinyRISCVTargetMachine &TM,
                                     CodeGenOpt::Level OptLevel) {
  return new TinyRISCVDAGToDAGISel(TM, OptLevel);
}
