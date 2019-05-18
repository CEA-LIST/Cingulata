/*
    (C) Copyright 2017 CEA LIST. All Rights Reserved.
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

/**
 * @file priority.hxx
 * @brief implements scheduler priority functions
 */

#ifndef PRIORITY_HXX
#define PRIORITY_HXX

#include "blif_circuit.hxx"

#include <boost/graph/adjacency_list.hpp>
#include <unordered_map>

class Priority {
public:
  virtual int value(const Circuit::vertex_descriptor node) = 0;
  virtual ~Priority() {}
};

class PriorityStatic : public Priority {
protected:
  std::unordered_map<Circuit::vertex_descriptor, int> priorities;

public:
  virtual int value(const Circuit::vertex_descriptor node) = 0;
};

/**
 * @brief Node in lowest topological group takes precedence
 */
class PriorityTopological : public PriorityStatic {
public:
  PriorityTopological(const Circuit &circuit);
  virtual int value(const Circuit::vertex_descriptor node);
};

/**
 * @brief Node in highest topological group takes precedence
 */
class PriorityInverseTopological : public PriorityStatic {
public:
  PriorityInverseTopological(const Circuit &circuit);
  virtual int value(const Circuit::vertex_descriptor node);
};

/**
 * @brief Earliest available node takes precedence
 */
class PriorityEarliest : public PriorityStatic {
private:
  int lastValue = 0;

public:
  virtual int value(const Circuit::vertex_descriptor node);
};

/**
 * @brief Latest available node takes precedence
 */
class PriorityLatest : public PriorityStatic {
private:
  int lastValue = 0;

public:
  virtual int value(const Circuit::vertex_descriptor node);
};

/**
 * @brief Node with maximum out degree takes precedence
 */
class PriorityMaxOutDegree : public PriorityStatic {
public:
  PriorityMaxOutDegree(const Circuit &circuit);
  virtual int value(const Circuit::vertex_descriptor node);
};

/**
 * @brief Node with minimum out degree takes precedence
 */
class PriorityMinOutDegree : public PriorityStatic {
public:
  PriorityMinOutDegree(const Circuit &circuit);
  virtual int value(const Circuit::vertex_descriptor node);
};

#endif
