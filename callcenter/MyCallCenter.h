#ifndef MYCALLCENTER_H
#define MYCALLCENTER_H

#include "CallCenter.h"
#include <queue>

class MyCallCenter: public CallCenter {
    // Member Variables
    std::vector<Employee> mEmployees;                                       //Employees listed by id numbers (start 0)
    std::vector<Employee> EmployeeSorted;                                   //Employees sorted by skills (from low to high)
    std::queue<int> unanswerd;                                              //Calls that goes straight into the pool
                                                                            //(only know call_id)
    std::vector<Call*> answered;                                            //Calls that has been answered and put to pool
                                                                            //(assume sorted by importance * wait_time from low to high)
    std::vector<int> lastResult;                                            //The result vector of the previous minute
    int time;                                                               //Current time

private:
    int findAblePosSorted(Call* c);                                         //Find the position of the able employee in EmployeeSorted
    int findAble(Call* c);                                                  //Find the employee id just able to solve the call
                                                                            //(return -1 for not find, not possible to trigger)
    void remove(std::vector<int>& action, int indexAction, int indexAns);   //Remove this->answered[indexAns], 
                                                                            //add the id into action[indexAction]

public:
    // Constructor
    MyCallCenter(std::vector<Employee> employeeInfo);
    // Destructor?
    ~MyCallCenter();

    // Required Member Functions
    std::vector<int> calls(int minute, const std::vector<int>& call_ids);
    void learn(int minute, const std::vector<Call>& calls);

    // Other Member Functions
    std::vector<int> distributeWork();                                      //Distribute calls in answered vector to employees
    void printInfo();
};

#endif
