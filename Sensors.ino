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
  if(temperatureModulo > maxTemp ){
    maxTemp = temperatureModulo;
  }
  else if(temperatureModulo < minTemp){
    minTemp = temperatureModulo;
  }

  

  //-----------------------------------------------------------
  debugln( Serial.printf("Temperature Outside: %f °C\n", temperatureModulo) );
  debugln(Serial.printf("Humidity Outside: %f %%\n", humidityModulo));
  

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
  debugln(  Serial.printf("Temperature Estufa: %f °C\n", temperatureEstufa)); 
  debugln(  Serial.printf("Humidity Estufa: %f %%\n", humidityEstufa));

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
  delay(100);
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

  // Use strftime to format in one String
  strftime(formattedTime, sizeof(formattedTime), "%H:%M:%S", ptm);

}
void muxReadings(){
  //MUX Readings
  for (int channel = 0; channel < 8; channel++) {
    //Chanel 0,1 and 3 are avaiable
    selectChannel(channel);  // Seleciona o canal do MUX

    //Varistor Temperature
    if (channel == 2) {
      leitura = 0;
      leitura = readPin();
      Vout = Vin * ((float)(media) / 1023.0);  // calculo de V0 e leitura de A0
      Rout = (Rt * Vout / (Vin - Vout));       // calculo de Rout
      TempKelvin = (beta / log(Rout / Rinf));  // calculo da temp. em Kelvins
      TempCelsius = TempKelvin - 273.15;       // calculo da temp. em Celsius
      debug("Temperatura em Celsius: ");       // imprime no monitor serial
      debugln(TempCelsius);                    // imprime temperatura Celsius

    }

    else if (channel == 4) {
      //Have a 10k resistor between pin and 3.3v
      //Soil Moisture Resistive sensor
      //wILL HAVE TO CHANGE TYPE OF SENSOR
      leitura = 0;
      leitura = readPin();
      soilUmUm = map(leitura, 0, 1023, 0, 100);
      debug("Umidade Resistiva do Solo 1 em : ");
      debug(soilUmUm);
      debugln("%");
    }

    else if (channel == 5) {
      //Have a 10k resistor between pin and 3.3v
      //Luminosity Reading
      leitura = 0;
      leitura = readPin();
      lumPC = map(leitura, 0, 1023, 100, 0);
      debugln();
      debug("Luminosidade em : ");
      debug(lumPC);
      debugln("%");

    }

    else if (channel == 6) {

      //Capacitive Sensor
      //As the soil gets wetter, the output value decreases
      //Configure your on values by tests
      //Internet Values : < 277 is too wet 
      //                  277 – 380 is the target range 
      //                  > 380 is too dry
      leitura = 0;
      leitura = readPin();
      Serial.println();
      Serial.print("esse é o valor da leitura média: ");
      Serial.print(leitura);
      Serial.println('----------');
      soilCap1 = map(leitura, 327, 600, 100, 0);
      //Valor de leitura com solo seco ~600
      //Valor de leitura com solo umido superficialmente ~400
      //Valor com solo umido maximo 350
      //Valor com àgua 327
      //Observed a variation of 10%+ from just irrigated soil
      debugln();
      debug("Capacity Humidity Reading : ");
      debug(soilCap1);
      debugln("%");

    } else if (channel == 7) {

      //Capacitive Sensor
      leitura = 0;
      leitura = readPin();
      soilCap2 = map(leitura, 327, 600, 100, 0);
      debugln();
      debug("Capacity Humidity Reading : ");
      debug(soilCap2);
      debugln("%");
    }
  }
}