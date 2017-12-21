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

import argparse
import networkx as nx
import utils

#TODO: inputs are only in the ".inputs" section and not all nodes whose names start with "i_"

def printMultDepthMax():
  print (max(multDepths.values()))

def printMultDepthPerOut():
  outputs = utils.getOutputNodes(circuit)
  outputs = sorted(outputs, key=lambda n: int(n[2:]))
  print ('\n'.join(map(lambda out: out + ': ' + str(multDepths[out]), outputs)))

def printMultGateCount():
  cnt = 0
  if 'and' in nodeCountPerType:
    cnt += nodeCountPerType['and']
  if 'or' in nodeCountPerType:
    cnt += nodeCountPerType['or']

  cnt_cc = 0
  if 'and_cc' in nodeCountPerType:
    cnt_cc += nodeCountPerType['and_cc']
  if 'or_cc' in nodeCountPerType:
    cnt_cc += nodeCountPerType['or_cc']

  if cnt_cc:
    print ("%d (%d)" % (cnt + cnt_cc, cnt_cc))
  else:
    print (cnt)


def printAddGateCount():
  cnt = 0
  if 'xor' in nodeCountPerType:
    cnt += nodeCountPerType['xor']
  if 'or' in nodeCountPerType:
    cnt += 2 * nodeCountPerType['or']

  cnt_cc = 0
  if 'not' in nodeCountPerType:
    cnt_cc += nodeCountPerType['not']
  if 'or_cc' in nodeCountPerType:
    cnt_cc += 2 * nodeCountPerType['or']

  if cnt_cc:
    print ("%d (%d)" % (cnt + cnt_cc, cnt_cc))
  else:
    print (cnt)

def printGateCount():
  print ('\n'.join(  map(lambda k,v: k + ': '+ str(v)),nodeCountPerType.iteritems()))

def printInputCount():
  print (len(utils.getInputNodes(circuit)))

def printInputs():
  print (" ".join(sorted(utils.getInputNodes(circuit))))
  
def printOutputCount():
  print (len(utils.getOutputNodes(circuit)))

def printOutputs():
  print (" ".join(sorted(utils.getOutputNodes(circuit))))

def readClearInps(fileName):
  f = open(fileName)
  tokens = f.read().split()
  clearInps = dict(zip(tokens[::2], map(lambda i: bool(int(i)), tokens[1::2])))
  f.close()
  return clearInps


allChoices = {
  'mult_depth_max': ('Maximal multiplicative depth', printMultDepthMax),
  'mult_depth_per_out': ('Multiplicative depth per output', printMultDepthPerOut),
  'mult_gate_cnt': ('Multiplicative gate count', printMultGateCount),
  'add_gate_cnt': ('Additive gate count', printAddGateCount),
  'gate_cnt': ('Gate count', printGateCount),
  'inp_cnt': ('Input count', printInputCount),
  'inps': ('Inputs', printInputs),
  'out_cnt': ('Output count', printOutputCount),
  'outs': ('Outputs', printOutputs)
}

parser = argparse.ArgumentParser(description='Compute graph characteristics (multiplicative depht,...)')
parser.add_argument('blifFile', type=str, help='input BLIF file')
for k, (desc, func) in allChoices.items():
  parser.add_argument('--' + k, action="store_true", default=False, help=desc)
parser.add_argument('--all', action="store_true", default=False, help="print all characteristics")
parser.add_argument('-v', action="store_true", default=False, dest="verbose")
parser.add_argument('--clear_inps', type=str, default = "", dest = "clear_inps_file", help='clear inputs file')
args = parser.parse_args()

if args.all:
  choices = allChoices
else:
  choices=dict(filter(lambda key_value: args.__dict__[key_value[0]] , allChoices.items()))

#exit if no choices selected
if not choices:
  print (parser.usage)
  exit(0)

circuit = utils.readBlifFile(args.blifFile)

clearInputs = dict()
if args.clear_inps_file:
  clearInputs = readClearInps(args.clear_inps_file)
  utils.updateClearCipherExecGates(circuit, clearInputs)

if 'mult_depth_max' in choices or 'mult_depth_per_out' in choices:
  multDepths = utils.getMultiplicativeDepths(circuit)

if 'mult_gate_cnt' in choices or 'add_gate_cnt' in choices or 'gate_cnt' in choices:
  nodeCountPerType = utils.getNodeCountPerType(circuit)

for k, (desc, func) in choices.items():
  if args.verbose:
    print (desc, ':')
  func()
