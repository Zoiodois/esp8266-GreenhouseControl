//Connect Wifi
void connectWiFi() {
  debugln("Tentando conectar ao WiFi...");
  WiFi.disconnect();
  delay(100);
  //LCD Display
  lcd.setCursor(0, 0);
  lcd.print(F("Conecting Wifi: "));  //Latter mabe show the network beeing tryed

  for (uint8_t i = 0; i < numNetworks; i++) {
    WiFi.mode(WIFI_OFF);
    delay(1000);

    //This line hides the viewing of ESP as wifi hotspot
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid[i], password);  // Tentativa de conexão com a rede atual

    unsigned long startTime = millis();  // Tempo inicial de tentativa de conexão

    int attemptCount = 0;
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 50000 && attemptCount < 10) {
      delay(1000);
      debug(".");
      attemptCount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      debugln("\nConectado ao WiFi!");
      updated = testInternetConnection();
      return;  // Se a conexão for bem-sucedida, interrompa o loop e retorne
    } else {
      debugln("\nFalha ao conectar ao WiFi!");
      // WiFi.disconnect();  // Desconecta do WiFi atual antes de tentar o próximo
    }
  }

  debugln("Todas as tentativas de conexão falharam.");
}

bool testInternetConnection() {

  debugln("Iniciando o Teste de Conexão");
  HTTPClient http;
  http.begin(wifiClient, "http://www.google.com");  // URL para teste de conexão

  int httpCode = -1;
  unsigned long startTestTime = millis();

  while ((millis() - startTestTime) < 10000) {

    httpCode = http.GET();
    if (httpCode > 0) {
      break;
    }
    //delay(100);
    yield();  // Permite que o watchdog seja reiniciado
  }


  http.end();

  Serial.println(httpCode);

  // Verifica se a requisição foi bem-sucedida
  if (httpCode > 0) {
    // Código de resposta HTTP 200 indica sucesso
    if (httpCode == HTTP_CODE_OK) {
      debugln("Conexão Passou no teste");
      return true;
    }
  }

  debugln("Conexão NAOO Passou no teste");


  return false;
}
