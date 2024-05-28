void Load_DHT11_Data() {

  //Leitura Modulo
  //-----------------------------------------------------------
  temperatureModulo = dhtModulo.readTemperature();  //Celsius
  humidityModulo = dhtModulo.readHumidity();
  //-----------------------------------------------------------
  // Check if any reads failed.
  if (isnan(temperatureModulo) || isnan(humidityModulo)) {
    debugln("Failed to read from DHT Modulo sensor!");
    temperatureModulo = 0.0;
    humidityModulo = 0.0;
  }
  //-----------------------------------------------------------
  Serial.printf("Temperature Outside: %f °C\n", temperatureModulo);
  Serial.printf("Humidity Outside: %f %%\n", humidityModulo);

  //Greenhouse Readings
  //-----------------------------------------------------------
  const int maxTentativas = 10;
  int tentativas = 0;
  bool leituraBemSucedida = false;


  while (tentativas < maxTentativas && !leituraBemSucedida) {
    temperatureEstufa = dhtEstufa.readTemperature();  // Celsius
    humidityEstufa = dhtEstufa.readHumidity();

    if (!isnan(temperatureEstufa) && !isnan(humidityEstufa)) {
      leituraBemSucedida = true;
    } else {
      tentativas++;
      delay(100);  // Pequena espera entre as tentativas
    }
  }
  if (!leituraBemSucedida) {
    debugln("Failed to read from DHT Estufa sensor after multiple attempts!");
    temperatureEstufa = 0.0;
    humidityEstufa = 0.0;
  }

  //-----------------------------------------------------------
  Serial.printf("Temperature Estufa: %f °C\n", temperatureEstufa);
  Serial.printf("Humidity Estufa: %f %%\n", humidityEstufa);
}


//Leitura Sensores Resistivos
int readPin() {

  media = 0;
  somatoria = 0;
  analogRead(A0);
  for (int i = 1; i <= NUMERO_AMOSTRAS; i++) {
    int r;
    r = analogRead(A0);
    somatoria = somatoria + r;
  }
  media = somatoria / NUMERO_AMOSTRAS;
  delay(1000);
  return media;
}

//Mux port selection code
void selectChannel(int channel) {
  digitalWrite(MUXA, bitRead(channel, 0));
  digitalWrite(MUXB, bitRead(channel, 1));
  digitalWrite(MUXC, bitRead(channel, 2));
}

//Calculate Epoch Time Dependet Variables
void epochCalculate(time_t epochTime) {
  //Hey delete me
  //Inserir data e hora
  debug("Epoch Time: ");
  debugln(epochTime);

  //Get a time structure
  struct tm *ptm = gmtime((time_t *)&epochTime);

  monthDay = ptm->tm_mday;
  debug("Month day: ");
  debugln(monthDay);

  currentMonth = ptm->tm_mon + 1;
  debug("Month: ");
  debugln(currentMonth);

  currentYear = ptm->tm_year + 1900;
  debug("Year: ");
  debugln(currentYear);

  //Print complete date:
  currentDate = String(currentMonth) + "-" + String(monthDay) + "-" + String(currentYear);
  debug("Current date: ");
  debugln(currentDate);

  formattedTime = timeClient.getFormattedTime();
  debug("Formatted Time: ");
  debugln(formattedTime);
}