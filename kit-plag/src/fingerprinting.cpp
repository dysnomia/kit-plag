#include "common.h"
#include "consts.h"
#include "fingerprinting.h"
#include "fileutils.h"

using namespace std;
using boost::format;

/*
 * Implementation of the fingerprinting heuristic.
 */

/* Different algorithms for choosing the subsequences. */
// Return all subsequences of length GRANULARITY
static const auto all_subsequences = [] (const vector<string>& words) {
	vector<WordIt> res;
	for (auto it = words.begin(); it + GRANULARITY < words.end(); ++it) {
		res.push_back(it);
	}
	return res;
};
// Return random subsequences of length GRANULARITY
static const auto random_subsequences = [] (const vector<string>& words) {
	default_random_engine engine((unsigned int) time(NULL));
	auto random_float = bind(uniform_real_distribution<double>(), engine);

	vector<WordIt> res;
	for (auto it = words.begin(); it + GRANULARITY < words.end(); ++it) {
		if (random_float() > 0.3) {
			res.push_back(it);
		}
	}
	return res;
};

static const FingerprintType _fingerprint_variants[] = {
	FingerprintType("Full fingerprinting", "full", all_subsequences),
	FingerprintType("Random fingerprinting", "random", random_subsequences),
};
const vector<FingerprintType> fingerprint_variants(begin(_fingerprint_variants), end(_fingerprint_variants));

/* Hash function from Ramakrishna and Zobel, 1997. */
static int hashWords(WordIt from, WordIt to) {
	int h = 0;
	for(auto it = from; it != to; ++it) {
		for(char c: *it) {
			h = h ^ (c + (h << 6) + (h >> 2));
		}
	}
	return h;
}

/* Adds a reference text */
void Fingerprint::add_reference(const Normalized& ref) {
	for (auto hash: getHashes(ref)) {
		RefHashes[hash.first][index] = hash.second;
	}
	index++;
}

/* Visualize fingerprints. Mark matching subsequences. */
void Fingerprint::visualize(std::ostream &os, const Normalized& nquery, const string& origquer, 
							const Normalized& nref, const string& origref) const {
	typedef multimap<int, pair<bool, string>> EventType;
	auto queryHash = getHashes(nquery);
	auto   refHash = getHashes(nref);

	/* Add begin and end events for the sequences in common between query and reference */
	EventType eventsRef, eventsQuery;
	for (auto queryPair: queryHash) {
		if (refHash.count(queryPair.first)) {
			string id = "id-" + get_unique();

			for(int posq: queryPair.second) {
				eventsQuery.insert(make_pair(nquery.indexes[posq], make_pair(false, id)));
				eventsQuery.insert(make_pair(nquery.end_indexes[posq + GRANULARITY - 1], make_pair(true, id)));
			}
			for(int posr: refHash[queryPair.first]) {
				eventsRef.insert(make_pair(nref.indexes[posr], make_pair(false, id)));
				eventsRef.insert(make_pair(nref.end_indexes[posr + GRANULARITY - 1], make_pair(true, id)));
			}
		}
	}

	// Processing the events to produce a marked output
	auto mark = [&os] (const EventType& events, const string& orig) {
		int pos = 0;
		for(auto it = events.begin(); it != events.end(); ++it) {
			os << string(orig.begin() + pos, orig.begin() + it->first);
			pos = it->first;
			os << (it->second.first ? MARK_END : (format(MARK_BEGIN) % it->second.second).str());
		}
		os << string(orig.begin() + pos, orig.end());
	};

	// Produce marked output
	os << format(HTML_COMPARE_BEGIN) % nquery.name % nref.name;
	mark(eventsQuery, origquer);
	//string plag = nquery.plagiarises.count(nref.name) ? "" : "no";
	os << format(HTML_COMPARE_MID) % nref.name;
	mark(eventsRef, origref);
	os << HTML_COMPARE_END;
}

/* Actual computation of fingerprinting measure. */
vector<double> Fingerprint::compute(const Normalized& query) const {
	vector<double> res(index);

	/* Get all hashes of the query and determine how many
		times they occur in the references. */
	auto query_hashes = getHashes(query); 
	for (auto hash: query_hashes) {
		int hash_value = hash.first;
		auto per_ref = RefHashes.find(hash_value);

		if (per_ref != RefHashes.end()) {
			for (const pair<int,vector<int>> &occurences: per_ref->second) {
				res[occurences.first]++;
			}
		}
	}

	/* Normalize: divide by number of minutiae in query */
	for	(double& val: res) {
		val /= query_hashes.size();
	}

	return res;
}

/* 
 * Gets the hash values of a normalized text.
 * Format: map from hash value -> list of positions the hash value occurs at 
 */
unordered_map<int, vector<int>> Fingerprint::getHashes(const Normalized& ref) const {
	unordered_map<int, vector<int>> hashes;
	for(auto it: type.getPositions(ref.words)) {
		hashes[hashWords(it, it + GRANULARITY)].push_back(int(it - ref.words.begin()));
	}
	return hashes;
}


