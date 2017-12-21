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
#include <ostream>
#include <sstream>
#include <string>

/* local includes */
#include "storage.hxx"

/* namespaces */
using namespace std;

/* see base header */
Storage::AssignmentException::AssignmentException
  (const std::string &id_i)
: BasicException("assignment to in-use storage "+id_i)
{ }

/* see base header */
void Storage::assign
  (void)
{
  if (this->set())
  { /* case: storage already holds a value */
    /* description of storage location */
    stringstream location_v;
    location_v << *this;

    /* prevent assignment */
    throw AssignmentException(location_v.str());
  }
}

/* see base header */
ostream& operator<<
  (ostream &out_io,
   const Storage &node_i)
{
  switch (node_i.location())
  { /* set location description depending on location */
    case Storage::REGISTER: out_io << "REGISTER"; break;
    case Storage::MEMORY: out_io << "MEMORY"; break;
    case Storage::INPUT: out_io << "INPUT"; break;
    case Storage::OUTPUT: out_io << "OUTPUT"; break;
  }

  /* add storage id */
  out_io << "::" << node_i.id();
  return out_io;
}

/* see base header */
void OutputStorage::assign
  (void)
{
  /* call parent implementation and update value's state */
  this->Storage::assign ();
  assigned_m = true;
}

/* see base header */
void OutputStorage::reset
  (void)
{
  /* reset assignment state of storage */
  assigned_m = false;
}

/* see base header */
ImmediateStorage::ImmediateStorage
  (const bool &value_i)
: Storage(INPUT, unsigned(value_i)),
  value_m(value_i? Gate::TRUE : Gate::FALSE)
{ }


