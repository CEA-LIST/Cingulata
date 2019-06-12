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

#include <bit_exec/clear.hxx>

using namespace std;
using namespace cingulata;

ObjHandle BitExecClear::encode(const bit_plain_t pt_val) {
  ObjHandleT<bit_plain_t> out = mm.new_handle();
  *out = pt_val;
  return out;
}

ObjHandle BitExecClear::encrypt(const bit_plain_t pt_val) {
  ObjHandleT<bit_plain_t> out = mm.new_handle();
  *out = pt_val;
  return out;
}

bit_plain_t BitExecClear::decrypt(const ObjHandle &in) {
  return *(in.get<bit_plain_t>());
}

ObjHandle BitExecClear::read(const std::string &name) { return ObjHandle(); }

void BitExecClear::write(const ObjHandle &in, const std::string &name) {}

ObjHandle BitExecClear::op_and(const ObjHandle &in1, const ObjHandle &in2) {
  ObjHandleT<bit_plain_t> out = mm.new_handle();
  *out = *in1.get<bit_plain_t>() & *in2.get<bit_plain_t>();
  return out;
}

ObjHandle BitExecClear::op_xor(const ObjHandle &in1, const ObjHandle &in2) {
  ObjHandleT<bit_plain_t> out = mm.new_handle();
  *out = *in1.get<bit_plain_t>() ^ *in2.get<bit_plain_t>();
  return out;
}
