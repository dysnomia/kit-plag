#include "common.h"
#include "consts.h"
#include "visualize.h"
#include "ranking.h"
#include "fingerprinting.h"

using namespace std;
using boost::format;

/* Print selection item for a single measure. */
static void add_select(ostream& os, const AlgorithmType& measure) {
	os << format(HTML_OPTION) % ("#measure-" + measure.short_name) % measure.name;
}

/* Start of html file with boxes for selecting the measure/reference. */
void visualize_begin(ostream& os, const string& title, const vector<Normalized>& normal) {
	os << format(HTML_BEGIN) % title;

	// Selection box for choosing the metric
	os << format(INFO_TEXT) % "Choose a measure and a reference text:";
	os << format(HTML_SELECT_BEGIN) % "measure";
	for (const auto& type: fingerprint_variants) {
		add_select(os, type);
	}
	for (const auto& type: ranking_variants) {
		add_select(os, type);
	}
	
	// Selection box for choosing the compared files
	os << format(HTML_SELECT_MID) % "query";
	for (const auto &query: normal) {
		os << format(HTML_OPTION) % (".query-" + query.name) % query.name;
	}
	os << HTML_SELECT_END;

	// Start body
	os << HTML_MAIN_BEGIN;
}

/* Per measure table summarizing the results */
void visualize_summary(ostream& os, const vector<Normalized>& refs, const Normalized& query, vector<double> res, const string& measure_name) {
	os << format(INFO_TEXT) % ("Local summary for &lsquo;" + measure_name + "&rsquo;:");

	/* Head: names of reference texts with marks whether for
	   whether it should be recognized as a plagiarism. */
	os << HTML_SUMMARY_BEGIN;
	for(auto &ref: refs) {
		string html_class = query.plagiarises.count(ref.name) ? CLASS_PLAG : CLASS_NOPLAG;
		os << (format("          <th class=\"%1%\">") % html_class) << ref.name << "</th>" << endl;
	}

	/* Body: calculated measure value. */
	os << HTML_SUMMARY_MID;
	for(double d: res) {
		os << "          <td>" << d << "</td>" << endl;
	}
	os << HTML_SUMMARY_END;
}

/* End of html file */
void visualize_end(std::ostream& os) {
	os << HTML_MAIN_END << HTML_END;
}

/* Logging to stdout. */
void log_normalize(const std::string& name) {
	cout << "Reading and normalizing " << name << "..." << endl;
}

void log_header(const string& str) {
	cout << str << endl;
	cout << string(str.size(), '-') << endl;
}

void log_begin(const AlgorithmType& type) {
	log_header(type.name);
}

void log_results(const std::string& name, const std::vector<double>& results, const std::vector<Normalized>& refs) {
	/* Get sorted results with corresponding names */
	typedef pair<double, string> Sorted;
	vector<Sorted> sorted_results;
	int max_len = 0;
	for (int i = 0; i < int(results.size()); ++i) {
		sorted_results.push_back(make_pair(results[i], refs[i].name));
		max_len = max(max_len, int(refs[i].name.size()));
	}
	sort(begin(sorted_results), end(sorted_results), greater<Sorted>());

	cout << "  Query " << name << endl;
	for (const auto& val: sorted_results) {
		cout << "    vs. " << setw(max_len) << left << val.second << setw(0) << " = " << val.first << endl;
	}
}
