//===- VectorOps.h - MLIR Super Vectorizer Operations -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the Vector dialect.
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_DIALECT_VECTOR_VECTOROPS_H
#define MLIR_DIALECT_VECTOR_VECTOROPS_H

#include "mlir/IR/Attributes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/IR/StandardTypes.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"

namespace mlir {
class MLIRContext;
class OwningRewritePatternList;
namespace vector {

/// Structure to control the behavior of vector transform patterns.
struct VectorTransformsOptions {
  /// Let vector.contract lower to vector.matrix_multiply and LLVM matrix
  /// intrinsics.
  bool lowerToLLVMMatrixIntrinsics = false;
};

/// Collect a set of vector-to-vector canonicalization patterns.
void populateVectorToVectorCanonicalizationPatterns(
    OwningRewritePatternList &patterns, MLIRContext *context);

/// Collect a set of vector-to-vector transformation patterns.
void populateVectorToVectorTransformationPatterns(
    OwningRewritePatternList &patterns, MLIRContext *context);

/// Collect a set of vector slices transformation patterns:
///    ExtractSlicesOpLowering, InsertSlicesOpLowering
/// Useful for clients that want to express all vector "slices"
/// ops in terms of more elementary vector "slice" ops. If all
/// "produced" tuple values are "consumed" (the most common
/// use for "slices" ops), this lowering removes all tuple related
/// operations as well (through DCE and folding). If tuple values
/// "leak" coming in, however, some tuple related ops will remain.
void populateVectorSlicesLoweringPatterns(OwningRewritePatternList &patterns,
                                          MLIRContext *context);

/// Collect a set of transformation patterns that are related to contracting
/// or expanding vector operations:
///   ContractionOpLowering,
///   ShapeCastOp2DDownCastRewritePattern,
///   ShapeCastOp2DUpCastRewritePattern
///   BroadcastOpLowering,
///   TransposeOpLowering
///   OuterproductOpLowering
/// These transformation express higher level vector ops in terms of more
/// elementary extraction, insertion, reduction, product, and broadcast ops.
void populateVectorContractLoweringPatterns(
    OwningRewritePatternList &patterns, MLIRContext *context,
    VectorTransformsOptions vectorTransformOptions = VectorTransformsOptions());

/// Returns the integer type required for subscripts in the vector dialect.
IntegerType getVectorSubscriptType(Builder &builder);

/// Returns an integer array attribute containing the given values using
/// the integer type required for subscripts in the vector dialect.
ArrayAttr getVectorSubscriptAttr(Builder &b, ArrayRef<int64_t> values);

#define GET_OP_CLASSES
#include "mlir/Dialect/Vector/VectorOps.h.inc"

#include "mlir/Dialect/Vector/VectorOpsDialect.h.inc"

} // end namespace vector
} // end namespace mlir

#endif // MLIR_DIALECT_VECTOR_VECTOROPS_H
