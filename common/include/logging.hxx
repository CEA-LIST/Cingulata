/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.

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

#ifndef CINGULATA_LOGGING
#define CINGULATA_LOGGING

/* for logging user-formated objects */
#include <fmt/ostream.h>

#define CINGU_LOG_LEVEL_TRACE 0
#define CINGU_LOG_LEVEL_DEBUG 1
#define CINGU_LOG_LEVEL_WARN 2
#define CINGU_LOG_LEVEL_ERROR 3
#define CINGU_LOG_LEVEL_OFF 4

#ifndef CINGU_LOG_LEVEL
#ifdef NDEBUG
/* logging disabled in release mode */
#define CINGU_LOG_LEVEL CINGU_LOG_LEVEL_OFF
#else
#define CINGU_LOG_LEVEL CINGU_LOG_LEVEL_TRACE
#endif
#endif

#include <cstring>
#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if CINGU_LOG_LEVEL <= CINGU_LOG_LEVEL_TRACE
#define CINGU_LOG_TRACE(_fmt, ...)                                             \
  fmt::print("[trace] [{}:{}]: " _fmt "\n", __FILENAME__, __LINE__,            \
             ##__VA_ARGS__)
#else
#define CINGU_LOG_TRACE(...) (void)0
#endif

#if CINGU_LOG_LEVEL <= CINGU_LOG_LEVEL_DEBUG
#define CINGU_LOG_DEBUG(_fmt, ...)                                             \
  fmt::print("[debug] [{}:{}]: " _fmt "\n", __FILENAME__, __LINE__,            \
             ##__VA_ARGS__)
#else
#define CINGU_LOG_DEBUG(...) (void)0
#endif

#if CINGU_LOG_LEVEL <= CINGU_LOG_LEVEL_WARN
#define CINGU_LOG_WARN(_fmt, ...)                                              \
  fmt::print("[warn] [{}:{}]: " _fmt "\n", __FILENAME__, __LINE__,             \
             ##__VA_ARGS__)
#else
#define CINGU_LOG_WARN(...) (void)0
#endif

#if CINGU_LOG_LEVEL <= CINGU_LOG_LEVEL_ERROR
#define CINGU_LOG_ERROR(_fmt, ...)                                             \
  fmt::print("[error] [{}:{}]: " _fmt "\n", __FILENAME__, __LINE__,            \
             ##__VA_ARGS__)
#else
#define CINGU_LOG_ERROR(...) (void)0
#endif

#endif
