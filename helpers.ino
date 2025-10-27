

// Função para verificar se um ano é bissexto
bool isLeapYear(int year) {
  if (year % 4 == 0) {
    if (year % 100 == 0) {
      return (year % 400 == 0);
    }
    return true;
  }
  return false;
}

// Função para calcular o dia, mês e ano a partir do epoch time
void epochToDateTime(unsigned long epochTime, int &year, int &month, int &day, int &hour, int &minute) {
  epochTime -= 18000; // Ajuste para UTC+1, se necessário (modifique conforme o fuso horário)

  // Calcula o número total de dias desde 1970
  unsigned long days = epochTime / 86400L;
  unsigned long secondsInDay = epochTime % 86400L;

  // Calcula o horário do dia
  hour = secondsInDay / 3600;
  minute = (secondsInDay % 3600) / 60;

  // Calcula o ano
  year = 1970;
  while (days >= (isLeapYear(year) ? 366 : 365)) {
    days -= (isLeapYear(year) ? 366 : 365);
    year++;
  }

  // Ajusta os dias restantes para meses e dias
  month = 0;
  while (days >= daysInMonth[month]) {
    // Verifica fevereiro em ano bissexto
    if (month == 1 && isLeapYear(year)) {
      if (days < 29) break;
      days -= 29;
    } else {
      days -= daysInMonth[month];
    }
    month++;
  }
  month += 1; // Ajuste para meses começando de 1
  day = days + 1; // Ajuste para dias começando de 1
}

//Helper Fila converter

String gerarStringReleEnfileirados(char* resultado, int tamanhoBuffer) {

  int pos = 0;

  resultado[pos++] = '{';  // Adiciona a chave de abertura

  for (int i = inicioFila; i != fimFila; i = (i + 1) % MAX_TAREFAS) {

    int espacoRestante = tamanhoBuffer - pos;
     if (espacoRestante <= 0) {
      Serial.println("Buffer Cheio na Fila!");
      break;  // Evita overflow se o buffer estiver cheio
    }


    pos += snprintf(&resultado[pos], espacoRestante, "%d", fila[i].rele);  // Adiciona o número do relé

   // Se não for o último item, adiciona a vírgula
    if ((i + 1) % MAX_TAREFAS != fimFila && pos < tamanhoBuffer - 1) {
      resultado[pos++] = ',';
    }
  }

    // Adiciona a chave de fechamento e finaliza a string
  if (pos < tamanhoBuffer - 1) {
    resultado[pos++] = '}';
  }

  resultado[pos] = '\0';   // Garante o final da string

  return resultado;
}
