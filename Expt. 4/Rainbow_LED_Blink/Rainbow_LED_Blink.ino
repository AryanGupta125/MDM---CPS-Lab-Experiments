// RGB LED Blink for Arduino Nano 33 BLE / Nano 33 BLE Sense

void setup() { 
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Turn all LEDs OFF (The built-in RGB on these boards is active LOW)
  digitalWrite(LEDR, HIGH); 
  digitalWrite(LEDG, HIGH); 
  digitalWrite(LEDB, HIGH); 

  // Initialize Serial Monitor at 9600 baud rate
  Serial.begin(9600);
  Serial.println("Nano 33 BLE RGB Test Initialized.");
}

void loop() { 
  
  // RED
  digitalWrite(LEDR, LOW);      
  Serial.println("LED: RED");
  delay(1000);
  digitalWrite(LEDR, HIGH);     

  // GREEN
  digitalWrite(LEDG, LOW);      
  Serial.println("LED: GREEN");
  delay(1000);
  digitalWrite(LEDG, HIGH);     

  // BLUE
  digitalWrite(LEDB, LOW);      
  Serial.println("LED: BLUE"); 
  delay(1000);
  digitalWrite(LEDB, HIGH);  
}