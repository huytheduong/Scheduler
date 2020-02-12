#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <string>
#include <iostream>
using namespace std;

const unsigned DEFAULT_LENGTH = 100;
const string FREE = "FREE";

class Scheduler;
class ScheduleNode;


class ScheduleNode
{
  friend Scheduler;
private:
  string name;
  unsigned length;
  ScheduleNode* prev;
  ScheduleNode* next;
public:
  ScheduleNode(string name, unsigned len = 0) : name(name) 
    { length = len; next = prev = nullptr; cout << "CREATE" << endl; }
  ~ScheduleNode() { cout << "DESTROY" << endl; }
  void merge(ScheduleNode* other);
};

class Scheduler
{
private:
  ScheduleNode* head;
  unsigned length;
public:
  Scheduler(unsigned len = DEFAULT_LENGTH)
    { length = len; head = new ScheduleNode(FREE, len); }
  bool isAvailable(unsigned start, unsigned duration) const;
  void schedule(const string& name, unsigned start, unsigned duration);
  void free(unsigned start);
  void printSchedule() const;
  ~Scheduler();
};


#endif //__SCHEDULER_H