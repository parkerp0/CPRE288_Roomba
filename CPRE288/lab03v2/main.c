
#define THRESHHOLD 30
#define REJECTANGLE 20
#define REJECTDIST

#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include "puTTy.h"
#include "cyBot_Scan.h"
#include "open_interface.h"
#include "obstacle.h"
#include "movement.h"
#include <stdlib.h>


int main() {
    float avg,prevAvg;
    int minDist;
    int i;
    char c;
    char message[90];
    struct obstacle* obs = NULL;
    int obsCount = 0;
    int obsOpen;
    struct obstacle* smallestOBS = NULL;
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    lcd_init();
    timer_init();
    cyBot_uart_init();
    cyBOT_Scan_t* scan = malloc(sizeof(cyBOT_Scan_t));
    cyBOT_init_Scan(0b0111);
    right_calibration_value=290500;//cybot 10
    left_calibration_value=1319500;



    while(1)
    {
        minDist = 500;
        obsCount = 0;
        obsOpen =0;
        prevAvg=100;
        sendString("---------------------\n\r");
        c=cyBot_getByte();
        if(c=='m')
        {
            for(i=15;i<165;i+=1)
            {
                avg=0;
                cyBOT_Scan(i,scan);
                avg+=scan->sound_dist;
                cyBOT_Scan(i,scan);
                avg+=scan->sound_dist;
                cyBOT_Scan(i,scan);
                avg+=scan->sound_dist;
                avg/=3;

                sprintf(message,"angle: %d, IR: %d, Distance: %f \n\r",i,scan->IR_raw_val,avg);
                sendString(message);


                if (obsOpen && minDist > avg)
                    minDist = avg;

                if(!obsOpen && prevAvg!=0 && prevAvg-avg>THRESHHOLD)
                {
                    obsOpen=1;
                    obsCount++;
                    //sendString("we made it\n\r");
                    obs = realloc(obs,sizeof(struct obstacle)*obsCount);
                    obs[obsCount-1].start = i;
                    obs[obsCount-1].distance = avg;
                    obs[obsCount-1].num = obsCount-1;
                    sprintf(message,"OBSSTART: %d, start: %d, Distance: %f \n\r",obs[obsCount-1].num,obs[obsCount-1].start,obs[obsCount-1].distance);
                    sendString(message);
                }
                if(obsOpen && prevAvg!=0 && avg-prevAvg >THRESHHOLD)
                {
                    obsOpen=0;
                    obs[obsCount-1].end = i;
                    obs[obsCount-1].distance = minDist; //(obs[obsCount-1].distance + avg) /2;
                    obs[obsCount-1].degrees = (obs[obsCount-1].end - obs[obsCount-1].start);
                    obs[obsCount-1].center = (obs[obsCount-1].start + (obs[obsCount-1].degrees/2));
                    sprintf(message,"OBSEND: %d, start: %d, End: %d, width: %d, Distance: %f ,center: %f\n\r",obs[obsCount-1].num,obs[obsCount-1].start,obs[obsCount-1].end,obs[obsCount-1].degrees, obs[obsCount-1].distance,obs[obsCount-1].center);
                    if(obs[obsCount-1].degrees<REJECTANGLE)
                    {
                        obsCount--;
                    }
                    sendString(message);
                    minDist = 500;
                }

                prevAvg=avg;
            }
            if(obsOpen)
            {
                obs[obsCount-1].end = i;
                obs[obsCount-1].distance = minDist; // (obs[obsCount-1].distance + scan->sound_dist) /2;
                obs[obsCount-1].degrees = (obs[obsCount-1].end - obs[obsCount-1].start);
                obs[obsCount-1].center = (obs[obsCount-1].start + (obs[obsCount-1].degrees/2));
                if(obs[obsCount-1].degrees<REJECTANGLE)
                {
                    obsCount--;
                }
            }

            for(i=0; i<obsCount; i++)
            {
                sprintf(message,"num: %d, start: %d, End: %d, width: %d, Distance: %f ,center: %f\n\r",obs[i].num,obs[i].start,obs[i].end,obs[i].degrees, obs[i].distance,obs[i].center);
                sendString(message);
                if(i==0 || smallestOBS->degrees > obs[i].degrees)
                    smallestOBS = &obs[i];
            }

            if(smallestOBS->center>90)
                turn_left(sensor_data,smallestOBS->center - 90);
            else
                turn_right(sensor_data,90- smallestOBS->center);
            move_forward(sensor_data,(smallestOBS->distance-4)*10);

            free(obs);
            obs = NULL;
            smallestOBS = NULL;


        }
        if(c=='q')
            break;
    }




    oi_free(sensor_data);
}
