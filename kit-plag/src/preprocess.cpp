#include "common.h"
#include "consts.h"
#include "preprocess.h"
#include "fileutils.h"

using namespace std;
using namespace boost::algorithm;

/* Extract words and sentences and return them in a normal form (words separated
 * by one space and a sentence ending with one dot) */
Normalized normalize(const std::string& name, const std::string& text) {
	Normalized out;
	out.name = name;
	ifstream in_stop; open_input(FILE_STOP_WORDS, in_stop);
	auto stop_words = read_word_set(in_stop);

	/* Iterate over words in text */
	auto word_begin = REGEX_NAMESPACE::sregex_iterator(begin(text), end(text), REGEX_WORD);
	auto word_end = REGEX_NAMESPACE::sregex_iterator();
	//bool in_quote = false;
	//char quote_start = '"';
	for (auto it = word_begin; it != word_end; ++it) {
		const auto& match = *it;
		string word = to_lower_copy(match.str());
		string filtered;
		
		for(auto it = word.begin(); it != word.end(); ++it) {
			if (isalnum(*it)) {
				filtered.push_back(*it);
			}
		}

		/* Strip out stop words. */
		if (stop_words.find(filtered) != stop_words.end()) {
			continue;
		}

		/* Add word */
		out.words.push_back(filtered);
		out.indexes.push_back(int(match.position()));
		out.end_indexes.push_back(int(match.position() + match.length()));
	}

	return out;
}
