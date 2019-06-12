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

#ifndef MULTIPLIER_OPER
#define MULTIPLIER_OPER

#include <int_op_gen/impl/operator.hxx>

#include <functional>
#include <cassert>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      Wallace Multiplier implementations
     */
    class WallaceMultiplier : public BinaryOper {
      public:
        WallaceMultiplier(const std::function<BinaryOper::signature>& adder_p) : adder(adder_p) {}

      private:
      /**
       * @brief      Wallace multiplier
       *
       * @details    lhs and rhs have the same length in bits
       *
       * @param[in]  lhs   The left hand side
       * @param[in]  rhs   The right hand side
       *
       * @return     Product between inputs, having the same bit length as inputs
       */
      CiBitVector oper(const CiBitVector& lhs, const CiBitVector& rhs) const override;

      std::function<BinaryOper::signature> adder;


   };
  }
}

#endif
