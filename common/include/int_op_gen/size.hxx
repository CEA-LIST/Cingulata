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

#ifndef INT_OP_GEN_SIZE
#define INT_OP_GEN_SIZE

#include <int_op_gen/interface.hxx>
#include <int_op_gen/impl/all.hxx>

namespace cingulata
{
  class IntOpGenSize : public IIntOpGen {
  public:
    IntOpGenSize();

    virtual CiBitVector add     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

    virtual CiBitVector neg     ( const CiBitVector& lhs) const override;

    virtual CiBitVector mul     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

    virtual CiBit equal         ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

    virtual CiBit lower         ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

  private:
    int_ops::RippleCarryAdder   m_add;
    int_ops::Negate             m_neg;
    int_ops::WallaceMultiplier  m_mul;
    int_ops::EqualSize          m_equal;
    int_ops::LowerCompSize      m_lower;
  };
}

#endif
