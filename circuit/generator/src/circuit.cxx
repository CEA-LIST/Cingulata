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
#include <cstring>
#include <iostream>

/* local includes */
#include "circuit.hxx"
#include <basic-exception.hxx>

/* namespaces */
using namespace std;


/* log configuration */
#define _LOG 0
#if _LOG
#   define log(_s) clog << _s << endl
#else
#   define log(_s) ((void)0)
#endif


/* see base header */
InputNode *BooleanCircuit::constant_c[2] = {NULL, NULL};

/* see base header */
BooleanCircuit::BooleanCircuit
  (void)
: inputCount_m(0),
  outputCount_m(0)
{
  /* reset all gate counters */
  memset (counter_m, 0, sizeof(unsigned)*(Gate::OPERATORS+1));
}

/* see base header */
void BooleanCircuit::blif
  (ostream &out_io) const
{
  out_io << ".model " << "CIRCUIT" << endl;
  
  out_io << ".inputs ";
  for (unsigned i=0; i<this->inputCount(); ++i) {
    sources_m[i].blifId(out_io);
    out_io << " ";
  }
  out_io << endl;
  
  out_io << ".outputs ";
  for (unsigned i=0; i<this->outputCount(); ++i) {
    sinks_m[i]->blifId(out_io);
    out_io << " ";
  }
  out_io << endl;

  out_io << ".names ";
  constant(false).blifId(out_io);
  out_io << endl << "0" << endl;
  
  out_io << ".names ";
  constant(true).blifId(out_io);
  out_io << endl << "1" << endl;

  Node::ConstIterator p=constant(false).beginChild();
  while (p != constant(false).endChild()) {
    if (!(*p)->outputEmpty() && (*p)->parents()==1) {
      out_io<<".names ";
      (*p)->blifId(out_io);
      out_io << endl<<"0"<< endl;
    }
    ++p;
  }
  
  p=constant(true).beginChild();
  while (p != constant(true).endChild()) {
    if (!(*p)->outputEmpty() && (*p)->parents()==1) {
      out_io<<".names ";
      (*p)->blifId(out_io);
      out_io << endl<<"1"<< endl;
    }
    ++p;
  }
  
  for (unsigned i=0; i<this->inputCount(); ++i) {
    p=sources_m[i].beginChild();
    while (p != sources_m[i].endChild()) {
      if (!(*p)->outputEmpty() && (*p)->parents()==1) {
        out_io<<".names ";
        sources_m[i].blifId(out_io);
        out_io<<" ";
        (*p)->blifId(out_io);
        out_io << endl<< sources_m[i].predefined()<<" "<<sources_m[i].predefined()<< endl;
      }
      ++p;
    }
  }

  for (unsigned int i = 0; i < this->size(); i++) { 
    graph_m[i].blif(out_io);
  }
  
  out_io << ".end" << endl;
}

void BooleanCircuit::predefined
  (std::ostream &out_io) const
{
  constant(false).blifId(out_io);
  out_io << " 0" << endl;
  
  constant(true).blifId(out_io);
  out_io << " 1" << endl;
  
  for (unsigned i=0; i<this->inputCount(); ++i) {
    sources_m[i].blifId(out_io);
    out_io << " " << sources_m[i].predefined() << endl;
  }  
  for (unsigned int i = 0; i < this->size(); i++) {
    graph_m[i].blifId(out_io);
    out_io << " " << graph_m[i].predefined() << endl;
  }
}


/* see base header */
unsigned BooleanCircuit::size
  (void) const
{
  /* total size of the circuit in gates */
  return counter_m[Gate::OPERATORS];
}

/* see base header */
const unsigned& BooleanCircuit::counter
  (const Gate::operator_t &operator_i) const
{
  /* interface contract */
  assert (unsigned(operator_i) < Gate::OPERATORS);
  return counter_m[unsigned(operator_i)];
}

/* see base header */
InputNode& BooleanCircuit::constant
  (const bool &value_i) const
{
  /* index of the constant node */
  unsigned index_v = unsigned(value_i);

  if (!constant_c[index_v])
  { /* case: node not yet instantiated */
    constant_c[index_v] = new InputNode(value_i);
    constant_c[index_v]->id (index_v);
    constant_c[index_v]->predefined (value_i);
  }

  return *constant_c[index_v];
}

/* see base header */
Node& BooleanCircuit::input
  (const bool &value_i)
{
  /* rank of created node */
  const unsigned rank_v = this->inputCount();

  if (2 + rank_v == MAX_INPUT) //The two constants 0 and 1 are encrypted as input0.ct and input1.ct
  { /* case: cannot add input */
    throw BasicException("circuit: input: reached input limit");
  }

  log ("circuit: create new input node");

  /* create new storage in circuit */
  input_m[rank_v] = Storage(Storage::INPUT, rank_v+2);
  ++inputCount_m;

  /* append a new node to first level and leave */
  sources_m[rank_v] = InputNode(input_m[rank_v]);
  sources_m[rank_v].id (rank_v+2);
  sources_m[rank_v].predefined (value_i);
  return sources_m[rank_v];
}

/* see base header */
Node& BooleanCircuit::gate
  (const Node &node0_i,
   const Node &node1_i,
   const Gate::operator_t &operator_i,
   const bool &value_i)
{
  /* create node with one predecessor */
  Node &pNode_v = gate(node0_i, operator_i, value_i);

  /* mutable references to given nodes (see conception notes) */
  Node &node1_v = this->reference(node1_i);

  /* connect nodes together */
  this->connect(pNode_v, node1_v);

  /* return reference to selected node */
  return pNode_v;
}

/* see base header */
Node& BooleanCircuit::gate
  (const Node &node0_i,
   const Gate::operator_t &operator_i,
   const bool &value_i)
{
  /* mutable references to given nodes (see conception notes) */
  Node &node0_v = this->reference (node0_i);

  /* pointer to new gate node (or the result of gate if predictible) */
  Node *pNode_v = NULL;
  
  log ("circuit: create new gate node (" << unsigned(operator_i) << ")");
  graph_m[this->size()] = GateNode(operator_i);
  pNode_v = graph_m + this->size();

  /* add new node to its level */
  pNode_v->id (counter_m[Gate::OPERATORS]+MAX_INPUT);
  pNode_v->predefined (value_i);
  ++counter_m[operator_i];
  ++counter_m[Gate::OPERATORS];

  /* connect nodes together */
  this->connect(*pNode_v, node0_v);

  /* return reference to selected node */
  return *pNode_v;
}

/* see base header */
void BooleanCircuit::output
  (const Node &node_i)
{
  /* mutable reference to given node (see conception notes) */
  Node &node_v = this->reference (node_i);
  
  /* rank of created output storage */
  const unsigned rank_v = this->outputCount ();

  if (rank_v == MAX_OUTPUT)
  { /* case: cannot add output */
    throw BasicException("circuit: input: reached output limit");
  }

  log ("circuit: node " << node_v << " is an output");
  
  /* the node is a constant or a input (source) */
  if(node_v.id() <this->inputCount()+ 2)
  {
    Node *pNode_v =NULL;
    pNode_v=new Node();
  
    /* create new storage in circuit, assign it to node */
    output_m[rank_v] = OutputStorage(Storage::OUTPUT, rank_v);
    pNode_v->output (output_m[rank_v]);
    ++outputCount_m;

    /* add new node to output set */
    pNode_v->id(rank_v);
    pNode_v->predefined(node_v.value());
  
    this->connect(*pNode_v,node_v);
  
    sinks_m.push_back(pNode_v);
  }
  else
  {
    /* create new storage in circuit, assign it to node */
    output_m[rank_v] = OutputStorage(Storage::OUTPUT, rank_v);
    node_v.output (output_m[rank_v]);
    ++outputCount_m;
    
    //add the reference of the node to the output list
    sinks_m.push_back (&node_v);
  } 
}

/* see base header */
Node& BooleanCircuit::reference
  (const Node &node_i)
{
  /* pointer to mutable node */
  Node *pNode_v = NULL;

  if (node_i.id() < 2)
  { /* case: node is a source */
    pNode_v = &this->constant(node_i.id()? true:false);
  }
  else if (node_i.id() < MAX_INPUT)
  { /* case: node is a source */
    assert (node_i.id() < this->inputCount()+2);
    pNode_v = sources_m + node_i.id() - 2;
  }
  else
  { /* case: node is a gate */
    assert (node_i.id()-MAX_INPUT < counter_m[Gate::OPERATORS]);
    pNode_v = graph_m + (node_i.id()-MAX_INPUT);
  }

  return *pNode_v;
}

/* see base header */
void BooleanCircuit::connect
  (Node &node_io,
   Node &parent_io)
{
  /* add parents to node */
  node_io.connectParent(parent_io);

  /* add node to parents */
  parent_io.connectChild(node_io);
}
