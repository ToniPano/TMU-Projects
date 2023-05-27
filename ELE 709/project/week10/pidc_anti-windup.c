#include "dlab_def.h"
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

#define MAXS 10000  //Maximum number of samples in plot
float theta[MAXS];  //stores motor position (as angles)
float ref[MAXS];    //stores reference input to system (as angles)
float Kp = 1.0;         //proportional gain
float Ti = 1.0;         //integral gain
float Td = 1.0;         //derivative gain
float N = 1.0;          //related to cutoff for low pass filter in derivative term (reduces high-freq noise from derivative)
float Tt = 1.0;         //anti-windup integral gain (stable when above 20, if Ku = 51.5 and Pu = 62)
float Fs;
unsigned int samples;
sem_t data_avail;   //Do not change the name of this semaphore

const float upper = 3.0;
const float lower = -3.0;
float satblk(float v){
    float u;
    if(v < lower){
        u = lower;
    }
    else if(upper < v){
        u = upper;
    }
    else{
        u = v;
    }
    return u;
}

void* controller(void* arg){
    unsigned int k = 0;
    float motor_angle, error, p, i, d, v, u, a;
    float prev_error, prev_a;
    float T = 1.0/Fs;
    //assume input is 0 before t = 0 (which makes the ssystem state = 0)
    prev_error = 0;
    i = 0;
    d = 0;
    prev_a = 0;
    
    //calculate control signal for each time step
    for(k = 0; k < samples; k++){
        sem_wait(&data_avail);  //wait for A to D converter to sample system output
        
        motor_angle = EtoR(ReadEncoder());  //output of system, aka theta (read Encoder, convert Encoder value to Radians)
        theta[k] = motor_angle;             //save motor angle (the system output) to the theta array        
        error = ref[k] - motor_angle;       //error between output of system and reference input angle
        
        p = Kp*error;                                                   //proportional control path
        i = T*((Kp/Ti)*prev_error + (1/Tt)*prev_a) + i;                 //integral control path + anti windup integral path
        d = (Td/(N*T + Td))*(Kp*N*(error-prev_error) + d);              //derivative path with low pass filter
        
        v = p + i + d;  //initial control signal we want to send to motor (how many volts it should receive)
        u = satblk(v);  //model the D2A converter voltge limits (aka saturation)
        a = u-v;        //back calculation for anti-windup integral path for control signal in next sample period
        
        DtoA(VtoD(u));  //does a zero-order hold on the control signal (holds value constant until it can be updated with a new signal), AND
                        //sends control signal to DtoA converter (round Volts of control_signal to nearest voltage DtoA can output, and convert to Digital number)
        
        //signals which are used to calculate control signal in next sample period
        prev_error = error;
        prev_a = a;
    }
    
    pthread_exit(NULL);
}

//clears input (used before reading a character in)
void clearStdin(){
    char c;
    //reads from stdin until a '\n' from the ENTER key or an EOF marker have been found 
    do{
        c = getchar();
    }while(c != '\n' && c != EOF);
}

int main(){
    pthread_t control_thread;
    
    const float Ku = 50.93;
    const float Pu = 3.5/67.0;
    Kp = 0.6*Ku;
    Ti = Pu/2.0;
    Td = Pu/8.0;
    N = 20.0;
    Tt = 0.01;
        
    float run_time = 10.0;    //run simulation for 10 seconds
    Fs = 200.0;         //set sampling frequency to 200.0 Hz
    int student_id[] = {5,0,0,8,2,2,8,2,8};
    int motor_number = student_id[8] + 1;   //add 1 to last digit of student number
    printf("9th student # = %d, Motor # = %d\n", student_id[8], motor_number);
    
    //initialize reference input array to unit step function with amplitude of 1 radian
    char ref_type = 'w';                //type of reference input signal
    float ref_ampl = 5.0*M_PI/18.0;     //amplitude of reference input signal (50 degrees = 5*PI/18)
    float ref_freq = 0.5;                //frequency of reference input signal (default value for no frequency)
    float ref_dc = 50.0;                  //duty cycle of reference input signal (default value for no duty cycle)
    int k;
    samples = (int)(run_time*Fs);
    Square((float*)&ref, MAXS, Fs, ref_ampl, ref_freq, ref_dc);
    
    char input;
    char graphTitle[200];
    float maxFs, maxTf;
    int newSamples;
    while(1){
        printf("\nMain Menu\n");
        printf("----------------------------------------------------------\n");
        printf("Proportional Gain (Kp) = \t\t%f V/rad\n", Kp);
        printf("Integral Gain (Ti) = \t\t\t%f s\n", Ti);
        printf("Derivative Gain (Td) = \t\t\t%f s\n", Td);
        printf("Derivative Low-Pass Cutoff (N) = \t%d\n", (int)N);
        printf("Integral Anti-Windup Gain (Tt) = \t%f\n s*rad/V", Tt);
        printf("----------------------------------------------------------\n");
        if(ref_type == 'u'){
            printf("Reference Input Type = \t\t\tUnit Step\n");
            printf("Reference Input Amplitude = \t\t%f rad\n", ref_ampl);
        }
        else{
            printf("Reference Input Type = \t\t\tSquare Wave\n");
            printf("Reference Input Amplitude = \t\t%f rad\n", ref_ampl);
            printf("Reference Input Frequency = \t\t%f Hz\n", ref_freq);
            printf("Reference Input Duty Cycle = \t\t%f %\n", ref_dc);
        }
        printf("----------------------------------------------------------\n");
        printf("Sampling Frequency (Fs) = \t\t%f Hz\n", Fs);
        printf("Total Run Time (Tf) = \t\t\t%f s\n", run_time);
        printf("Total Time Samples = \t\t\t%d\n", samples);
        printf("----------------------------------------------------------\n");
        printf("\tr: Run the control algorithm\n");
        printf("\tp: Change value of Kp\n");
        printf("\ti: Change value of Ti\n");
        printf("\td: Change value of Td\n");
        printf("\tn: Change value of N\n");
        printf("\tf: Change value of sampling frequency Fs\n");
        printf("\tt: Change value of total run time Tf\n");
        printf("\tu: Change the type of inputs (Step or Square)\n");
        printf("\tg: Plot motor position on screen\n");
        printf("\th: Save a hard copy of the plot in Postscript\n");
        printf("\tq: exit\n");
        printf("Please type a letter to select an option: ");
        scanf("%c", &input);
        printf("input was \"%c\"\n", input);
        
        switch(input){
            case 'r':
                printf("Running controller algorithm for Motor #%d\n", motor_number);                
                sem_init(&data_avail, 0, 0);    //first 0 = shared between threads, second 0 means initial value of semaphore = locked
                Initialize(Fs, motor_number);               //set up appropriate model of specific motor module (or connect to microcontroller over USB) 
                pthread_create(&control_thread, NULL, &controller, NULL);   //create thread to run controller
                pthread_join(control_thread, NULL);                         //wait until thread is done running
                Terminate();                                //disconnect from microcontroller over USB
                sem_destroy(&data_avail);       //turn off semaphore struct and destroy it
                
                //plots motor angle on screen immediately after test is done for convenience, but not to replace menu option 'g'
                //plot(ref, theta, Fs, samples, SCREEN, "Run the control algorithm", "time (s)", "magnitude (rads)");
                break;
            
            case 'p':
                do{
                    printf("Enter a non-negative decimal number for Kp in V/rad: ");
                    scanf("%f", &Kp);
                }while(Kp < 0);
                break;
            
            
            case 'i':
                do{
                    printf("Enter a non-negative decimal number for Ti in s*rad/V: ");
                    scanf("%f", &Ti);
                }while(Ti < 0);
                break;
            
            case 'd':
                do{
                    printf("Enter a non-negative decimal number for Td in V*s/rad: ");
                    scanf("%f", &Td);
                }while(Td < 0);
                break;
            
            case 'n':
                do{
                    printf("Enter a non-negative integer for N: ");
                    scanf("%d", (int*)&N);
                }while(N < 0);
                N = (float)N;   //cast N from int back to float
                break;
            
            case 'f':
                printf("The max # of possible samples is %d\n", MAXS);
                maxFs = ((float)MAXS)/run_time;
                printf("Please choose a value for Fs less than or equal to %f Hz\n", maxFs);
                while(1){
                    do{
                        printf("Enter a non-negative decimal number for Fs in Hz: ");
                        scanf("%f", &Fs);
                    }while(Fs < 0);
                    
                    if(Fs > maxFs){
                        printf("Please choose a value for Fs less than or equal to %f s\n", maxFs);
                    }
                    else{
                        //fill in reference signal if new sampling frequency value (Fs) caused the number of required samples to change
                        newSamples = (int)(run_time*Fs);
                        if(ref_type == 'u'){
                            //only increase samples if unit step, since it is constant everywhere other than 0 
                            for(k = samples; k < newSamples; k++){
                                ref[k] = ref_ampl;
                            }
                        }
                        else{
                            if(newSamples != samples){
                                Square((float*)&ref, MAXS, Fs, ref_ampl, ref_freq, ref_dc);
                            }
                        }
                        samples = newSamples;
                        break;
                    }
                }
                break;
            
            case 't':
                printf("The max # of possible samples is %d\n", MAXS);
                maxTf = ((float)MAXS)/Fs;
                printf("Please choose a value for Tf less than or equal to %f s\n", maxTf);
                while(1){
                    do{
                        printf("Enter a non-negative decimal number for Tf in s: ");
                        scanf("%f", &run_time);
                    }while(run_time < 0);
                    
                    if(run_time > maxTf){
                        //printf("Tf = %f would make %d samples\n, which exceeds the max limit of %d samples\n", Fs, run_time, samples, MAXS);
                        printf("Please choose a value for Tf less than or equal to %f s\n", maxTf);
                    }
                    else{
                        //fill in reference signal if new run_time value caused the number of required samples to increase
                        newSamples = (int)(run_time*Fs);
                        if(ref_type == 'u'){
                            for(k = samples; k < newSamples; k++){
                                ref[k] = ref_ampl;
                            }
                        }
                        else{
                            if(newSamples > samples){
                                Square((float*)&ref, MAXS, Fs, ref_ampl, ref_freq, ref_dc);
                            }
                        }
                        samples = newSamples;
                        break;
                    }
                }
                break;
            
            case 'u':
                do{
                    clearStdin();
                    printf("Enter 'u' for a unit step or 'w' for a square wave: ");
                    scanf("%c", &ref_type);
                }while(ref_type != 'u' && ref_type != 'w');
                
                printf("Enter a decimal number for the amplitude in rads: ");
                scanf("%f", &ref_ampl);
                
                if(ref_type == 'u'){
                    samples = (int)(run_time*Fs);
                    if(MAXS < samples){
                        samples = MAXS;     // makes sure array index does not go outside of array limits
                    }
                    for(k = 0; k < samples; k++){
                        ref[k] = ref_ampl;
                    }
                }
                else if(ref_type == 'w'){
                    do{
                        printf("Enter a non-negative decimal number for the frequency in rads: ");
                        scanf("%f", &ref_freq);
                    }while(ref_freq < 0);
                    
                    do{
                        printf("Enter a decimal number between 0%% and 100%% for the duty cycle: ");
                        scanf("%f", &ref_dc);
                    }while(ref_dc < 0 || 100 < ref_dc);
                    
                    Square((float*)&ref, MAXS, Fs, ref_ampl, ref_freq, ref_dc);
                }
                break;
            
            case 'g':
                clearStdin();                
                printf("Enter a title for the graph: ");
                scanf("%s", graphTitle);
                plot(ref, theta, Fs, samples, SCREEN, graphTitle, "time (s)", "magnitude (rads)");
                break;
            
            case 'h':
                clearStdin();                
                printf("Enter a title for the graph: ");
                scanf("%s", graphTitle);
                plot(ref, theta, Fs, samples, PS, graphTitle, "time (s)", "magnitude (rads)");
                break;
            
            case 'q':
               printf("Program stopped\n");
               exit(0);
            
            default:
                printf("\"%c\" is not recognized.\n");
                break;
        }
        
        //clear whitespace or other chars from input to avoid interpreting them as answers to coming prompts
        clearStdin();
    }
}
