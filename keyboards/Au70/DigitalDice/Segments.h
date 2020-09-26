#ifndef Segments_h
#define Segments_h

#include "Segment.h"

#define MAX_FRAMES 64

class Segments {
  public:
    Segments();
    void SetValue(int i);
    void SetValue(int i, byte new_value);
    void SetValue(int i, char new_value);
    void SetValue(int i, int new_value);
    void SetValue(char* msg);
    int GetValue();
    byte GetByte(int i);
    void SetDP(int i, bool decimal_on);
    bool GetDP(int i);
    void Clear();
    void ClearAnimation();
    void SetAnimation(byte bytes[][4], int len, bool additive);
    void SetAnimation(char chars[][4], int len);
    void StepAnimation();
    bool IsAnimating();
  private:
    int AnimationIndex(int animation_index);
    long last_animation_frame_millis;
    int millis_per_frame = 10;
    int animation_index;
    int animation_frames_left;
    byte *animation_frames[MAX_FRAMES][4];
    int value;
    Segment *segments[4];
};

#endif
