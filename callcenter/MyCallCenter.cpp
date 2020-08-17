#include "MyCallCenter.h"
#include <algorithm>
#include <iostream>

bool sortBySkill(Employee lhs, Employee rhs){
    return (lhs.skill < rhs.skill);
}

int findexpWeightedWaitTime(Call* c, int time){                          //Calculate the expected weighted wait time
    int result;
    result = c->importance * (time + c->work_required - c->work_performed + 1 - c->recieved);
    return result;
}

bool sortByWeightedWaitTime(Call* lhs, Call* rhs, int time){             //Compare the expected weighted wait time
    int leftWeight, rightWeight;
    leftWeight = findexpWeightedWaitTime(lhs, time);
    rightWeight = findexpWeightedWaitTime(rhs, time);
    return (leftWeight < rightWeight);
}

void sortEmployeeSkill(std::vector<Employee>& result){
    int len = result.size();
    for (int i = 0; i < len; i++){
        for (int j = i + 1; j < len; j++){
            if (sortBySkill(result[j], result[i])){
                Employee temp = result[i];
                result[i] = result[j];
                result[j] = temp;
            }
        }
    }
}

void sortExpWeightedWaitTime(std::vector<Call*>& result, int time){
    int len = result.size();
    for (int i = 0; i < len; i++){
        for (int j = i + 1; j < len; j++){
            if (sortByWeightedWaitTime(result[j], result[i], time)){
                Call* temp = result[i];
                result[i] = result[j];
                result[j] = temp;
            }
        }
    }
}

CallCenter* CallCenter::create(std::vector<Employee> employees) {
    return new MyCallCenter(employees);
}

// MyCallCenter Member Functions
MyCallCenter::MyCallCenter(std::vector<Employee> employeeInfo){
    this->mEmployees = employeeInfo;
    this->EmployeeSorted = employeeInfo;
    sortEmployeeSkill(this->EmployeeSorted);
}

MyCallCenter::~MyCallCenter(){

}

int MyCallCenter::findAblePosSorted(Call* c){
    for (auto itr = (this->EmployeeSorted).begin(); itr != (this->EmployeeSorted).end(); itr++){
        if (itr->skill >= c->difficulty) return (itr - (this->EmployeeSorted).begin());
    }
    return -1;
}

int MyCallCenter::findAble(Call* c){
    int pos = this->findAblePosSorted(c);
    if (pos != -1){
        return (this->EmployeeSorted[pos]).id;
    }
    return -1;
}

void MyCallCenter::remove(std::vector<int>& action, int indexAction, int indexAns){
    action[indexAction] = (this->answered[indexAns])->id;
    (this->answered).erase((this->answered).begin() + indexAns);
}

std::vector<int> MyCallCenter::distributeWork(){
    std::vector<int> result;
    bool empIndicator[(this->mEmployees).size()];
    result.resize((this->mEmployees).size(), 0);
    for (unsigned int i = 0; i < (this->mEmployees).size(); i++){
        empIndicator[i] = true;
    }
    int ansLen = (this->answered).size();

    for (int i = ansLen - 1; i >= 0; i--){
        int pos = this->findAblePosSorted(this->answered[i]);
        int empId1 = this->findAble(this->answered[i]);
        Employee* emp1 = &(this->mEmployees[empId1]);
        if (!emp1->call){                //Empty case for just able employee
            emp1->call = this->answered[i];
            this->remove(result, empId1, i);
            if (this->lastResult[empId1] != emp1->call->id) empIndicator[empId1] = false;
            continue;
        }
        else{
            int empId2;
            Employee* emp2;
            bool tempIndicator = false;
            for (unsigned int j = pos; j < (this->EmployeeSorted).size(); j++){
                empId2 = (this->EmployeeSorted[j]).id;
                emp2 = &(this->mEmployees[empId2]);
                if (!emp2->call){                                           //Empty case for overqualified employee
                    emp2->call = this->answered[i];
                    this->remove(result, empId2, i);
                    if (this->lastResult[empId2] != emp2->call->id) empIndicator[empId2] = false;
                    tempIndicator = true;
                    break;
                }
            }
            if (tempIndicator) continue;
        }

        if (empIndicator[empId1]){                                          //Case for optimizing the expected weighted time
            int ansWeight = findexpWeightedWaitTime(this->answered[i], this->time);
            int empWeight = findexpWeightedWaitTime(emp1->call, this->time);
            if (ansWeight > empWeight){
                int temp = emp1->call->id;
                (this->answered).push_back(emp1->call);
                emp1->call = this->answered[i];
                this->remove(result, empId1, i);
                empIndicator[empId1] = false;
            }
        }
    }
    sortExpWeightedWaitTime(this->answered, this->time);

    for (unsigned int i = 0; i < result.size(); i++){                                //Do the calls or finish the calls or learn the call
        Employee* emp = &(this->mEmployees[i]);
        if (empIndicator[i] && emp->call){
            if (emp->call->work_required > 0 && emp->call->work_performed == emp->call->work_required){
                result[i] = -1;
            }
            else if (emp->call->work_required > 0){
                result[i] = emp->call->id;
                emp->call->work_performed++;
            }
            else{
                result[i] = emp->call->id;
            }
        }
    }

    return result;
}

std::vector<int> MyCallCenter::calls(int minute, const std::vector<int>& call_ids){
    this->time = minute;
    for (auto id: call_ids){
        (this->unanswerd).push(id);
    }
    std::vector<int> result = this->distributeWork();

    for (auto itr = result.begin(); itr != result.end(); itr++){
        if (*itr == 0 && (this->unanswerd).size() > 0){                     //Answer unanswered calls if there's remaining employees
            *itr = (this->unanswerd).front();
            (this->unanswerd).pop();
            Call* ctemp = new Call{*itr, this->time, -1, -1, -1, -1};
            Employee* empTemp = &(this->mEmployees[itr - result.begin()]);
            empTemp->call = ctemp;
            continue;
        }
        if (*itr == -1){                                                    //Finish calls
            Call* cFin = this->mEmployees[itr - result.begin()].call;
            this->mEmployees[itr - result.begin()].call = nullptr;
            delete cFin;
        }
    }

    this->lastResult = result;
    return result;
}

void MyCallCenter::learn(int minute, const std::vector<Call>& calls){
    this->time = minute;
    for (auto call: calls){
        Call* c = new Call(call);
        (this->answered).push_back(c);
        for (auto& emp: this->mEmployees){
            if (!emp.call) continue;
            if ((emp.call)->id == c->id){
                Call* temp = emp.call;
                emp.call = nullptr;
                delete temp;
                break;
            }
        }
    }
    sortExpWeightedWaitTime(this->answered, this->time);
}
