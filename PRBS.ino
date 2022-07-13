float voltage;
// Declare variable for PRBS
int cnt               = 0;
int j                 = 0;
int k                 = 0;
int temp              = 0;
int stop_             = 0;
unsigned char data[8] = {1,1,1,1,1,1,1,1}, prbs[8]; // 8-bit PRBS
int pwm_prbs          = 0;


void setup() {
  // Set Timer1 in Arduino
  noInterrupts();       // disable all interrupts
  TCCR1A  = 0;          // Reset entire TCCR1A register to 0
  TCCR1B  = 0;          // Reset entire TCCR1B register to 0
  TCNT1   = 64753;      // Set preload timer
  TCCR1B  = 0b00000101; // Set the prescalar to 1024
  TIMSK1  = 0x05;       // enable timer overflow interrupt
  interrupts();         // enable all interrupts


  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);

  Serial.begin(9600);
}

//Interrupt Timer1
ISR(TIMER1_OVF_vect) {
  TCNT1 = 64753; // Set preload timer
  getSensorMeas();
  printData();
}

void computePRBS(){
  temp = data[1]^data[2]^data[3]^data[7]; //output bit 2,3,4,8 will perform XOR
  for (j = 7; j >= 1; j--){ //shift bit
    prbs[j] = data[j-1];
  }
  prbs[0] = temp; //Fill register data with the next PRBS
  for (k = 0; k <= 7; k++){
    data[k] = prbs[k];
  }
}

void printData () {
  if(cnt < 256 && stop_ == 0){
    computePRBS();
    Serial.print(pwm_prbs);
    Serial.print("\t ");      
    Serial.print(voltage);
    Serial.print("\t");
    if (data[0] == 1){
      pwm_prbs = 50; // prbs high
      Serial.println("2.14");
    }
    else{
      pwm_prbs = 20; // prbs low
      Serial.println("0");
    }
  }
  else if(cnt == 2001 && stop_ == 0) {
    pwm_prbs = 0;
    Serial.println("PRBS Done");
    cnt = 257;
    stop_ = 1;
  }
}

void getSensorMeas()
{
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  voltage = sensorValue * (25.22 / 1023.0);
}

void loop() {
  analogWrite(6,pwm_prbs);
}  
