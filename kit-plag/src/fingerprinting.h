#pragma once
#ifndef FINGERPRINTING_H
#define FINGERPRINTING_H
#include "common.h"
#include "plag_algorithm.h"
#include "preprocess.h"

/*
 * Fingerprinting heuristic for plagiarism detection.
 * Compares the number of matching minutiae (hashes of some substrings).
 */

/* 
 * Type of fingerprinting heuristic. 
 *
 * Function for the selecting starts of the respective subsequences. 
 *   Takes: a list of words
 *   Returns: list of iterators it such that [it, it + GRANULARITY) is dereferenceable. 
 */
typedef std::vector<std::string>::const_iterator WordIt;
typedef std::function<std::vector<WordIt>(const std::vector<std::string>&)> SubsequenceFunction;

struct FingerprintType : AlgorithmType {
	SubsequenceFunction getPositions;

	FingerprintType(const std::string& name, const std::string& short_name, 
		            const SubsequenceFunction& getPositions) :
		AlgorithmType(name, short_name), getPositions(getPositions) {
	}
};
extern const std::vector<FingerprintType> fingerprint_variants; /* All the variants of the fingerprint algorithm. */

/* Fingerprint algorithm. Any subsequence selection can be plugged in. */
class Fingerprint : public Algorithm {
public:
	Fingerprint(const FingerprintType& type) : type(type), RefHashes() {
	}

	/* Adds a reference text (numbered consecutively from 0). */
	virtual void add_reference(const Normalized& ref) override; 

	/* Visualize fingerprints. Mark matching subsequences. */
	virtual void visualize(std::ostream &os, const Normalized& nquery, const std::string& origquer, 
		                   const Normalized& nref, const std::string& origref) const override;

	/* Computes the similarity measure of a query to the reference. */
	virtual std::vector<double> compute(const Normalized& query) const override;

private:
	/* Gets the hash values of a normalized text.
	      Format: map from hash value -> list of positions the hash value occurs at */
	std::unordered_map<int, std::vector<int>> getHashes(const Normalized& ref) const;

	/* Selected subsequence algorithm. */
	FingerprintType type;

	/* 
	 * Stored hashes of reference texts
	 *   map hash value -> (map reference index -> positions of occurences)
	 */
	std::unordered_map<int, std::unordered_map<int, std::vector<int>>> RefHashes;
};

#endif
