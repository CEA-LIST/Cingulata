/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team

    This software is governed by the CeCILL-C license under French law and
    abiding by the rules of distribution of free software.  You can  use,
    modify and/ or redistribute the software under the terms of the CeCILL-C
    license as circulated by CEA, CNRS and INRIA at the following URL
    "http://www.cecill.info".

    As a counterpart to the access to the source code and  rights to copy,
    modify and redistribute granted by the license, users are provided only
    with a limited warranty  and the software's author,  the holder of the
    economic rights,  and the successive licensors  have only  limited
    liability.

    The fact that you are presently reading this means that you have had
    knowledge of the CeCILL-C license and that you accept its terms.
*/

#ifndef LOWER_OPER
#define LOWER_OPER

#include <functional>

#include <int_op_gen/impl/operator.hxx>

namespace cingulata {
  namespace int_ops {
    /**
     * @brief      Logarithmic depth lower comparator
     * @details    The multiplicative depth of generated circuit is logaritmic
     *             in input size.
     * @note       Circuit described in Garay J, Schoenmakers B, Villegas J.
     *             *Practical and secure solutions for integer comparison*
     */
    class LowerCompDepth : public CompOper {
    public:
      LowerCompDepth(const std::function<CompOper::signature>& equal_p) : equal(equal_p) {}

    private:
      /**
       * @brief      Implementation
       *
       * @param[in]  lhs    The left hand side
       * @param[in]  rhs    The right hand side
       *
       * @return     Comparison result
       */
      CiBit oper(const CiBitVector& lhs, const CiBitVector& rhs) const override;

      std::function<CompOper::signature> equal;
    };


    /**
     * @brief      Small size lower comparator
     * @details    The multiplicative depth of generated circuit is linear in
     *             input size. It uses @c lhs.size() AND gates and @c
     *             3*lhs.size() XOR gates
     * @note       Circuit described in Kolesnikov V, Sadeghi AR, Schneider T.
     *             *Improved garbled circuit building blocks and applications to
     *             auctions and computing minima*
     */
    class LowerCompSize : public CompOper {
      /**
       * @brief      Implementation
       *
       * @param[in]  lhs   The left hand side
       * @param[in]  rhs   The right hand side
       *
       * @return     Comparison result
       */
      CiBit oper(const CiBitVector& lhs, const CiBitVector& rhs) const override;
    };
  }
}
#endif
