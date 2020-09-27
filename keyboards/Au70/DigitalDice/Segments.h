#pragma once

#include "Segment.h"

#define MAX_FRAMES 64

typedef struct {
    long last_animation_frame_millis;
    int millis_per_frame;
    int animation_index;
    int animation_frames_left;
    unsigned char *animation_frames[MAX_FRAMES][4];
    int value;
    Segment *segments[4];
} Segments;

Segments* Segments__Create(void);
void Segments__SetValue(Segments* self, int i);

void Segments__SetValue(Segments* self, int i);
void Segments__SetValueWithByte(Segments* self, int i, unsigned char new_value);
void Segments__SetValueWithChar(Segments* self, int i, char new_value);
void Segments__SetValueWithInt(Segments* self, int i, int new_value);
void Segments__SetValueWithString(Segments* self, char* msg);
int Segments__GetValue(Segments* self);
unsigned char Segments__GetByte(Segments* self, int i);
void Segments__SetDP(Segments* self, int i, bool decimal_on);
bool Segments__GetDP(Segments* self, int i);
void Segments__Clear(Segments* self);
void Segments__ClearAnimation(Segments* self);
void Segments__SetAnimationWithAdditive(Segments* self, unsigned char bytes[][4], int len, bool additive);
void Segments__SetAnimation(Segments* self, char chars[][4], int len);
void Segments__StepAnimation(Segments* self);
bool Segments__IsAnimating(Segments* self);
int Segments__AnimationIndex(int animation_index);
