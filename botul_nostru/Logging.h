/*
 * =============================================================================
 *
 *       Filename:  Logging.h
 *        Authors:  Dodon Victor <dodonvictor at gmail dot com>
 *                  Cernov Vladimir <kaspersky_gg@gmail.com>
 *                  Maldur Vitalie <maldur.vitalik@yahoo.com>
 *                  Straticiuc Vicu <straticiuc_vicu@yahoo.com>
 *        Created:  02/25/2012
 *    Description:  
 *
 * =============================================================================
 */

/**
  * Logging functionality implementations. Hopefully, this will spare us a lot
  * of trouble in the future.
  *
  * For an object to be loggable, operator<< has to be defined on it.
  */

#ifndef LOGGING_H_
#define LOGGING_H_

#include "global.h"

#ifdef DEBUG

/** Logs message to standard error file. We can change this in the future. */
#define LOG(message) (gparam::logFile << message << std::endl).flush()

#else

/** Disable message logging. */
#define LOG(message) do { } while (false)

#endif

#endif

