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
 * path - a string designating the file's location
 * mode - a string indicating what will be dne with the file
 *        acceptable file modes:
 *            "r"  reading
 *            "a"  appending
 *
 * returns: a file object
 */
function fopen(path, mode);

/**
 * Reads a string of text from a file in terminated by a newline.
 *
 * file - a file object
 *
 * returns: a string
 */
function freadln(file);

/**
 * Writes a string of text to a file.
 *
 * returns: a string
 */
function fwrite(file, data);

/**
 * Writes a string of text to a file in appended by a newline.
 *
 * returns: a string
 */
function fwriteln(file, data);
