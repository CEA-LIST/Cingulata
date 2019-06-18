/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
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

#include <bit_exec/interface_she.hxx>
#include <bit_exec/obj_handle.hxx>

using namespace cingulata;

ObjHandle IBitExecSHE::op_not(const ObjHandle& in1) {
  return op_xor(in1, encode(1));
}

ObjHandle IBitExecSHE::op_nand(const ObjHandle& in1, const ObjHandle& in2) {
  return op_not(op_and(in1, in2));
}

ObjHandle IBitExecSHE::op_andyn(const ObjHandle& in1, const ObjHandle& in2) {
  return op_and(in1, op_not(in2));
}

ObjHandle IBitExecSHE::op_andny(const ObjHandle& in1, const ObjHandle& in2) {
  return op_and(op_not(in1), in2);
}

ObjHandle IBitExecSHE::op_or(const ObjHandle& in1, const ObjHandle& in2) {
  return op_xor(in2, op_xor(in1, op_and(in1, in2)));
}

ObjHandle IBitExecSHE::op_nor(const ObjHandle& in1, const ObjHandle& in2) {
  return op_not(op_or(in1, in2));
}

ObjHandle IBitExecSHE::op_oryn(const ObjHandle& in1, const ObjHandle& in2) {
  return op_or(in1, op_not(in2));
}

ObjHandle IBitExecSHE::op_orny(const ObjHandle& in1, const ObjHandle& in2) {
  return op_or(op_not(in1), in2);
}

ObjHandle IBitExecSHE::op_xnor(const ObjHandle& in1, const ObjHandle& in2) {
  return op_not(op_xor(in1, in2));
}

ObjHandle IBitExecSHE::op_mux(const ObjHandle& c, const ObjHandle& in1, const ObjHandle& in2) {
  return op_xor(op_and(c, op_xor(in1, in2)), in2);
}
