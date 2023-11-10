#ifndef MACROS_H
#define MACROS_H

#include "uthash.h"

#define MACRO_ITERABLEHASHFIELDS \
int id; \
UT_hash_handle hh;

#define MACRO_COMPONENTFIELDS MACRO_ITERABLEHASHFIELDS \
 int parent_id;

/* Defines empty functions for C files.
* A = Init
* B = Start
* C = Update
* 
*/
#define MACRO_DEFINECFUNCTIONS(COMPONENTNAME,A,B,C)

#define MACRO_DEFINEHFUNCTIONS(COMPONENTNAME,A,B,C)

#endif