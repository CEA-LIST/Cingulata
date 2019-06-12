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

#ifndef DEC_OPER
#define DEC_OPER

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      1-of-n decoder
     * @details    The decoder operator transforms an n-bit input word into a
     *             2^n-bit vector with a single bit set to one and others to
     *             zero. The set bit is the bit whose index is given by integer
     *             value of n-bit input word.
     * @note       The multiplicative depth of generated circuit is @c
     *             ceil(log2(n)). It has approximatively @c n.2^n AND gates. Here
     *             @c n is the number of input bits to decode.
     */
    class Decoder : public UnaryOper {
      CiBitVector oper(const CiBitVector& inp) const;
    };
  }
}
#endif
