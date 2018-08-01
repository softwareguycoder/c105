/*
 * stdafx.h
 *
 *  Created on: Jul 31, 2018
 *      Author: bhart
 */

#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

typedef enum {false,true} bool;

#include <include/socket_errors.h>
#include <include/inetsock_core.h>
#include <include/conversion_core.h>
#include <include/debug_core.h>

#endif /* __STDAFX_H__ */
