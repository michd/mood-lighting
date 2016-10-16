// PWM output pins for LEDs
#define PIN_RED 5
#define PIN_GREEN 3
#define PIN_BLUE 10

// Analog inputs for pots
#define PIN_RED_IN A0
#define PIN_GREEN_IN A1
#define PIN_BLUE_IN A2

// Mode selection switch
#define PIN_MODE_SELECT 15

// Base increment for hue
#define HUE_STEP 0.00001

double hue = 0;

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_RED_IN, INPUT);
  pinMode(PIN_GREEN_IN, INPUT);
  pinMode(PIN_BLUE_IN, INPUT);
  
  // Initialize to black (all LEDs off)
  writeColor(0, 0, 0);
}

void loop() {
  bool manualMode = digitalRead(PIN_MODE_SELECT);
  
  // In manual mode we output the levels read from
  // potentiometers at the analog inputs
  if (manualMode) {
    byte manualColor[3];
    readColor(manualColor);
    writeColor(manualColor);
  } else {
    // In program mode we loop through the hue wheel
    byte rgbColor[3];
    double saturation = normalizeAnalog(analogRead(PIN_GREEN_IN));
    double lightness = normalizeAnalog(analogRead(PIN_BLUE_IN)) * 0.5;
    hslToRgb(hue, saturation, lightness, rgbColor);
    writeColor(rgbColor);
    double speed = expCurve(normalizeAnalog(analogRead(PIN_RED_IN)), 4);
    hue += HUE_STEP * ((speed * 2500) + 1);
    if (hue > 1) hue = 0;
  }
  
  delay(5);
}

// Read the currently configured color from the 3 analog
// inputs, writing it to the array outColor
void readColor(byte* outColor) {
  outColor[0] = analogRead(PIN_RED_IN) >> 2;
  outColor[1] = analogRead(PIN_GREEN_IN) >> 2;
  outColor[2] = analogRead(PIN_BLUE_IN) >> 2;
}

// Shorthand to use arrays for writing color to outputs
void writeColor(byte* color) {
  writeColor(color[0], color[1], color[2]);
}

void writeColor(byte red, byte green, byte blue) {
  analogWrite(PIN_RED, red);
  analogWrite(PIN_GREEN, green);
  analogWrite(PIN_BLUE, blue);
}

double normalizeAnalog(int in) {
  return ((double)in / 1024);
}

// in should be 0...1
double expCurve(double in, int power) {
  double out = in;
  int i = power;
  while (i-- > 0) out *= in;
  return out;
}

// Thanks to https://github.com/ratkins/RGBConverter/blob/master/RGBConverter.cpp
byte* hslToRgb(double h, double s, double l, byte* rgb) {
    double r, g, b;

    if (s == 0) {
        r = g = b = l; // achromatic
    } else {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3.0);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3.0);
    }
    
    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
}

double hue2rgb(double p, double q, double t) {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1/6.0) return p + (q - p) * 6 * t;
    if (t < 1/2.0) return q;
    if (t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
    return p;
}
