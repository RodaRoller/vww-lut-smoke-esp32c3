// Constants and variables
const int LED_PIN_OK = 4;
const int LED_PIN_KO = 5;                 

const int N = 4;                       
const int X[N] = {3, -1, 4, 2};        // fixed input vector
const int W[N] = {2, 5, -1, 3};        // weights
const int B = 2;                       // bias

const int Y_EXPECTED = 5;              // Expected for the test
const int CLASS_EXPECTED = 1;          // y > 0  ->  class 1

bool testPassed = false;               

// Functions

int dot(const int x[], const int w[], int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum = sum + x[i] * w[i];
  }
  return sum;
}

void printVector(const int v[], int n) {
  for (int i = 0; i < n; i++) {
    Serial.print(v[i]);
    if (i < n - 1) {
      Serial.print(",");
    }
  }
  Serial.println();
}

bool check(const char* name, int got, int expected) {
  bool ok = (got == expected);
  Serial.print("[test] ");
  Serial.print(name);
  Serial.print("=");
  Serial.print(got);
  Serial.print(" expected=");
  Serial.print(expected);
  if (ok) {
    Serial.println(" OK");
  } else {
    Serial.println(" MISMATCH");
  }
  return ok;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN_OK, OUTPUT);
  pinMode(LED_PIN_KO, OUTPUT);

  // Env
  Serial.println("[env] project=vww-lut-smoke v0.1");
  Serial.print("[env] arduino-esp32=");
  Serial.print(ESP_ARDUINO_VERSION_MAJOR);
  Serial.print(".");
  Serial.print(ESP_ARDUINO_VERSION_MINOR);
  Serial.print(".");
  Serial.println(ESP_ARDUINO_VERSION_PATCH);
  Serial.print("[env] esp-idf=");
  Serial.println(ESP.getSdkVersion());
  Serial.print("[env] chip=");
  Serial.println(ESP.getChipModel());
  Serial.print("[env] cplusplus=");
  Serial.println(__cplusplus);


  // Smoke test
  Serial.print("[test] x=");
  printVector(X, N);
  Serial.print("[test] w=");
  printVector(W, N);
  Serial.print("[test] b=");
  Serial.println(B);

  int y = dot(X, W, N) + B;
  int predictedClass = 0;
  if (y > 0) {
    predictedClass = 1;
  }

  bool yOk = check("y", y, Y_EXPECTED);
  bool classOk = check("class", predictedClass, CLASS_EXPECTED);
  testPassed = yOk && classOk;

  if (testPassed) {
    Serial.println("SMOKE TEST PASSED");
  } else {
    Serial.println("SMOKE TEST FAILED");
  }
}

void loop() {
  int ledPin = LED_PIN_KO;
  int blinkTime = 150;                 // fast blink, red led = test failed
  if (testPassed) {
    ledPin = LED_PIN_OK;
    blinkTime = 700;                   // slow blink, green led = test passed
  }

  // loop on/off led
  digitalWrite(ledPin, HIGH);         
  delay(blinkTime);
  digitalWrite(ledPin, LOW);          
  delay(blinkTime);
  
}
