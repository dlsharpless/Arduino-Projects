const int analogPin = A0;   // Analog pin connected to the voltage divider
const float V_in = 5.0;     // Arduino's 5V supply voltage
const float R_reference = 10000.0; // Reference resistor value in ohms (10k ohm)

void setup() {
  Serial.begin(9600);
}

void loop() {
  int analogValue = analogRead(analogPin);  // Read the analog pin value (0-1023)
  float V_out = (analogValue / 1023.0) * V_in; // Convert to voltage
  float R_unknown = R_reference * (V_out / (V_in - V_out)); // Calculate unknown resistor

  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" | V_out: ");
  Serial.print(V_out, 3);
  Serial.print(" V | R_unknown: ");
  Serial.print(R_unknown, 1);
  Serial.println(" ohms");

  delay(1000); // Wait 1 second before next reading
}
