//Connect Wifi
void connectWiFi() {
  debugln("Tentando conectar ao WiFi...");

  for (int i = 0; i < numNetworks; i++) {
    WiFi.mode(WIFI_OFF);
    delay(1000);
    //This line hides the viewing of ESP as wifi hotspot
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid[i], password);  // Tentativa de conexão com a rede atual

    unsigned long startTime = millis();  // Tempo inicial de tentativa de conexão

    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 60000) {
      delay(500);
      debug(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      debugln("\nConectado ao WiFi!");
      return;  // Se a conexão for bem-sucedida, interrompa o loop e retorne
    } else {
      debugln("\nFalha ao conectar ao WiFi!");
      WiFi.disconnect();  // Desconecta do WiFi atual antes de tentar o próximo
    }
  }

  debugln("Todas as tentativas de conexão falharam.");
}