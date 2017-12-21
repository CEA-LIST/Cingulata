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
#include <cassert>
#include <iostream>
#include <typeinfo>

/* local includes */
#include "node.hxx"

/* namespaces */
using namespace std;

/* see base header */
const unsigned& Node::id
  (void) const
{
  /* interface contract */
  assert (id_m != unsigned(-1));
  return id_m;
}

/* see base header */
bool Node::predefined
  (void) const
{
  /* interface contract */
  assert (value_m != unsigned(-1));
  return value_m != 0;
}

/* see base header */
void Node::id
    (const unsigned &id_i)
{
  /* interface contract */
  assert (id_i != unsigned(-1));
  assert (id_m == unsigned(-1));
  id_m = id_i;
}

/* see base header */
void Node::predefined
    (const bool &value_i)
{
  /* interface contract */
  assert (value_m == unsigned(-1));
  value_m = value_i;
}

/* see base header */
void Node::output
    (OutputStorage &output_io)
{
  /* interface contract */
  assert (output_io.location() == Storage::OUTPUT);
  output_m.insert (&output_io);
}

/* see base header */
Node::Node()
: id_m(-1),
  value_m(-1),
  indegree_m(0),
  outdegree_m(0)
{ }

/* see base header */
void Node::connectParent
  (Node &node_io)
{
  parents_m.push_back(&node_io);
  indegree_m = parents_m.size();
}

/* see base header */
void Node::connectChild
  (Node &node_io)
{
  children_m.push_back(&node_io);
  outdegree_m = children_m.size();
}

/* see base header */
ostream& operator<<
  (ostream &out_io,
   const Node &node_i)
{
  out_io << "{";
  if (typeid(node_i) == typeid(InputNode)) {
    out_io << "in";
  }
  out_io << node_i.id() << "}";
  return out_io;
}

/* see base header */
ImmediateStorage *InputNode::zero_c = NULL;

/* see base header */
ImmediateStorage *InputNode::one_c = NULL;

/* see base header */
InputNode::InputNode
  (void)
: input_m(&InputNode::zero())
{ }

/* see base header */
InputNode::InputNode
  (const bool &value_i)
: input_m(!value_i? &InputNode::zero() : &InputNode::one())
{ }

/* see base header */
InputNode::InputNode
  (Storage &input_io)
: input_m(&input_io)
{
  /* interface contract */
  assert (input_io.location() == Storage::INPUT);
}

/* see base header */
ImmediateStorage& InputNode::zero
  (void)
{
  if (!zero_c)
  { /* case: first call to getter */
    /* allocate storage */
    zero_c = new ImmediateStorage(false);
  }

  return *zero_c;
}

/* see base header */
ImmediateStorage& InputNode::one
  (void)
{
  if (!one_c)
  { /* case: first call to getter */
    /* allocate storage */
    one_c = new ImmediateStorage(true);
  }

  return *one_c;
}

/* see base header */
GateNode::GateNode(const Gate::operator_t &operator_i)
  : operator_m(operator_i) {}

void GateNode::blif
  (ostream &out_io) const
{
  ConstIterator p = this->beginParent();
  
  out_io << ".names ";

  while (p != this->endParent())
  {
    (*p)->blifId(out_io);
    out_io << " ";
    ++p;
  }
  
  this->blifId(out_io);
  out_io << endl;

  switch (operator_m)
  {
    case Gate::XOR: out_io << "01 1" << endl << "10 1"; break;
    case Gate::AND: out_io << "11 1"; break;
    case Gate::OR: out_io << "00 0"; break;
    case Gate::NOT: out_io << "0 1"; break;
  }  
  out_io << endl;
}

void Node::blifId(ostream &out_io) const
{
  if (output_m.empty()) {
    out_io << "n" << this->id();
  } else {
    set<OutputStorage*>::const_iterator o = output_m.begin();
    out_io << "m";
    while (o != output_m.end())
    {
      out_io << "_" << (*o)->id();
      ++o;
    }    
  }
}

void InputNode::blifId(ostream &out_io) const
{
  out_io << "i_" << this->id();
}


