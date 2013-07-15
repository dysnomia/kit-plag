#pragma once
#ifndef RANKING_H
#define RANKING_H
#include "common.h"
#include "plag_algorithm.h"
#include "preprocess.h"
#include "trie.h"

/*
 * Generic implementation of frequency based similarity measures
 * with exchangeable functions.
 */

/*
 * Normalization function. Takes: 
 *   f_d (number of words in document/reference), 
 *   f_q (number of words in query)
 */
typedef std::function<double(int, int)> NormalisationFunc;      

/*
 * Per word contribution function. Takes:
 *   N:   number of documents/references,
 *  f_t:  number of occurrences of the word in all of the documents,
 *  f_dt: number of occurrences of the word in this document,
 *  f_qt: number of occurrences of the word in this query
 */
typedef std::function<double(int, int, int, int)> PerWordFunc;  

/* Selection of similarity measure functions. */
struct RankingType : public AlgorithmType {
	NormalisationFunc factor;
	PerWordFunc perWord;

	RankingType(const std::string& name, const std::string& short_name, 
		        const NormalisationFunc& factor, const PerWordFunc& perWord) :
		AlgorithmType(name, short_name), factor(factor), perWord(perWord) {
	}
};
extern const std::vector<RankingType> ranking_variants; /* All the ranking variants */

/* Generic ranking algorithm (plug in normalization function and per-word contribution). */
class Ranking : public Algorithm {
public:
	Ranking() : type(ranking_variants[0]), freqs(), sum_freqs(), sum_words() {
	}

	/* Adds a reference text (numbered consecutively from 0). */
	virtual void add_reference(const Normalized& ref) override;

	/* Generic summation based similarity measure: plug in in normalization function
	   and a per-word contribution function. */
	virtual std::vector<double> compute(const Normalized& text) const override;

	/* Generic visualization: mark words with huge contributions. */
	virtual void visualize(std::ostream &os, 
		                   const Normalized& nquery, const std::string& origquer, 
			               const Normalized& nref, const std::string& origref) const override;

	/* Set currently used ranking function. */
	void set_type(const RankingType& type);
private:
	/* Current ranking type */
	RankingType type;

	/* Storage for frequencies. Tries for all words with list of
	   pairs: #1 = idx of reference text, #2 = frequency as content. */
	typedef Trie<std::vector<std::pair<int, int>>> Freq;
	Freq freqs;

	/* Summed word frequencies over all reference texts. */
	Trie<int> sum_freqs;

	/* Lengths of reference texts */
	std::vector<int> sum_words;
};

#endif
