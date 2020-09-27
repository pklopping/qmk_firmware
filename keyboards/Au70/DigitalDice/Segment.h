#pragma once

typedef struct {
    unsigned char value;
    bool a,b,c,d,e,f,g,dp, decimal_point;
} Segment;

Segment* Segment__Create(void);
void Segment__SetValueWithByte(Segment* self, unsigned char new_value);
void Segment__SetValueWithChar(Segment* self, char new_value);
void Segment__SetValueWithInt(Segment* self, int new_value);
unsigned char Segment__GetValue(Segment* self);
void Segment__SetDP(Segment* self, bool decimal_on);
void Segment__GetDP(Segment* self);
void Segment__Clear(Segment* self);
unsigned char Segment__GetValueForInt(int new_value);
unsigned char Segment__GetValueForChar(char new_value);
void Segment__SetValueFromBools(Segment* self);
