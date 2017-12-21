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

#include "priority.hxx"

#include <vector>
#include <boost/graph/topological_sort.hpp>

using namespace std;

PriorityTopological::PriorityTopological(const Circuit& circuit) {
  vector<Circuit::vertex_descriptor> topo_order;
  topological_sort(circuit, back_inserter(topo_order));
  int prior = 0;
  for (auto it = topo_order.rbegin(); it != topo_order.rend(); it++) {
    priorities.emplace(*it, prior--);
  }
}

int PriorityTopological::value(const Circuit::vertex_descriptor node) {
  return priorities.at(node);
}

PriorityInverseTopological::PriorityInverseTopological(const Circuit& circuit) {
  vector<Circuit::vertex_descriptor> topo_order;
  topological_sort(circuit, back_inserter(topo_order));
  int prior = 0;
  for (auto it = topo_order.rbegin(); it != topo_order.rend(); it++) {
    priorities.emplace(*it, prior++);
  }
}

int PriorityInverseTopological::value(const Circuit::vertex_descriptor node) {
  return priorities.at(node);
} 

int PriorityEarliest::value(const Circuit::vertex_descriptor node) {
  if (priorities.find(node) == priorities.end()) {
    priorities.emplace(node, lastValue--);
  }
  return priorities.at(node);
}

int PriorityLatest::value(const Circuit::vertex_descriptor node) {
  if (priorities.find(node) == priorities.end()) {
    priorities.emplace(node, lastValue++);
  }
  return priorities.at(node);
}

PriorityMaxOutDegree::PriorityMaxOutDegree(const Circuit& circuit) {
  for(auto it = vertices(circuit); it.first != it.second; ++it.first) {
    const Circuit::vertex_descriptor& node = *(it.first);
    priorities.emplace(node, out_degree(node, circuit));
  }
}

int PriorityMaxOutDegree::value(const Circuit::vertex_descriptor node) {
  return priorities.at(node);
}

PriorityMinOutDegree::PriorityMinOutDegree(const Circuit& circuit) {
  for(auto it = vertices(circuit); it.first != it.second; ++it.first) {
    const Circuit::vertex_descriptor& node = *(it.first);
    priorities.emplace(node, -out_degree(node, circuit));
  }
}

int PriorityMinOutDegree::value(const Circuit::vertex_descriptor node) {
  return priorities.at(node);
}
