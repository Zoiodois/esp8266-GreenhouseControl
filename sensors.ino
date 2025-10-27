void Load_DHT11_Data() {

  //Greenhouse Readings
  //-----------------------------------------------------------
  const uint8_t maxTentativas = 100;
  uint8_t tentativas = 0;
  bool leituraBemSucedida = false;
  int temperature;
  int humidity;


  while (tentativas < maxTentativas && !leituraBemSucedida && temperature != 255) {
    temperature = dhtEstufa.readTemperature();  // Celsius
    humidity = dhtEstufa.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      leituraBemSucedida = true;
      temperatureEstufa = static_cast<int>(temperature);
      humidityEstufa = static_cast<int>(humidity);
    } else {
      tentativas++;
      delay(100);  // Pequena espera entre as tentativas
    }
  }
  // if (!leituraBemSucedida || temperatureEstufa == 255) {
  //   //debugln("Failed to read from DHT Estufa sensor after multiple attempts!");
  //   temperatureEstufa = 50;
  //   humidityEstufa = 50;
  // }

}

//Leitura Sensores Resistivos
uint16_t readPin() {
  media = 0;
  somatoria = 0;
  analogRead(A0);
  for (uint8_t i = 1; i <= NUMERO_AMOSTRAS; i++) {
    int r;
    r = analogRead(A0);
    somatoria = somatoria + r;
  }
  media = somatoria / NUMERO_AMOSTRAS;
  return media;
}

//Mux port selection code
void selectChannel(uint8_t channel) {

  bool muxA = bitRead(channel, 0);
  bool muxB = bitRead(channel, 1);
  bool muxC = bitRead(channel, 2);
 // bool muxD = bitRead(channel, 3);

  // Configura cada pino do MUX
  digitalWrite(MUXA, muxA);
  digitalWrite(MUXB, muxB);
  digitalWrite(MUXC, muxC);
  //digitalWrite(MUXD, muxD);
}


void muxReadings() {
  //UPDATE
  //16 channles

  //MUX Readings
  for (uint8_t channel = 0; channel < 7; channel++) {  //Only the number in use to avoid time waste
    //Chanel 0: Temperatura do Módulo
    //Chanel 1: Luminosidade
    //Chanel 2: Check Ir Cant1
    //Chanel 3: Check Ir Cant2
    //Chanel 4: Check Ir Cant3
    //Chanel 5: Sensor solo Cap 1
    //Chanel 6: Sensor solo Cap 2
    //Channek 7: Resistive Soil Sensor
    selectChannel(channel);  // Seleciona o canal do MUX

    switch (channel) {
      case 0:
        //Módulo de Temperatura

        leitura = 0;
        leitura = readPin();
        // Lê o valor do ADC (de 0 a 1023)
        Vout = (leitura / 1023.0) * Vin;  // Converte para tensãoTempCelsius
        // Calcula a resistência do termistor
        R_thermistor = R_fixed * (Vin - Vout) / Vout;
        // Aplica a fórmula de Steinhart-Hart para calcular a temperatura em Kelvin
        tempK = 1.0 / (A + B * log(R_thermistor) + C * pow(log(R_thermistor), 3));
        // Converte a temperatura para Celsius
        tempC = tempK - 273.15;
        TempCelsius = tempC;
        // TempCelsius = 50;

        debug("Temperatura em Celsius: ");  // imprime no monitor serial
        debugln(TempCelsius);               // imprime temperatura Celsius
        break;

      case 1:

        leitura = 0;
        leitura = readPin();
        //Have a 10k resistor between pin and 3.3v
        //Luminosity Reading
        lumPC = map(leitura, 0, 1023, 0, 100);

        debugln();
        debug("Luminosidade em : ");
        debug(lumPC);
        debugln("%");

        break;
      case 2:

        //Check if cant 1 Was Irrigated


        break;

      case 3:  //Luminisity readings
         //Check if cant 2 Was Irrigated


        break;

      case 4:  //Resistive sensor
        //Check if cant 3 Was Irrigated
        break;


      case 5:

        leitura = 0;
        leitura = readPin();
        //Capacitive Sensor
        //As the soil gets wetter, the output value decreases
        //Configure your on values by tests
        //Internet Values : < 277 is too wet
        //                  277 – 380 is the target range
        //                  > 380 is too dry
        soilCap1 = map(leitura, 180, 580, 100, 0);
        soilCap1 = 50;
        //soilCap1 = leitura;
        //Dados para o sensor resistivo
        //Valor de leitura com solo seco ~1024
        //Valor de leitura com solo umido superficialmente ~450
        //500 o solo já esta em mucha permanente. Uma irrigação causou escorrimento pelos drenos
        //Valor com solo umido maximo 330
        //Valor com àgua 327
        //no lodinho estabilizou em 460... ?
        //Estabilizou em capacidade de campo em torno de 550

        //Observed a variation of 10%+ from just irrigated soil
        debugln();
        debug("sensor fio azul (pino 6) em : ");
        //debug("Capacity Humidity Reading : ");
        debug(soilCap1);
        debugln("%");
        break;
      case 6:

        leitura = 0;
        leitura = readPin();
        soilCap2 = map(leitura, 180, 580, 100, 0);
        soilCap2 = 50;

        //Observed a variation of 10%+ from just irrigated soil
        debugln();
        debug("sensor fio azul (pino 6) em : ");
        //debug("Capacity Humidity Reading : ");
        debug(soilCap2);
        debugln("%");
        break;

      case 7:  //Resistive sensor
        leitura = 0;
        leitura = readPin();
        //Soil Moisture Resistive sensor
        //after mapping the data. Highest 600. Lowest 350
        soilUmUm = map(leitura, 100, 650, 100, 0);
        soilUmUm = leitura;
        soilUmUm = 50;
        break;
    }
  }
}

void aht10read() {

  //Loop
  sensors_event_t humidity, temp;

  bool leituraValida = false;
  while (!leituraValida) {
    aht.getEvent(&humidity, &temp);  // Preenche os objetos temp e humidity com dados atualizados

    temperatureExterna = temp.temperature;
    pressaoExterna = humidity.relative_humidity;

    // Verifica se as leituras estão dentro de um intervalo aceitável
    if (temperatureExterna >= -10 && temperatureExterna <= 110 && pressaoExterna >= 0 && pressaoExterna <= 300) {
      leituraValida = true;  // Sai do loop se os valores forem válidos
  
      } else {
      debugln(F("Leitura inválida. Tentando novamente..."));
      
    }
  }

  aht.getEvent(&humidity, &temp);  // populate temp and humidity objects with fresh data

  temperatureExterna = temp.temperature;
  pressaoExterna = humidity.relative_humidity;

      if (temperatureExterna > maxTemp) {
      maxTemp = temperatureExterna;
    }

    if (temperatureExterna < minTemp) {
      minTemp = temperatureExterna;
    }

  debug("temperatura Estufa em: ");
  debugln(temperatureExterna);

  debug("Umidade Estufa em :");
  debugln(pressaoExterna);
}


// void bmpReading() {

//   sensors_event_t temp_event, pressure_event;
//   bmp_temp->getEvent(&temp_event);
//   bmp_pressure->getEvent(&pressure_event);

//   temperatureExterna = temp_event.temperature;
//   pressaoExterna = pressure_event.pressure;

//   debug("temperatureExterna em: ");
//   debugln(temperatureExterna);

//   debug("pressaoExterna em :");
//   debugln(pressaoExterna);
// }
