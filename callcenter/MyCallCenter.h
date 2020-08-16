#ifndef MYCALLCENTER_H
#define MYCALLCENTER_H

#include "CallCenter.h"
#include <queue>

class MyCallCenter: public CallCenter {
    // Member Variables
    std::vector<Employee> mEmployees;                                   //Employees listed by id numbers (start 0)
    std::vector<Employee> EmployeeSorted;                               //Employees sorted by skills (from low to high)
    std::queue<int> unanswerd;                                          //Calls that goes straight into the pool
                                                                        //(only know call_id)
    std::vector<Call*> answered;                                        //Calls that has been answered and put to pool
                                                                        //(assume sorted by importance * wait_time from low to high)
    int time;                                                           //Current time

private:
    int findAble(Call* c);                                              //Find the employee id just able to solve the call
                                                                        //(return -1 for not find, not possible to trigger)
    int findexpWeightedWaitTime(Call c);                                //Calculate the expected weighted wait time
    bool sortBySkill(const Employee& lhs, const Employee& rhs);
    bool sortByWeightedWaitTime(const Call& lhs, const Call& rhs);      //Compare the expected weighted wait time
    void addInCall(std::vector<int>& vec, int indexVec, int indexAns);

public:
    // Constructor
    MyCallCenter(std::vector<Employee> employeeInfo);
    // Destructor?
    ~MyCallCenter();

    // Required Member Functions
    std::vector<int> calls(int minute, const std::vector<int>& call_ids);
    void learn(int minute, const std::vector<Call>& calls);

    // Other Member Functions
    std::vector<int> distribute();                                      //Distribute calls in answered vector to employees
};

#endif
