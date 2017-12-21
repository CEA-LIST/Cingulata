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
#if !defined(__CIRCUIT_H__)
#define __CIRCUIT_H__

/* compiler includes */
#include <vector>
#include <list>
#include <set>

/* local includes */
#include "gate.hxx"
#include "node.hxx"
#include "storage.hxx"



/*! \brief record operations into a boolean circuit for future analyse and code
 *  generation */
class BooleanCircuit
{
  public:
    /*! \brief maximum size and count of interfaces of a boolean circuit */
    enum { MAX_INPUT=100000, MAX_OUTPUT=100000, MAX_SIZE=3333330 };

    /*! \brief instantiate an empty boolean circuit with its two constant
     *  input nodes */
    BooleanCircuit
      (void);

    /*! \brief release all nodes stored in the circuit, all pointers
     *  returned by methods of this circuit are invalidated */
    ~BooleanCircuit(void) { }

    /*! \brief print the circuit in blif format to given stream
     *  \param out_io stream where BLIF description is written */
    void blif
      (typename std::ostream &out_io) const;

    /*! \brief print the circuit predefined gate values to given stream
     *  \param out_io stream where predefined values description are written */
    void predefined
      (typename std::ostream &out_io) const;

      /*! \brief access size of the circuit */
    unsigned size
      (void) const;

    /*! \brief access count of gates in circuit performing given operation
     *  \param[in] operator_i operation performed by counted gates
     *  \return count of gates in circuit performing given operation */
    const unsigned& counter
      (const Gate::operator_t &operator_i) const;

    /*! \brief access count of allocated input interface storage
     *  \return count of allocated input interface storage */
    const unsigned& inputCount(void) const { return inputCount_m; }

    /*! \brief access count of allocated output interface storage
     *  \return count of allocated output interface storage */
    const unsigned& outputCount(void) const { return outputCount_m; }

    /*! \brief access one of the constant input nodes of the circuit
     *  \param[in] value_i value of constant node to access
     *  \return reference to the input node with given constant value */
    class InputNode& constant
      (const bool &value_i) const;

    /*! \brief add an input node to the circuit
     *  \param[in] value_i predefined input value
     *  \return reference to a new input node, which has been added at
     *  first level of the circuit */
    class Node& input
      (const bool &value_i = false);

    /*! \brief add a new gate node to the circuit
     *  \param[in] node0_i first gate operand
     *  \param[in] node1_i second gate operand
     *  \param[in] operator_i operation performed by created gate
     *  \param[in] value_i expected value with predefined input
     *  \return node that represents the operation in the circuit, which may
     *  be a new gate node */
    class Node& gate
      (const class Node &node0_i,
       const class Node &node1_i,
       const Gate::operator_t &operator_i,
       const bool &value_i = false);
       
    /*! \brief add a new gate node to the circuit
     *  \param[in] node0_i gate operand
     *  \param[in] operator_i operation performed by created gate
     *  \param[in] value_i expected value with predefined input
     *  \return node that represents the operation in the circuit, which may
     *  be a new gate node */
    class Node& gate
      (const class Node &node0_i,
       const Gate::operator_t &operator_i,
       const bool &value_i = false);

    /*! \brief assign interface storage to a node in the circuit
     *  \param[in] node_i node which gets interface storage */
    void output
      (const class Node &node_i);

  private:
    /*! \brief input nodes with constant values */
    static class InputNode *constant_c[2];
    /*! \brief set of variable input nodes in the circuit */
    InputNode sources_m[MAX_INPUT-2];
    /*! \brief set of variable input storage */
    Storage input_m[MAX_INPUT-2];
    /*! \brief list of nodes tagged as output in the circuit */
    std::vector<class Node*> sinks_m;
    /*! \brief set of output storage */
    OutputStorage output_m[MAX_OUTPUT];
    /*! \brief set of nodes in the circuit, connected as a levelled directed
     *  acyclic graph */
    GateNode graph_m[MAX_SIZE];
    /*! \brief size per level in the graph */
    //std::vector<unsigned> size_m;
    /*! \brief count of gates, one per gate operation, with a total */
    unsigned counter_m[Gate::OPERATORS+1];
    /*! \brief count of variable inputs to the circuit */
    unsigned inputCount_m;
    /*! \brief count of outputs to the circuit */
    unsigned outputCount_m;

    /*! \brief get reference to given node in the circuit
     *  \param[in] node_i reference to a node supposed to be in the circuit
     *  \return mutable reference to the node */
    class Node& reference
      (const class Node &node_i);

    /*! \brief connect given nodes together */
    void connect
      (class Node &node_io,
       class Node &parent_io);
};

#endif /* !defined(__CIRCUIT_H__) */

