#pragma once
#ifndef FILEUTILS_H
#define FILEUTILS_H
#include "common.h"

/* 
 * Utility functions for reading from files & general streams 
 */

/* Opens an input/output file with appropriate error handling. */
void open_input(const std::string& filename, /* out */ std::ifstream& file);
void open_output(const std::string& filename, /* out */ std::ofstream& file);

/* Reads everything from a stream into a long string. */
std::string read_whole_stream(std::istream& in);

/* Reads words (simplified, stuff separated by whitespace [ \t\n]) into a vector. */
std::vector<std::string> read_word_list(std::istream& in);

/* Reads the set of words (simplified, stuff separated by whitespace [ \t\n]) into an unordered_set */
std::unordered_set<std::string> read_word_set(std::istream& in);

/* Gets a unique string. (Uses a global state, not thread safe) */
std::string get_unique();

#endif
