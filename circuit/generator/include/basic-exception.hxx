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
#if !defined(__BASIC_EXCEPTION_H__)
#define __BASIC_EXCEPTION_H__

/* compiler includes */
#include <exception>



/*! \brief basic exception that only holds an error message */
class BasicException :
  public std::exception
{
  public:
    /*! \brief constructor, instantiate exception with given message */
    BasicException
      (const std::string &what_i)
    : what_m(what_i)
    { }

    /*! \brief destructor throwing no exception */
    ~BasicException(void) throw() { };

    /*! \brief access error message describing exception cause
     *  \return character string identifying exception cause */
    const char* what(void) const throw () { return what_m.c_str(); }

  protected:
  /*! \brief append given string to the message
   *  \param[in] message_i a string object to concatenate to current
   *  message */
  void append(const std::string &message_i) { what_m += message_i; }

  private:
    /*! \brief error message desribing exception cause */
    std::string what_m;
};



/*! \brief exception thrown when creation of a new element failed in underlying
 *  library */
class CreationFailure :
  public BasicException
{
  public:
    /*! \brief build generic error string following format: \"failed to
     *  \p context_i \p id_i\"
     *  \param[in] id_i identification of element
     *  \param[in] context_i context of element creation (copy, create,
     *  parse...) */
    CreationFailure
      (const std::string &id_i,
       const std::string &context_i) throw ()
    : BasicException("failed to " + context_i + " " + id_i)
    { }
};


#endif /* !defined(__BASIC_EXCEPTION_H__) */


