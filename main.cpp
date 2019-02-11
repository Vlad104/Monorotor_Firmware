#include "mbed.h"
#include "AccelStepper.h"
#include <FlashIAP.h>
#include <stdlib.h>
#include <math.h>

//#define TEST 1

#define STEP_A  PC_0
#define STEP_B  PC_2
#define DIR_A   PC_1
#define DIR_B   PC_3
#define EN_A    PB_0
#define EN_B    PA_4
#define UART_EN PC_12
#define UART_TX PC_10 
#define UART_RX PC_11 
#define USB_TX  PA_2
#define USB_RX  PA_3
//#define LED     PA_5
//#define USB_TX  PA_9
//#define USB_RX  PA_10

#define UART_BAUDRATE 19200

#define PARAMS_SIZE 14
#define WASH_SIZE   11
#define START_SIZE  2
#define SENT_SIZE   4

#define FLASH_START_ADR  0x0800FC00
#define FLASH_PAGE_SIZES 1024
#define FLASH_FIELD_SIZE 8

#define MOTOR_MAX_SPEED  50000
#define PULSE_WIDTH      10

void FlashInit();
void FlashRead();
void FlashSave();
void MotorsInit();
void RS485Event();
void USBEvent();
void SerialEvent(Serial& port);
bool Read_Params(Serial& port);
bool Read_Wash(Serial& port);
bool Read_Start(Serial& port);
bool is_number(char x);
void Calculate();
void Calculate_Wash();
void Moves(uint32_t loc_SA, uint32_t loc_SB, uint32_t loc_VA, uint32_t loc_VB, float loc_AA, float loc_AB, int8_t loc_dir);

#ifdef TEST
    void print_all();       // TEST
#endif 

AccelStepper  Motor_A(1, STEP_A, DIR_A);
AccelStepper  Motor_B(1, STEP_B, DIR_B);
Serial        rs485(UART_TX, UART_RX);
Serial        usb(USB_TX, USB_RX);
Timer         t;
FlashIAP      flash;
///DigitalOut    ena(EN_A);
//DigitalOut    enb(EN_B);
DigitalInOut    ena(EN_A);
DigitalInOut    enb(EN_B);
DigitalOut    enu(UART_EN);
//DigitalOut    led(LED);

#ifdef TEST
    Timer         t_test;     // TEST
    long ms1 = 0;             // TEST
    long ms2 = 0;             // TEST
#endif

volatile char button = '0';
volatile char isMove = '0';
volatile char mode   = '2';
uint32_t      S = 0;
uint32_t      V = 0;
float         RA = 1.0;
float         RB = 1.0;
uint32_t      Srev = 0;

const float   WA = 2028.766605;
const float   WB = 2028.766605;
const float   A  = 20.0;

uint32_t      SA = 0;
uint32_t      SB = 0;
uint32_t      SA_rev = 0;
uint32_t      SB_rev = 0;
float         VA = 0;
float         VB = 0;
float         AA = 0;
float         AB = 0;
uint32_t      T  = 0;

volatile char mode_wash = '2';
uint32_t      S_wash = 0;
uint32_t      V_wash = 0;
uint32_t      Srev_wash = 0;
uint32_t      Sx = 0;
uint32_t      Sx_rev = 0;
uint32_t      Vx = 0;
float         Ax = 0;

volatile bool flagRun  = false;
volatile bool flagCont = false;
volatile bool flagRev  = false;

int main()
{
    t.start();
    FlashInit();
    Calculate();
    MotorsInit();
    rs485.baud(UART_BAUDRATE);
    usb.baud(UART_BAUDRATE);
    rs485.attach(&RS485Event, Serial::RxIrq);
    usb.attach(&USBEvent,     Serial::RxIrq);
    enu = 0;

    #ifdef TEST 
        t_test.start();     // TEST
        usb.printf("Monorotor Firmware\r\n");
        print_all();     // TEST
    #endif

    while(1) 
    {
        Motor_A.run();
        Motor_B.run();      
      
        if ( flagRun && (Motor_A.distanceToGo() == 0) && (Motor_B.distanceToGo() == 0) ) {
            flagRun = false; 
            
            #ifdef TEST   
                ms2 = t_test.read_ms();            // TEST
                t_test.stop();                     // TEST
                printf("T: %ld\r\n", ms2-ms1);     // TEST 
            #endif
            
            if (flagRev) {
                flagRev = false;
                ena = 0;
                enb = 0;
                //
                //printf("Stop\r\n");
                //
                continue;
            }                                              
            
            if ( ( button == '0' ) || ( button == '1') ) {
                #ifdef TEST
                    usb.printf("Reversing\r\n");
                #endif
                if (mode == '2') {
                    Moves(SA_rev, SB_rev, VA, VB, AA, AB, -1);
                }
                else if (mode == '1') {
                    Moves(0, SB_rev, VA, VB, AA, AB, -1);
                }
                else if (mode == '0') {
                    Moves(SA_rev, 0, VA, VB, AA, AB, -1);
                }
                flagRev = true;
                flagRun = true;
            }
            else if (button == '2') {
                if (mode_wash == '0') {
                    Moves(Sx_rev, 0, Vx, 0, Ax, AB, -1);
                    flagRev = true;
                    flagRun = true;
                }
                else if (mode_wash == '1') {
                    Moves(0, Sx_rev, 0, Vx, AA, Ax, -1);  
                    flagRev = true;
                    flagRun = true;              
                }                    
            }             
        }              
    }    
}

void FlashInit()
{
    flash.init(); 
    FlashRead();
    FlashSave();
}

void FlashRead()
{
    flash.read(&S,    FLASH_START_ADR,                      FLASH_FIELD_SIZE); 
    flash.read(&V,    FLASH_START_ADR + FLASH_FIELD_SIZE,   FLASH_FIELD_SIZE); 
    flash.read(&RA,   FLASH_START_ADR + FLASH_FIELD_SIZE*2, FLASH_FIELD_SIZE); 
    flash.read(&RB,   FLASH_START_ADR + FLASH_FIELD_SIZE*3, FLASH_FIELD_SIZE);  
    flash.read(&Srev, FLASH_START_ADR + FLASH_FIELD_SIZE*4, FLASH_FIELD_SIZE);
    
    //flash.read(&W1,   FLASH_START_ADR + FLASH_FIELD_SIZE*5, FLASH_FIELD_SIZE); 
    //flash.read(&W2,   FLASH_START_ADR + FLASH_FIELD_SIZE*6, FLASH_FIELD_SIZE);       
}

void FlashSave()
{
    flash.erase(FLASH_START_ADR, FLASH_PAGE_SIZES);
     
    flash.program(&S,    FLASH_START_ADR,                      FLASH_FIELD_SIZE); 
    flash.program(&V,    FLASH_START_ADR + FLASH_FIELD_SIZE,   FLASH_FIELD_SIZE); 
    flash.program(&RA,   FLASH_START_ADR + FLASH_FIELD_SIZE*2, FLASH_FIELD_SIZE); 
    flash.program(&RB,   FLASH_START_ADR + FLASH_FIELD_SIZE*3, FLASH_FIELD_SIZE);  
    flash.program(&Srev, FLASH_START_ADR + FLASH_FIELD_SIZE*4, FLASH_FIELD_SIZE);
    
    //flash.program(&W1,   FLASH_START_ADR + FLASH_FIELD_SIZE*5, FLASH_FIELD_SIZE); 
    //flash.program(&W2,   FLASH_START_ADR + FLASH_FIELD_SIZE*6, FLASH_FIELD_SIZE);     
}


void MotorsInit()
{
    Motor_A.setMinPulseWidth(PULSE_WIDTH);
    Motor_B.setMinPulseWidth(PULSE_WIDTH);
    Motor_A.setMaxSpeed(MOTOR_MAX_SPEED);
    Motor_B.setMaxSpeed(MOTOR_MAX_SPEED);
    Motor_A.setCurrentPosition(0);
    Motor_B.setCurrentPosition(0);
    Motor_A.setPinsInverted(true, true, false);
    Motor_B.setPinsInverted(true, true, false);
    ena.output();
    enb.output();
    ena.mode(OpenDrain);
    enb.mode(OpenDrain);
    ena = 0;
    enb = 0;
    //led = 0;
}

void RS485Event()
{
    SerialEvent(rs485);    
}

void USBEvent()
{
    SerialEvent(usb);    
}

void SerialEvent(Serial& port)
{
    char message_type = port.getc();
    bool error = false;
    #ifdef TEST   
        usb.putc(message_type); // TEST           
    #endif     
   
    if (message_type == '@') {
        error = Read_Params(port);
        if (!error) {
            FlashSave();
            Calculate();
        }
    }
    else if (message_type == 'P') {
        error = Read_Wash(port);
        if (!error) {
            Calculate_Wash();  
        }   
    }
    else if (message_type == '=') {
        error = Read_Start(port);
        if (!error) {
            if (button == '0') {
                flagRun = false;
                Moves(SA, SB, VA, VB, AA, AB, 1);
                flagRun = true;
                
            }
            else if (button == '1') {
                flagRun = false;
                Moves(1000000000, 1000000000, VA, VB, AA, AB, 1);
                flagRun = true;
            }
            else if (button == '2') {
                flagRun = false;
                if (mode_wash == '0') {
                    Moves(Sx, 0, Vx, 0, Ax, AB, 1);
                }
                else if (mode_wash == '1') {
                    Moves(0, Sx, 0, Vx, AA, Ax, 1);                
                }
                flagRun = true;                
            }
            else {
                error = true;
            }
        }
    }
    else if ( (message_type == '\r') || (message_type == '\n') ) {
        return;
    }
    else {
        error = true;
    }
    
    enu = 1;
    if (error) {
        port.putc('?');
        #ifdef TEST   
            usb.putc('?');   //TEST         
        #endif 
    }
    else {
        port.putc('!');
        #ifdef TEST   
            usb.putc('!');  //TEST    
        #endif 
    }
    wait_ms(20);
    enu = 0;  
    

    #ifdef TEST   
        print_all();     // TEST        
    #endif 
}


bool is_number(char x)
{
    bool number = false;
    if ( (x >= '0') && (x <= '9') ) {
        number = true;
    }
    else {
        number = false;
        //number = true;
    }
    return number;
}

bool Read_Params(Serial& port)
{
    char data[PARAMS_SIZE];
    bool error = false;
    
    for(uint8_t i = 0; i < PARAMS_SIZE; i++) {
            char x = port.getc();
            #ifdef TEST   
                usb.putc(x); // TEST           
            #endif         
            if ( is_number(x) ) {
                data[i] = x;
            }
            else {
                return true;
            }
    }
   
    mode = data[0];
    S  = (data[1]-48)*1000 + (data[2]-48)*100 + (data[3]-48)*10 + (data[4]-48);
    V  = ( (data[5]-48)*100 + (data[6]-48)*10 + (data[7]-48) );
    RA = ( (data[8]-48)*100 + (data[9]-48)*10 + (data[10]-48) );
    if (RA > 0) {  
        RA /= 1000.0;
        RB = 1.0 - RA;
    }
    else {
        RA = 1.0;
        RB = 1.0;    
    }
    Srev = (data[11]-48)*100 + (data[12]-48)*10 + (data[13]-48);

    return error;
}

bool Read_Wash(Serial& port)
{
    char data[WASH_SIZE];
    bool error = false;
    
    for(uint8_t i = 0; i < WASH_SIZE; i++) {
            char x = port.getc();
            #ifdef TEST   
                usb.putc(x); // TEST           
            #endif        
            if ( is_number(x) ) {
                data[i] = x;
            }
            else {
                return true;
            }
    }
       
    mode_wash = data[0];
    S_wash    = (data[1]-48)*1000 + (data[2]-48)*100 + (data[3]-48)*10 + (data[4]-48);
    V_wash    = (data[5]-48)*100 + (data[6]-48)*10 + (data[7]-48);
    Srev_wash = (data[8]-48)*100 + (data[9]-48)*10 + (data[10]-48);

    return error;
}

bool Read_Start(Serial& port)
{  
    bool error = false;
    button = port.getc();
    isMove = port.getc();     

    #ifdef TEST  
        usb.putc(button); //TEST
        usb.putc(isMove); //TEST           
    #endif     
    
    if ( is_number(button) && is_number(isMove) ) {
        error = false;
    }
    else {
        error = true;
    }   
    
    return error;
}

void Calculate()
{   
    float Sd = S; 
    Sd /= 10; // S = 10 means 1 ml, S = 1 means 0.1 ml
    float Vd = V;
    float Ad = A;
    float Sd_rev = Srev;
    Sd_rev = Sd_rev * Sd / 1000; // %

    float SdA = Sd * WA * RA;
    float SdB = Sd * WB * RB;
    
    float SdA_round = (SdA > 0.0) ? floor(SdA + 0.5) : ceil(SdA - 0.5);
    float SdB_round = (SdB > 0.0) ? floor(SdB + 0.5) : ceil(SdB - 0.5);

    SA = (uint32_t) SdA_round;
    SB = (uint32_t) SdB_round;
    
    //SA_rev = (uint32_t) (Sd_rev * WA * RA / 1000);
    //SB_rev = (uint32_t) (Sd_rev * WB * RB / 1000);
    
    SA_rev = (uint32_t) (Sd_rev * WA * RA );
    SB_rev = (uint32_t) (Sd_rev * WB * RB );

    float kka = SdA_round / SdA;
    float kkb = SdB_round / SdB;

    VA = Vd * WA * RA / (kka * 60);
    VB = Vd * WB * RB / (kkb * 60);      
    
    AA = Ad * WA * RA / (kka * 60);
    AB = Ad * WB * RB / (kkb * 60);
    
    #ifdef TEST
        float sd = Sd;
        float vd = Vd / 60;
        float ad = Ad / 60;
        float td = sd / vd + vd / ad;
        td = ceil(td);
        T = (uint32_t) td;
    #endif
}

void Calculate_Wash()
{   
    float Sd = S_wash;
    Sd /= 10; // S = 10 means 1 ml, S = 1 means 0.1 ml
    float Vd = V_wash;
    float Ad = A;
    float Sd_rev = Srev_wash;
    Sd_rev = Sd_rev * Sd / 1000; // %
    float Wx = (WA + WB) / 2; 

    float Sdx = Sd * Wx;    
    float Sdx_round = (Sdx > 0.0) ? floor(Sdx + 0.5) : ceil(Sdx - 0.5);
    Sx = (uint32_t) Sdx_round;
    
    //Sx_rev = (uint32_t) (Sd_rev * Wx / 1000);
    Sx_rev = (uint32_t) (Sd_rev * Wx);

    float kkx = Sdx_round / Sdx;

    Vx = Vd * Wx / (kkx * 60);    
    Ax = Ad * Wx / (kkx * 60);
    
    #ifdef TEST
        float sd = Sd;
        float vd = Vd / 60;
        float ad = Ad / 60;
        float td = sd / vd + vd / ad;
        td = ceil(td);
        uint32_t T_wash = (uint32_t) td;
        usb.printf("T_wash = %ld", T_wash);
    #endif
}

void Moves(uint32_t loc_SA, uint32_t loc_SB, uint32_t loc_VA, uint32_t loc_VB, float loc_AA, float loc_AB, int8_t loc_dir)
{  
    int32_t signed_loc_SA = loc_dir * loc_SA;
    int32_t signed_loc_SB = loc_dir * loc_SB;
     
    ena = 1;
    enb = 1;
    
    Motor_A.setCurrentPosition(0);
    Motor_B.setCurrentPosition(0);
       
    if ( ( (isMove == '0') || (mode == '1') ) && (loc_dir > 0) ) { 
        signed_loc_SA = 0;   
    } 
    if ( ( (isMove == '0') || (mode == '0') ) && (loc_dir > 0) ) {   
        signed_loc_SB = 0;   
    }
       
    Motor_A.setAcceleration(loc_AA);    
    Motor_B.setAcceleration(loc_AB);
    Motor_A.move(signed_loc_SA);
    Motor_B.move(signed_loc_SB);  
    Motor_A.setMaxSpeed(loc_VA);
    Motor_B.setMaxSpeed(loc_VB);
    

    #ifdef TEST   
        usb.printf("Moving\r\n");  // TEST
        t_test.stop();              // TEST
        t_test.reset();             // TEST
        t_test.start();             // TEST
        ms1 = t_test.read_ms();     // TEST         
    #endif          
}

#ifdef TEST
    void print_all()       // TEST 
    {      
        usb.printf("\r\n\r\nMain params:\r\nS = %ld\r\nV = %ld\r\nWa = %f\r\nWb = %f\r\nRa = %f\r\nRb = %f\r\nSrev = %ld\r\n", S, V, WA, WB, RA, RB, Srev); 
        usb.printf("Sa = %ld\r\nSb = %ld\r\nVa = %f\r\nVb = %f\r\nT = %ld\r\nAa = %f\r\nAb = %f\r\n", SA, SB, VA, VB, T, AA, AB); 
        usb.printf("Sa_rev = %ld\r\nSb_rev = %ld\r\n", SA_rev, SB_rev); 
        usb.printf("\r\nWash\r\nS_wash = %ld\r\nV_wash = %ld\r\nSrev_wash = %ld\r\n", S_wash, V_wash, Srev_wash);
        usb.printf("Sx = %ld\r\nVx = %ld\r\nAx = %f\r\nSx_rev = %ld\r\n", Sx, Vx, Ax, Sx_rev);
    }
#endif
