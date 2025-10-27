//Average Sender Loop
void sendAndClearData() {

  for (auto it = averageStorage.begin(); it != averageStorage.end();) {
    if (sendLaravelAverageData(*it)) {
      it = averageStorage.erase(it);  // Remove o elemento e avança o iterador
    } else {
      ++it;  // Apenas avança o iterador
    }
  }
}


void sendLaravelLiveData() {


  char relaysString[MAX_TAREFAS * 4 + 2];                           // Buffer para a string
  gerarStringReleEnfileirados(relaysString, sizeof(relaysString));  // Chamada corrigida
  debugln(relaysString);

  WiFiClientSecure wifiClient;
  // Cria o cliente WiFi HTTPS
  // Ignorar verificação de certificado (não recomendado para produção)
  wifiClient.setInsecure();

  HTTPClient https;
  https.begin(wifiClient, liveDatApi);
  https.addHeader("Authorization", laravelBarear);
  https.addHeader("Accept", "application/json");
  https.addHeader("Content-Type", "application/json");

  StaticJsonDocument<1024> jsonDoc;
  jsonDoc["UrGHouse"] = String(humidityEstufa);
  jsonDoc["tempGhouse"] = String(temperatureEstufa);
  jsonDoc["moduleTemp"] = String(TempCelsius);  //add to laravel
  jsonDoc["lum"] = String(lumPC);
  jsonDoc["sen1"] = String(soilCap1);
  jsonDoc["sen2"] = String(soilCap2);
  jsonDoc["sen3"] = String(soilUmUm);
  jsonDoc["tempExternal"] = String(temperatureExterna);
  jsonDoc["uRExternal"] = String(pressaoExterna);
  jsonDoc["maxTemp"] = String(maxTemp);
  jsonDoc["minTemp"] = String(minTemp);
  jsonDoc["queue"] = String(relaysString);
  jsonDoc["lastShiftData"] = String(lastShiftData);
  jsonDoc["lastIrGHEpooch"] = String(lastIrrigatedEpoch[0]);
  jsonDoc["lastIr1Epoch"] = String(lastIrrigatedEpoch[1]);
  jsonDoc["lastIr2Epoch"] = String(lastIrrigatedEpoch[2]);
  jsonDoc["lastIr3Epoch"] = String(lastIrrigatedEpoch[3]);
  jsonDoc["lastIr4Epoch"] = String(lastIrrigatedEpoch[4]);
  jsonDoc["lastIr5Epoch"] = String(lastIrrigatedEpoch[5]);
  jsonDoc["lastCycleStart"] = String(epochstartCycleTime);

  // Converter para string
  String jsonData;
  serializeJson(jsonDoc, jsonData);

  debugln(jsonData);

  // Envia o GET request e obtém a resposta
  int httpResponseCode = https.POST(jsonData);
  // Verifica o código de resposta
  if (httpResponseCode > 0) {
    debug("Resposta do servidor: ");
    debugln(httpResponseCode);

    if (httpResponseCode == 200) {
      String response = https.getString();  // Lê a resposta
      debugln("Resposta: " + response);

      // Exibe os cabeçalhos de resposta
      debugln("Cabeçalhos:");
      for (int i = 0; i < https.headers(); i++) {
        debugln(https.headerName(i) + ": " + https.header(i));
      }

    } else {
      debug("Erro no envio dos dados: ");
      debugln(https.errorToString(httpResponseCode).c_str());
      // Exibe os cabeçalhos de resposta
      Serial.println("Cabeçalhos:");
      for (int i = 0; i < https.headers(); i++) {
        Serial.println(https.headerName(i) + ": " + https.header(i));
      }
    }
  } else {
    debug("Erro na requisição: ");
    debugln(https.errorToString(httpResponseCode).c_str());
    // Exibe os cabeçalhos de resposta
    Serial.println("Cabeçalhos:");
    for (int i = 0; i < https.headers(); i++) {
      Serial.println(https.headerName(i) + ": " + https.header(i));
    }
  }

  // Encerra a conexão
  https.end();
}

bool sendLaravelAverageData(const Dados& dados) {

  WiFiClientSecure wifiClient;
  // Cria o cliente WiFi HTTPS
  // Ignorar verificação de certificado (não recomendado para produção)
  wifiClient.setInsecure();
  HTTPClient https;
  https.begin(wifiClient, postAvgAPI);
  https.addHeader("Authorization", laravelBarear);
  https.addHeader("Accept", "application/json");
  https.addHeader("Content-Type", "application/json");

  StaticJsonDocument<1024> jsonDoc;

  jsonDoc["epoochTime"] = String(dados.epochTime);
  jsonDoc["tempExternal"] = String(dados.temperatureExterna);
  jsonDoc["uRExternal"] = String(dados.pressaoExterna);
  jsonDoc["moduleTemp"] = String(dados.TempCelsius);  //add to laravel
  jsonDoc["lum"] = String(dados.lumPC);
  jsonDoc["sen1"] = String(dados.soilCap1);
  jsonDoc["sen2"] = String(dados.soilCap2);
  jsonDoc["sen3"] = String(dados.soilUmUm);
  jsonDoc["tempGhouse"] = String(dados.temperatureEstufa);
  jsonDoc["UrGHouse"] = String(dados.humidityEstufa);
  jsonDoc["maxTemp"] = String(dados.maxTemp);
  jsonDoc["minTemp"] = String(dados.minTemp);



  // Converter para string
  String jsonData;
  serializeJson(jsonDoc, jsonData);

  Serial.println(jsonData);

  // Envia o GET request e obtém a resposta
  int httpResponseCode = https.POST(jsonData);
  // Verifica o código de resposta
  if (httpResponseCode > 0) {
    Serial.print("Resposta do servidor: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode == 200) {
      String response = https.getString();  // Lê a resposta
      Serial.println("Resposta: " + response);
      https.end();
      return true;

    } else {
      Serial.print("Erro no envio das Médias: ");
      Serial.println(https.errorToString(httpResponseCode).c_str());
      https.end();
      return false;
    }
  } else {
    Serial.print("Erro na requisição para médias: ");
    Serial.println(https.errorToString(httpResponseCode).c_str());
    https.end();
    return false;
  }

  // Encerra a conexão
  https.end();
}


//Overall AND Greenhouse Configuration Reciver
void receberConfigMasterLaravel() {

  debugln("Receber Dados  de Configuração Iniciando");
  debugln("Receber Dados  de Configuração Laravel Iniciando");

  if (millis() - mr > 15000) {
    mr = millis();
    debugln("--Recebendo dados Laravel--");

    WiFiClientSecure wifiClient;
    // Cria o cliente WiFi HTTPS
    // Ignorar verificação de certificado (não recomendado para produção)
    wifiClient.setInsecure();

    HTTPClient https;
    https.begin(wifiClient, configAPI);
    https.addHeader("Authorization", laravelBarear);
    https.addHeader("Accept", "application/json");
    https.addHeader("Content-Type", "application/json");

    int httpResponseCode = https.GET();
    debug("Resposta do servidor Laravel: ");
    debugln(httpResponseCode);

    if (httpResponseCode > 0) {  // Verifica o código de resposta

      if (httpResponseCode == HTTP_CODE_OK) {  // Se resposta for 200
        String payload = https.getString();
        debugln("Resposta do servidor Laravel:");
        debugln(payload);

        // Tamanho estimado para o buffer do JSON
        const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(16) + 400;
        DynamicJsonDocument doc(capacity);

        // Parseia a string JSON
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          debug("Erro ao parsear JSON: ");
          debugln(error.c_str());
          return;
        }

        // Acessa os valores do JSON
        readingInterval = doc["configs"]["tempoLeitura"];
        recivingDataInterval = doc["configs"]["tempoAttConfig"];
        greenhouseOn = doc["configs"]["greenhouseOn"];
        limiarEstufa = doc["configs"]["greenhouseLimit"];
        intervalOn = doc["configs"]["greenhouseTimerOn"];
        intervalOff = doc["configs"]["greenhouseTimerOff"];
        autoIr = doc["configs"]["autoIr"];
        tIrGH = doc["configs"]["IrGHtime"];
        betweenIrIntervalGH = doc["configs"]["IrGHInterval"];
        tIrCant1 = doc["configs"]["Ir1time"];
        betweenIrIntervalCant1 = doc["configs"]["Ir1Interval"];
        tIrCant2 = doc["configs"]["Ir2time"];
        betweenIrIntervalCant2 = doc["configs"]["Ir2Interval"];
        tIrCant3 = doc["configs"]["Ir3time"];
        betweenIrIntervalCant3 = doc["configs"]["Ir3Interval"];
        tIrCant4 = doc["configs"]["Ir4time"];
        betweenIrIntervalCant4 = doc["configs"]["Ir4Interval"];
        tIrCant5 = doc["configs"]["Ir5time"];
        betweenIrIntervalCant5 = doc["configs"]["Ir5Interval"];
        fanSystemOn = doc["configs"]["fans_system_on"];
        fansTreshold = doc["configs"]["fans_treshold"];

        // Reatribuição dos dados aos arrays
        tempoIrrigacao[0] = tIrGH;
        tempoIrrigacao[1] = tIrCant1;
        tempoIrrigacao[2] = tIrCant2;
        tempoIrrigacao[3] = tIrCant3;
        tempoIrrigacao[4] = tIrCant4;
        tempoIrrigacao[5] = tIrCant5;

        intervaloEntreIrrigacoes[0] = betweenIrIntervalGH;
        intervaloEntreIrrigacoes[1] = betweenIrIntervalCant1;
        intervaloEntreIrrigacoes[2] = betweenIrIntervalCant2;
        intervaloEntreIrrigacoes[3] = betweenIrIntervalCant3;
        intervaloEntreIrrigacoes[4] = betweenIrIntervalCant4;
        intervaloEntreIrrigacoes[5] = betweenIrIntervalCant5;



        debugln("New Config Values");
        debug("Reading Interval:");
        debugln(readingInterval);
        debug("Reciviong data Interval:");
        debugln(recivingDataInterval);
        debug("GHouse status:");
        debugln(greenhouseOn);
        debug("GHouse on timer:");
        debugln(intervalOn);
        debug("Auto IR status:");
        debugln(autoIr);
        debugln("New Time Values");
        for (int n = 0; n < 6; n++) {

          debug("Tempos para o Rele: ");
          debugln(n);
          debug("Tempo de Irrigação:");
          debugln(tempoIrrigacao[n]);
          debug("Intervalo de  Irrigação:");
          debugln(intervaloEntreIrrigacoes[n]);
        }

        debug("Fans System On/Off: ");
        debugln(fanSystemOn);
        debug("Fans Treshold Limit :");
        debugln(fansTreshold);


      }
    }

    https.end();
  }
}

bool unqueCommand(int externalIrReqID) {

  WiFiClientSecure wifiClientPatch;
  // Cria o cliente WiFi HTTPS
  // Ignorar verificação de certificado (não recomendado para produção)
  wifiClientPatch.setInsecure();

  HTTPClient httpsPatch;
  httpsPatch.begin(wifiClientPatch, queueRelay);
  httpsPatch.addHeader("Authorization", laravelBarear);
  httpsPatch.addHeader("Accept", "application/json");
  httpsPatch.addHeader("Content-Type", "application/json");

  StaticJsonDocument<1024> jsonDoc;
  jsonDoc["id"] = externalIrReqID;
  jsonDoc["processed"] = 1;

  // Converter para string
  String jsonData;
  serializeJson(jsonDoc, jsonData);

  debugln(jsonData);

  // Envia o GET request e obtém a resposta
  // int httpResponseCode = httpsPatch.PUT(jsonData);
  //  int httpResponseCode = httpsPatch.GET();
  //   debug("Código de resposta HTTP: ");
  //   debugln(httpResponseCode);
  int httpResponseCode = httpsPatch.PUT(jsonData);
  // Verifica o código de resposta
  debug("Resposta do servidor para PATCH COMMAND: ");
  debugln(httpResponseCode);


  if (httpResponseCode > 0) {
    debug("Resposta do servidor para PATCH COMMAND: ");
    debugln(httpResponseCode);
    String redirectUrl = httpsPatch.header("Location");
    Serial.println("Redirecionado para: " + redirectUrl);

    if (httpResponseCode == 200) {

      String response = httpsPatch.getString();  // Lê a resposta
      debugln("Resposta PATCH COMMAND: " + response);

      httpsPatch.end();
      externalIrReqID = -1;
      return true;

    } else {

      debug("Erro no envio dos dados PATCH COMMAND: ");
      debugln(httpsPatch.errorToString(httpResponseCode).c_str());
    }
  } else {
    debug("Erro na requisição: ");
    debugln(httpsPatch.errorToString(httpResponseCode).c_str());
  }

  // Encerra a conexão
  httpsPatch.end();
  return false;
}

bool reciveQueue() {

  // if (millis() - mr > 15000) {
  //   mr = millis();
  debugln("--Recebendo dados Queue do Laravel--");

  WiFiClientSecure wifiClient;
  // Cria o cliente WiFi HTTPS
  // Ignorar verificação de certificado (não recomendado para produção)
  wifiClient.setInsecure();

  HTTPClient https;
  https.begin(wifiClient, queueRelay);
  https.addHeader("Authorization", laravelBarear);
  https.addHeader("Accept", "application/json");
  https.addHeader("Content-Type", "application/json");

  int httpResponseCode = https.GET();
  debug("Código de resposta HTTP: ");
  debugln(httpResponseCode);

  if (httpResponseCode > 0) {  // Verifica o código de resposta
    debug("Código de resposta HTTP: ");
    debugln(httpResponseCode);

    if (httpResponseCode == HTTP_CODE_OK) {  // Se resposta for 200
      String payload = https.getString();
      debugln("Resposta do servidor Laravel:");
      debugln(payload);

      // Tamanho estimado para o buffer do JSON
      const size_t capacity = 256;
      DynamicJsonDocument doc(capacity);

      // Parseia a string JSON
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        debug("Erro ao parsear JSON: ");
        debugln(error.c_str());
        https.end();
        return false;
      }

      Serial.print("Doc status: ");
      int status = doc["status"];
      Serial.print(status);


      if (status == 200) {
        // Acessa os valores do JSON
        externalIrReqRelay = doc["data"][0]["command"];
        externalIrReqID = doc["data"][0]["id"];

        debugln("New Task Recived: ID:");
        debug(externalIrReqID);

        enfileirarTarefa(externalIrReqRelay);

        https.end();
        return true;

      } else {

        Serial.print("Status não é 200");
      }
    }
    https.end();
  }

  return false;
}