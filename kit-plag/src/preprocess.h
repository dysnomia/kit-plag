#pragma once
#ifndef PREPROCESS_H
#define PREPROCESS_H
#include "common.h"

/*
 * Functions for normalizing the input and splitting it into words and sentences.
 */

/* Output of normalization: normalized text and index into original text */
struct Normalized {
	std::string name;                      /* name of the query */
	std::set<std::string> plagiarises;  /* references that should be recognized as plagiarisms of this query */
	std::vector<std::string> words;        /* list of words in normalized text */
	std::vector<int> indexes, end_indexes; /* map: index in normalized text -> index in unnormalised text */
};

/* Normalize a text. */
Normalized normalize(const std::string& name, const std::string& text);

#endif