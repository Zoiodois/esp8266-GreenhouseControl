void switchTela() {

  switch (telaAtual) {
    case 0:
      exibirSensores();
      break;
    case 1:
      exibirReles();
      break;
    case 2:
      exibirConexaoWiFi();
      break;

    case 3:
      exibirLastIr(0);
      break;

    case 4:
      exibirLastIr(1);
      break;

    case 5:
      exibirLastIr(2);
      break;

    case 6:
      exibirLastIr(3);
      break;

    case 7:
      exibirLastIr(4);

      break;

    case 8:
      memoryStats();

      break;

    case 9:
      exibirSensoresModulo();
      break;
  }
}


// Função para exibir dados dos sensores
void exibirSensores() {
  lcd.setCursor(0, 0);
  lcd.print(F("Temp Estufa: "));
  lcd.print(temperatureEstufa);
  lcd.print(F("C"));

  lcd.setCursor(0, 1);
  lcd.print(F("Umid Estufa: "));
  lcd.print(humidityEstufa);
  lcd.print(F("%"));
}

void exibirSensoresModulo() {
  lcd.setCursor(0, 0);
  lcd.print(F("Temp Modulo: "));
  lcd.print(temperatureExterna);
  lcd.print(F("C"));

  lcd.setCursor(0, 1);
  lcd.print(F("Umid Modulo: "));
  lcd.print(pressaoExterna);
  lcd.print(F("%"));
}

// Função para exibir status dos relés
void exibirReles() {
  lcd.setCursor(0, 0);
  lcd.print(F("Last Shift D: "));
  lcd.setCursor(0, 1);
  lcd.print(lastShiftData, BIN);
}

// Função para exibir status da conexao WiFi
void exibirConexaoWiFi() {
  lcd.setCursor(0, 0);
  lcd.print(F("Updated: "));
  lcd.print(updated ? "YES " : "NO");
}

// Função para exibir status da Greenhouse
// void exibirGrennhgouseCycleStatus() {

//   int year, month, day, hour, minute;
//   transformEpoch = epochstartCycleTime;

//   epochToDateTime(transformEpoch, year, month, day, hour, minute);

//   //em que parte do loop greenhouse esta
//   lcd.setCursor(0, 0);
//   lcd.print(F("Cycle Compl: "));
//   lcd.print(isCycleComplete ? "YES" : "NO");

//   //Cycle Start
//   lcd.setCursor(0, 1);

//   lcd.print(F("Str: "));
//   if (day < 10) lcd.print(F("0"));
//   lcd.print(day);
//   lcd.print("/");
//   if (month < 10) lcd.print(F("0"));
//   lcd.print(month);
//   lcd.print(" ");
//   if (hour < 10) lcd.print(F("0"));
//   lcd.print(hour);
//   lcd.print(":");
//   if (minute < 10) lcd.print(F("0"));
//   lcd.print(minute);
// }

// void exibirKpStatus() {


//   //KP
//   lcd.setCursor(0, 0);
//   lcd.print(F("GHouse on: "));
//   lcd.print(greenhouseOn ? "Yes" : "No");
//   lcd.setCursor(0, 1);
//   lcd.print(F("EstufTime: "));
//   transformEpoch = (estufaTime / 60000);  //transform in minutes
//   lcd.print(transformEpoch);
// }



// Função para exibir Last Irrigated Status
void exibirLastIr(int index) {

  transformEpoch = lastIrrigatedEpoch[index];
  int year, month, day, hour, minute;
  epochToDateTime(transformEpoch, year, month, day, hour, minute);


  lcd.setCursor(0, 0);
  lcd.print(F("Last Ir "));
  lcd.print((index + 1));
  lcd.print(F(":"));
  lcd.setCursor(0, 1);
  if (day < 10) lcd.print(F("0"));
  lcd.print(day);
  lcd.print("/");
  if (month < 10) lcd.print(F("0"));
  lcd.print(month);
  lcd.print(" ");
  if (hour < 10) lcd.print(F("0"));
  lcd.print(hour);
  lcd.print(":");
  if (minute < 10) lcd.print(F("0"));
  lcd.print(minute);
}

void memoryStats() {

  lcd.setCursor(0, 0);
  lcd.print(F("Free Heap: "));
  lcd.setCursor(0, 1);
  lcd.print(ESP.getFreeHeap());
}


// Função para rolar texto longo na linha especificada
void scrollText(String text, int row) {
  if (text.length() <= 16) {
    // Se o texto é curto, exibe normalmente
    lcd.setCursor(0, row);
    lcd.print(text);
    delay(2000);  // Mantém o texto na tela por 2 segundos
  } else {
    // Se o texto é longo, exibe com rolagem
    for (int i = 0; i < text.length() - 15; i++) {
      lcd.setCursor(0, row);
      lcd.print(text.substring(i, i + 16));  // Mostra uma fatia de 16 caracteres
      delay(300);                            // Controle da velocidade de rolagem
    }
  }
}
