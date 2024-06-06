//Gsheets --
void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_error) {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  } else {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}

//Data Sender
void sendRamData() {

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
      //EpochLogic
      epochTime = dados.epochTime;
      epochCalculate(epochTime);

      //Dados em linha
      valueRange.add("majorDimension", "ROWS");
      //rawdate
      valueRange.set("values/[0]/[0]", dados.epochTime);
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
        dadosArmazenados.erase(dadosArmazenados.begin() + i);

        if (numAverages > 0) {
          numAverages--;
        }
        dadosArmazenados.shrink_to_fit();

      } else {

        debugln(GSheet.errorReason());
        debugln();
      }
      debugln(ESP.getFreeHeap());

    } else {
      debugln("GSheet not ready or waiting for next interval");
    }
  }
  epochTime = timeClient.getEpochTime();
}

//Data Recivier
void loop_receberDados() {
  bool ready = GSheet.ready();
  debugln("Receber Dados Iniciando");

  if (ready && millis() - mr > 15000) {
    mr = millis();
    debugln("--Gsheet Ready--");

    FirebaseJson response2;
    FirebaseJsonData result;   //Delay betwen Readings
    FirebaseJsonData result2;  //Limiar Estufa
    FirebaseJsonData result3;  //Estufa Ligada
    FirebaseJsonData result4;  //Tempo Irrigacao
    FirebaseJsonData result5;  //Valor Limite Irrigação
    FirebaseJsonData result6;  //Time between Sending Data
    FirebaseJsonData result7;  //Time between Average Calculation
    FirebaseJsonData result8;  //Time between Updating Configurations


    debugln("Get spreadsheet values from range...");
    debugln("---------------------------------------------------------------");

    bool success = GSheet.values.get(&response2, "1ghTXvGyQEu-iUEI7eEYKt2wTln5SJcn2WORhD5_FF_o", sheetValues + "A2:H2");

    response2.get(result /* FirebaseJsonData */, "values/[0]/[0]" /* key or path */);

    response2.get(result2 /* FirebaseJsonData */, "values/[0]/[1]" /* key or path */);

    response2.get(result3 /* FirebaseJsonData */, "values/[0]/[2]" /* key or path */);

    response2.get(result4 /* FirebaseJsonData */, "values/[0]/[3]" /* key or path */);

    response2.get(result5 /* FirebaseJsonData */, "values/[0]/[4]" /* key or path */);

    response2.get(result6 /* FirebaseJsonData */, "values/[0]/[5]" /* key or path */);

    response2.get(result7 /* FirebaseJsonData */, "values/[0]/[6]" /* key or path */);

    response2.get(result8 /* FirebaseJsonData */, "values/[0]/[7]" /* key or path */);



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

    debugln("Time between Average Calculation");
    averageCalculationInterval = result7.to<int>();
    debugln(averageCalculationInterval);

    debugln("Time between Updating Config");
    recivingDataInterval = result8.to<int>();
    debugln(recivingDataInterval);

    

  } else {

    return;
  }
}


/*
//Enviador de Dados
void loop_enviadorDados() {
  //Apagar, não mais necessário
  bool ready = GSheet.ready();
  if (ready && millis() - ms > 15000) {
    ms = millis();

    FirebaseJson response;

    debugln("\nAppend spreadsheet values From READINGS...");
    debugln("----------------------------");

    FirebaseJson valueRange;
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
    valueRange.set("values/[0]/[6]", soilUmUm);
    //Luminosidade
    valueRange.set("values/[0]/[7]", lumPC);
    //Umidade Solo Capacitivo 1 - Provavelmente Estufas
    valueRange.set("values/[0]/[8]", soilCap1);
    //Umidade Solo Capacitivo 2
    valueRange.set("values/[0]/[9]", soilCap2);
    //Temperatura Estufa
    valueRange.set("values/[0]/[10]", temperatureEstufa);
    //Umidade do AR Estufa
    valueRange.set("values/[0]/[11]", humidityEstufa);

*/
    //Uncoment this next line (bool success)
    //bool success = GSheet.values.append(&response /* returned response */, "1ghTXvGyQEu-iUEI7eEYKt2wTln5SJcn2WORhD5_FF_o" /* spreadsheet Id to append */, sheetAppend + "A2" /* range to append */, &valueRange /* data range to append */);
/*
    if (success) {
      response.toString(Serial, true);

    } else {

      debugln(GSheet.errorReason());
      debugln();
      saveValues();  //Saving Values in RAM
    }
    debugln(ESP.getFreeHeap());
  } else {

    saveValues();  //Saving Values in RAM
    return;
  }
}
*/