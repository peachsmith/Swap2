/**
 * io - basic input and output routines
 * 
 * author: John Powell
 */

/**
 * Writes a string of text to standard out. If the object passed in is not
 * already a string, it will be converted to a string representation.
 *
 * data - the object to be written
 */
function write(data);

/**
 * Writes a string of text to standard out appened by a newline. If the object
 * passed in is not already a string, it will be converted to a string
 * representation.
 *
 * data - the object to be written
 */
function writeln(data);

/**
 * Reads a string of text from standard in terminated by a newline.
 *
 * returns: a string
 */
function readln();

/**
 * Opens a file.
 *
 * returns: a file object
 */
function fopen(path);

/**
 * Reads a string of text from a file in terminated by a newline.
 *
 * returns: a string
 */
function freadln(file);
