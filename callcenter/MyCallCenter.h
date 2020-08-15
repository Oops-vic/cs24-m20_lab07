#ifndef MYCALLCENTER_H
#define MYCALLCENTER_H

#include "CallCenter.h"

class MyCallCenter: public CallCenter {
  // Member Variables
public:
  // Constructor
  // Destructor?

  // Required Member Functions
  std::vector<int> calls(int minute, const std::vector<int>& call_ids);
  void learn(int minute, const std::vector<Call>& calls);

  // Other Member Functions
};

#endif
