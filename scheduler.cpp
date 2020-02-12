#include "scheduler.h"
#include <string>
#include <cmath> 
#include <iostream>
using namespace std;

bool Scheduler::isAvailable(unsigned start, unsigned duration) const // this check the free node available or not
{
    //check invalid data first
    if(start < 0 || start >= DEFAULT_LENGTH)
    {
        return false;
    }
    if(duration <= 0)// user length less than 0
    {
        return false;
    }
    if(start + duration >  DEFAULT_LENGTH)//if user duraiton greater than 100
    {
        return false;
    }

    ScheduleNode* current = head;
    
    unsigned counter = 0;

    while(current != nullptr)//traversal all the way to the back
    {
        if(current->name == FREE && start >= counter && start + duration <= counter + current->length)
        {
            return true;
        }
        else
        {
            counter += current->length;

            current = current->next;
        }
    }

    return false;
}
void Scheduler::schedule(const string& name, unsigned start, unsigned duration)
{
    unsigned counter = 0;

    ScheduleNode* current = head;

    while(current != nullptr)
    {
        if(current->name == FREE && start >= counter && start + duration <= counter + current->length)
        {
            ScheduleNode* newNode = nullptr,*newFree = nullptr,*newUsed = nullptr;

            bool bMergeWithNext = false, bMergeWithPrev = false;

            if(start == counter && start + duration == counter + current->length)//check the first case if free node has same start and end
            {
                current->name = name;

                bMergeWithNext = true, bMergeWithPrev = true, newUsed = current;
            }
            else if(start > counter && start + duration == counter + current->length)// if user want time at the end of free node
            {
                current->length -= duration;

                newNode = new ScheduleNode(name, duration);

                newNode->next = current->next;

                if(current->next != nullptr)//insert after
                {
                    current->next->prev = newNode;
                }

                current->next = newNode;

                newNode->prev = current;

                bMergeWithNext = true, newUsed = newNode;
            }
            else if(start == counter && start + duration < counter + current->length)//if user want time at the beginning of free node
            {
                
                newNode = new ScheduleNode(FREE, current->length - duration);

                current->name = name;

                current->length = duration;

                newNode->next= current->next;

                if(current->next != nullptr)//insert after
                {
                    current->next->prev = newNode;
                }

                current->next = newNode;

                newNode->prev = current;

                bMergeWithPrev = true, newUsed = current;
            }
            else // if duration in the middle of the free node
            {
                unsigned newFreeDur = (counter + current->length) - (start + duration);

                current->length -= (duration + newFreeDur);

                newNode = new ScheduleNode(name, duration);
                
                newFree = new ScheduleNode(FREE, newFreeDur);

                newNode->next = current->next;

                if(current->next != nullptr)//insert newNode after current node
                {
                    current->next->prev = newNode;
                }

                current->next = newNode;

                newNode->prev = current;


                newFree->next = newNode->next; // insert new freeNode after newNode

                if(newNode->next != nullptr)
                {
                    newNode->next->prev = newFree;
                }

                newNode->next = newFree;

                newFree->prev = newNode;
            }

            if(bMergeWithNext)
            {
                newUsed->merge(newUsed->next);
            }
            if(bMergeWithPrev)
            {
                newUsed->merge(newUsed->prev);
            }

        return;
        }

    counter += current->length;

    current = current->next;

    }
}
void Scheduler::free(unsigned start)
{
    ScheduleNode* current = head;

    unsigned counter = 0;

    while(current != nullptr)
    {   
        if(counter == start && current->name != FREE)
        {
            current->name = FREE;
            //after change name to free and check next node and prev node if it has the same name then free it
            current->merge(current->next);

            current->merge(current->prev);

            break;
        }
        else
        {
            counter += current->length;

            current = current->next;
        }
    }
}
void Scheduler::printSchedule() const
{
    ScheduleNode* current = head;

    unsigned counter = 0;

    while(current != nullptr)
    {   
    
        cout<< counter << ": " << current->name << " (" << current->length << ")"<<endl;

        counter += current->length;//add to counter to get the start time

    current = current->next;//move to next node
    }
}
Scheduler::~Scheduler()
{
    ScheduleNode* current = head;

    while(current != nullptr)
    {
        ScheduleNode* next = current->next;

        delete(current);//delate the node then update the node to the new one

        current = next;
    }
}
void ScheduleNode::merge(ScheduleNode* other)
{
    // empty pointers
    if(this == nullptr || other == nullptr)
        return;
    // different names, no merging
    if(this->name != other->name)
        return;

    // this node must be just before or after the other
    if(this == other->prev)
    {
        length += other->length;

        this->next = other->next;

        if(next != nullptr)
        {
            next->prev = this;
        }

        delete other;

        other = nullptr;

        return;
    }
    if(this == other->next) // after other, similar procedure
    {
        other->length += length;

        other->next = next;

        if(next != nullptr)
        {
            next->prev = other;
        }

        delete this;

        return;
    }
}

