#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <exception>
#include "framecalc.h"

using namespace std;

// ==========================================================================
// PRIVATE methods
// ==========================================================================
void FrameCalculator::readTimeCodes() {
    FILE *file;
    char line[512];
    int frame, display_time, src_num, src_den;

    if(strlen(filename) == 0) throw exception("Mandatory timecodes filename not provided!");
    fopen_s(&file, filename, "r");
    if(!file) {
        sprintf(line, "Failed to open timecodes file: %s", filename);
        throw exception(line);
    }

    for(frame=0; fgets(line,sizeof(line),file) != NULL;) {
        if(line[0] < '0' || line[0] > '9') continue;  // not a display number, so skip it.
        display_time = atoi(line);

        if(display_time < 0) throw exception("Invalid / overflowed display time found");
        if(frame < 0)        throw exception("Invalid / overflowed frame number encountered. Please split the source clip into smaller chunks!");

        timecode tc = {frame, display_time};
        timecodes.push_back(tc);
        frame++;
    }
    fclose(file);
    src_num = timecodes.size();
    src_den = timecodes[timecodes.size()-1].display_time/1000;
    src_fps = (double)src_num/(double)src_den;
    adjust_display_times();
}

void FrameCalculator::adjust_display_times() {
    unsigned i, j, new_dt, disp_delta, delta_frames;
    double delta_fps;

    // For each frame, check whether it is followed by a bunch of invalid 1ms frames. If so,
    // calculate the distance to the next valid (non 1ms) frame and adjust the display times
    // for the 1ms frames in between those frames, giving them a linear distance.
    // In other words, make all points (display times) in the range A to B linear.
    for(i=0; i<timecodes.size(); i++) {
        for(j=0; i+j < timecodes.size() - 2 && timecodes[i+j].display_time == timecodes[i+j+1].display_time - 1; j++);
        if(j == 0) continue;
        disp_delta   = timecodes[i+j+1].display_time - timecodes[i].display_time;  // <rant>
        delta_fps    = ( (j+1) ) / ( (double)disp_delta ) * 1000;                  // Oh how I hate C, with its constant risk of overflow.
        delta_frames = j;                                                          // That's why there are a gazillion type conversions and
        for(j=1; j <= delta_frames; j++) {                                         // odd operational order, to keep the ints within bounds! 
            new_dt = timecodes[i].display_time + (int)( j / delta_fps * 1000);     // </rant>
            timecodes[i+j].display_time = new_dt;
        }
        i += (j-1);         // fast-forward to the next un-processed frame, and compensate for the ++ in the outer loop
    }
}

// Returns a vector containing one time code if an exact match on the sought display time was found,
// or two time codes if the sought display time falls in between the two time codes.
vector<timecode> FrameCalculator::findClosestSourceFrames(int display_time, int left, int right) {
    vector<timecode> ret;
    int mid;

    if(timecodes.size() < 1) {
        timecode t = {0,0};
        ret.push_back(t);
        return ret;
    } else if (display_time <= timecodes[0].display_time) {
        ret.push_back(timecodes[0]);
        return ret;
    } else if (display_time >= timecodes[timecodes.size()-1].display_time) {
        ret.push_back(timecodes[timecodes.size()-1]);
        return ret;
    }

    if(left > right) {                     // Return the two frames having display times surrounding the
        ret.push_back(timecodes[right]);   // time we're after, if no exact frame (display time) was found.
        ret.push_back(timecodes[left]);    // I.e. f1 < display_time < f2
        return ret;
    }

    mid = left + ((right - left) / 2);

    if(timecodes[mid].display_time == display_time) {
        ret.push_back(timecodes[mid]);
        return ret;
    } else if(timecodes[mid].display_time > display_time) {
        return findClosestSourceFrames(display_time, left, mid - 1);
    }
    return findClosestSourceFrames(display_time, mid + 1, right);

}

// ==========================================================================
// PUBLIC methods
// ==========================================================================
int FrameCalculator::getInterpolationInfo(InterpolationInfo* interp, int frame) {
    int display_time;
    if(frame == 0) {
        interp->frame1 = 0;
        interp->frame2 = 0;
        interp->pct = 0;
        return 0;
    }

    display_time = (int)(frame / fps * 1000);

    vector<timecode> v = findClosestSourceFrames(display_time ,0, timecodes.size()-1);

    if(v.size() == 1) {                   // exact match
        interp->frame1 = v[0].frameno;
        interp->frame2 = v[0].frameno;
        interp->pct = 0;
    } else {                              // sought frame is in between two real frames (doesn't exist and would need to be created)
        interp->frame1 = v[0].frameno;
        interp->frame2 = v[1].frameno;
        interp->pct = 100 * (v[1].display_time - display_time) / (v[1].display_time - v[0].display_time);
    }
    return 0;
}

vector<timecode> FrameCalculator::getTimeCodes() {
    return timecodes;
}

void FrameCalculator::initialize() {
    readTimeCodes();
}

// Constructor implementation
FrameCalculator::FrameCalculator(const char* _timecodes_filename, double _num, double _den) :
  filename(_timecodes_filename), fps(_num/_den) {
}

FrameCalculator::~FrameCalculator() {}
