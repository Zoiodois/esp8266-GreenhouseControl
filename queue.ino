
// Função para enfileirar uma tarefa
void enfileirarTarefa(int rele) {
  debugln("Enfileirar tarefas adicionando o rele : ");
  debugln(rele);

  //Encontrar a respectiva duracao
  intervaloIr = tempoIrrigacao[rele];
  int releState = releStates[rele];
  debugln("Esse é o intervalo registrado na fila: ");
  debugln(intervaloIr);
  debugln("Esse é o Estado ro rele para o Shift Register: ");
  debugln(releState);

  if ((fimFila + 1) % MAX_TAREFAS != inicioFila) {  // Verifica se a fila não está cheia
    fila[fimFila].rele = rele;
    fila[fimFila].duracao = intervaloIr;
    fila[fimFila].releState = releState;

    fila[fimFila].ativa = true;
    fimFila = (fimFila + 1) % MAX_TAREFAS;

  } else {
    Serial.println("Fila cheia!");  // Tratar overflow da fila
  }

  char relaysString[MAX_TAREFAS * 4 + 2];                           // Buffer para a string
  gerarStringReleEnfileirados(relaysString, sizeof(relaysString));  // Chamada corrigida

  debugln("relaysString: ");
  debugln(relaysString);
}

// Função para desenfileirar uma tarefa
Tarefa desenfileirarTarefa() {
  Tarefa tarefaVazia = { -1, 0, false };  // Tarefa vazia para retorno
  if (inicioFila != fimFila) {            // Verifica se a fila não está vazia
    Tarefa tarefa = fila[inicioFila];
    inicioFila = (inicioFila + 1) % MAX_TAREFAS;
    return tarefa;
  }
  return tarefaVazia;
}

void irEpochUpdate(bool epochUpdated) {

  if (epochUpdated) {
    Serial.printf("Esse é o Epoch no irEpochUpdate do SETUP: %d", epochTime);

    // Calcula valores iniciais de timer[] com base no Epoch
    for (int i = 0; i < NUM_RELES; i++) {

      if (lastIrrigatedEpoch[i] > 0) {  // Verifica se há um valor salvo

        unsigned long tempoDecorrido = (epochTime - lastIrrigatedEpoch[i]) * 1000;  // Em milissegundos

        if (tempoDecorrido < intervaloEntreIrrigacoes[i]) {
          timer[i] = millis() - tempoDecorrido;  // Ajusta o timer com base no tempo decorrido
        } else {

          enfileirarTarefa(i);  // Se o intervalo já passou, agende imediatamente
        }
      } else {

        timer[i] = millis();  // Se não houver dado salvo, inicialize o timer
      }
    }

  } else {

    for (int i = 0; i < NUM_RELES; i++) {
      timer[i] = millis() - (intervaloEntreIrrigacoes[i] / 2);
      Serial.printf("Timer[%d] inicializado em: %lu\n", i, timer[i]);
    }
  }
}
