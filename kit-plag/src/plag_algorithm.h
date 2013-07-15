#pragma once
#ifndef	PLAGIARISM_ALGORITHM_H
#define PLAGIARISM_ALGORITHM_H

#include "common.h"
#include "preprocess.h"

/*
 * Common interface for plagiarism detection algorithms.
 */

/* Selector for a variant of one of the algorithms. */
struct AlgorithmType {
	std::string name, short_name;

	AlgorithmType(std::string name, std::string short_name) : 
		name(name), short_name(short_name) {
	}
};

/* Common interface for the plagiarism detection algorithm. */
class Algorithm {
public:
	/* Adds a reference text (numbered consecutively from 0). */
	virtual void add_reference(const Normalized& ref) = 0; 

	/* Print a visualization of the result into os. */
	virtual void visualize(std::ostream& os, const Normalized& nquery, const std::string& origquer, 
		                   const Normalized& nref, const std::string& origref) const = 0;

	/* Computes the similarity measure of a query to the references. */
	virtual std::vector<double> compute(const Normalized& query) const = 0;

	Algorithm() : index(0) {
	}

protected:
	/* Index of next reference text. */
	int index;
};

#endif