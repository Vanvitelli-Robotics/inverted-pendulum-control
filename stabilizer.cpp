#include <iostream>
#include "src/encoder/Encoder.hpp"
#include "src/timer/Timer.hpp"
#include "Constants.hpp"
#include <inttypes.h>
#include "src/robot/Robot.hpp"
#include "signal.h"

Robot robot(Constants::ROBOT_IP , Constants::BYPASS_ROBOT);

void cleanup(int signum) {
    robot.deactivate();
    robot.disconnect();
    printf("ho ricevuto il segnale %d. termino...\n" , signum);
    exit(0);
}


int main() {
    float delay_feedback_gain;
    if(Constants::TIMER_AGGRESSIVE_MODE) {
        delay_feedback_gain = Constants::AGGRESSIVE_DELAY_FEEDBACK_GAIN;
    }
    else {
        delay_feedback_gain = Constants::DELAY_FEEDBACK_GAIN;
    }
    Timer timer(Constants::TARGET_CYCLE_TIME_MICROSECONDS , delay_feedback_gain,
        Constants::TIMER_AGGRESSIVE_MODE);
    Encoder encoder(Constants::ENCODER_CLK_PIN, 
        Constants::ENCODER_DT_PIN, Constants::ENCODER_PPR,
        Constants::ENCODER_START_ANGLE_DEGREES);
    signal(2,cleanup);
    robot.connect();
    robot.activate();
    robot.home();
    while(true) {
        timer.start_cycle();
        /*
            Qui andranno le operazioni da eseguire in ciclo
        */

        /*
            Operazioni inutilmente complesse per testare come si comporta l'algoritmo
        */
        double a = 1.7;
        for(int i=0;i<10;i++) {
            a = sqrt(pow(cos(encoder.get_angle_degrees()),a)) + 1.7 + sin(sqrt(a/3.6));
        }
        robot.reset_error();
        /*
            Fine operazioni inutilmente complesse
        */
        printf("\nenc_angle=%-10.3f mean time=%-10.3f sigma_time=%-10.3f max_time=%-10u min_time=%-10u ignore=%-10.3f\n\n" , 
            encoder.get_angle_degrees() , timer.get_mean_cycle_time(),
            timer.get_standard_deviation_cycle_time() , timer.get_max_cycle_time(),
            timer.get_min_cycle_time() , a);
        /*
            Fine operazioni
        */
        timer.end_cycle();
    }
}