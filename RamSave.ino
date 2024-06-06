// Struct definition to save all data formats
struct Dados {
  time_t epochTime;
  //String currentDate;
  //String formattedTime;
  float temperatureModulo;
  float humidityModulo;
  float TempCelsius;
  float soilUmUm;
  float lumPC;
  float soilCap1;
  float soilCap2;
  float temperatureEstufa;
  float humidityEstufa;
  float maxTemp;
  float minTemp;
};

//Method for saving values in RAM
void saveValues() {

  debugln("Saving Data on Ram");

  Dados dadosEstruturados;
  dadosEstruturados.epochTime = epochTime;
  dadosEstruturados.temperatureModulo = temperatureModulo;
  dadosEstruturados.humidityModulo = humidityModulo;
  dadosEstruturados.TempCelsius = TempCelsius;
  dadosEstruturados.soilUmUm = soilUmUm;
  dadosEstruturados.lumPC = lumPC;
  dadosEstruturados.soilCap1 = soilCap1;
  dadosEstruturados.soilCap2 = soilCap2;
  dadosEstruturados.temperatureEstufa = temperatureEstufa;
  dadosEstruturados.humidityEstufa = humidityEstufa;
  dadosEstruturados.minTemp = minTemp;
  dadosEstruturados.maxTemp = maxTemp;

  // Adicione o conjunto de dados ao vetor dadosArmazenados
  dadosArmazenados.push_back(dadosEstruturados);
}

void calculateSubsetAveragesAndStore() {

  size_t dataStartIndex = numAverages;  // Ignorar as médias já calculadas
  size_t numElements = dadosArmazenados.size() - dataStartIndex;

  if (numElements == 0) {
    // Não há dados novos para processar
    return;
  }

  float totalEpoochTime = 0;
  float totalTemperatureModulo = 0;
  float totalHumidityModulo = 0;
  float totalTempCelsius = 0;
  float totalSoilUmUm = 0;
  float totalLumPC = 0;
  float totalSoilCap1 = 0;
  float totalSoilCap2 = 0;
  float totalTemperatureEstufa = 0;
  float totalHumidityEstufa = 0;
  float totalMaxTemp = 0;
  float totalMinTemp = 80;

  for (size_t j = dataStartIndex; j < dadosArmazenados.size(); ++j) {
    totalEpoochTime += dadosArmazenados[j].epochTime;
    totalTemperatureModulo += dadosArmazenados[j].temperatureModulo;
    totalHumidityModulo += dadosArmazenados[j].humidityModulo;
    totalTempCelsius += dadosArmazenados[j].TempCelsius;
    totalSoilUmUm += dadosArmazenados[j].soilUmUm;
    totalLumPC += dadosArmazenados[j].lumPC;
    totalSoilCap1 += dadosArmazenados[j].soilCap1;
    totalSoilCap2 += dadosArmazenados[j].soilCap2;
    totalTemperatureEstufa += dadosArmazenados[j].temperatureEstufa;
    totalHumidityEstufa += dadosArmazenados[j].humidityEstufa;
    totalMaxTemp = max(totalMaxTemp, dadosArmazenados[j].maxTemp);
    totalMinTemp = min(totalMinTemp, dadosArmazenados[j].minTemp);
  }

  Dados averageData;
  averageData.epochTime = totalEpoochTime / numElements;
  averageData.temperatureModulo = totalTemperatureModulo / numElements;
  averageData.humidityModulo = totalHumidityModulo / numElements;
  averageData.TempCelsius = totalTempCelsius / numElements;
  averageData.soilUmUm = totalSoilUmUm / numElements;
  averageData.lumPC = totalLumPC / numElements;
  averageData.soilCap1 = totalSoilCap1 / numElements;
  averageData.soilCap2 = totalSoilCap2 / numElements;
  averageData.temperatureEstufa = totalTemperatureEstufa / numElements;
  averageData.humidityEstufa = totalHumidityEstufa / numElements;
  averageData.maxTemp = totalMaxTemp;
  averageData.minTemp = totalMinTemp;

  // Incrementar o número de médias calculadas
  numAverages++;
  Serial.print("Número de Médias no arquivo: ");
  Serial.println(numAverages);

  // Remover os dados processados
  dadosArmazenados.erase(dadosArmazenados.begin() + dataStartIndex, dadosArmazenados.end());

  // Inserir a nova média no final
  dadosArmazenados.push_back(averageData);

  lastAverage = millis();  // Atualiza o último tempo de cálculo da média
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
