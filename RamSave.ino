//Method to add readings to the Average Storage
void addReading() {

  Dados dadosDeLeitura;

  dadosDeLeitura.temperatureModulo = temperatureModulo;
  dadosDeLeitura.humidityModulo = humidityModulo;
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

  uint8_t totalTemperatureModulo = 0;
  uint8_t totalHumidityModulo = 0;
  uint8_t totalTempCelsius = 0;
  uint8_t totalSoilUmUm = 0;
  uint8_t totalLumPC = 0;
  uint8_t totalSoilCap1 = 0;
  uint8_t totalSoilCap2 = 0;
  uint8_t totalTemperatureEstufa = 0;
  uint8_t totalHumidityEstufa = 0;
  uint8_t holdMinTemp = 0;
  uint8_t holdMaxTemp = 0;
  uint8_t tempMaxTemp = 0;
  uint8_t tempMinTemp = 80;

  for (uint8_t i = 0; i < numReadings; i++) {
    totalTemperatureModulo += readings[i].temperatureModulo;
    totalHumidityModulo += readings[i].humidityModulo;
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
  //Time data will be avaiable through Global Var

  averageReading.epochTime = epochTime;
  averageReading.temperatureModulo = totalTemperatureModulo / numReadings;
  averageReading.humidityModulo = totalHumidityModulo / numReadings;
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
  //updating the Fix Values
  if (tempSoilUmUm > 100) {
    AveRes1 = 100;
  } else {
    AveRes1 = tempSoilUmUm;
  }

  if (tempsoilCap1 > 100) {
    AveCap1 = 100;
  } else {
    AveCap1 = tempsoilCap1;
  }

  if (tempsoilCap2 > 100) {
    AveCap2 = 100;
  } else {
    AveCap2 = tempsoilCap2;
  }

  //Reset Temperature data
  maxTemp = 0;
  minTemp = 80;

  averageStorage.push_back(averageReading);
}


/*
//Function not in use, but working
void showDatainRAM() {
  Serial.println("Show data in RAM***");
  for (size_t i = 0; i < dadosArmazenados.size(); ++i) {
    const auto &dados = dadosArmazenados[i];
    Serial.println("Epoch Time: " + String(dados.epochTime));
    Serial.println("Temperature Modulo: " + String(dados.temperatureModulo));
    Serial.println("Humidity Modulo: " + String(dados.humidityModulo));
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
