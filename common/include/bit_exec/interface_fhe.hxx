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

#ifndef BIT_EXEC_INTERFACE_FHE
#define BIT_EXEC_INTERFACE_FHE

#include <bit_exec/interface.hxx>

#include <memory>
#include <stdint.h>
#include <string>

namespace cingulata {

/**
 * @brief      Abstract interface to fully HE schemes with boolean plaintext
 *             space.
 * @note       This interface is adapted for fast bootstrapping HE schemes (ie
 *             TFHE).
 */
class IBitExecFHE : public IBitExec {
public:
  /* clang-format off */

  IBitExecFHE()          = default;
  virtual ~IBitExecFHE() = default;

  /* clang-format on */
};

} // namespace cingulata

#endif
