

#include <xc.h>
#include "relogio.h"





/*void interrupt SerialRxPinInterrupt()
{
    //check if the interrupt is caused by RX pin
    if(PIR1bits.RCIF == 1)
    {
        if(i<7)
        {
        while(!RCIF);                            // Wait until RCIF gets low
            BUFFCOM[i]= RCREG;                                   // Retrieve data from reception register
            
          i++;  
        }else
        {
            PIR1bits.RCIF = 0; // clear rx flag
            i=0;
            estado = VERIFICACRC;
            
        }
        
            
    }

}
*/
