#include "common.h"
#include "consts.h"
#include "ranking.h"
#include "fileutils.h"

using namespace std;
using boost::format;

/*
 * Implementations of various ranking strategies.
 */

/* Different similarity measures. */
//static auto normalise1 = [] (int f_d, int f_q) { 
//	return 1.0 / (1 +           abs((f_d - f_q)/f_q)); 
//};
static auto normalise2 = [] (int f_d, int f_q) { 
	return 1.0 / (1 + log(1.0 + abs((f_d - f_q)/f_q))); 
};

//static auto ranking1   = [] (int N, int f_t, int f_dt, int f_qt) { 
//	return log(double(N) / f_t) / (1.0 + abs(f_dt - f_qt)); 
//};
static auto ranking2   = [] (int N, int f_t, int f_dt, int f_qt) { 
	return log(1.0 + double(N) / f_t) / (1.0 + abs(f_dt - f_qt)); 
};
static auto ranking3   = [] (int N, int f_t, int f_dt, int f_qt) { 
	return (f_dt + f_qt) * log(1.0 + double(N) / f_t) / (1.0 + abs(f_dt - f_qt));
};
//static auto ranking4   = [] (int N, int f_t, int f_dt, int f_qt) { 
//	return log(double(N) / f_t) / (1.0 + abs(f_dt - f_qt));
//};
static auto ranking5   = [] (int N, int f_t, int f_dt, int f_qt) { 
	return double(N) / f_t / (1.0 + abs(f_dt - f_qt));
};

static const RankingType _ranking_variants[] = {
	//RankingType("Ranking variant 1", "variant1", normalise1, ranking1),
	RankingType("Ranking 2 (log normal, log per word)", "variant2", normalise2, ranking2),
	RankingType("Ranking 3 (log normal, sum per word)", "variant3", normalise2, ranking3),
	//RankingType("Ranking variant 4", "variant4", normalise2, ranking4),
	RankingType("Ranking 5 (log normal, linear per word)", "variant5", normalise2, ranking5),
};
const vector<RankingType> ranking_variants(begin(_ranking_variants), end(_ranking_variants));

/* Gets the word frequencies in a list of words. */
static Trie<int> get_freqs(const vector<string>& text) {
	Trie<int> out;
	for (const string& word: text) {
		out[word]++;
	}
	return out;
}

/* Adds a new reference text. Inserts word frequencies into freqs trie
   and summed freqs trie. Also stores the number of words of the reference text. */
void Ranking::add_reference(const Normalized& ref) {
	auto cur_freqs = get_freqs(ref.words);
	sum_words.push_back(0);
	for (auto& cur_freq: cur_freqs) {
		freqs[cur_freq.first].push_back(make_pair(index, cur_freq.second));
		sum_freqs[cur_freq.first] += cur_freq.second;
		sum_words[index] += cur_freq.second;
	}
	index++;
}

/* Generic summation based measure with a normalization (factor) function
   and a per-word addition. */
std::vector<double> Ranking::compute(const Normalized& text) const {
	vector<double> result(index, 0.0);

	/* Summation over words in query and text */
	auto textFreqs = get_freqs(text.words);
	for (const auto &freq: freqs) {
		if (textFreqs.has(freq.first)) {
			for (pair<int,int> f: freq.second) {
				result[f.first] += type.perWord(index, sum_freqs[freq.first], f.second, textFreqs[freq.first]);
			}
		}
	}

	/* Normalization */
	for (int i = 0; i < index; ++i) {
		result[i] *= type.factor(sum_words[i], (int)text.words.size());
	}

	return result;
}

/* Generic visualization. Marks the words that contribute most to the results. */
void Ranking::visualize(std::ostream &os, 
						const Normalized& nquery, const std::string& origquer, 
						const Normalized& nref, const std::string& origref) const {
	/* Vector with pairs (contribution, word) */
	vector<pair<double, string>> per_word_contrib;
	auto query_freqs = get_freqs(nquery.words);
	auto ref_freqs   = get_freqs(nref.words);

	/* Get the per-word contributions */
	double sum_contrib = 0.0;
	for (const auto &freq: ref_freqs) {
		if (query_freqs.has(freq.first)) {
			per_word_contrib.push_back(make_pair(
				type.perWord(index, sum_freqs[freq.first], freq.second, query_freqs[freq.first]), 
				freq.first
			));
			sum_contrib += per_word_contrib.back().first;
		}
	}

	/* Get words with most important contributions */
	sort(begin(per_word_contrib), end(per_word_contrib), greater<pair<double, string>>());
	map<string, string> ids;
	int max_idx = 0;
	double cur_sum = 0.0;
	while (max_idx < int(per_word_contrib.size()) && cur_sum < sum_contrib * RANKING_MAX_VISUALIZE) {
		ids[per_word_contrib[max_idx].second] = "id-" + get_unique();
		cur_sum += per_word_contrib[max_idx].first;
		max_idx++;
	}

	// Produce a marked text
	auto mark = [&os, &ids] (const Normalized& normal, const std::string& orig) {
		int pos = 0;
		for	(int i = 0; i < int(normal.words.size()); ++i) {
			const string& word = normal.words[i];
			int start = normal.indexes[i];
			int end = normal.end_indexes[i];

			/* Marked word */
			if (ids.count(word)) {
				os << orig.substr(pos, start - pos);
				os << format(MARK_BEGIN) % ids[word] 
				   << orig.substr(start, end - start) << MARK_END;
				pos = end;
			}
		}
		os << orig.substr(pos, orig.size() - pos);
	};

	// Produce marked output
	os << format(HTML_COMPARE_BEGIN) % nquery.name % nref.name;
	mark(nquery, origquer);
	//string plag = nquery.plagiarises.count(nref.name) ? "" : "no";
	os << format(HTML_COMPARE_MID) % nref.name;
	mark(nref, origref);
	os << HTML_COMPARE_END;
}

void Ranking::set_type(const RankingType& type) {
	this->type = type;
}
