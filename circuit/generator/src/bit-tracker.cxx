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

/* compiler includes */
#include <iostream>

/* local includes */
#include "bit-tracker.hxx"
#include "node.hxx"

/* namespaces */
using namespace std;


/* see base header */
BooleanCircuit BitTracker::circuit_c;

/* see base header */
void BitTracker::write
  (ostream &out_io) const
{
  /* mark assigned node as output in associated circuit */
  out_io << boolalpha << this->value() << " " << noboolalpha;
  circuit_c.output (*pNode_m);
}

/* see base header */
void BitTracker::read
  (istream &in_io)
{
  /* create a new input node in circuit and assign it to this bit */
  in_io >> boolalpha >> value_m >> noboolalpha;
  pNode_m = &circuit_c.input(value_m);
}

/* see base header */
BitTracker BitTracker::operator!
  (void) const
{
  /* create a new bit to track */
  BitTracker result_v(!value_m);
  this->gate(Gate::NOT, result_v);
  return result_v;
}

/* see base header */
BitTracker BitTracker::operator^
  (const BitTracker &operand_i) const
{
  /* create a new bit to track */
  BitTracker result_v(value_m ^ operand_i.value_m);
  this->gate (operand_i, Gate::XOR, result_v);
  return result_v;
}

/* see base header */
BitTracker BitTracker::operator&
  (const BitTracker &operand_i) const
{
  /* create a new bit to track */
  BitTracker result_v(value_m & operand_i.value_m);
  this->gate (operand_i, Gate::AND, result_v);
  return result_v;
}

/* see base header */
BitTracker BitTracker::operator&&
  (const BitTracker &operand_i) const
{
  return (*this) & operand_i; 
}   

/* see base header */
BitTracker BitTracker::operator|
  (const BitTracker &operand_i) const
{
  /* create a new bit to track */
  BitTracker result_v(value_m | operand_i.value_m);
  this->gate (operand_i, Gate::OR, result_v);
  return result_v;
}

/* see base header */
BitTracker BitTracker::operator||
  (const BitTracker &operand_i) const
{
  return (*this) | operand_i; 
}   

/* see base header */
BitTracker BitTracker::operator<
  (const BitTracker &operand_i) const
{
  return operand_i & !(*this);
}

/* see base header */
BitTracker BitTracker::operator>
  (const BitTracker &operand_i) const
{
  return (*this) & !operand_i;
}

/* see base header */
BitTracker BitTracker::operator!=
  (const BitTracker &operand_i) const
{
  return (*this) ^ operand_i;
}

/* see base header */
BitTracker BitTracker::operator==
  (const BitTracker &operand_i) const
{
  return !((*this) != operand_i);
}

/* see base header */
BitTracker& BitTracker::operator^=
  (const BitTracker &operand_i)
{
  return (*this) = (*this) ^ operand_i;
}

/* see base header */
BitTracker& BitTracker::operator&=
  (const BitTracker &operand_i)
{
  return (*this) = (*this) & operand_i;
}

/* see base header */
BitTracker& BitTracker::operator|=
  (const BitTracker &operand_i)
{
  return (*this) = (*this) | operand_i;
}

/* see base header */
void BitTracker::gate
  (const BitTracker &operand_i,
   const Gate::operator_t &gate_i,
   BitTracker &result_io) const
{
  /* assign created node to output bit */
  result_io.pNode_m = &this->circuit().gate(this->node(), operand_i.node(),
    gate_i, result_io.value_m);
}

/* see base header */
void BitTracker::gate
  (const Gate::operator_t &gate_i,
   BitTracker &result_io) const
{
  /* assign created node to output bit */
  result_io.pNode_m = &this->circuit().gate(this->node(), gate_i, result_io.value_m);
}

/* see base header */
BitTracker BitTracker::operator+
  (const BitTracker &operand_i) const
{
  /* wrapper */
  return (*this) ^ operand_i; 
}

/* see base header */
BitTracker BitTracker::operator*
  (const BitTracker &operand_i) const
{
  /* wrapper */
  return (*this) & operand_i; 
}

/* see base header */
BitTracker& BitTracker::operator+=
  (const BitTracker &operand_i)
{
  /* wrapper */
  return (*this) = (*this) + operand_i;
}

/* see base header */
BitTracker& BitTracker::operator*=
  (const BitTracker &operand_i)
{
  /* wrapper */
  return (*this) = (*this) * operand_i;
}
