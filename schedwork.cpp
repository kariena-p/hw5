#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


/* OVERVIEW
d = needed workers 
k = possible workers per day 
n = n days 
m = max shifts 

Given d, m, and the k workers’ availability for each of the n days, write a backtracking search function to schedule the nurses

that exactly d work on each of the n days 

no one works more than m shifts.

You should produce a schedule solution that is an n by d matrix
  row = day 
  d = ID of workers scheduled to work that day (value in the range 0 to k-1)

It is likely easiest to recurse over each place in the schedule

Each recursive call would be responsible for filling in one of the n*d schedule openings, ensuring the constraints of availability and the maximum number of shifts allowed for each worker is met

You may have up to three loops in your code: two for set up and one in your recursive search.

*/

// Add prototypes for any helper functions here


//helper function to fill in sched 
bool fill (int day, int slot, const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, vector<int>& shifts); 

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

////////////////////////////////////////////////////////

//sched: grid of size n × d (days × needed workers).
    //for n days on the schedule: 
    
  int n = avail.size();//n = no of rows of availability matrix 
  int k = avail[0].size();  // k = no of columns = no of workers 


  //reset the schedule, filling in all spots with invalid ID 
  for (int i = 0; i < n; i++) {
    sched.push_back(vector<Worker_T>(dailyNeed, INVALID_ID));
  }
    //add available workers - 
      // check the availability of each worker. if they are available add them to the availability matrix till we have d workers scheduled for a day 
      //go through the k workers that are available for that day 

//recursion: iterates over each "slot".

// Slot 0 = day 0, worker index 0

// Slot 1 = day 0, worker index 1

// …

// Slot d = day 1, worker index 0

// Slot (day * d + pos)

    //backtrack when someone gets scheduled for more than m shifts 

    //vector of ints 
    //each shift[i] holds no of shifts worker i is scheduled for
    //k spots for k workers  
    //initialize to 0
    vector<int> shifts(k,0);

    //when d workers are scheduled and we are sure there are no more than m shifts scheduled per worker, the schedule is done, return true 
    //else the schedule could not be made, return false 
    
    return fill(0, 0, avail, dailyNeed, maxShifts, sched, shifts);
}

//day = rows = number from 0 on wards 
//slot = cols = number from 0 onwards to d 
//avail = alias to avail matrix with availability of each worker
//dailyNeed = no of shifts needed to fill today 
//maxShifts = limit of no of shifts per worker
//sched = alias to schedule matrix 
//shifts = alias to vector shifts 

bool fill(
    int day,
    int slot,
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<int>& shifts) {

      //n = no of rows 
    int n = avail.size();

    //k = no of columns 
    int k = avail[0].size();

    // If we filled all days return true, sched is filled properly 
    if (day == n) return true;

    // If we filled all of slots for the day, move to next day, starting at slot 0 
    if (slot == dailyNeed)
        return fill(day + 1, 0, avail, dailyNeed, maxShifts, sched, shifts);

    // Try assigning each worker for k workers 
    for (int w = 0; w < k; w++) {

        // 1. if they are un available move onto next loop
        if (!avail[day][w]) continue;

        // 2. check if more shifts allowed, otherwise move onto next worker 
        if (shifts[w] >= (int)maxShifts) continue;

        // 3. is worker already scheduled for today you can't double schedule them 
        bool scheduled = false;
        for (int s = 0; s < slot; s++) {
            if (sched[day][s] == w) {
                scheduled = true;
                break;
            }
        }
        if (scheduled) continue;

        // assign worker, increment no of shifts 
        sched[day][slot] = w;
        shifts[w]++;

        // recurse
        if (fill(day, slot + 1, avail, dailyNeed, maxShifts, sched, shifts)==true) {
          return true;
        }
        else { //otherwise you have to BACKRACK
        shifts[w]--;
        sched[day][slot] = INVALID_ID;
        }

    }

    // at this point it has not returned true so failed to assign 
    return false;
}
