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
#if !defined(__GATE_H__)
#define __GATE_H__

namespace Gate
{
  /*! \brief operator identifiers */
  typedef enum __operator_e
  {
    /*! \brief exclusive or operator */
    XOR,
    /*! \brief and operator */
    AND,
    /*! \brief inclusive or operator */
    OR,
    /*! \brief unary not operator */
    NOT
  } operator_t;

  /*! \brief count of different operators */
  enum { OPERATORS = 4 };

  /*! \brief possible input/output values of gates */
  typedef enum __value_e
  {
    /*! \brief value is known, it is false */
    FALSE,
    /*! \brief value is known, it is true */
    TRUE,
    /*! \brief value is unknown, it is either true or false */
    UNKNOWN
  } value_t;
};

#endif /* !defined(__GATE_H__) */

