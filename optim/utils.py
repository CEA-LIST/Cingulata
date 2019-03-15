#
#    (C) Copyright 2017 CEA LIST. All Rights Reserved.
#    Contributor(s): Cingulata team (formerly Armadillo team)
#
#    This software is governed by the CeCILL-C license under French law and
#    abiding by the rules of distribution of free software.  You can  use,
#    modify and/ or redistribute the software under the terms of the CeCILL-C
#    license as circulated by CEA, CNRS and INRIA at the following URL
#    "http://www.cecill.info".
#
#    As a counterpart to the access to the source code and  rights to copy,
#    modify and redistribute granted by the license, users are provided only
#    with a limited warranty  and the software's author,  the holder of the
#    economic rights,  and the successive licensors  have only  limited
#    liability.
#
#    The fact that you are presently reading this means that you have had
#    knowledge of the CeCILL-C license and that you accept its terms.
#
#

import sys
import networkx as nx
import re
import itertools
import operator

def readBlifFile(fileName):
  f = open(fileName)
  lines = f.readlines()
  f.close()
  return parseBlif(lines)

def is_mult(tt):
  return tt and tt in is_mult.tt
is_mult.tt = [
  '11 1',       # and
  '11 0',       # nand
  '01 1',       # andny
  '10 1',       # andyn
  '00 0',       # or
  '00 1',       # nor
  '10 0',       # orny
  '01 0',       # oryn
]

def is_add(tt):
  return tt and tt in is_add.tt
is_add.tt = [
  # '0 1',        # not
  '01 1\n10 1', # xor
  '10 1\n01 1', # xor
  '00 1\n11 1', # xnor
  '11 1\n00 1', # xnor
]

def parseBlif(lines):
  cmds = "".join(lines).split('.')
  G = nx.DiGraph()
  for cmd in cmds:
    if cmd.startswith('names'):
      cmd = cmd.strip().split('\n')
      var = cmd[0].split()[1:]
      out = var[-1]
      G.add_nodes_from(var)
      edges = [(v, out) for v in var[:-1]]
      G.add_edges_from(edges)
      G.node[out]['gate'] = True
      G.node[out]['truth_table_var'] = var
      G.node[out]['truth_table'] = "\n".join(cmd[1:]).strip()
    elif cmd.startswith('inputs'):
      cmd = cmd.replace('\\','').split()
      nodes = cmd[1:]
      G.add_nodes_from(nodes, input=True)
    elif cmd.startswith('outputs'):
      cmd = cmd.replace('\\','').split()
      nodes = cmd[1:]
      G.add_nodes_from(nodes, output=True)

  return G

def getMultiplicativeDepth(G):
  depths = getMultiplicativeDepths(G)
  outs = getOutputNodes(G)
  return max(dict(filter(lambda e: e[0] in outs, depths.items())).values())

def getMultiplicativeDepths(G):
  depth = dict()
  for v in nx.topological_sort(G):
    if G.in_degree(v) == 0:
      depth[v] = 0
    else:
      depth[v] = max([depth[u] for u in G.predecessors(v)]) + is_mult(G.node[v].get('truth_table'))
  return depth

def getInputNodes(G):
  return list(filter(lambda n: 'input' in G.node[n], G.nodes()))

def getOutputNodes(G):
  return list(filter(lambda n: 'output' in G.node[n], G.nodes()))

truthTable2GateType = {
  '0' : 'const_0',
  '1' : 'const_1',
  '0 1' : 'not',
  '1 1' : 'buf',
  '11 1' : 'and',
  '11 0' : 'nand',
  '01 1' : 'andny',
  '10 1' : 'andyn',
  '00 0' : 'or',
  '00 1' : 'nor',
  '10 0' : 'orny',
  '01 0' : 'oryn',
  '01 1\n10 1' : 'xor',
  '10 1\n01 1' : 'xor',
  '00 1\n11 1' : 'xnor',
  '11 1\n00 1' : 'xnor',
}


def getNodeCountPerType(G):
  nodes = list(filter(lambda n: 'truth_table' in G.node[n], G.nodes()))

  nodeTypes = list(map(lambda n: truthTable2GateType[G.node[n]['truth_table']], nodes))

  nodeCount = dict()
  for nodeType in set(nodeTypes):
    nodeCount[nodeType] = len(list(filter(lambda nt: nodeType == nt, nodeTypes)))

  return nodeCount

def getMultiplicativeNodes(G):
  return list(filter(lambda n: is_mult(G.node[n].get('truth_table')), G.nodes()))

def getAdditiveNodes(G):
  return list(filter(lambda n: is_add(G.node[n].get('truth_table')), G.nodes()))

def getMultiplicativeNodeCnt(G):
  return len(getMultiplicativeNodes(G))

def getAdditiveNodeCnt(G):
  return len(getAdditiveNodes(G))
