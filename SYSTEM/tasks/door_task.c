#include "door_task.h"


void openDoor(u8 doorID){
    //open door 1 just for test
    switch(doorID){
        case 1:
            Door1 = 1;
            delay_ms(500);
            Door1 = 0;            
            break;
        case 2:
            Door2 = 1;
            delay_ms(500);
            Door2 = 0;  
            break;
        case 3:
            Door3 = 1;
            delay_ms(500);
            Door3 = 0;             
            break;
        case 4:
            Door4 = 1;
            delay_ms(500);
            Door4 = 0;
            break;
    }


}
