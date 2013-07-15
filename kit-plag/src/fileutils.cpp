#include "common.h"
#include "fileutils.h"

using namespace std;

/* Opens a file for reading with error handling */
void open_input(const std::string& filename, /* out */ std::ifstream& file) {
	file.open(filename, ios_base::in | ios_base::binary);
	if (!file.is_open()) {
		throw domain_error("could not open file '" + filename);
	}
}

/* Opens a file for writing with error handling */
void open_output(const std::string& filename, /* out */ std::ofstream& file) {
	file.open(filename, ios_base::out | ios_base::binary);
	if (!file.is_open()) {
		throw domain_error("could not open file '" + filename);
	}
}

/* Reads a whole file/stream: somewhat inelegant */
string read_whole_stream(istream& in) {
	in.seekg(0, ios::end);
	string buf(in.tellg(), '$');
	in.seekg(0, ios::beg);
	in.read(&buf[0], buf.size());
	return buf;
}

/* Reads a list of words from a stream */
vector<string> read_word_list(istream& in) {
	vector<string> out;
	string tmp;
	while (in >> tmp) {
		out.push_back(tmp);
	}
	return out;
}

/* Reads the words from a stream and returns them as a set (e.g. for stop-words) */
unordered_set<string> read_word_set(istream& in) {
	unordered_set<string> out;
	string tmp;
	while (in >> tmp) {
		out.insert(tmp);
	}
	return out;
}

/* Gets a unique string */
static int current_id = 0;
string get_unique() {
	return "unique-" + to_string(current_id++);
}
