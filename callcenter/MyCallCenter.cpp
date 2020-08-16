#include "MyCallCenter.h"
#include <algorithm>
#include <iostream>

CallCenter* CallCenter::create(std::vector<Employee> employees) {
    return new MyCallCenter(employees);
}

// MyCallCenter Member Functions
MyCallCenter::MyCallCenter(std::vector<Employee> employeeInfo){
    this->mEmployees = employeeInfo;
    this->EmployeeSorted = employeeInfo;
    std::sort((this->EmployeeSorted).begin(), (this->EmployeeSorted).end(), this->sortBySkill);
}

MyCallCenter::~MyCallCenter(){

}

bool MyCallCenter::sortBySkill(const Employee& lhs, const Employee& rhs){
    return (lhs.skill < rhs.skill);
}

int MyCallCenter::findexpWeightedWaitTime(Call c){
    int result;
    result = c.importance * (this->time + c.work_required - c.work_performed + 1 - c.recieved);
    return result;
}

bool MyCallCenter::sortByWeightedWaitTime(const Call& lhs, const Call& rhs){
    int leftWeight, rightWeight;
    leftWeight = this->findexpWeightedWaitTime(lhs);
    rightWeight = this->findexpWeightedWaitTime(rhs);
    return (leftWeight < rightWeight);
}

int MyCallCenter::findAble(Call* c){
    for (auto itr = (this->EmployeeSorted).begin(); itr != (this->EmployeeSorted).end(); itr++){
        if (itr->skill >= c->difficulty) return (itr->id);
    }
    return -1;
}

std::vector<int> MyCallCenter::distribute(){
    int ansLen = (this->answered).size();
    for (int i = ansLen - 1; i >= 0; i--){
        int empId = this->findAble(this->answered[i]);
        Employee emp = this->mEmployees[empId];
        if (!emp.call){
            
        }
    }
}

std::vector<int> MyCallCenter::calls(int minute, const std::vector<int>& call_ids){
    this->time = minute;
    for (auto id: call_ids){
        (this->unanswerd).push(id);
    }
    std::vector<int> result = this->distribute();

    for (auto itr = result.begin(); itr != result.end(); itr++){
        if (*itr == 0 && (this->unanswerd).size() > 0){                     //Answer unanswered calls if there's remaining employees
            *itr = (this->unanswerd).front();
            (this->unanswerd).pop();
            continue;
        }
        if (*itr == -1){                                                    //Finish calls
            Call cFin = this->mEmployees[itr - result.begin()].call;
            this->mEmployees[itr - result.begin()].call = nullptr;
            delete cFin;
        }
    }
    return result;
}

void MyCallCenter::learn(int minute, const std::vector<Call>& calls){
    this->time = minute;
    for (auto call: calls){
        Call c = new Call(call);
        (this->answered).push_back(temp);
    }
    std::sort((this->answered).begin(), (this->answered).end(), this->sortByWeightedWaitTime);
}
