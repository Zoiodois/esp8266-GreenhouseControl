//Gsheets --
void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_error) {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  } else {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}

//Enviador de Dados
void loop_enviadorDados() {

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


    bool success = GSheet.values.append(&response /* returned response */, "1ghTXvGyQEu-iUEI7eEYKt2wTln5SJcn2WORhD5_FF_o" /* spreadsheet Id to append */, sheetAppend + "A2" /* range to append */, &valueRange /* data range to append */);
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

//Recebedor de Dados
void loop_receberDados() {
  bool ready = GSheet.ready();
  debugln("Receber Dados Iniciando");

  if (ready && millis() - mr > 15000) {
    mr = millis();
    debugln("--Gsheet Ready--");

    FirebaseJson response2;
    FirebaseJsonData result;   //Tempo Ciclo
    FirebaseJsonData result2;  //Limiar Estufa
    FirebaseJsonData result3;  //Estufa Ligada
    FirebaseJsonData result4;  //Tempo Irrigacao
    FirebaseJsonData result5;  //Valor Limite Irrigação


    debugln("Get spreadsheet values from range...");
    debugln("---------------------------------------------------------------");

    bool success = GSheet.values.get(&response2, "1ghTXvGyQEu-iUEI7eEYKt2wTln5SJcn2WORhD5_FF_o", sheetValues + "A2:E2");

    response2.get(result /* FirebaseJsonData */, "values/[0]/[0]" /* key or path */);

    response2.get(result2 /* FirebaseJsonData */, "values/[0]/[1]" /* key or path */);

    response2.get(result3 /* FirebaseJsonData */, "values/[0]/[2]" /* key or path */);

    response2.get(result4 /* FirebaseJsonData */, "values/[0]/[3]" /* key or path */);

    response2.get(result5 /* FirebaseJsonData */, "values/[0]/[4]" /* key or path */);

    response2.toString(Serial, true);

    debugln("Delay betwen Cicles:");
    tempoCiclo = result.to<int>();
    debugln(tempoCiclo);

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

  } else {

    return;
  }
}