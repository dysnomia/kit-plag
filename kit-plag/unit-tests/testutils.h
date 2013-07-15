#pragma once
#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

/*
 * Utility functions and macros for unit tests. (e.g. emulate initializer list, operator<< for several data
 * structures that are used in CHECK_EQUAL, ...)
 */

#include "common.h"
#include "preprocess.h"

/* Print a vector. */
template<typename T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	for (auto &elem : v) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
	return os;
}

/* Print an unordered set. */
template<typename T> std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& v) {
	for (auto &elem : v) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
	return os;
}

/* Print a normalized text */
std::ostream& operator<<(std::ostream& os, const Normalized& normal);

#endif
