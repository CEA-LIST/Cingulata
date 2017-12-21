/*
    (C) Copyright 2017 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team (formerly Armadillo team)

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

/* prevent multiple inclusions */
#if !defined(__BIT_TRACKER_H__)
#define __BIT_TRACKER_H__

/* local includes */
#include "circuit.hxx"


/*! \brief offer bit manipulation interface with each operation recorded in
 *  a boolean circuit */
class BitTracker
{
  public:
    /*! \brief access the boolean circuit in which operations are recorded
     *  to read circuit statistics */
    static const BooleanCircuit& record(void) { return circuit_c; }

  private:
    /*! \brief the boolean circuit in which operations are recorded */
    static BooleanCircuit circuit_c;

  public:
    /*! \brief instantiate a bit and assign it one of the constant nodes in
     *  the circuit
     *  \param[in] value_i initial value of the bit (default: false) */
    BitTracker
      (const bool &value_i = false)
    : value_m(value_i),
      pNode_m(&circuit_c.constant(value_i))
    { }

    /*! \brief read current value of tracked bit */
    const bool& value(void) const { return value_m; }

    /*! \brief mark assigned node as being an output node in the circuit,
     *  and add its value to given stream */
    void write
      (typename std::ostream &out_io) const;

    /*! \brief create a new input node with unknown value and assign it to
     *  this bit, the value is still read from input */
    void read
      (typename std::istream &in_io);

    BitTracker operator!(void) const;
    BitTracker operator^(const BitTracker &) const;
    BitTracker operator&(const BitTracker &) const;
    BitTracker operator&&(const BitTracker &) const;
    BitTracker operator|(const BitTracker &) const;
    BitTracker operator||(const BitTracker &) const;
    BitTracker operator<(const BitTracker &) const;
    BitTracker operator>(const BitTracker &) const;
    BitTracker operator!=(const BitTracker &) const;
    BitTracker operator==(const BitTracker &) const;
    BitTracker operator+(const BitTracker &) const;
    BitTracker operator*(const BitTracker &) const;

    BitTracker& operator^=(const BitTracker &);
    BitTracker& operator&=(const BitTracker &);
    BitTracker& operator|=(const BitTracker &);
    BitTracker& operator+=(const BitTracker &);
    BitTracker& operator*=(const BitTracker &);

  private:
    /*! \brief current value of this bit */
    bool value_m;
    /*! \brief node whose value is represented by this bit */
    const class Node *pNode_m;

    /*! \brief access assigned circuit */
    BooleanCircuit& circuit(void) const { return circuit_c; }

    /*! \brief access assigned node */
    const class Node& node(void) const { return *pNode_m; }

    /*! \brief create a new gate in circuit representing requested operation
     *  between this bit and another, assign the gate node to given bit
     *  \param[in] operand_i other operand of created gate
     *  \param[in] gate_i operation performed by created gate
     *  \param result_io bit to which gate result is assigned */
    void gate
      (const BitTracker &operand_i,
       const Gate::operator_t &gate_i,
       BitTracker &result_io) const;

    /*! \brief create a new gate in circuit representing requested unary
     *  operation on this bit, assign the gate node to given bit
     *  \param[in] gate_i operation performed by created gate
     *  \param result_io bit to which gate result is assigned */
    void gate
      (const Gate::operator_t &gate_i,
       BitTracker &result_io) const;
};

#endif /* !defined(__BIT_TRACKER_H__) */

