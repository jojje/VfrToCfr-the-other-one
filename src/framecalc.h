#include <vector>

typedef struct {
    int frameno;
    int display_time;
} timecode;

typedef struct {
    int frame1;                                                  // Frame to the left of the sought frame
    int frame2;                                                  // Frame to the right of the sought frame
    int pct;                                                     // to interpolate between frame 1 and 2 in order to get the temporally perfect frame.
} InterpolationInfo;

class FrameCalculator {
    const char* filename;                                        // Name of the time codes file.
    double fps;                                                  // FPS to convert to
    std::vector<timecode> timecodes;
public:
    int getInterpolationInfo(InterpolationInfo* interp, int n);  // Ok the name.. Was intended to be used to provide data to
                                                                 //   support temporal "perfect" VFR->CFR by using motion interpolation, 
                                                                 //   but that seems more like a user space task atm, so only actually
                                                                 //   using one info element from the reqult get a "rough hit" on the 
                                                                 //   source frame. In other words, favor perfect *picture* to perfect
                                                                 //   temporal *position*.
    std::vector<timecode> getTimeCodes();                        // Returns the underlying vector of all time codes
    void initialize();                                           // Throw exception! used to support lazy loading.
    FrameCalculator(const char* timecodes_filename, double fps_num, double fps_den);
    ~FrameCalculator();
private:
    void readTimeCodes();
    std::vector<timecode> findClosestSourceFrames(int frame, int start, int end);

    // Fixes the intra display times, by replacing frames that are listed as one millisecond
    // greater than a previous frame, through estimating the real display time given surrounding
    // frames with perceived correct display times.
    void adjust_display_times();
};

