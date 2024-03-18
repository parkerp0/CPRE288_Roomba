
#define THRESHHOLD 100
#define REJECTANGLE 10
#define REJECTDIST  150

#include "Timer.h"
#include "lcd.h"
#include "uart-interrupt.h"
#include "cyBot_Scan.h"
#include "open_interface.h"
#include "obstacle.h"
#include "movement.h"
#include <stdlib.h>

//#define sonarAVG(val,scan) \



int main() {
    float avg,prevAvg;
    int i;
    char message[90];
    struct obstacle* obs = NULL;
    int obsCount = 0;
    int obsOpen;
    struct obstacle* smallestOBS = NULL;
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    timer_init();
    lcd_init();
    uart_interrupt_init();
    cyBOT_Scan_t* scan = malloc(sizeof(cyBOT_Scan_t));
    cyBOT_init_Scan(0b0111);
    right_calibration_value=290500;//cybot 10
    left_calibration_value=1319500;



    while(1)
    {
        obsCount = 0;
        obsOpen =0;
        prevAvg=610;

        if(command_byte=='m')
        {
            uart_sendStr("---------------------\n\r");
            command_byte = -1;
            for(i=15;i<165;i+=2)
            {
                if(command_byte == 's')
                {
                     command_byte = -1;
                     break;
                }

                avg=0;
                cyBOT_Scan(i,scan);
                avg+=scan->IR_raw_val;
                cyBOT_Scan(i,scan);
                avg+=scan->IR_raw_val;
                cyBOT_Scan(i,scan);
                avg+=scan->IR_raw_val;
                avg/=3;

                sprintf(message,"angle: %d, IR: %d, Distance: %f \n\r",i,scan->IR_raw_val,avg);
                uart_sendStr(message);

                if(!obsOpen && prevAvg!=0 && avg-prevAvg > THRESHHOLD)
                {
                    obsOpen=1;
                    obsCount++;
                    //uart_sendStr("we made it\n\r");
                    obs = realloc(obs,sizeof(struct obstacle)*obsCount);
                    obs[obsCount-1].start = i;
                    obs[obsCount-1].distance = avg;
                    obs[obsCount-1].num = obsCount-1;
                    sprintf(message,"OBSSTART: %d, start: %d, Distance: %f \n\r",obs[obsCount-1].num,obs[obsCount-1].start,obs[obsCount-1].distance);
                    uart_sendStr(message);
                }
                if(obsOpen && prevAvg!=0 && prevAvg-avg > THRESHHOLD)
                {
                    obsOpen=0;
                    obs[obsCount-1].end = i;
                    obs[obsCount-1].distance = 0; //(obs[obsCount-1].distance + avg) /2;
                    obs[obsCount-1].degrees = (obs[obsCount-1].end - obs[obsCount-1].start);
                    obs[obsCount-1].center = (obs[obsCount-1].start + (obs[obsCount-1].degrees/2));

                    cyBOT_Scan(obs[obsCount-1].center,scan);
                    obs[obsCount-1].distance+=scan->sound_dist;
                    cyBOT_Scan(obs[obsCount-1].center,scan);
                    obs[obsCount-1].distance+=scan->sound_dist;
                    cyBOT_Scan(obs[obsCount-1].center,scan);
                    obs[obsCount-1].distance+=scan->sound_dist;
                    obs[obsCount-1].distance/=3;

                    obs[obsCount-1].width = 2*3.14*obs[obsCount-1].distance * (obs[obsCount-1].degrees / 360.0);

                    sprintf(message,"OBSEND: %d, start: %d, End: %d, degrees: %d, Distance: %f ,center: %f, width %f\n\r",obs[obsCount-1].num,obs[obsCount-1].start,obs[obsCount-1].end,obs[obsCount-1].degrees, obs[obsCount-1].distance,obs[obsCount-1].center, obs[obsCount-1].width);
                    if(obs[obsCount-1].degrees<REJECTANGLE /*|| obs[obsCount-1].distance > REJECTDIST*/)
                    {
                        obsCount--;
                    }
                    uart_sendStr(message);
                }

                prevAvg=avg;
            }
            if(obsOpen)
            {
                obs[obsCount-1].end = i;
                obs[obsCount-1].distance = 0; // (obs[obsCount-1].distance + scan->sound_dist) /2;
                obs[obsCount-1].degrees = (obs[obsCount-1].end - obs[obsCount-1].start);
                obs[obsCount-1].center = (obs[obsCount-1].start + (obs[obsCount-1].degrees/2));

                cyBOT_Scan(obs[obsCount-1].center,scan); //bad paste
                                    obs[obsCount-1].distance+=scan->sound_dist;
                                    cyBOT_Scan(obs[obsCount-1].center,scan);
                                    obs[obsCount-1].distance+=scan->sound_dist;
                                    cyBOT_Scan(obs[obsCount-1].center,scan);
                                    obs[obsCount-1].distance+=scan->sound_dist;
                                    obs[obsCount-1].distance/=3;
                obs[obsCount-1].width = 2*3.14*obs[obsCount-1].distance * (obs[obsCount-1].degrees / 360.0);

                if(obs[obsCount-1].degrees<REJECTANGLE /*|| obs[obsCount-1].distance > REJECTDIST*/)
                {
                    obsCount--;
                }
            }

            for(i=0; i<obsCount; i++)
            {
                sprintf(message,"num: %d, start: %d, End: %d, degrees: %d, Distance: %f ,center: %f, width %f\n\r",obs[i].num,obs[i].start,obs[i].end,obs[i].degrees, obs[i].distance,obs[i].center, obs[obsCount-1].width);
                uart_sendStr(message);
                if(i==0 || smallestOBS->degrees > obs[i].degrees)
                    smallestOBS = &obs[i];
            }

            if(smallestOBS->center>90)
                turn_left(sensor_data,smallestOBS->center - 90);
            else
                turn_right(sensor_data,90- smallestOBS->center);
            move_forward(sensor_data,(smallestOBS->distance-6)*10);

            free(obs);
            obs = NULL;
            smallestOBS = NULL;


        }
        if(command_byte == 'q')
            break;
    }




    oi_free(sensor_data);
}
