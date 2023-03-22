#include "Timer.hpp"

Timer::Timer(unsigned int target_cycle_time_microseconds) : 
    TARGET_CYCLE_TIME_MICROSECONDS(target_cycle_time_microseconds) , time_stats(target_cycle_time_microseconds){
        struct sched_param sp;
        memset( &sp, 0, sizeof(sp) );
        sp.sched_priority = 99;
        sched_setscheduler( 0, SCHED_FIFO, &sp );
        current_cycle_start_microseconds = 0;
}

uint64_t Timer::microseconds() {
    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return us; 
}

void Timer::start_cycle() {
    uint64_t last_cycle_time_microseconds = current_cycle_start_microseconds;
    current_cycle_start_microseconds = Timer::microseconds();
    if(last_cycle_time_microseconds != 0)
        time_stats.set_last_cycle_time(current_cycle_start_microseconds - last_cycle_time_microseconds);
}

void Timer::end_cycle() {
    usleep(TARGET_CYCLE_TIME_MICROSECONDS - (Timer::microseconds()-current_cycle_start_microseconds) - 1.1*(time_stats.get_mean() - TARGET_CYCLE_TIME_MICROSECONDS));
}

float Timer::get_mean_cycle_time() {
    return time_stats.get_mean();
}
        
float Timer::get_standard_deviation_cycle_time() {
    return time_stats.get_standard_deviation();
}

uint32_t Timer::get_max_cycle_time() {
    return time_stats.get_max();
}

uint32_t Timer::get_min_cycle_time() {
    return time_stats.get_min();
}