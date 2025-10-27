void shiftData(int number) {

  // Serial.print("numero recebido pelo shiftData Inteiro: ");
  // Serial.println(number);
  // Serial.print("numero recebido pelo shiftData Binario: ");
  // Serial.println(number, BIN);

  int invertedNumber = ~number & 0xFF;
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, invertedNumber);  // Send the binary number that turns that rele on
  digitalWrite(latchPin, HIGH);
  debugln("Escrito no shift Register : ");
  Serial.println(invertedNumber,BIN);

  //Epooch Atualization
  epochstartCycleTime = epochTime;
  lastShiftData = number;

}


void fanControl(bool command) {

  int newState;

  if (command) {
    // Liga o bit 7 (bit mais significativo) sem alterar os outros bits
    newState = lastState | FAN_MASK;
    fansOn = true;
  } else {
    // Desliga o bit 7 (bit mais significativo) sem alterar os outros bits
    newState = lastState & ~FAN_MASK;
    fansOn = false;
  }

  shiftData(newState);
  lastState = newState;  // Atualiza o estado global

}

void relayControl(int relayHexNum){

  int newState;

  if (fansOn) {
    newState = relayHexNum | FAN_MASK;
  } else {
    newState = relayHexNum & ~FAN_MASK;
  }

  shiftData(newState);  
  lastState = newState; 

}

