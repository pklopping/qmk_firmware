#ifndef Segment_h
#define Segment_h

class Segment {
  public:
    Segment();
    void SetValue(byte new_value);
    void SetValue(char new_value);
    void SetValue(int new_value);
    byte GetByte();
    void SetDP(bool decimal_on);
    bool GetDP();
    void Clear();
    static byte GetValueFor(int new_value);
    static byte GetValueFor(char new_value);
  private:
    byte value;
    bool a,b,c,d,e,f,g,dp, decimal_point;
    void SetValueFromBools();
};

#endif
