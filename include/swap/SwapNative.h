#ifndef JEP_SWAP_NATIVE_H
#define JEP_SWAP_NATIVE_H

#include "swap/object.h"
#include "swap/native.h"
#include "swap/stringbuilder.h"

#if defined(_WIN32)
#	if defined(SWAPNATIVE_EXPORTS) || defined(__CYGWIN__)
#		define SWAPNATIVE_API __declspec(dllexport)
#	else
#		define SWAPNATIVE_API __declspec(dllimport)
#	endif
#	define SWAPNATIVE_CALL __cdecl
#elif defined(__linux__) || defined(__unix__)
#	define SWAPNATIVE_API
#	define SWAPNATIVE_CALL
#endif



/**
 * Calculates the length of an object.
 *
 * For primitive types such as integers and characters, the length is 1.
 * For strings, the length is the number of characters in the string.
 * For arrays, the length is the number of objects in the array.
 * For all other objects, the length is returned as a null object.
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_len(jep_obj *args, jep_obj* list);

/**
 * Evaluates the type of an object.
 *
 * The type of an object is returned as a string.
 * Example:
 *     n = 4;          // n is an integer
 *     t = typeof(n);  // t is a string with the value "int"
 *
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_typeof(jep_obj *args, jep_obj* list);

/**
 * Converts an object into an integer representation
 *
 * Numerical values are truncated.
 * Strings are converted to the value portrayed by their contents
 * Example:
 *     n = int("123"); // n now has the integer value 123
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_int(jep_obj *args, jep_obj* list);

/**
* Converts an object into a character representation
*
* Numerical values are truncated.
* Strings are converted to the value portrayed by their contents
* Example:
*     n = char("65"); // n now has the character value 'A'
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_char(jep_obj *args, jep_obj* list);

/**
* Converts an object into an double representation
*
* Numerical values are converted into their double equivalents
* Strings are converted to the value portrayed by their contents
* Example:
*     d = int("123.4"); // d now has the integer value 123.4
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_double(jep_obj *args, jep_obj* list);

/**
* Converts an object into an unsigned byte representation
*
* Integers of more than 1 byte in size are truncated to a single byte
* Characters are converted to the byte value
* Valid argument types are integer, long integer, and character
* Example:
*     b = byte(1000000); // b now has the byte value of 255
*
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_byte(jep_obj *args, jep_obj* list);

/**
* Converts an array of objects into an unsigned byte representation
*
* Integers of more than 1 byte in size are truncated to a single byte
* Characters are converted to the byte value
* Valid argument types are integer, long integer, and character
* Example:
*     b = bytes(1000000, "c"); // b now has the byte value of { 255, 'c' }
*
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_bytes(jep_obj *args, jep_obj* list);

/**
 * Writes a string of bytes to standard output
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_write(jep_obj* args, jep_obj* list);

/**
* Writes a string of bytes to standard output appended by a newline
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_writeln(jep_obj* args, jep_obj* list);

/**
 * Reads a line from standard input
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_readln(jep_obj* args, jep_obj* list);

/**
 * Reads a line from a file
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_freadln(jep_obj* args, jep_obj* list);

/**
 * Opens a file for reading and/or writing
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_fopen(jep_obj* args, jep_obj* list);

/**
* Writes a string to a file
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_fwrite(jep_obj* args, jep_obj* list);

/**
* Writes a string to a file appended by a newline
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_fwriteln(jep_obj* args, jep_obj* list);

/**
 * Reads a single byte from a file
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_freadb(jep_obj* args, jep_obj* list);

/**
 * Writes a single byte to a file
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_fwriteb(jep_obj* args, jep_obj* list);

/**
 * Creates a socket object
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_createSocket(jep_obj* args, jep_obj* list);

/**
* Binds a socket object
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_bindSocket(jep_obj* args, jep_obj* list);

/**
* Listens for connections to a socket
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_listenSocket(jep_obj* args, jep_obj* list);

/**
* Accepts connections from a client
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_acceptSocket(jep_obj* args, jep_obj* list);

/**
* Connects to a server socket
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_connectSocket(jep_obj* args, jep_obj* list);

/**
* Reads data from a socket
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_readSocket(jep_obj* args, jep_obj* list);

/**
* Writes data to a socket
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_writeSocket(jep_obj* args, jep_obj* list);

/**
* Closes a socket object
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_closeSocket(jep_obj* args, jep_obj* list);

/**
* Creates a thread
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_createThread(jep_obj* args, jep_obj* list);

/**
* Starts a thread
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_startThread(jep_obj* args, jep_obj* list);

/**
* sleeps for approximately the specified amount of milliseconds
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_sleep(jep_obj* args, jep_obj* list);

/**
 * prints the amount of object creation and destruciton in the native library
 */
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_print_native_call_count();

#endif
