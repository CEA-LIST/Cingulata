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

#ifndef MUX_OPER
#define MUX_OPER

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      Multiplexer operator (small depth)
     * @details    The multiplicative depth of generated circuit is @c
     *             ceil(log2(ceil(log2(m)))) + 1. It has approximatively @c
     *             2m+mn AND gates. Here @c m is the number of inputs and @c n
     *             the input word bit-size.
     * @note       Circuit MUX_{DNFs} from Buescher N, Holzer A, Weber A,
     *             Katzenbeisser S. *Compiling low depth circuits for practical
     *             secure computation*
     */
    class MuxDepth : public MuxOper {
      CiBitVector oper(const CiBitVector& cond, const std::vector<CiBitVector>& inps) const;
    };
  }
}
#endif
