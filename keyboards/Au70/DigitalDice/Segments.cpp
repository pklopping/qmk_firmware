#include "Segments.h"

Segments::Segments() {
  value = 0;
  for (int i = 0; i < 4; i++) {
    segments[i] = new Segment();
  }

  ClearAnimation();
}

byte Segments::GetByte(int i) {
  return segments[i]->GetByte();
}

void Segments::SetValue(int i, byte new_value) {
  segments[i]->SetValue(new_value);
}

void Segments::SetValue(int i, char new_value) {
  segments[i]->SetValue(new_value);
}

void Segments::SetValue(int i, int new_value) {
  segments[i]->SetValue(new_value);
}

void Segments::Clear() {
  SetValue("\0");
  ClearAnimation();
}

void Segments::SetDP(int segment, bool value) {
  segments[segment]->SetDP(value);
}

bool Segments::GetDP(int segment) {
  return segments[segment]->GetDP();
}

void Segments::ClearAnimation() {
  animation_frames_left = 0;
  animation_index = 0;
  for (int i = 0; i < MAX_FRAMES; i++) {
    for (int j = 0; j < 4; j++) {
      animation_frames[i][j] = 0;
    }
  }
}

void Segments::SetValue(char* msg) {
  bool clear_rest = false;
  for (int i = 0; i < 4; i++) {
    if (clear_rest || msg[i] == '\0') {
      clear_rest = true;
      segments[i]->SetValue(' ');
    }
    else
      segments[i]->SetValue(msg[i]);
  }
}

int Segments::AnimationIndex(int index) {
  return index % MAX_FRAMES;
}

void Segments::SetAnimation(byte bytes[][4], int len, bool additive) {
    last_animation_frame_millis = millis();

    animation_frames_left = (animation_frames_left > len) ? animation_frames_left : len ;

    for (int i = 0; i < len; i++) {
      for (int j = 0; j < 4; j++) {
        if (additive)
          animation_frames[AnimationIndex(i+animation_index)][j] = (byte)animation_frames[AnimationIndex(i+animation_index)][j] | (byte)bytes[i][j];
        else
          animation_frames[AnimationIndex(i+animation_index)][j] = bytes[i][j];
      }
    }
}

void Segments::SetAnimation(char chars[][4], int len) {
    last_animation_frame_millis = millis();

    animation_frames_left = len;

    bool clear_rest = false;
    for (int i = 0; i < len; i++) {
      clear_rest = false;
      for (int j = 0; j < 4; j++) {
        if (clear_rest || chars[j] == '\0') {
          clear_rest = true;
          animation_frames[AnimationIndex(i+animation_index)][j] = 0;
        } else {
          animation_frames[AnimationIndex(i+animation_index)][j] = Segment::GetValueFor(chars[i][j]);
        }
      }
    }
}

void Segments::StepAnimation() {
    // If there are no frames, do nothing
    if (animation_frames_left < 1)
      return;

    // Update all segments
    for (int i = 0; i < 4; i++) {
      segments[i]->SetValue((byte)animation_frames[animation_index][i]);
      // segments[i]->SetValue(255); // for to debug with
    }

    // segments[0]->SetValue(animation_index); // for to debug with
    // segments[1]->SetValue(AnimationIndex(animation_index+1)); // for to debug with
    // segments[2]->SetValue(AnimationIndex(animation_index+2)); // for to debug with
    // segments[3]->SetValue(AnimationIndex(animation_index+3)); // for to debug with

    // If called before the next frame is due, do nothing
    if (millis() < last_animation_frame_millis + millis_per_frame)
      return;

    // Clear the memory so it's not goofy for the next loop
    for (int i = 0; i < 4; i++)
      animation_frames[animation_index][i] = 0;

    // Move to next frame and record the last frame time
    animation_index = AnimationIndex(animation_index + 1);
    animation_frames_left--;
    last_animation_frame_millis = millis();
}

bool Segments::IsAnimating() {
  return (animation_frames_left > 0);
}

void Segments::SetValue(int new_value) {
  // This math stolen from here: https://www.quora.com/How-do-I-separate-ones-tens-hundreds-and-thousands-from-a-four-digit-user-input-in-C-programming
  // I'm lazy.
  int thousands, hundreds, tens, units;
  units     = (new_value/1)    % 10;
  tens      = (new_value/10)   % 10;
  hundreds  = (new_value/100)  % 10;
  thousands = (new_value/1000) % 10;

  segments[3]->SetValue(units);
  
  if (new_value >= 10)
    segments[2]->SetValue(tens);
  else
    segments[2]->Clear();
    
  if (new_value >= 100)
    segments[1]->SetValue(hundreds);
  else
    segments[1]->Clear();

  if (new_value >= 1000)
    segments[0]->SetValue(thousands);
  else
    segments[0]->Clear();
}
