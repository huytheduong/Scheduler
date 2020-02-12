#include <iostream>
#include <string>
#include <cctype>
using namespace std;

#include "scheduler.h"

void makeUppercase(string& str)
{
  for (char& c : str)
    c = toupper(c);
}

int main()
{
  Scheduler schedule;
  string cmd, opt;
  unsigned time, duration;

  schedule.printSchedule();
  cin >> cmd;
  makeUppercase(cmd);
  while (cmd != "QUIT")
  {
    if (cmd == "CANCEL")
    {
      cin >> time;
      schedule.free(time);
      schedule.printSchedule();
    }
    else if (cmd == "ADD")
    {
      time = duration = -1;
      string temp;
      //add EVENT from X to Y
      cin >> opt >> temp >> time >> temp >> duration;
      makeUppercase(opt);
      duration -= time;
      if (opt == FREE)
        cout << "Cannot add event " << FREE << ":  use cancel instead" << endl;
      else if (schedule.isAvailable(time, duration))
      {
        schedule.schedule(opt, time, duration);
        cout << "Added to schedule" << endl;
      }
      else
        cout << "Scheduling conflict:  not added" << endl;      
      schedule.printSchedule();
    }
    else if (cmd == "PRINT")
      schedule.printSchedule();
    cin.clear();
    cin.ignore(10000,'\n');
    cin >> cmd;
    makeUppercase(cmd);
  }
  
  return 0;
}