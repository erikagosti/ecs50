#include "MyFloat.h"

MyFloat::MyFloat(){
  sign = 0;
  exponent = 0;
  mantissa = 0;
}

MyFloat::MyFloat(float f){
  unpackFloat(f);
}

MyFloat::MyFloat(const MyFloat & rhs){
	sign = rhs.sign;
	exponent = rhs.exponent;
	mantissa = rhs.mantissa;
}

ostream& operator<<(std::ostream &strm, const MyFloat &f){
	//this function is complete. No need to modify it.
	strm << f.packFloat();
	return strm;
}


MyFloat MyFloat::operator+(const MyFloat& rhs) const{

  MyFloat new_float;

  MyFloat larger = *this;
  MyFloat smaller = rhs;

  if (larger.mantissa == 0) {
    new_float = smaller;
    if (smaller.mantissa == 0)
    {
      new_float.sign = 0;
    }
    return new_float;
  }

  if (smaller.mantissa == 0) {
    new_float = larger;
    return new_float;
  }


  if (larger.mantissa == smaller.mantissa && larger.exponent == smaller.exponent && larger.sign != smaller.sign) {
    new_float.mantissa = 0;
    new_float.exponent = 0;
    new_float.sign = 0;

    return new_float;
  }

    if (larger.exponent < smaller.exponent || (larger.mantissa < smaller.mantissa && larger.exponent == smaller.exponent))
    {
      larger = rhs;
      smaller = *this;
    }

  unsigned int new_mantissa;
  unsigned int new_sign;
  unsigned int new_exp;


  unsigned int bit1 = (1 << 23); //adding bit 1 in 24th position
  larger.mantissa |= bit1;
  smaller.mantissa |= bit1;

  int exp_offset;

  bool borrow;
  if (larger.exponent > smaller.exponent ) {

    exp_offset = larger.exponent - smaller.exponent;

    if ((smaller.mantissa >> (exp_offset - 1)) & 1 ) {
      borrow = true;

    }
    else {

      borrow = false;

    }


    smaller.exponent = larger.exponent;
    smaller.mantissa = smaller.mantissa >> exp_offset;

  }

  if (this->sign == rhs.sign) { // addition
    new_mantissa = larger.mantissa + smaller.mantissa;
    new_exp = larger.exponent;
    new_sign = larger.sign;
  
  }
  else { // subtraction

    new_mantissa = larger.mantissa - smaller.mantissa;
    new_exp = larger.exponent;
    new_sign = larger.sign;
    
    if (borrow == 1) {

      new_mantissa -= 1;

    }
  }

  if (new_mantissa & 0x1000000) {
    if (new_exp == 254) {
      new_exp = 255;
      new_mantissa = 0;
    }
    else{
      new_exp++;
      new_mantissa >>= 1;
    }
  }

  if (new_mantissa != 0) {
    while ((new_mantissa & 0x800000) == 0 && new_exp > 0){
      new_mantissa <<= 1;
      new_exp--;
      if (new_exp == 0) {
        new_mantissa = 0;
        break;
      }
    }
  }

  new_float.sign = new_sign;
  new_float.exponent = new_exp;
  new_float.mantissa = new_mantissa & 0x7FFFFF; //only want the lower 23 bits

	return new_float;
}

MyFloat MyFloat::operator-(const MyFloat& rhs) const{

	/*
  negate
  call add
  */
  MyFloat neg_float = rhs;
  neg_float.sign = !rhs.sign;

  return *this + neg_float; // you don't have to return *this. it's just here right now so it will compile
}

bool MyFloat::operator==(const float rhs) const{

  MyFloat unpacked_rhs(rhs);
  if (this->mantissa == unpacked_rhs.mantissa && 
      this->exponent == unpacked_rhs.exponent && 
      this->sign == unpacked_rhs.sign)
  {
    return true;
  }
  return false; //this is just a stub so your code will compile
}


void MyFloat::unpackFloat(float f) {
    //this function must be written in inline assembly
    //extracts the fields of f into sign, exponent, and mantissa
    /*
    mantissa (in 1m form)
    - and with ~(0x1FF << 23)
    - or with (1 << 23)
    - right shift by 23
    - assign to this.mantissa

    exponent (in 127 - exp form)
    - right shift by 23
    - and with ~(1 << 8)
    - assign to this.exponent
    - subtract 127

    sign (1 or 0)
    - right shift by 31
    - assign to this.sign
    
    movl f, reg1 # reg1 = f
    movl f, reg2 # reg2 = f
    movl f, reg3 # reg3 = f

    movl 0x1FF, reg4
    shll 23, reg4
    not reg4
    and reg4, reg1
    movl $1, reg4 # reg4 = 1
    shll 23, reg4 # reg4 = 1 << 23
    or reg4, reg1
    # reg1 now holds mantissa, any other reg is free to use

    shrl $23, reg2 # reg2 = f >> 23
    movl $1, reg4 # reg4 = 1
    shll $8, reg4 # reg2 = 1 << 8
    not reg4 # reg4 = ~(1 << 8)
    and reg4, reg2 # reg2 &= ~(1 << 8)
    # reg2 now holds exponent, reg1 and reg2 are in use

    shrl $31, reg3 # reg3 >>= 31
    # reg3 now holds sign

    used 4 registers
    3 outputs 
      [mantissa] "=r" (mantissa),
      [exponent] "=r" (exponent),
      [sign] "=r" (sign) :
    1 input # f will be in %eax
      [f] "r" (f) :
    1 clobber
      "cc", "%ecx" (reg4 is ecx)
    */
    unsigned int mantissa;
    unsigned int exponent;
    unsigned int sign;

    __asm__(
        "movl %[f], %%edx;"        // edx = f

        /*
          bextr
          movl $1, reg4 # reg4 = 1
          shll 23, reg4 # reg4 = 1 << 23
          or reg4, reg1
        */
        // want to move 0 into 0:7, 23 into 8:15
        "movw $(23 << 8) | 0, %%cx;"       // bits 15:18 are 23, 0:7 are 0
        "bextr %%ecx, %%edx, %[mantissa];" // mantissa = bits 0-22 of f

        // "movl $1, %%ecx;"
        // "shll $23, %%ecx;"
        // "or %%ecx, %[mantissa];"

        // want to move 23 into 0:7, 8 into 8:15
        "movw $(8 << 8) | 23, %%cx;" // bits 15:18 are 8, 0:7 are 23
        "bextr %%ecx, %%edx, %[exponent];" // exponent = bits 23-30 of f

        // want to move 31 into 0:7, 1 into 8:15
        "movw $(1 << 8) | 31, %%cx;" // bits 15:18 are 1, 0:7 are 31
        "bextr %%ecx, %%edx, %[sign];" // sign = bit 31 of f
        :
        [mantissa] "=r"(mantissa), // mantissa is reg1
        [exponent] "=r"(exponent), // exponent is reg2
        [sign] "=r"(sign) :        // sign is reg3
        [f] "r"(f) :               // f is in %[f]
        "%ecx", "%edx"       // (reg4 is ecx)
    );

    this->mantissa = mantissa;
    this->exponent = exponent;
    this->sign = sign;
} // unpackFloat

float MyFloat::packFloat() const{
    //this function must be written in inline assembly
    //returns the floating point number represented by this
        /*
      - remove leading 1 in mantissa
      - add to float f

      - shift 23 positions left
      - add to float f
      
      - shift sign 32 positions left
      - add to float f

      movl f, reg1
      movl mantissa, reg2 # reg2 = mantissa
      movl exponent, reg3 # reg3 = exponent
      movl sign, reg4 # reg4 = sign

      movl $0, reg5 # reg5 = 0
      shll $23, reg5 # reg5 = 0 << 23
      and reg5, reg2 # mantissa &= 0 << 23
      addl reg2, reg1 # reg1 = mantissa

      shll $23, reg3 # reg3 = exponent << 23
      addl reg3, reg1 # reg1 = exponent + mantissa

      shll $32, reg4 # reg4 = sign << 31
      addl reg4, reg1 # reg1 = sign + exponent + mantissa

      1 output, 3 inputs, 1 extra register
    */

    float f = 0;

    const unsigned int mantissa = this->mantissa;
    const unsigned int exponent = this->exponent;
    const unsigned int sign = this->sign;

    __asm__(

        // "movl $1, %%ecx;"         // ecx = 1
        // "shll $23, %%ecx;"        // ecx = 1 << 23
        // "not %%ecx;" // ecx = ~(1 << 23)
        // "and %%ecx, %[mantissa];" // mantissa &= ~(1 << 23)
        "addl %[mantissa], %[f];" // f = mantissa

        "shll $23, %[exponent];"  // exponent = exponent << 23
        "or %[exponent], %[f];" // f = exponent + mantissa

        "shll $31, %[sign];" // sign <<= 31
        "or %[sign], %[f]" // f = sign + exponent + mantissa
        :
        [f] "=m"(f) : // f is only output
        [mantissa] "r"(mantissa),
        [exponent] "r"(exponent),
        [sign] "r"(sign) : // three inputs
        "%ecx" // reg5 is ecx
    );

    return f;
}//packFloat
//