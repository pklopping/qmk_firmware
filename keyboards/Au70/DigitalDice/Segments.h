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

/* Definitions */


Segments* Segments__Create(void) {
  Segments* result = (Segments*) malloc(sizeof(Segments));
  result->millis_per_frame = 10;
  result->value = 0;
  for (int i = 0; i < 4; i++) {
    result->segments[i] = Segment__Create();
  }

  Segments__ClearAnimation(result);
  return result;
}

unsigned char Segments__GetByte(Segments* self, int i) {
  return slef->segments[i]->GetByte();
}

void Segments__SetValueWithByte(Segments* self, int i, unsigned char new_value) {
  Segment__SetValueWithByte(self->segments[i], new_value);
}

void Segments__SetValueWithChar(Segments* self, int i, char new_value) {
  Segment__SetValueWithChar(self->segments[i], new_value);
}

void Segments__SetValueWithInt(Segments* self, int i, int new_value) {
  Segment__SetValueWithInt(self->segments[i], new_value);
}

void Segments__Clear(Segments* self) {
  Segments__SetvalueWithString(self, "\0");
  Segments__ClearAnimation(self);
}

void Segments__SetDP(Segments* self, int segment, bool value) {
  Segment__SetDP(self->segments[segment], value);
}

bool Segments__GetDP(Segments* self, int segment) {
  return self->segments[segment]->GetDP();
}

void Segments__ClearAnimation(Segments* self) {
  self->animation_frames_left = 0;
  self->animation_index = 0;
  for (int i = 0; i < MAX_FRAMES; i++) {
    for (int j = 0; j < 4; j++) {
      self->animation_frames[i][j] = 0;
    }
  }
}

void Segments__SetValueWithString(Segments* self, char* msg) {
  bool clear_rest = false;
  for (int i = 0; i < 4; i++) {
    if (clear_rest || msg[i] == '\0') {
      clear_rest = true;
      Segment__SetValueWithChar(self->segments[i], ' ');
    }
    else
      Segment__SetValueWithChar(self->segments[i], msg[i]);
  }
}

int Segments__AnimationIndex(int index) {
  return index % MAX_FRAMES;
}

void Segments__SetAnimationWithAdditive(Segments* self, byte bytes[][4], int len, bool additive) {
    self->last_animation_frame_millis = millis();

    self->animation_frames_left = (self->animation_frames_left > len) ? self->animation_frames_left : len ;

    for (int i = 0; i < len; i++) {
      for (int j = 0; j < 4; j++) {
        if (self->additive)
          self->animation_frames[Segments__AnimationIndex(i+self->animation_index)][j] = (byte)self->animation_frames[Segments__AnimationIndex(i+self->animation_index)][j] | (byte)bytes[i][j];
        else
          self->animation_frames[Segments__AnimationIndex(i+self->animation_index)][j] = bytes[i][j];
      }
    }
}

void Segments__SetAnimation(Segments* self, char chars[][4], int len) {
    self->last_animation_frame_millis = millis();

    self->animation_frames_left = len;

    bool clear_rest = false;
    for (int i = 0; i < len; i++) {
      clear_rest = false;
      for (int j = 0; j < 4; j++) {
        if (clear_rest || chars[j] == '\0') {
          clear_rest = true;
          self->animation_frames[Segments__AnimationIndex(i+self->animation_index)][j] = 0;
        } else {
          self->animation_frames[Segments__AnimationIndex(i+self->animation_index)][j] = Segment__GetValueFor(chars[i][j]);
        }
      }
    }
}

void Segments__StepAnimation(Segments* self) {
    // If there are no frames, do nothing
    if (self->animation_frames_left < 1)
      return;

    // Update all segments
    for (int i = 0; i < 4; i++) {
      self->segments[i]->Segment__SetValue((byte)self->animation_frames[self->animation_index][i]);
      // segments[i]->SetValue(255); // for to debug with
    }

    // segments[0]->SetValue(animation_index); // for to debug with
    // segments[1]->SetValue(AnimationIndex(animation_index+1)); // for to debug with
    // segments[2]->SetValue(AnimationIndex(animation_index+2)); // for to debug with
    // segments[3]->SetValue(AnimationIndex(animation_index+3)); // for to debug with

    // If called before the next frame is due, do nothing
    if (millis() < self->last_animation_frame_millis + self->millis_per_frame)
      return;

    // Clear the memory so it's not goofy for the next loop
    for (int i = 0; i < 4; i++)
      self->animation_frames[self->animation_index][i] = 0;

    // Move to next frame and record the last frame time
    self->animation_index = Segments__AnimationIndex(self->animation_index + 1);
    self->animation_frames_left--;
    self->last_animation_frame_millis = millis();
}

bool Segments__IsAnimating(Segments* self, ) {
  return (self->animation_frames_left > 0);
}

void Segments__SetValue(Segments* self, int new_value) {
  // This math stolen from here: https://www.quora.com/How-do-I-separate-ones-tens-hundreds-and-thousands-from-a-four-digit-user-input-in-C-programming
  // I'm lazy.
  int thousands, hundreds, tens, units;
  units     = (new_value/1)    % 10;
  tens      = (new_value/10)   % 10;
  hundreds  = (new_value/100)  % 10;
  thousands = (new_value/1000) % 10;

  self->segments[3]->Segment__SetValue(units);
  
  if (new_value >= 10)
    self->segments[2]->Segment__SetValue(tens);
  else
    self->segments[2]->Segment__Clear();
    
  if (new_value >= 100)
    self->segments[1]->Segment__SetValue(hundreds);
  else
    self->segments[1]->Segment__Clear();

  if (new_value >= 1000)
    self->segments[0]->Segment__SetValue(thousands);
  else
    self->segments[0]->Segment__Clear();
}
