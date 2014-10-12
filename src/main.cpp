#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include <stdint.h>

#include "cmdline.h"

struct greater_second {
  bool operator ()(
      const std::pair<std::string, uint64_t>& lhs,
      const std::pair<std::string, uint64_t>& rhs) const {
    return lhs.second > rhs.second;
  }
};

struct lower_second {
  bool operator ()(
      const std::pair<std::string, uint64_t>& lhs,
      const std::pair<std::string, uint64_t>& rhs) const {
    return lhs.second < rhs.second;
  }
};

template <typename C>
void sort_data(
    std::vector<std::pair<std::string, uint64_t> >& pairs,
    std::size_t count,
    C compare) {
  if (count >= pairs.size()) {
    std::sort(pairs.begin(), pairs.end(), compare);
  } else {
    std::partial_sort(pairs.begin(),
                      pairs.begin() + count,
                      pairs.end(),
                      compare);
  }
}

int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.set_program_name("cnt");
  p.add<size_t>("max", 'm', "Maximum nubmer of words to show", false, 0);
  p.add("reverse", 'r', "Reverse the result");
  p.parse_check(argc, argv);

  size_t count = p.get<size_t>("max");

  std::unordered_map<std::string, uint64_t> counts;

  std::string key;
  while (std::getline(std::cin, key)) {
    counts[key] += 1;
  }

  std::vector<std::pair<std::string, uint64_t> > pairs(counts.begin(),
                                                       counts.end());

  if (count == 0 || count > pairs.size()) {
    count = pairs.size();
  }

  if (p.exist("reverse")) {
    sort_data(pairs, count, lower_second());
  } else {
    sort_data(pairs, count, greater_second());
  }

  for (std::size_t i = 0; i < count; ++i) {
    std::cout << pairs[i].second << '\t' << pairs[i].first << std::endl;
  }
}
