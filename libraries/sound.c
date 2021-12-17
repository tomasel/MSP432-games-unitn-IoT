#include "sound.h"


#define OCTAVE_SHIFT 3  //with respect to range C4~B4
uint16_t notePeriods[12] = {22933, 21647, 20432, 19285, 18202, 17181, 16217, 15306, 14447, 13636, 12871, 12149};
//note periods, used to set the timer period, same for each octave

/* Timer_A Compare Configuration Parameter  (PWM) */
static Timer_A_CompareModeConfig compareConfig_PWM = {
        TIMER_A_CAPTURECOMPARE_REGISTER_4,                  // Use CCR3
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_SET,              // Toggle output but
        0                                           // off initially
        };


/* Timer_A Up Configuration Parameter */
static Timer_A_UpModeConfig noteClock = {
        TIMER_A_CLOCKSOURCE_SMCLK,             // SMCLK = 48 MhZ
        TIMER_A_CLOCKSOURCE_DIVIDER_16,
        0xff,
        TIMER_A_TAIE_INTERRUPT_DISABLE,        // disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,   // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                       // Clear value
        };

static Timer_A_ContinuousModeConfig delayTimer = {
        TIMER_A_CLOCKSOURCE_ACLK,              // ACLK = 64 kHz
        TIMER_A_CLOCKSOURCE_DIVIDER_64,        // ACLK/64 = 1 kHz
        TIMER_A_TAIE_INTERRUPT_DISABLE,        // disable Overflow ISR
        TIMER_A_DO_CLEAR                       // Clear value
        };

static uint8_t volumeDivider=32;

void _soundInit(){
   /* Configures P2.7 to PM_TA0.4 for using Timer PWM to control LED */
   GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,
   GPIO_PRIMARY_MODULE_FUNCTION);

   //enable interrupts on CCR0 of timer A1 for playing music asynchronously
   Interrupt_enableInterrupt(INT_TA1_0);
   Timer_A_enableCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);

}

void playNote(uint8_t noteNum){
    //divide by 64 is used for the lowest octave, by 1 for the highest
    noteClock.clockSourceDivider = 64>>(noteNum/12-MIDDLE_C/12+OCTAVE_SHIFT);

    //set the period for the note
    noteClock.timerPeriod = notePeriods[noteNum%12];

    //calculate duty cycle to change volume
    compareConfig_PWM.compareValue = noteClock.timerPeriod/volumeDivider;

    //update the timer system with new values
    Timer_A_configureUpMode(TIMER_A0_BASE, &noteClock);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);
}


void stopSound(){
    Timer_A_configureUpMode(TIMER_A0_BASE, &noteClock);//this stops the timer
}

uint32_t _numNotes;
uint32_t _notesPlayed;
npd* _notes;

void playSong( uint32_t numNotes, npd* notes){
    Timer_A_configureContinuousMode(TIMER_A1_BASE, &delayTimer);
    if(numNotes==0||notes==NULL){
        //stops timer and exits
        return;
    }
    _numNotes=numNotes;
    _notes=notes;
    _notesPlayed=1;

    Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, _notes->duration);
    playNote(_notes->note);
    _notes++;
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
}

void TA1_0_IRQHandler(void) //handles delay between notes
{
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
                                         TIMER_A_CAPTURECOMPARE_REGISTER_0);
    if(_notesPlayed==_numNotes){
        _notes-=_numNotes;
        _notesPlayed=0;
    }

    uint8_t note=_notes->note;
    if(note<C1||note>B7)
        stopSound();
    else
        playNote(note);
    Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0,
                            Timer_A_getCounterValue(TIMER_A1_BASE)+_notes->duration);

    _notes++;
    _notesPlayed++;
}
