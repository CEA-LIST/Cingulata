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
#if !defined(__NODE_H__)
#define __NODE_H__

/* compiler includes */
#include <set>
#include <vector>

/* local includes */
#include "gate.hxx"
#include "storage.hxx"

/*! \brief abstract class representing a node in the boolean circuit */
class Node
{
  public:
    /*! \brief maximum possible input for a node */
    enum { MAX_INPUT=2 };

    /*! \brief iterator to parse node children or parents */
    typedef std::vector<Node*>::iterator Iterator;

    /*! \brief iterator to parse constant node children or parents */
    typedef std::vector<Node*>::const_iterator ConstIterator;

    /*! \brief virtual destructor for polymorphic class */
    virtual ~Node(void) { }

    /*! \brief add node to output stream in blif format
     *  \param out_io stream where to write blif description */
    virtual void blif
      (typename std::ostream&) const {}

    /*! \brief output blif format node id to output stream
     *  \param out_io stream where to write blif id */
    virtual void blifId
      (typename std::ostream &out_io) const;

    /*! \brief access assigned id of node
     *  \pre node must have been assigned an id by a call to the
     *  corresponding setter */
    const unsigned& id(void) const;

    /*! \brief access value of node
     *  \pre node must have been assigned a value by a call to the
     *  corresponding setter */
    bool predefined(void) const;

    /*! \brief access value of the node */
    virtual Gate::value_t value(void) const { return Gate::UNKNOWN; }

    /*! \brief access count of children using node's result as input */
    const unsigned& children(void) const { return outdegree_m; }

    /*! \brief access iterator positioned on first child */
    ConstIterator beginChild(void) const { return children_m.begin(); }

    /*! \brief access iterator positioned on first child */
    ConstIterator endChild(void) const
      { return children_m.end(); }

    /*! \brief access count of parents providing node inputs */
    const unsigned& parents(void) const { return indegree_m; }

    /*! \brief access iterator positioned on first parent */
    ConstIterator beginParent(void) const { return parents_m.begin(); }

    /*! \brief access iterator positioned on first parent */
    ConstIterator endParent(void) const
      { return parents_m.end(); }

    /*! \brief access iterator positioned on first child */
    Iterator beginChild(void) { return children_m.begin(); }

    /*! \brief access iterator positioned on first child */
    Iterator endChild(void) { return children_m.end(); }

    /*! \brief access iterator positioned on first parent */
    Iterator beginParent(void) { return parents_m.begin(); }

    /*! \brief access iterator positioned on first parent */
    Iterator endParent(void) { return parents_m.end(); }

    /*! \brief assign id of node
     *  \pre no id has been assigned before */
    void id
      (const unsigned &id_i);

    /*! \brief assign value of node
     *  \pre no value has been assigned before */
    void predefined
      (const bool &value_i);

    /*! \brief assign additional output storage to node
     *  \param output_io storage where to write output after completion */
    void output
      (class OutputStorage &output_io);
    
    bool outputEmpty(void) {return output_m.empty(); }  

    /*! \brief add given node as parent to this node
     *  \param node_io parent to add */
    void connectParent
      (Node &node_io);

    /*! \brief add given node as child to this node
     *  \param node_io child to add */
    void connectChild
      (Node &node_io);
  
    /*! \brief constructor, initialize node */
    Node();

  private:
    /*! \brief assigned id */
    unsigned id_m;
    /*! \brief value of the node */
    unsigned value_m;
    /*! \brief count of nodes this node depends on */
    unsigned indegree_m;
    /*! \brief nodes this node depends on */
    std::vector<Node*> parents_m;
    /*! \brief count of nodes depending on result of this node */
    unsigned outdegree_m;
    /*! \brief nodes depending on this node */
    std::vector<Node*> children_m;
    /*! \brief list of assigned output storage */
    std::set<class OutputStorage*> output_m;
};

/*! \brief insert representation of given node to given stream
 *  \param out_io stream where to insert node representation
 *  \param[in] node_i node whose representation is inserted */
typename std::ostream& operator<<
  (typename std::ostream &out_io,
   const Node &node_i);

/*! \brief class representing an input node in the boolean circuit */
class InputNode : public Node
{
  public:
    /*! \brief constructor, create a default input node with value 0 */
    InputNode
      (void);

    /*! \brief constructor, create an input node with given value
     *  \param[in] value_i node value */
    InputNode
      (const bool &value_i);

    /*! \brief constructor, create an input node at given rank of the
     *  first level with given storage allocated as input
     *  \param[in] rank_i rank of this node in its level
     *  \param[in] input_io storage to use to define node value
     *  \pre storage must be located at input */
    InputNode
      (class Storage &input_io);

    /*! \brief access value of the node */
    Gate::value_t value(void) const { return input_m->value(); }

    /*! \brief output blif format node id to output stream
     *  \param out_io stream where to write blif id */
    void blifId
      (typename std::ostream &out_io) const;     

  private:
    /*! \brief storage where node value is found, can be interface or one of
     *  the static storages for constants */
    class Storage *input_m;
    /*! \brief storage for constant 0 */
    static class ImmediateStorage *zero_c;
    /*! \brief storage for constant 1 */
    static class ImmediateStorage *one_c;

    /*! \brief access storage for constant zero, allocate if first call */
    static class ImmediateStorage& zero(void);

    /*! \brief access storage for constant one, allocate if first call */
    static class ImmediateStorage& one(void);
};



/*! \brief class representing an operation node in the boolean circuit */
class GateNode : public Node
{
  public:
    /*! \brief constructor, create a gate node at given position, performing
     *  requested operation on the result of given parent gates
     *  \param[in] operator_i operation performed by this gate node */
    GateNode
      (const Gate::operator_t &operator_i = Gate::XOR);

    /*! \brief add node to output stream in blif format
     *  \param out_io stream where to write blif description */
    void blif
      (typename std::ostream &out_io) const;

  private:
    /*! \brief operation performed by this gate */
    Gate::operator_t operator_m;
};

#endif

