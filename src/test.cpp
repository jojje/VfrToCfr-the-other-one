#include "framecalc.h"

int main2(int argc, char* argv[]){
    int frame = 100; //atoi(argv[1]);
    FrameCalculator fc = FrameCalculator("C:\\temp\\a\\work\\timecodes.txt",30.0,1.001);
    fc.initialize();
    InterpolationInfo ip;
    fc.getInterpolationInfo(&ip, frame);
    printf("frame1: %d\n", ip.frame1);
    printf("frame2: %d\n", ip.frame2);
    printf("pct:    %d\n", ip.pct);
    return 0;
}

int main(int argc, char* argv[]){
    int frame;
    FrameCalculator fc = FrameCalculator("C:\\temp\\a\\work\\timecodes.txt",30,1.001);
    fc.initialize();
    InterpolationInfo ip;

    int size = fc.getTimeCodes().size();

    for(frame=0; frame<size; frame++){
        fc.getInterpolationInfo(&ip, frame);
        printf("frame: %-8d  sframe1: %-8d  sframe2: %-8d  pct: %d\n", frame,ip.frame1,ip.frame2,ip.pct);
    }
    return 0;
}
