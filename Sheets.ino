//Gsheets --
void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_error) {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  } else {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}

//Average Sender Loop
void sendAndClearData() {
  for (auto it = averageStorage.begin(); it != averageStorage.end();) {
    if (sendAverageData(*it)) {
      it = averageStorage.erase(it);  // Remove o elemento e avança o iterador
    } else {
      ++it;  // Apenas avança o iterador
    }
  }
}

//Data Sender
bool sendAverageData(const Dados& dados) {

  //Coping the sending data method in here
  bool ready = GSheet.ready();
  if (ready) {

    time_t epTime = dados.epochTime - 1800;
    epochCalculate(epTime);

    FirebaseJson response;
    FirebaseJson valueRange;

    debugln("\nAppend RAM VALUES to spreadsheet ...");
    debugln("*********");

    //Dados em linha
    valueRange.add("majorDimension", "ROWS");
    //rawdate
    valueRange.set("values/[0]/[0]", epTime);
    //data
    valueRange.set("values/[0]/[1]", currentDate);
    //hora
    valueRange.set("values/[0]/[2]", formattedTime);
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
    //Max Temp
    valueRange.set("values/[0]/[12]", dados.maxTemp);
    //Min Temp
    valueRange.set("values/[0]/[13]", dados.minTemp);


    bool success = GSheet.values.append(&response /* returned response */, "1ghTXvGyQEu-iUEI7eEYKt2wTln5SJcn2WORhD5_FF_o" /* spreadsheet Id to append */, sheetAppend + "A2" /* range to append */, &valueRange /* data range to append */);
    if (success) {

      response.toString(Serial, true);
      return true;

    } else {
      debugln(GSheet.errorReason());
      return false;
    }

  } else {
    debugln("GSheet not ready or waiting for next interval");
    return false;
  }
}

//Live Data Sender
void sendLiveData() {

  //Coping the sending data method in here
  bool ready = GSheet.ready();
  if (ready) {

    FirebaseJson response;
    FirebaseJson valueRange;

    debugln("\nUpdating Sensors VALUES to spreadsheet ...");
    debugln("*********");

    //Dados em linha
    valueRange.add("majorDimension", "ROWS");
    //rawdate
    valueRange.set("values/[0]/[0]", epochTime);
    //data
    valueRange.set("values/[0]/[1]", currentDate);
    //hora
    valueRange.set("values/[0]/[2]", formattedTime);
    //Temperatura Externa
    valueRange.set("values/[0]/[3]", temperatureModulo);
    //Umidade do AR Externo
    valueRange.set("values/[0]/[4]", humidityModulo);
    //Temperatura Termistor
    valueRange.set("values/[0]/[5]", TempCelsius);
    //Umidade Solo 1 Resistivo Externo Mudas
    valueRange.set("values/[0]/[6]", AveRes1);
    //Luminosidade
    valueRange.set("values/[0]/[7]", lumPC);
    //Umidade Solo Capacitivo 1 - Provavelmente Estufas
    valueRange.set("values/[0]/[8]", AveCap1);
    //Umidade Solo Capacitivo 2
    valueRange.set("values/[0]/[9]", AveCap2);
    //Temperatura Estufa
    valueRange.set("values/[0]/[10]", temperatureEstufa);
    //Umidade do AR Estufa
    valueRange.set("values/[0]/[11]", humidityEstufa);
    //Max Temp
    valueRange.set("values/[0]/[12]", maxTemp);
    //Min Temp
    valueRange.set("values/[0]/[13]", minTemp);
    //Current State
    valueRange.set("values/[0]/[14]", stateToString(currentState));

    bool success = GSheet.values.update(&response /* returned response */, "1ghTXvGyQEu-iUEI7eEYKt2wTln5SJcn2WORhD5_FF_o" /* spreadsheet Id to append */, sheetLive + "A2:O2" /* range to append */, &valueRange /* data range to append */);

    if (success) {

      response.toString(Serial, true);

    } else {

      debugln(GSheet.errorReason());
      debugln();
    }

  } else {
    debugln("GSheet not ready or waiting for next interval");
  }
}
//Helper State converter
const char* stateToString(State state) {
  switch (state) {
    case IDLE: return "IDLE";
    case IRRIGATION_ON: return "IRRIGATION_ON";
    case IRRIGATION_WAIT: return "IRRIGATION_WAIT";
    case IRRIGATION_OFF: return "IRRIGATION_OFF";
    default: return "UNKNOWN";
  }
}

//Data Recivier
void loop_receberDados() {

  bool ready = GSheet.ready();
  delay(100);

  debugln("Receber Dados Iniciando");

  //ready &&
  if (ready && millis() - mr > 15000) {
    mr = millis();
    debugln("--Gsheet Ready--");

    FirebaseJson response2;
    /*
    FirebaseJsonData result;    //Delay betwen Readings
    FirebaseJsonData result2;   //Limiar Estufa
    FirebaseJsonData result3;   //Estufa Ligada
    FirebaseJsonData result4;   //Tempo Irrigacao
    FirebaseJsonData result5;   //Valor Limite Irrigação
    FirebaseJsonData result6;   //Time between Sending Data
    FirebaseJsonData result7;   //Time between Average Calculation
    FirebaseJsonData result8;   //Time between Updating Configurations
    FirebaseJsonData result9;   //Automatic Irrigation on/off
    FirebaseJsonData result10;  //External Irrigation Request
    */


    debugln("Get spreadsheet values from range...");
    debugln("---------------------------------------------------------------");

    bool success = GSheet.values.get(&response2, "1ghTXvGyQEu-iUEI7eEYKt2wTln5SJcn2WORhD5_FF_o", sheetValues + "A2:J2");

    if (success) {

      processGSheetResponse(response2);
/*
      response2.get(result , "values/[0]/[0]");

      response2.get(result2 , "values/[0]/[1]" );

      response2.get(result3 , "values/[0]/[2]");

      response2.get(result4 , "values/[0]/[3]");

      response2.get(result5, "values/[0]/[4]");

      response2.get(result6, "values/[0]/[5]");

      response2.get(result7, "values/[0]/[6]");

      response2.get(result8, "values/[0]/[7]");

      response2.get(result9, "values/[0]/[8]");

      response2.get(result10, "values/[0]/[9]");

      response2.toString(Serial, true);

      debugln("Delay betwen Readings:");
      readingInterval = result.to<int>();
      debugln(readingInterval);

      debugln("Greenhouse Humidity Treshold Limit");
      limiarEstufa = result2.to<int>();
      debugln(limiarEstufa);

      debugln("Grenhouse on/off Status:");
      grenhouseON = result3.to<int>();
      debugln(grenhouseON);

      debugln("New Irrigation Time:");
      tempoIrrigacao = result4.to<int>();
      debugln(tempoIrrigacao);

      debugln("Soil Humidity Treshold Limit");
      limiteUmidade = result5.to<int>();
      debugln(limiteUmidade);

      debugln("Time between Sending Data");
      sendDataInterval = result6.to<int>();
      debugln(sendDataInterval);

      debugln("Time between Updating Config");
      recivingDataInterval = result8.to<int>();
      debugln(recivingDataInterval);

      debugln("Automatic Irrigation Status");
      autoIr = result9.to<int>();
      debugln(autoIr);

      Serial.println("External Irrigation Request");
      externalIrReq = result10.to<int>();
      Serial.println(externalIrReq);
      */
    }
  
  } else {
      debugln("G-sheets not ready");
    return;
  }

}

void processGSheetResponse(FirebaseJson& response2) {
  FirebaseJsonData result;    //Delay betwen Readings
  FirebaseJsonData result2;   //Limiar Estufa
  FirebaseJsonData result3;   //Estufa Ligada
  FirebaseJsonData result4;   //Tempo Irrigacao
  FirebaseJsonData result5;   //Valor Limite Irrigação
  FirebaseJsonData result6;   //Time between Irrigation (Control)
  //FirebaseJsonData result7;   //Time between Average Calculation
  FirebaseJsonData result8;   //Time between Updating Configurations
  FirebaseJsonData result9;   //Automatic Irrigation on/off
  FirebaseJsonData result10;  //External Irrigation Request

  response2.get(result /* FirebaseJsonData */, "values/[0]/[0]" /* key or path */);
  response2.get(result2 /* FirebaseJsonData */, "values/[0]/[1]" /* key or path */);
  response2.get(result3 /* FirebaseJsonData */, "values/[0]/[2]" /* key or path */);
  response2.get(result4 /* FirebaseJsonData */, "values/[0]/[3]" /* key or path */);
  response2.get(result5 /* FirebaseJsonData */, "values/[0]/[4]" /* key or path */);
  response2.get(result6 /* FirebaseJsonData */, "values/[0]/[5]" /* key or path */);
  //response2.get(result7 /* FirebaseJsonData */, "values/[0]/[6]" /* key or path */);
  response2.get(result8 /* FirebaseJsonData */, "values/[0]/[7]" /* key or path */);
  response2.get(result9 /* FirebaseJsonData */, "values/[0]/[8]" /* key or path */);
  response2.get(result10 /* FirebaseJsonData */, "values/[0]/[9]" /* key or path */);

  Serial.println("Dados recebidos do Google Sheets:");
  response2.toString(Serial, true);

  Serial.println("Delay betwen Readings:");
  readingInterval = result.to<int>();
  Serial.println(readingInterval);

  Serial.println("Greenhouse Humidity Treshold Limit");
  limiarEstufa = result2.to<int>();
  Serial.println(limiarEstufa);

  Serial.println("Grenhouse on/off Status:");
  grenhouseON = result3.to<int>();
  Serial.println(grenhouseON);

  Serial.println("New Irrigation Time:");
  tempoIrrigacao = result4.to<int>();
  Serial.println(tempoIrrigacao);

  Serial.println("Soil Humidity Treshold Limit");
  limiteUmidade = result5.to<int>();
  Serial.println(limiteUmidade);

  Serial.println("Time between Irrigation");
  betweenIrInterval = result6.to<int>();
  Serial.println(betweenIrInterval);

  //Serial.println("Time between Average Calculation");
  //sendDataInterval = result7.to<int>();
  //Serial.println(sendDataInterval);

  Serial.println("Time between Updating Config");
  recivingDataInterval = result8.to<int>();
  Serial.println(recivingDataInterval);

  Serial.println("Automatic Irrigation Status");
  autoIr = result9.to<int>();
  Serial.println(autoIr);

  Serial.println("External Irrigation Request");
  externalIrReq = result10.to<int>();
  Serial.println(externalIrReq);

    //Reset the external Irrigation Request
  if (externalIrReq) {

    updateExterReq();
  }

}

void updateExterReq() {

  //Coping the sending data method in here
  bool ready = GSheet.ready();
  if (ready) {

    FirebaseJson response;
    FirebaseJson valueRange;

    debugln("\nUpdating external Request to false ...");
    debugln("*********");

    //Dados em linha
    valueRange.add("majorDimension", "ROWS");
    //rawdate
    valueRange.set("values/[0]/[0]", 0);


    bool success = GSheet.values.update(&response /* returned response */, "1ghTXvGyQEu-iUEI7eEYKt2wTln5SJcn2WORhD5_FF_o" /* spreadsheet Id to append */, sheetValues + "J2" /* range to append */, &valueRange /* data range to append */);
    if (success) {

      response.toString(Serial, true);

    } else {
      debugln(GSheet.errorReason());
    }

  } else {
    debugln("GSheet not ready or waiting for next interval");
  }
}
