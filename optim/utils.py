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
# import tribool

def readBlifFile(fileName):
  f = open(fileName)
  lines = f.readlines()
  f.close()
  return parseBlif(lines)

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
      G.node[out]['truth_table'] = ";".join(cmd[1:]).strip()
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
  depth = getMultiplicativeDepths(G)
  return max(depth.values())

def getMultiplicativeDepths(G, const_inps = ['i_0', 'i_1']):
  depth = dict()
  for v in nx.topological_sort(G):
    if G.in_degree(v) == 0:
      depth[v] = 0
    else:
      if (G.node[v]['truth_table'] == '00 0' or G.node[v]['truth_table'] == '11 1') and 'clear_cipher_exec' not in G.node[v]:
        depth[v] = max([depth[u] + 1 for u in G.predecessors(v)])
      else:
        depth[v] = max([depth[u] for u in G.predecessors(v)])
  return depth

def getInputNodes(G):
  return filter(lambda n: n.startswith('i_'), G.nodes_iter())

def getOutputNodes(G):
  return filter(lambda n: n.startswith('m_'), G.nodes_iter())

truthTable2GateType = {
  '0': 'const_0',
  '1': 'const_1',
  '0 1': 'not',
  '00 0': 'or',
  '10 1;01 1': 'xor',
  '01 1;10 1': 'xor',
  '11 1': 'and',
  '1 1': 'buf'}

def getNodeCountPerType(G):
  nodes = filter(lambda n: 'truth_table' in G.node[n], G.nodes_iter())

  clearCipherNodes = filter(lambda n: 'clear_cipher_exec' in G.node[n], nodes)
  cipherNodes = filter(lambda n: 'clear_cipher_exec' not in G.node[n], nodes)

  nodeTypes = map(lambda n: truthTable2GateType[G.node[n]['truth_table']], cipherNodes)
  nodeTypes += map(lambda n: truthTable2GateType[G.node[n]['truth_table']] + '_cc', clearCipherNodes)

  clearCipherExecNodes = map(lambda n: G.node[n]['truth_table'], nodes)

  nodeCount = dict()
  for nodeType in set(nodeTypes):
    nodeCount[nodeType] = len(filter(lambda nt: nodeType == nt, nodeTypes))

  return nodeCount

# def updateClearCipherExecGates1(G, clearInps):
#   if not clearInps:
#     return

#   clearCipherExecNodes = clearInps
#   for v in nx.topological_sort(G):
#     if G.in_degree(v) > 0:
#       predClearCipher = filter(lambda u: u in clearCipherExecNodes, G.predecessors_iter(v))
#       if predClearCipher and 'truth_table' in G.node[v]:
#         G.node[v]['clear_cipher_exec'] = True

#         gt = truthTable2GateType[G.node[v]['truth_table']]

#         u = predClearCipher.pop()
#         if predClearCipher:
#           w = predClearCipher.pop()
#           if gt == 'xor':
#             clearCipherExecNodes[v] = clearCipherExecNodes[u] ^ clearCipherExecNodes[w]
#           elif gt == 'and':
#             clearCipherExecNodes[v] = clearCipherExecNodes[u] & clearCipherExecNodes[w]
#           elif gt == 'or':
#             clearCipherExecNodes[v] = clearCipherExecNodes[u] | clearCipherExecNodes[w]

#         elif gt == 'not':
#           clearCipherExecNodes[v] = not clearCipherExecNodes[u]
          
#         elif gt == 'and' and clearCipherExecNodes[u] == False:
#           clearCipherExecNodes[v] = False

#         elif gt == 'or' and clearCipherExecNodes[u] == True:
#           clearCipherExecNodes[v] = True


def updateClearCipherExecGates(G, clearInps):
  if not clearInps:
    return

  inpsToExam = filter(lambda v: v in G and G.in_degree(v) == 0, clearInps)
  nodesToExam = reduce(lambda a,b: a+b, map(lambda v: G.successors(v), inpsToExam))
  for v in nodesToExam:
    if 'truth_table' in G.node[v]:
      G.node[v]['clear_cipher_exec'] = True


def computeLevels(G):
  levels = dict()
  for v in nx.topological_sort(G):
    if G.in_degree(v) == 0:
      levels[v] = 0
    else:
        levels[v] = max([levels[u] + 1 for u in G.predecessors(v)])
  return levels

def getTopologicalOrder(G):
  return list(nx.topological_sort(G))




def readPredefFile(fileName):
  f = open(fileName)
  lines = f.readlines()
  f.close()
  return parsePredefined(lines)

def parsePredefined(lines):
  values = dict()
  for line in lines:
    k,v = line.split()
    values[k] = v
  return values

def getInputValues(values):
  return dict(filter(lambda k,v: k.startswith('i_'), values.iteritems()))

def getOutputValues(values):
  return dict(filter(lambda k,v: k.startswith('m_'), values.iteritems()))

def buildTruthTable(rtt_str):
  negBit = {"0": "1", "1": "0", "-": "-"}  
  spRtt = rtt_str.split(";")
  nrInputVars = len(spRtt[0]) - 2

  rtt = dict([(s[:nrInputVars], s[-1]) for s in spRtt])

  if len(set(rtt.values())) != 1:
    print ("ERROR: more than one default value")
    return rtt

  defVal = negBit[rtt.values()[0]]

  tt = dict([("".join(e), defVal) for e in itertools.product(list("01"), repeat = nrInputVars)])

  for k, v in rtt.iteritems():
    k = k.replace("-", ".")
    mtt = filter(lambda ttk: re.match(k, ttk), tt.keys())
    for se in mtt:
      tt[se] = v

  return tt

def symbolicExecution(G, inputValues):
  values = dict()
  tts = dict()
  for v in nx.topological_sort(G):
    if G.in_degree(v) == 0:
      if v not in inputValues:
        values[v] = '0'
        print ('Error: node', v, 'doesn\'t have a defined input value') 
      else:
        values[v] = inputValues[v]
    else:
      rtt_str = G.node[v]['truth_table']
      if rtt_str not in tts:
        tts[rtt_str] = buildTruthTable(rtt_str)
      tt = tts[rtt_str]
        
      inVals = ''.join(map(lambda u: values[u], G.node[v]['truth_table_var'][:-1]))
      values[v] = tt[inVals]
      
  return values
