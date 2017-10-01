#pragma once

#include <fstream>
#include <unordered_set>
#include <queue>
#include <vector>
#include <stdlib.h>

std::vector<std::string> get_imported_files(std::string filename) {
	std::vector<std::string> rtn;
	std::ifstream myfile(filename);
	if (myfile.is_open()) {
		std::string line;
		while (getline(myfile, line)) {
			uint64_t i = 0;
			while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) { ++i; }
			if (line.substr(i, 6) != "import") {
				return rtn;
			}
			line = line.substr(i + 6, line.length());

			i = 0;
			while (i < line.length()) {
				if (line[i++] != '"') { continue; }
				uint64_t start = i;
				while (i < line.length() && line[i++] != '"') {}
				std::string x = line.substr(start, i - start - 1);
				rtn.push_back(x);
			}
		}
		myfile.close();
	}
	else {
		throw std::runtime_error("Error: couldn't find file '" + filename + "'\n");
	}
	return rtn;
}

std::vector<std::string> read_file_lines(std::string filename) {
	std::vector<std::string> rtn;
	std::ifstream myfile(filename);
	if (myfile.is_open()) {
		std::string line;
		while (getline(myfile, line)) {
			rtn.push_back(line);
		}
	}
	return rtn;
}

std::string read_file(std::string filename) {
	std::string text = "";
	std::ifstream myfile(filename);
	if (myfile.is_open()) {
		std::string line;
		while (getline(myfile, line)) {
			text += line;
			text += '\n';
		}
	}
	return text;
}

unsigned long hash_file(std::string filename) {

	// TODO: use a better hashing algorithm

	std::string text = read_file(filename);

	while (text.length() % 8 != 0) {
		text += ' ';
	}
	unsigned long* numbers = (unsigned long*) &text[0];
	const uint64_t n = text.length() * sizeof(text[0]) / sizeof(unsigned long);

	unsigned long rtn = 0;
	for (uint64_t i = 0; i < n; ++i) {
		rtn ^= numbers[i];
	}

	return rtn;
}

std::vector<std::string> findModifiedFiles(std::string rootFile) {

	std::unordered_set<std::string> processed;
	std::queue<std::string> unprocessed;
	unprocessed.push(rootFile);

	std::vector<std::pair<std::string, unsigned long>> A;

	while (!unprocessed.empty()) {

		std::string file = unprocessed.front();
		unprocessed.pop();

		if (processed.count(file) > 0) {
			continue;
		}

		// process file here
		A.push_back(std::pair<std::string, unsigned long>(file, hash_file(file)));

		processed.insert(file);
		std::vector<std::string> importedFiles = get_imported_files(file);
		for (auto it = importedFiles.begin(); it != importedFiles.end(); ++it) {
			if (processed.count(*it) == 0) {
				unprocessed.push(*it);
			}
		}
	}

	std::sort(A.begin(), A.end(), [](std::pair<std::string, unsigned long> a, std::pair<std::string, uint64_t> b) {
		return a.first < b.first;
	});

	// 'A' is an lexicographically-ordered list of files and their hashes
	// now we load the old hashes and compare their file-int pairs

	std::vector<std::string> oldHashFile = read_file_lines("last_hashes.txt");
	std::vector<std::pair<std::string, unsigned long>> oldHashes(oldHashFile.size());
	for (uint64_t i = 0; i < oldHashFile.size(); ++i) {
		uint64_t j = 0;
		while (oldHashFile[i][++j] != ':') {}
		oldHashes[i].first = oldHashFile[i].substr(j + 1, oldHashFile[i].size());
		oldHashes[i].second = strtoul(&oldHashFile[i][0], nullptr, 0);
	}

	uint64_t i = 0;
	uint64_t j = 0;
	std::vector<std::string> modifiedFiles;
	while (i < A.size() && j < oldHashes.size()) {
		if (A[i].first == oldHashes[j].first) {
			if (A[i].second != oldHashes[j].second) {
				modifiedFiles.push_back(oldHashes[j].first);
			}
			++i;
			++j;
		}
		else if (A[i].first < oldHashes[j].first) {
			modifiedFiles.push_back(A[i++].first);
		}
		else {
			modifiedFiles.push_back(oldHashes[j++].first);
		}
	}
	while (i < A.size()) {
		modifiedFiles.push_back(A[i++].first);
	}
	while (j < oldHashes.size()) {
		modifiedFiles.push_back(oldHashes[j++].first);
	}

	return modifiedFiles;
}