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

#ifndef NEGATE_OPER
#define NEGATE_OPER

#include <functional>

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      2's complement negation operator
     */
    class Negate : public UnaryOper {
    public:
      Negate(const std::function<BinaryOper::signature>& add_p) : add(add_p) {}

    private:
      /**
       * @brief      Implementation
       *
       * @param[in]  a     bit-vector to negate
       *
       * @return     2's complement negated bit-vector
       */
      CiBitVector oper(const CiBitVector& a) const override;

      std::function<BinaryOper::signature> add;
    };
  }
}

#endif
