#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <elapsedMillis.h>

#define btnPin A0

const int rBtn = 510;
const int gBtn = 330;
const int bBtn = 250;
const int modeBtn = 1010;

#define PIN 2
#define LEDNum 132

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNum, PIN, NEO_GRB + NEO_KHZ800);


bool rUp = true, bUp = true, gUp = true;
int pattern = 0;
int mode = 0;

elapsedMillis timer;

void setup() {
  timer = 0;
  pinMode(btnPin, INPUT);
  debugBegin()
  strip.begin();
  strip.setBrightness(50);
  strip.show();
  if (random(0, 100) % 2 == 1) {
    rUp = true;
  } else {
    rUp = false;
  }
  if (random(0, 100) % 2 == 1) {
    gUp = true;
  } else {
    gUp = false;
  }
  if (random(0, 100) % 2 == 1) {
    bUp = true;
  } else {
    bUp = false;
  }
  delay(1000);
}
int r = floor((random(0, 7650) / 30));
int g = floor((random(0, 7650) / 30));
int b = floor((random(0, 7650) / 30));

bool lastbtn = false;
int btn;

//backAndForth Variables::
int bnfi = 0;
bool bnfUp = true;

//Bounce variables::
int bouncei = 0, bouncej = LEDNum;
bool bounceiUp = true, bouncejUp = false;

//ColorWipe Variables::
int CWi = 0;
int CWstage = 0;

//randomDots Variables::
int RDi = 0, RDj = 0;

//Rainbow Variables::
int RBi = 0;
uint16_t RBj = 0;

//RainbowCycle Variables::
int RBCi = 0;
int RBCj = 0;

//Flash Variables::
bool stripState = false;

void loop() {

  btn = analogRead(btnPin);

  //Mode handling::

  if (mode == 0) {
    switch (pattern) {
      case 0:
        backAndForth(30);
        break;
      case 1:
        if (CWstage == 0) {
          colorWipe(strip.Color(255, 0, 0), 50);
          if (CWi == LEDNum) {
            CWstage = 1;
            CWi = 0;
          }
        } else if (CWstage == 1) {
          colorWipe(strip.Color(255, 127, 0), 50);
          if (CWi == LEDNum) {
            CWstage = 2;
            CWi = 0;
          }
        } else if (CWstage == 2) {
          colorWipe(strip.Color(0, 255, 0), 50);
          if (CWi == LEDNum) {
            CWstage = 3;
            CWi = 0;
          }
        } else if (CWstage == 3) {
          colorWipe(strip.Color(0, 255, 127), 50);
          if (CWi == LEDNum) {
            CWstage = 4;
            CWi = 0;
          }
        } else if (CWstage == 4) {
          colorWipe(strip.Color(0, 0, 255), 50);
          if (CWi == LEDNum) {
            CWstage = 5;
            CWi = 0;
          }
        } else {
          colorWipe(strip.Color(127, 0, 255), 50);
          if (CWi == LEDNum) {
            CWstage = 0;
            CWi = 0;
          }
        }
        break;
      case 2:
        if (RDi < 30) {
          randomDots(50);
        } else {
          strip.clear();
          RDi = 0;
        }
        break;
      case 3:
        bounce(30);
        break;
      case 4:
        if (RBj < 256) {
          if (RBi < LEDNum) {
            rainbow(20);
          } else {
            strip.clear();
            RBi = 0;
            RBj = 0;
          }
        }
        break;
      case 5:
        flash(strip.Color(random(0, 255), random(0, 255), random(0, 255)), 100);
        break;
    }
  } else if (mode == 1) {
    switch (pattern) {
      //Red
      case 0:
        colorWipe(strip.Color(255, 0, 0), 50);
        if (CWi == LEDNum) {
          CWi = 0;
          strip.clear();
        }
        break;
      case 1:
        bounce(strip.Color(255, 0, 0), 30);
        break;
      case 2:
        if (RDi < 30) {
          randomDots(strip.Color(255, 0, 0), 50);
        } else {
          strip.clear();
          RDi = 0;
        }
        break;
      case 3:
        flash(strip.Color(255, 0, 0), 100);
        break;
    }
  } else if (mode == 2) {
    //Green
    switch (pattern) {
      case 0:
        colorWipe(strip.Color(0, 255, 0), 50);
        if (CWi == LEDNum) {
          CWi = 0;
          strip.clear();
        }
        break;
      case 1:
        bounce(strip.Color(0, 255, 0), 30);
        break;
      case 2:
        if (RDi < 30) {
          randomDots(strip.Color(0, 255, 0), 50);
        } else {
          strip.clear();
          RDi = 0;
        }
        break;
      case 3:
        flash(strip.Color(0, 255, 0), 100);
        break;
    }
  } else if (mode == 3) {
    //Green
    switch (pattern) {
      case 0:
        colorWipe(strip.Color(0, 0, 255), 50);
        if (CWi == LEDNum) {
          CWi = 0;
          strip.clear();
        }
        break;
      case 1:
        bounce(strip.Color(0, 0, 255), 30);
        break;
      case 2:
        if (RDi < 30) {
          randomDots(strip.Color(0, 0, 255), 50);
        } else {
          strip.clear();
          RDi = 0;
        }
        break;
      case 3:
        flash(strip.Color(0, 0, 255), 100);
        break;
    }
  }

  //Button handling::
  debugln(btn) if (btn > 20) {
    if ((btn < modeBtn + 35 && btn > modeBtn - 35) && !lastbtn) {
      if (mode != 0) {
        mode = 0;
        pattern = 5;
        strip.clear();
      }
      if (pattern == 0) {
        //ColorWipe
        pattern = 1;
        CWi = 0;
        CWstage = 0;
        strip.clear();
      } else if (pattern == 1) {
        //BackAndForth
        pattern = 2;
        bnfi = 0;
        strip.clear();
      } else if (pattern == 2) {
        //RandomDots
        strip.clear();
        pattern = 3;
        RDi = 0;
        RDj = 0;
      } else if (pattern == 3) {
        //Bounce
        strip.clear();
        pattern = 4;
        bouncei = 0;
        bouncej = LEDNum;
        bounceiUp = true;
        bouncejUp = false;
      } else if (pattern == 4) {
        //Rainbow
        strip.clear();
        pattern = 5;
        RBj = 0;
        RBi = 0;
      } else {
        //Flash
        strip.clear();
        pattern = 0;
        stripState = false;
      }

    } else if ((btn < (rBtn + 35) && btn > (rBtn - 35)) && !lastbtn) {
      if (mode != 1) {
        mode = 1;
        pattern = 3;
        strip.clear();
      }
      if (pattern == 0) {
        //ColorWipe
        pattern = 1;
        CWi = 0;
        CWstage = 0;
        strip.clear();
      } else if (pattern == 1) {
        //Bounce
        pattern = 2;
        bouncei = 0;
        bouncej = LEDNum;
        bounceiUp = true;
        bouncejUp = false;
        strip.clear();
      } else if (pattern == 2) {
        //RandomDots(Red)
        pattern = 3;
        RDi = 0;
        RDj = 0;
      } else if (pattern == 3) {
        //Flash::
        pattern = 0;
        stripState = false;
      }
    } else if ((btn < (gBtn + 35) && btn > (gBtn - 35)) && !lastbtn) {
      if (mode != 2) {
        mode = 2;
        pattern = 3;
        strip.clear();
      }
      if (pattern == 0) {
        //ColorWipe
        pattern = 1;
        CWi = 0;
        CWstage = 0;
        strip.clear();
      } else if (pattern == 1) {
        //Bounce
        pattern = 2;
        bouncei = 0;
        bouncej = LEDNum;
        bounceiUp = true;
        bouncejUp = false;
        strip.clear();
      } else if (pattern == 2) {
        //RandomDots(Green)
        pattern = 3;
        RDi = 0;
        RDj = 0;
      } else if (pattern == 3) {
        //Flash::
        pattern = 0;
        stripState = false;
      }
    } else if ((btn < (bBtn + 35) && btn > (bBtn - 35)) && !lastbtn) {
      if (mode != 3) {
        mode = 3;
        pattern = 3;
        strip.clear();
      }
      if (pattern == 0) {
        //ColorWipe
        pattern = 1;
        CWi = 0;
        CWstage = 0;
        strip.clear();
      } else if (pattern == 1) {
        //Bounce
        pattern = 2;
        bouncei = 0;
        bouncej = LEDNum;
        bounceiUp = true;
        bouncejUp = false;
        strip.clear();
      } else if (pattern == 2) {
        //RandomDots(Green)
        pattern = 3;
        RDi = 0;
        RDj = 0;
      } else if (pattern == 3) {
        //Flash::
        pattern = 0;
        stripState = false;
      }
    }
    lastbtn = true;
  }
  else if (btn < 20) {
    lastbtn = false;
  }
}

void flash(uint32_t c, unsigned int del) {
  if (timer > del) {
    timer = 0;
    if (stripState) {
      for (int i = 0; i < LEDNum; i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      stripState = false;
    } else {
      strip.clear();
      strip.show();
      stripState = true;
    }
  }
}
void bounce(unsigned int del) {
  if (timer > del) {
    timer = 0;
    red();
    green();
    blue();

    if (bounceiUp) {
      if (bouncei < (LEDNum / 2)) {
        strip.setPixelColor(bouncei, strip.Color(r, g, b));
        bouncei++;

      } else {
        bounceiUp = false;
      }
      if (bouncei > 5) {
        strip.setPixelColor(bouncei - 6, strip.Color(0, 0, 0));
      }

    } else {
      if (bouncei > 0) {
        strip.setPixelColor(bouncei, strip.Color(r, g, b));
        bouncei--;

      } else {
        bounceiUp = true;
      }
      if (bouncei < ((LEDNum / 2) - 5)) {
        strip.setPixelColor(bouncei + 6, 0, 0, 0);
      }
    }
    if (bouncejUp) {
      if (bouncej < LEDNum) {
        strip.setPixelColor(bouncej, strip.Color(r, g, b));
        bouncej++;
      } else {
        bouncejUp = false;
      }
      if (bouncej > ((LEDNum / 2) + 5)) {
        strip.setPixelColor(bouncej - 6, strip.Color(0, 0, 0));
      }

    } else {
      if (bouncej > LEDNum / 2) {
        strip.setPixelColor(bouncej, strip.Color(r, g, b));
        bouncej--;
      } else {
        bouncejUp = true;
      }
      if (bouncej < (LEDNum - 5)) {
        strip.setPixelColor(bouncej + 6, strip.Color(0, 0, 0));
      }
    }
    strip.show();
  }
}
void bounce(uint32_t c, unsigned int del) {
  if (timer > del) {
    timer = 0;
    if (bounceiUp) {
      if (bouncei < (LEDNum / 2)) {
        strip.setPixelColor(bouncei, c);
        bouncei++;

      } else {
        bounceiUp = false;
      }
      if (bouncei > 5) {
        strip.setPixelColor(bouncei - 6, strip.Color(0, 0, 0));
      }

    } else {
      if (bouncei > 0) {
        strip.setPixelColor(bouncei, c);
        bouncei--;

      } else {
        bounceiUp = true;
      }
      if (bouncei < ((LEDNum / 2) - 5)) {
        strip.setPixelColor(bouncei + 6, 0, 0, 0);
      }
    }
    if (bouncejUp) {
      if (bouncej < LEDNum) {
        strip.setPixelColor(bouncej, c);
        bouncej++;
      } else {
        bouncejUp = false;
      }
      if (bouncej > ((LEDNum / 2) + 5)) {
        strip.setPixelColor(bouncej - 6, strip.Color(0, 0, 0));
      }

    } else {
      if (bouncej > LEDNum / 2) {
        strip.setPixelColor(bouncej, c);
        bouncej--;
      } else {
        bouncejUp = true;
      }
      if (bouncej < (LEDNum - 5)) {
        strip.setPixelColor(bouncej + 6, strip.Color(0, 0, 0));
      }
    }
    strip.show();
  }
}

void randomDots(unsigned int del) {
  if (timer > del) {
    timer = 0;
    if (RDi < 30) {
      int index = random(0, LEDNum);
      RDj = index - 2;
      while (RDj < index + 2) {
        strip.setPixelColor(RDj, strip.Color(random(0, 255), random(0, 255), random(0, 255)));
        strip.show();
        RDj++;
      }
      RDi++;
    }
  }
}

void randomDots(uint32_t c, unsigned int del) {
  if (timer > del) {
    timer = 0;
    if (RDi < 30) {
      int index = random(0, LEDNum);
      RDj = index - 2;
      while (RDj < index + 2) {
        strip.setPixelColor(RDj, c);
        strip.show();
        RDj++;
      }
      RDi++;
    }
  }
}

void backAndForth(int del) {

  if (timer > del) {
    timer = 0;
    red();
    green();
    blue();
    if (bnfUp) {
      if (bnfi < LEDNum) {
        strip.setPixelColor(bnfi, strip.Color(r, g, b));
        bnfi++;
      } else {
        bnfUp = false;
      }
      if (bnfi > 7) {
        strip.setPixelColor(bnfi - 8, strip.Color(0, 0, 0));
      }
    } else {
      if (bnfi >= 0) {
        strip.setPixelColor(bnfi, strip.Color(r, g, b));
        bnfi--;
      } else {
        bnfUp = true;
      }
      if (bnfi < LEDNum - 7) {
        strip.setPixelColor(bnfi + 8, strip.Color(0, 0, 0));
      }
    }
    strip.show();
  }
}

void red() {
  if (rUp) {
    if (r < 245) {
      r += random(1, 10);
      if (r > 245) {
        rUp = false;
      }
    } else {
      rUp = false;
    }
  } else {
    if (r > 10) {
      r -= random(1, 10);
      if (r < 10) {
        rUp = true;
      }
    } else {
      rUp = true;
    }
  }
}
void green() {

  if (gUp) {
    if (g < 245) {
      g += random(1, 10);
      if (g > 245) {
        gUp = false;
      }
    } else {
      gUp = false;
    }
  } else {
    if (g > 10) {
      g -= random(1, 10);
      if (g < 10) {
        gUp = true;
      }
    } else {
      gUp = true;
    }
  }
}
void blue() {
  if (bUp) {
    if (b < 245) {
      b += random(1, 10);
      if (b > 245) {
        bUp = false;
      }
    } else {
      bUp = false;
    }
  } else {
    if (b > 10) {
      b -= random(1, 10);
      if (b < 10) {
        bUp = true;
      }
    } else {
      bUp = true;
    }
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, unsigned int del) {

  if (timer > del) {
    timer = 0;
    if (CWi < LEDNum) {
      strip.setPixelColor(CWi, c);
      strip.show();
      CWi++;
    } else {
      if (CWstage == 0) {
        CWstage = 1;
      } else if (CWstage == 1) {
        CWstage = 2;
      } else {
        CWstage = 0;
      }
      return;
    }
  }
}


void rainbow(unsigned int del) {

  if (timer > del) {
    timer = 0;
    if (RBj < 256) {
      if (RBi < LEDNum) {
        strip.setPixelColor(RBi, Wheel((RBi + RBj) & 255));
        strip.show();
        RBi++;
      }
    }
    RBj++;
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
