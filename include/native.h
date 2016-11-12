#ifndef JEP_NATIVE_H
#define JEP_NATIVE_H

#include "object.h"

/**
 * calls a native function
 */
jep_obj* jep_call_native(const char* ident, jep_obj* args);

#endif
