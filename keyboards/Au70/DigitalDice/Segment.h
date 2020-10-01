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
bool Segment__GetDP(Segment* self);
void Segment__Clear(Segment* self);
unsigned char Segment__GetValueForInt(int new_value);
unsigned char Segment__GetValueForChar(char new_value);
void Segment__SetValueFromBools(Segment* self);

/* Definitions */

Segment* Segment__Create(void) {
  Segment* result = (Segment*) malloc(sizeof(Segment));
  Segment__Clear(result);
  return result;
}

void Segment__Clear(Segment* self) {
  self->value = 0;
  self->a = false;
  self->b = false;
  self->c = false;
  self->d = false;
  self->e = false;
  self->f = false;
  self->g = false;
  self->dp = false;
  self->decimal_point = false;
}

void Segment__SetDP(Segment* self, bool decimal_on) {
  self->decimal_point = decimal_on;
  self->dp = decimal_on;
}

bool Segment__GetDP(Segment* self) {
  return self->decimal_point;
}

void Segment__SetValueWithByte(Segment* self, unsigned char new_value) {
  self->value = new_value;
}

unsigned char Segment__GetByte(Segment* self) {
  return self->value | (self->decimal_point ? 0b00010000 : 0b00000000);
}

void Segment__SetValueFromBools(Segment* self) {
  // This method depends on the schematic of the circuit board.
  // It looks messy because I prioritized the board layout over
  // code complexity. Just look at the board, it's so pretty.
  self->value = 0;
  if (self->a)
    self->value += 1 << 1;
  if (self->b)
    self->value += 1 << 0;
  if (self->c)
    self->value += 1 << 5;
  if (self->d)
    self->value += 1 << 6;
  if (self->e)
    self->value += 1 << 7;
  if (self->f)
    self->value += 1 << 2;
  if (self->g)
    self->value += 1 << 3;
  if (self->dp)
    self->value += 1 << 4;
}

void Segment__SetValueWithInt(Segment* self, int new_value) {
  Segment__Clear(self);
  self->value = Segment__GetValueForInt(new_value);
}

unsigned char Segment__GetValueForInt(int new_value) {
  // a  b  c   d   e    f  g  dp
  // 2  1  32  64  128  4  8  16
  switch (new_value) {
    case 0:
      return (2 + 1 + 32 + 64 + 128 + 4);
    case 1:
      return (1 + 32);
    case 2:
      return (2 + 1 + 8 + 128 + 64);
    case 3:
      return (2 + 1 + 8 + 32 + 64);
    case 4:
      return (4 + 8 + 1 + 32);
    case 5:
      return (2 + 4 + 8 + 32 + 64);
    case 6:
      return (2 + 4 + 8 + 32 + 64 + 128);
    case 7:
      return (2 + 1 + 32);
    case 8:
      return (2 + 1 + 32 + 64 + 128 + 4 + 8);
    case 9:
      return (2 + 1 + 32 + 64 + 4 + 8);
    default:
      return 255;
  }
}

void Segment__SetValueWithChar(Segment* self, char new_value) {
    Segment__Clear(self);
    self->value = Segment__GetValueForChar(new_value);
}

unsigned char Segment__GetValueForChar(char new_value) {
  // a  b  c   d   e    f  g  dp
  // 2  1  32  64  128  4  8  16
  switch (new_value) {
    case '0':
      return (2 + 1 + 32 + 64 + 128 + 4);
    case '1':
      return (1 + 32);
    case '2':
      return (2 + 1 + 8 + 128 + 64);
    case '3':
      return (2 + 1 + 8 + 32 + 64);
    case '4':
      return (4 + 8 + 1 + 32);
    case '5':
      return (2 + 4 + 8 + 32 + 64);
    case '6':
      return (2 + 4 + 8 + 32 + 64 + 128);
    case '7':
      return (2 + 1 + 32);
    case '8':
      return (2 + 1 + 32 + 64 + 128 + 4 + 8);
    case '9':
      return (2 + 1 + 32 + 64 + 4 + 8);
    case 'a':
    case 'A':
      return (128 + 4 + 2 + 1 + 32 + 8);
    case 'b':
    case 'B':
      return (4 + 128 + 64 + 32 + 8);
    case 'c':
    case 'C':
      return (2 + 4 + 128 + 64);
    case 'd':
    case 'D':
      return (8 + 128 + 64 + 32 + 1);
    case 'e':
    case 'E':
      return (2 + 4 + 8 + 128 + 64);
    case 'f':
    case 'F':
      return (2 + 4 + 8 + 128);
    case 'g':
    case 'G':
      return (2 + 4 + 128 + 64 + 32);
    case 'h':
    case 'H':
      return (4 + 128 + 8 + 1 + 32);
    case 'i':
    case 'I':
      return (8 + 128);
    case 'j':
    case 'J':
      return (64 + 32 + 1);
    case 'k':
    case 'K':
      return (2 + 4 + 8 + 128 + 32);
    case 'l':
    case 'L':
      return (4 + 128 + 64);
    case 'm':
    case 'M':
      return (2 + 128 + 8 + 32);
    case 'n':
    case 'N':
      return (128 + 8 + 32);
    case 'o':
    case 'O':
      return (128 + 8 + 32 + 64);
    case 'p':
    case 'P':
      return (128 + 4 + 2 + 1 + 8);
    case 'q':
    case 'Q':
      return (8 + 4 + 2 + 1 + 32);
    case 'r':
    case 'R':
      return (128 + 8);
    case 's':
    case 'S':
      return (2 + 4 + 8 + 32 + 64);
    case 't':
    case 'T':
      return (4 + 8 + 128 + 64);
    case 'u':
    case 'U':
      return (128 + 64 + 32);
    case 'v':
    case 'V':
      return (64 + 32);
    case 'w':
    case 'W':
      return (4 + 1 + 64);
    case 'x':
    case 'X':
      return (8 + 64);
    case 'y':
    case 'Y':
      return (4 + 8 + 1 + 32 + 64);
    case 'z':
    case 'Z':
      return (2 + 1 + 8 + 128 + 64);
    case '_':
      return(64);
      break;
    case '-':
      return (8);
      break;
    case ' ':
    case '\0':
      // Nothing, but don't show dp because it's legit
      return(0);
    default:
      // Error?
      return (16);
  }
}
