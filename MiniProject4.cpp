#include <iostream>
#include <queue>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>



using namespace std;
/*
struct EventList {
   // other stuff here
   void fill(istream& is);
   void simulate();
};*/
struct Event
{
    char type;
    int time;
    int length;
    bool operator>(Event* x)
    {
        return (time<x->time);
    }
    bool operator<(Event* x)
    {
        return (time>x->time);
    }
    Event(int arrival, int sLength, char letter)
    {
        time = arrival;
        length = sLength;
        type = letter;
    }
    Event(int arrival, char letter)
    {
        time = arrival;
        type = letter;
    }
    Event(int arrival, int sLength)
    {
        time = arrival;
        length = sLength;
        type = 'A';
    }
    Event(int arrival)
    {
        time = arrival;
        type = 'A';
    }
    Event()
    {
        type = 'A';
    }
};

        struct Compare : public std::binary_function<Event*, Event*, bool>
        {
        bool operator() (const Event* x, const Event* y)
        {
        return ((x->time)>(y->time));
        }
}       ;

void simulate(priority_queue<Event*, vector<Event*>, Compare> &eventList);
void Arrival(Event* newEvent, priority_queue<Event*, vector<Event*>, Compare> &eventList, queue<Event*> &bankQueue, bool &teller, int &currentTime, int &waitTime);
void Departure(Event* newEvent, priority_queue<Event*, vector<Event*>, Compare> &eventList, queue<Event*> &bankQueue, bool &teller, int &currentTime, int &waitTime);





int main()
{

    /*
    EventList eventList;

   // command-line parameter munging
   // also fills the event list with the input data
   char* progname = argv[0];            // simplify msgs a bit
   switch (argc) {
   case 1:
      eventList.fill(cin);
      break;
   case 2: {
      ifstream ifs(argv[1]);
      if (!ifs) {
         cerr << progname << ": couldn't open " << argv[1] << endl;
         return 1;
      }
      eventList.fill(ifs);
      break;
   }
   default:
      cerr << "Usage: " << progname << " [datafile]\n";
      return 2;
   }

   eventList.simulate();
}*/
    int arrivalTime, sLength, counter = 0, previousTime;
    ifstream file1;
    priority_queue<Event*, vector<Event*>, Compare> eventList;

    file1.open("data-1.txt");

    if(!file1)
    {
        cout << "Could not open the input file" << endl;
        exit(0);
    }

    while (file1 >> arrivalTime)
    {
        counter++;

        if(!eventList.empty())
        {
            if (arrivalTime<previousTime)
            {
                cout << "Customer #" << counter << " out of order (time= " << arrivalTime
                    << " previous time= " << previousTime << ")" << endl;
                exit(0);
            }
        }
        file1 >> sLength;
        eventList.push(new Event(arrivalTime, sLength));
        previousTime = arrivalTime;
    }

    simulate(eventList);

    file1.close();

   return 0;
}

void simulate(priority_queue<Event*, vector<Event*>, Compare> &eventList)
{
        int currentTime, tempTime, tempLength, counter = 0, waitTime,
        originalArrivalTimes[eventList.size()];
        float totalWaitTime = 0;
        char tempType;
        queue<Event*> bankQueue;
        bool teller = true;

    if(!eventList.empty())
        {
        currentTime = eventList.top()->time;
        }
            else{
            cout << "Event list is empty" << endl;
            }

                while(!eventList.empty())
                {
            tempTime = eventList.top()->time;
            tempLength = eventList.top()->length;
            tempType = eventList.top()->type;
            waitTime = 0;
            Event* newEvent = new Event(tempTime,tempLength, tempType);
            currentTime = eventList.top()->time;

            if(newEvent->type=='A')
            {
            Arrival(newEvent, eventList, bankQueue, teller, currentTime, waitTime);
            totalWaitTime += waitTime;
            originalArrivalTimes[counter] = newEvent->time;
            counter++;
            }
            else
            {
            Departure(newEvent, eventList, bankQueue, teller, currentTime, waitTime);
            totalWaitTime += waitTime;
            }

            delete newEvent;

            }

            cout << "Total Number of Guests: " << counter << endl;
            totalWaitTime /= counter;
            cout << "Average wait time: " << totalWaitTime << endl;

        return;
}



void Departure(Event* departureEvent, priority_queue<Event*, vector<Event*>, Compare> &eventList, queue<Event*> &bankQueue, bool &teller, int &currentTime, int &waitTime)
{
    int departureTime;

    cout << "Departure Time: " << currentTime << endl;
    eventList.pop();

    if(!bankQueue.empty())
    {

        Event* customer = bankQueue.front();
        bankQueue.pop();
        waitTime = currentTime - (customer->time);
        departureTime = currentTime + customer->length;
        eventList.push(new Event(departureTime, 'D'));
    }
    else
    {
        teller = true;
    }
}

void Arrival(Event* arrivalEvent, priority_queue<Event*, vector<Event*>, Compare> &eventList, queue<Event*> &bankQueue, bool &teller, int &currentTime, int &waitTime)
{
    int departureTime = currentTime + (arrivalEvent->length);
    cout << "Arrival Time: " << currentTime << " and length " << arrivalEvent->length << endl;
    eventList.pop();

    if(bankQueue.empty() && teller)
    {
        eventList.push(new Event(departureTime, 'D'));
        teller = 0;
    }
    else
    {
        int temp = arrivalEvent->time, tempLength = arrivalEvent->length, tempType = arrivalEvent->type;
        bankQueue.push(new Event(temp, tempLength, tempType));
    }
}
