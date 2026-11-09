// Constants and variables
const int LED_PIN_OK = 4;
const int LED_PIN_KO = 5;                 

const int N = 4;                       
const int X[N] = {3, -1, 4, 2};        // fixed input vector
const int W[N] = {2, 5, -1, 3};        // weights
const int B = 2;                       // bias

const int Y_EXPECTED = 6;              // Expected for the test
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
  int LED_PIN = LED_PIN_KO;
  int blinkTime = 150;                 // fast blink, red led = test failed
  if (testPassed) {
    LED_PIN = LED_PIN_OK;
    blinkTime = 700;                   // slow blink, yello led = test passed
  }

  // loop on/off led
  digitalWrite(LED_PIN, HIGH);         
  delay(blinkTime);
  digitalWrite(LED_PIN, LOW);          
  delay(blinkTime);
  
}
