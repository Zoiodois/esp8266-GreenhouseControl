// Struct definition to save all data formats
struct Dados {
  time_t epochTime;
  String currentDate;
  String formattedTime;
  float temperatureModulo;
  float humidityModulo;
  float TempCelsius;
  float soilUmUm;
  float lumPC;
  float soilCap1;
  float soilCap2;
  float temperatureEstufa;
  float humidityEstufa;
};

//Method for saving values in RAM
void saveValues() {
  debugln("Savind Data on Ram");

    Dados dadosEstruturados;
  dadosEstruturados.epochTime = epochTime;
  dadosEstruturados.currentDate = currentDate;
  dadosEstruturados.formattedTime = formattedTime;
  dadosEstruturados.temperatureModulo = temperatureModulo;
  dadosEstruturados.humidityModulo = humidityModulo;
  dadosEstruturados.TempCelsius = TempCelsius;
  dadosEstruturados.soilUmUm = soilUmUm;
  dadosEstruturados.lumPC = lumPC;
  dadosEstruturados.soilCap1 = soilCap1;
  dadosEstruturados.soilCap2 = soilCap2;
  dadosEstruturados.temperatureEstufa = temperatureEstufa;
  dadosEstruturados.humidityEstufa = humidityEstufa;

  // Adicione o conjunto de dados ao vetor dadosArmazenados
  dadosArmazenados.push_back(dadosEstruturados);
}

void showDatainRAM() {
  Serial.println("Show data in RAM***");
  for (size_t i = 0; i < dadosArmazenados.size(); ++i) {
    const auto &dados = dadosArmazenados[i];
    Serial.println("Epoch Time: " + String(dados.epochTime));
    Serial.println("Current Date: " + dados.currentDate);
    Serial.println("Formatted Time: " + dados.formattedTime);
    Serial.println("Temperature Modulo: " + String(dados.temperatureModulo));
    Serial.println("Humidity Modulo: " + String(dados.humidityModulo));
    Serial.println("Temp Celsius: " + String(dados.TempCelsius));
    Serial.println("Soil UmUm: " + String(dados.soilUmUm));
    Serial.println("Lum PC: " + String(dados.lumPC));
    Serial.println("Soil Cap1: " + String(dados.soilCap1));
    Serial.println("Soil Cap2: " + String(dados.soilCap2));
    Serial.println("Temperature Estufa: " + String(dados.temperatureEstufa));
    Serial.println("Humidity Estufa: " + String(dados.humidityEstufa));
  }
}

void sendRamData() {
   // Variável para armazenar o índice do início de um novo conjunto de dados
  int startIdx = 0;
  unsigned long lastAppendTime = 0;
  const unsigned long appendInterval = 5000;
  // Iterar sobre o vetor de dados
  for (size_t i = 0; i < dadosArmazenados.size(); ++i) {

    const auto &dados = dadosArmazenados[i];

    //Coping the sending data method in here
    bool ready = GSheet.ready();
    if (ready) {

      FirebaseJson response;
      FirebaseJson valueRange;

      debugln("\nAppend RAM VALUES to spreadsheet ...");
      debugln("*********");

      //Dados em linha
      valueRange.add("majorDimension", "ROWS");
      //rawdate
      valueRange.set("values/[0]/[0]", dados.epochTime);
      //data
      valueRange.set("values/[0]/[1]", dados.currentDate);
      //hora
      valueRange.set("values/[0]/[2]", dados.formattedTime);
      //Temperatura Externa
      valueRange.set("values/[0]/[3]", dados.temperatureModulo);
      //Umidade do AR Externo
      valueRange.set("values/[0]/[4]", dados.humidityModulo);
      //Temperatura Termistor
      valueRange.set("values/[0]/[5]", dados.TempCelsius);
      //Umidade Solo 1 Resistivo Externo Mudas
      valueRange.set("values/[0]/[6]", dados.soilUmUm);
      //Luminosidade
      valueRange.set("values/[0]/[7]", dados.lumPC);
      //Umidade Solo Capacitivo 1 - Provavelmente Estufas
      valueRange.set("values/[0]/[8]", dados.soilCap1);
      //Umidade Solo Capacitivo 2
      valueRange.set("values/[0]/[9]", dados.soilCap2);
      //Temperatura Estufa
      valueRange.set("values/[0]/[10]", dados.temperatureEstufa);
      //Umidade do AR Estufa
      valueRange.set("values/[0]/[11]", dados.humidityEstufa);


      bool success = GSheet.values.append(&response /* returned response */, "1ghTXvGyQEu-iUEI7eEYKt2wTln5SJcn2WORhD5_FF_o" /* spreadsheet Id to append */, sheetAppend + "A2" /* range to append */, &valueRange /* data range to append */);
      if (success) {

        response.toString(Serial, true);
        dadosArmazenados.erase(dadosArmazenados.begin() + i);

      } else {

        debugln(GSheet.errorReason());
        debugln();
      }
      debugln(ESP.getFreeHeap());

      delay(15000); //Change this delay for a milis() method
    } else {
      debugln("GSheet not ready or waiting for next interval");
    }
  }
}