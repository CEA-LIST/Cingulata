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

#ifndef SECRET_KEY
#define SECRET_KEY

#include <string>

#include "polyring.hxx"

class SecretKey {
public:
  const PolyRing &get() const { return m_sk; }

  /**
   * @brief      Read secret key
   *
   * @param      fileName  file name
   * @param[in]  binary    binary or text mode
   */
  void read(const std::string &fileName, const bool binary = true);

  /**
   * @brief      Write secret key
   *
   * @param      fileName  file name
   * @param[in]  binary    binary or text mode
   */
  void write(const std::string &fileName, const bool binary = true) const;

  /**
   * @brief      Generate secret key
   */
  void generate();

private:
  PolyRing m_sk;
};

#endif
