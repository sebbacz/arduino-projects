#include "display_lib.h"

int main(){
    initDisplay();

    writeNumberAndWait(1234,1000)

    writeStringAndWait("AFDF",1000)

    return 0;
}