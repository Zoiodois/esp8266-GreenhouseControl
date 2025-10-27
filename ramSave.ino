//Method to add readings to the Average Storage
void addReading() {

  Dados dadosDeLeitura;

  dadosDeLeitura.temperatureExterna = temperatureExterna;
  dadosDeLeitura.pressaoExterna = pressaoExterna;
  dadosDeLeitura.TempCelsius = TempCelsius;
  dadosDeLeitura.soilUmUm = soilUmUm;
  dadosDeLeitura.lumPC = lumPC;
  dadosDeLeitura.soilCap1 = soilCap1;
  dadosDeLeitura.soilCap2 = soilCap2;
  dadosDeLeitura.temperatureEstufa = temperatureEstufa;
  dadosDeLeitura.humidityEstufa = humidityEstufa;
  dadosDeLeitura.minTemp = minTemp;
  dadosDeLeitura.maxTemp = maxTemp;

  readings[currentIndex] = dadosDeLeitura;
  currentIndex++;

  // Verifique se completamos um ciclo de 60 leituras
  if (currentIndex >= numReadings) {
    // Calcule a média das leituras
    calculateAverages();
    // Reinicialize o índice para a próxima rodada de leituras
    currentIndex = 0;
  }
}

void calculateAverages() {

  uint32_t totaltemperatureExterna = 0;
  uint32_t totalpressaoExterna = 0;
  uint32_t totalTempCelsius = 0;
  uint32_t totalSoilUmUm = 0;
  uint32_t totalLumPC = 0;
  uint32_t totalSoilCap1 = 0;
  uint32_t totalSoilCap2 = 0;
  uint32_t totalTemperatureEstufa = 0;
  uint32_t totalHumidityEstufa = 0;
  uint8_t holdMinTemp = 0;
  uint8_t holdMaxTemp = 0;
  uint8_t tempMaxTemp = 0;
  uint8_t tempMinTemp = 80;

  for (uint8_t i = 0; i < numReadings; i++) {
    totaltemperatureExterna += readings[i].temperatureExterna;
    totalpressaoExterna += readings[i].pressaoExterna;
    totalTempCelsius += readings[i].TempCelsius;
    totalSoilUmUm += readings[i].soilUmUm;
    totalLumPC += readings[i].lumPC;
    totalSoilCap1 += readings[i].soilCap1;
    totalSoilCap2 += readings[i].soilCap2;
    totalTemperatureEstufa += readings[i].temperatureEstufa;
    totalHumidityEstufa += readings[i].humidityEstufa;
    holdMaxTemp = readings[i].maxTemp;
    holdMinTemp = readings[i].minTemp;

    if (holdMaxTemp > tempMaxTemp) {
      tempMaxTemp = holdMaxTemp;
    }

    if (holdMinTemp < tempMinTemp) {
      tempMinTemp = holdMinTemp;
    }
  }

  Dados averageReading;

  // updating the Fix Values
  averageReading.epochTime = epochTime;
  averageReading.temperatureExterna = totaltemperatureExterna / numReadings;
  averageReading.pressaoExterna = totalpressaoExterna / numReadings;
  averageReading.TempCelsius = totalTempCelsius / numReadings;
  averageReading.soilUmUm = totalSoilUmUm / numReadings;
  averageReading.lumPC = totalLumPC / numReadings;
  averageReading.soilCap1 = totalSoilCap1 / numReadings;
  averageReading.soilCap2 = totalSoilCap2 / numReadings;
  averageReading.temperatureEstufa = totalTemperatureEstufa / numReadings;
  averageReading.humidityEstufa = totalHumidityEstufa / numReadings;
  averageReading.minTemp = tempMinTemp;
  averageReading.maxTemp = tempMaxTemp;

  uint8_t tempSoilUmUm = averageReading.soilUmUm;
  uint8_t tempsoilCap1 = averageReading.soilCap1;
  uint8_t tempsoilCap2 = averageReading.soilCap2;



  //Reset Temperature data
  maxTemp = 0;
  minTemp = 80;

  averageStorage.push_back(averageReading);
}

void loadLastIrEpochs() {
  
  for (int i = 0; i < NUM_RELES; i++) {
    unsigned int endereco = i * sizeof(unsigned long); // Calcula o endereço base do relé
    unsigned long oldEpoch;
    EEPROM.get(endereco, oldEpoch);
    Serial.printf("Relé %d - Último Epoch salvo: %lu,no endereço %d\n", i, oldEpoch, endereco);
    lastIrrigatedEpoch[i] = oldEpoch;

  }

}

void eepromSaveLastIrEpochs(int rele, unsigned long epochAtual) {

  
  
  unsigned int endereco = rele * sizeof(unsigned long); // Calcula o endereço base do relé -> O rele 0 terá problemas
  EEPROM.put(endereco, epochAtual);
  EEPROM.commit();
  Serial.printf("Relé %d - Tempo Epoch salvo: %lu, no endereço: %d\n", rele, epochAtual, endereco);
 
}


/*
//Function not in use, but working
void showDatainRAM() {
  Serial.println("Show data in RAM***");
  for (size_t i = 0; i < dadosArmazenados.size(); ++i) {
    const auto &dados = dadosArmazenados[i];
    Serial.println("Epoch Time: " + String(dados.epochTime));
    Serial.println("Temperature Modulo: " + String(dados.temperatureExterna));
    Serial.println("Humidity Modulo: " + String(dados.pressaoExterna));
    Serial.println("Temp Celsius: " + String(dados.TempCelsius));
    Serial.println("Soil UmUm: " + String(dados.soilUmUm));
    Serial.println("Lum PC: " + String(dados.lumPC));
    Serial.println("Soil Cap1: " + String(dados.soilCap1));
    Serial.println("Soil Cap2: " + String(dados.soilCap2));
    Serial.println("Temperature Estufa: " + String(dados.temperatureEstufa));
    Serial.println("Humidity Estufa: " + String(dados.humidityEstufa));
    Serial.println("Maxima Temperatura Externa: " + String(dados.maxTemp));
    Serial.println("Minima Temperatura Externa: " + String(dados.minTemp));
  }
}
*/
