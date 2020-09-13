#include <iostream>
#include <stdlib.h>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // Done
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
    std::unique_lock<std::mutex> ulock(_mutex);
    _cond.wait(ulock, [this] {return !_queue.empty();} );
    T msg = std::move(_queue.back());
    _queue.pop_back();
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // Done
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> ulock(_mutex);
    _queue.push_back(std::move(msg));
    _cond.notify_one();
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // Done
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.

    while(true)
    {
        if(_message_queue.receive() == TrafficLightPhase::green)
            return; 
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // Done
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // Done
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 


    std::random_device rand_device;  
    std::mt19937 rand_engine(rand_device());
    std::uniform_int_distribution<std::mt19937::result_type> distr(4000, 6000);
    long cycleDuration = distr(rand_engine); 
    std::chrono::time_point<std::chrono::system_clock> time_now, time_prev;

    //Implementation:
    //1. Init stop watch 
    time_prev = std::chrono::system_clock::now();

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        time_now = std::chrono::system_clock::now();
            
        //2. Compute time difference to stop watch
        long current_cycleduration = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_prev).count();
        if (current_cycleduration >= cycleDuration)
        {   
            //3. Toggle between red and green 
            _currentPhase = getCurrentPhase() == TrafficLightPhase::red ? TrafficLightPhase::green :  TrafficLightPhase::red;

            //4. Send update messages to the queue
            _message_queue.send(std::move(_currentPhase));
            
            time_prev = std::chrono::system_clock::now();
            cycleDuration = distr(rand_engine);
        }
        

    }
}
