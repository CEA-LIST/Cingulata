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
#if !defined(__STORAGE_H__)
#define __STORAGE_H__

/* local includes */
#include <basic-exception.hxx>
#include "gate.hxx"

/*! \brief storage for intermediate result of a node in a boolean circuit */
class Storage
{
  public:
  /*! \brief exception thrown when a storage is assigned a value while
   *  it still holds a previous assignment */
  class AssignmentException : public BasicException
  {
    public:
    /*! \brief instantiate an exception with identification of
     *  storage
     *  \param[in] id_i string describing resource */
    AssignmentException
      (const typename std::string &id_i);
  };

  /*! \brief different storage locations */
  typedef enum __location_e
  {
    /*! \brief storage is a register */
    REGISTER,
    /*! \brief storage is a memory location */
    MEMORY,
    /*! \brief storage is at algorithm input */
    INPUT,
    /*! \brief storage is at algorithm output */
    OUTPUT
  } location_t;


  public:
  /*! \brief constructor, default storage location with default id */
  Storage
    (void)
  : location_m(MEMORY),
    id_m(-1)
  { }

  /*! \brief constructor, defines storage location and its unique
   *  identifier
   *  \param[in] location_i specification of storage location
   *  \param[in] id_i unique identifier of the storage in its location */
  Storage
    (const location_t &location_i,
     const unsigned &id_i)
  : location_m(location_i),
    id_m(id_i)
  { }

  /*! \brief virtual destructor for polymorphic class */
  virtual ~Storage(void) { }

  /*! \brief access storage location */
  const location_t& location(void) const { return location_m; }

  /*! \brief check if storage is at algorithm interface
   *  \return true if storage is at interface, false if in register or
   *  internal memory */
  bool interface(void) const
    { return location_m == INPUT || location_m == OUTPUT; }

  /*! \brief access storage id */
  const unsigned& id(void) const { return id_m; }

  /*! \brief access storage value */
  virtual Gate::value_t value(void) const { return Gate::UNKNOWN; }

  /*! \brief access storage value's state
   *  \return true if storage value is set and available for reading,
   *  false if it is unset and available for writing */
  virtual bool set(void) const { return true; }

  /*! \brief update value's state to assigned and available for reading
   *  \exception AssignmentException when assigning to a storage whose
   *  value is already available for reading */
  virtual void assign
    (void);

  /*! \brief reset value's state as after construction */
  virtual void reset(void) { }

  private:
  /*! \brief location of storage */
  location_t location_m;
  /*! \brief unique identifier of storage in its location */
  unsigned id_m;
};

/*! \brief insert representation of given storage to given stream
 *  \param out_io stream where to insert node representation
 *  \param[in] node_i node whose representation is inserted */
typename std::ostream& operator<<
  (typename std::ostream &out_io,
   const Storage &node_i);



/*! \brief storage for output of a node in a boolean circuit, which can be used
 *  for input too */
class OutputStorage : public Storage
{
  public:
  /*! \brief constructor, call default parent constructor and set state to
   *  unassigned */
  OutputStorage
    (void)
  : assigned_m(false)
  { }

  /*! \brief constructor, call parent constructor with given parameters
   *  and set state to unassigned */
  OutputStorage
    (const location_t &location_i,
     const unsigned &id_i)
  : Storage(location_i, id_i),
    assigned_m(false)
  { }

  /*! \brief access storage value's state
   *  \return true if storage value is set and available for reading,
   *  false if it is unset and available for writing */
  bool set(void) const { return assigned_m; }

  /*! \brief update value's state to assigned and available for reading
   *  \exception AssignmentException when assigning to a storage whose
   *  value is already available for reading */
  void assign
    (void);

  /*! \brief reset storage state as after construction */
  void reset
    (void);

  private:
  /*! \brief true if there was a prior assignment to this storage since
   *  last reset */
  bool assigned_m;
};



/*! \brief storage for a predefined input of a node in a boolean circuit */
class ImmediateStorage : public Storage
{
  public:
  /*! \brief constructor, instantiate an input storage at algorithm
   *  interface which holds a fixed value
   *  \param[in] value_i the fixed value to store in instruction word */
  ImmediateStorage
    (const bool &value_i);

  /*! \brief access storage value */
  Gate::value_t value(void) const { return value_m; }

  private:
  /*! \brief value of immediate storage */
  Gate::value_t value_m;
};


#endif /* !defined(__STORAGE_H__) */


