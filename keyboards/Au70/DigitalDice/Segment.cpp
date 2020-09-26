#include "Segment.h"

Segment::Segment() {
  Clear();
}

void Segment::Clear() {
  value = 0;
  a = b = c = d = e = f = g = dp = false;
}

void Segment::SetDP(bool decimal_on) {
  decimal_point = decimal_on;
  dp = decimal_on;
}

bool Segment::GetDP() {
  return decimal_point;
}

void Segment::SetValue(byte new_value) {
  value = new_value;
}

byte Segment::GetByte() {
  return value | (decimal_point ? 0b00010000 : 0b00000000);
}

void Segment::SetValueFromBools() {
  // This method depends on the schematic of the circuit board.
  // It looks messy because I prioritized the board layout over
  // code complexity. Just look at the board, it's so pretty.
  value = 0;
  if (a)
    value += 1 << 1;
  if (b)
    value += 1 << 0;
  if (c)
    value += 1 << 5;
  if (d)
    value += 1 << 6;
  if (e)
    value += 1 << 7;
  if (f)
    value += 1 << 2;
  if (g)
    value += 1 << 3;
  if (dp)
    value += 1 << 4;
}

void Segment::SetValue(int new_value) {
  a = b = c = d = e = f = g = dp = false;
  value = GetValueFor(new_value);
}

byte Segment::GetValueFor(int new_value) {
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

void Segment::SetValue(char new_value) {
    a = b = c = d = e = f = g = dp = false;
    value = GetValueFor(new_value);
}

byte Segment::GetValueFor(char new_value) {
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
