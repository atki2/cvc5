/******************************************************************************
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2026 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Implementation of bit-vectors proof checker.
 */

#include "theory/bv/proof_checker.h"

#include "expr/skolem_manager.h"
#include "theory/arith/arith_poly_norm.h"
#include "theory/bv/theory_bv_utils.h"
#include "util/bitvector.h"

namespace cvc5::internal {
namespace theory {
namespace bv {

BVProofRuleChecker::BVProofRuleChecker(NodeManager* nm) : ProofRuleChecker(nm)
{
}
void BVProofRuleChecker::registerTo(ProofChecker* pc)
{
  pc->registerTrustedChecker(ProofRule::MACRO_BV_BITBLAST, this, 2);
  pc->registerChecker(ProofRule::BV_BITBLAST_STEP, this);
  pc->registerChecker(ProofRule::BV_POLY_NORM, this);
  pc->registerChecker(ProofRule::BV_POLY_NORM_EQ, this);
  pc->registerChecker(ProofRule::BV_EAGER_ATOM, this);
  pc->registerChecker(ProofRule::BV_INTBLAST_STEP, this);
  pc->registerChecker(ProofRule::BV_INTBLAST_RANGE, this);
  pc->registerChecker(ProofRule::BV_INTBLAST_RANGE_QUANT, this);
}

Node BVProofRuleChecker::checkInternal(ProofRule id,
                                       const std::vector<Node>& children,
                                       const std::vector<Node>& args)
{
  if (id == ProofRule::MACRO_BV_BITBLAST)
  {
    Assert(children.empty());
    Assert(args.size() == 1);
    Assert(args[0].getKind() == Kind::EQUAL);
    return args[0];
  }
  else if (id == ProofRule::BV_BITBLAST_STEP)
  {
    Assert(children.empty());
    Assert(args.size() == 1);
    Assert(args[0].getKind() == Kind::EQUAL);
    return args[0];
  }
  else if (id == ProofRule::BV_EAGER_ATOM)
  {
    Assert(children.empty());
    Assert(args.size() == 1);
    Assert(args[0].getKind() == Kind::BITVECTOR_EAGER_ATOM);
    return args[0].eqNode(args[0][0]);
  }
  else if (id == ProofRule::BV_POLY_NORM)
  {
    Assert(children.empty());
    Assert(args.size() == 1);
    if (args[0].getKind() != Kind::EQUAL || !args[0][0].getType().isBitVector())
    {
      return Node::null();
    }
    if (!arith::PolyNorm::isArithPolyNorm(args[0][0], args[0][1]))
    {
      return Node::null();
    }
    return args[0];
  }
  else if (id == ProofRule::BV_POLY_NORM_EQ)
  {
    Assert(children.size() == 1);
    Assert(args.size() == 1);
    if (args[0].getKind() != Kind::EQUAL)
    {
      return Node::null();
    }
    Kind k = args[0][0].getKind();
    if (k != Kind::EQUAL)
    {
      return Node::null();
    }
    if (children[0].getKind() != Kind::EQUAL)
    {
      return Node::null();
    }
    Node l = children[0][0];
    Node r = children[0][1];
    if (l.getKind() != Kind::BITVECTOR_MULT
        || r.getKind() != Kind::BITVECTOR_MULT)
    {
      return Node::null();
    }
    Node cx = l[0];
    Node lr = l[1];
    Node cy = r[0];
    Node rr = r[1];
    if (lr.getKind() != Kind::BITVECTOR_SUB
        || rr.getKind() != Kind::BITVECTOR_SUB)
    {
      return Node::null();
    }
    if (cx.getKind() == Kind::CONST_BITVECTOR
        && cy.getKind() == Kind::CONST_BITVECTOR)
    {
      // must be odd
      if (!bv::utils::getBit(cx, 0) || !bv::utils::getBit(cy, 0))
      {
        return Node::null();
      }
    }
    else
    {
      return Node::null();
    }
    Node x1 = lr[0];
    Node x2 = lr[1];
    Node y1 = rr[0];
    Node y2 = rr[1];
    NodeManager* nm = nodeManager();
    Node ret = nm->mkNode(k, x1, x2).eqNode(nm->mkNode(k, y1, y2));
    if (ret != args[0])
    {
      return Node::null();
    }
    return ret;
  }
  else if (id == ProofRule::BV_INTBLAST_STEP)
  {
    Assert(children.empty());
    Assert(args.size() == 1);
    // the translation is checked by the Eunoia rule
    if (args[0].getKind() != Kind::EQUAL || !args[0][0].getType().isBoolean())
    {
      return Node::null();
    }
    return args[0];
  }
  else if (id == ProofRule::BV_INTBLAST_RANGE
           || id == ProofRule::BV_INTBLAST_RANGE_QUANT)
  {
    Assert(children.empty());
    Assert(args.size() == 1);
    Node range = args[0];
    if (id == ProofRule::BV_INTBLAST_RANGE_QUANT)
    {
      if (range.getKind() != Kind::FORALL)
      {
        return Node::null();
      }
      range = range[1];
    }
    // range is (and (>= k 0) (not (>= k 2^n))), k is an application of
    // (@bv_to_int_uf f) and n is the width of the return type of f
    if (range.getKind() != Kind::AND || range[0].getKind() != Kind::GEQ)
    {
      return Node::null();
    }
    Node k = range[0][0];
    SkolemId sid;
    Node f;
    if (k.getKind() != Kind::APPLY_UF
        || !SkolemManager::isSkolemFunction(k.getOperator(), sid, f)
        || sid != SkolemId::BV_TO_INT_UF)
    {
      return Node::null();
    }
    TypeNode tn = f.getType().getRangeType();
    if (!tn.isBitVector())
    {
      return Node::null();
    }
    NodeManager* nm = nodeManager();
    Node zero = nm->mkConstInt(Rational(0));
    Node ub = nm->mkConstInt(Rational(Integer(2).pow(tn.getBitVectorSize())));
    Node expected =
        nm->mkNode(Kind::AND,
                   nm->mkNode(Kind::GEQ, k, zero),
                   nm->mkNode(Kind::NOT, nm->mkNode(Kind::GEQ, k, ub)));
    return range == expected ? args[0] : Node::null();
  }
  // no rule
  return Node::null();
}

}  // namespace bv
}  // namespace theory
}  // namespace cvc5::internal
