/*******************************************************
 * ESP8266 Irrigation System
  *Controls Greenhouse Humidity
  *Automatic Irrigation (Between Time Based)
 * 10/2024 - Felipe Buhrer
  *V-0.2
 *Alter Greenhouse Relay duo raly Failure


 */
/********************************************************
Pin Conennection on Board

D0 - GPIO16 - High At boot - no i2c                 MUX - 1
D1 - GPIO5 - i2c                                    Display
D2 - GPIO4 - i2c                                    Display
D3 - GPIO0 - If Input - pulled up                   MUX - 2
D4 - GPIO2 - If Input - pulled up - High At boot    MUX - 3 ->Probably will use for pwm fan Controls
D5 - GPI14 - ok -                                   Estufa Sensor
D6 - GPI12 - ok                                     Shift Register - 1 - DATA PIN
D7 - GPIO13 - ok                                    Shift Register - 3 - LATCH PIN
D8 - GPIO15 - If Input - PullDown                  Shift Register - 2 - CLOCK PIN


*/

// Debug State Set
#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

//**Include Librayrys
#include <Arduino.h> //Overall librarie for commun methods
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>       //Wifi conection for esp8266
#include <NTPClient.h>         //Get the EPOCH time from web
#include <WiFiUdp.h>           // Get the EPOCH time from web
#include <ESP8266HTTPClient.h> //HTTP Client for Conection Test
#include <DHT.h>               //Read DHT Sensors
#include <vector>              //Save Ram Data
#include <String>              //Save Ram Data
#include <ctime>               //Save Ram Data
#include <WiFiClientSecure.h>  //SSL Client for http Requests
// #include <LCD-I2C.h>
#include <Wire.h>
#include <EEPROM.h>
// #include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>

#include "envKeys.h" //Passwords and Autentications keys

//**Define Pins and Sensors
// Arrays para armazenar os pinos e intervalos de tempo para cada relé
const uint8_t NUM_RELES = 6;
int releStates[NUM_RELES] = {32, 16, 8, 4, 2, 1}; // 5 relays, Will take the Greenhouse state out of this array
int greenhouseonState = 32;
int clearAll = 0;
int lastState = 0;

#define FAN_BIT 6
#define FAN_MASK (1 << FAN_BIT)

#define MUXA 16 // D0 Mux channel A
#define MUXB 0  // D3 Mux channel B
#define MUXC 2  // D4 Mux channel C
// #define MUXD 14  //D5 Mux channel D  ->Use in DHT

const int dataPin = 12;  // DS   D6  A no shift Register
const int latchPin = 13; // ST_CP    D7
const int clockPin = 15; // SH_CP   D8

//--
// OVERALL CONFIGURATIONS

//**Epoch Time Config
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org"); //? , pool , offset , update interval
int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// EEPROM Configuration
#define EEPROM_SIZE 512
bool epochUpdated = false; // True after update run

//**Variables for Sending Data

// Reading Data interval
uint32_t readingInterval = 60000; // 1min
// Delay between sending average data
uint32_t AvgSendInterval = 90000; // 1,5 min
// Time to Upate Config Interval
unsigned long recivingDataInterval = 60000; // change again 5 minutes
// Reciving Config Data TIMER
unsigned long mr = 0;
// Reciving Canteiros Data TIMER
unsigned long mc = 0;
// Holder of the last Update timer
unsigned long lastUpdate = 0;
// Flag for data comunication
bool requestOn = false;

//--
// INTERNET CONNECTION TESTS CONFIGURATION
//--**Loop Intervals Counter Holders
unsigned long reconnectInterval = 60000;
unsigned long lastConectionTest = 0; // Test Internet pinging google's IP
unsigned long lastAttemptedReconnect = 0;
// Bool to check the internet connection state
bool updated = 1;
// wWifi and WiFiClientSecure object.
WiFiClientSecure client;
WiFiClient wifiClient;

//--
// DATA AVERAGES STRUCTURE CONFIGURATION
// BMP280 data
uint8_t temperatureExterna = 0;
uint16_t pressaoExterna = 0;
// Module Temperatura
uint8_t TempCelsius = 0;
// AHT10 Data
uint8_t temperatureEstufa = 0;
uint8_t humidityEstufa = 0;
// Overall Data
uint8_t maxTemp = 0;  // Will be updated every cicle and send the max values
uint8_t minTemp = 80; // Will be updated every cicle and send the min values
// LDR sensor
uint8_t lumPC = 0;
// Resistive Soil Moisture
uint16_t soilUmUm = 0;
// Sensor Umidade Solo Capacitivo 1
uint16_t soilCap1 = 0;
// Sensor Umidade Solo Capacitivo 2
uint16_t soilCap2 = 0;
// Variaveis data e hora
time_t epochTime = 1720697145; // Set if bad conection, first readings may need it.

// Configurações do divisor de tensão
const float Vin = 3.3;         // Tensão de alimentação
const float R_fixed = 10000.0; // Resistor fixo de 10kΩ
float tempC = 0.0;
float Vout = 0;
float R_thermistor = 0;
float tempK;

// Coeficientes do termistor para a equação de Steinhart-Hart
const float A = 1.009249522e-03;
const float B = 2.378405444e-04;
const float C = 2.019202697e-07;

//**Average Calculation
// Start the index for the average Ccalculatrion
uint8_t currentIndex = 0; // Índex for the position
// Timer for the between reading waiting
unsigned long lastReading = 0;
// Holder fot the last average sended epoch
unsigned long lastAvgSend = 0;
// Counter fot the number of averages waiting to be sended
uint8_t numAverages = 0; // Contador de médias calculadas
// Counter os average calculator
long somatoria = 0;
// Average holder
uint16_t media;
// Reading holder
uint16_t leitura;
// Number of Samples to be readed before send the average sensor reading
uint8_t NUMERO_AMOSTRAS = 40; // Number of readings per Average
// Max reading number before Average Calculation
const uint8_t numReadings = 60; //***Cahnge to 60
uint16_t AveRes1 = 100;
uint16_t AveCap2 = 100;
uint16_t AveCap1 = 100;
// For Memory Debug
// size_t freeHeap = 0;

//**Struct definition to save all data formats
struct Dados
{
  time_t epochTime;
  uint8_t temperatureExterna;
  uint8_t pressaoExterna;
  uint16_t TempCelsius; // Change later. Loggin Test
  uint16_t soilUmUm;
  uint8_t lumPC;
  uint16_t soilCap1;
  uint16_t soilCap2;
  uint8_t temperatureEstufa;
  uint8_t humidityEstufa;
  uint8_t maxTemp;
  uint8_t minTemp;
};
// Struct definition to save all data formats
struct Dados;
// Struct Vector to storage readings
Dados readings[numReadings];
// Vector para armazenar os conjuntos de dados das médias ja calculatas
std::vector<Dados> averageStorage; // Uses array in stack for that

//--
// SENSORS CONFIGURATIONS
//**Sensores BMP280
// Adafruit_BMP280 bmp;  // use I2C interface
// Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
// Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

// AHT10 Initi
Adafruit_AHTX0 aht;

// Sensores DHT 11
#define DHTESTUFA 14 // D5 Sensor Temp Greenhouse
#define DHTTYPE DHT11
DHT dhtEstufa(DHTESTUFA, DHTTYPE);

//--
// IRRIGATION LOGIC CONFIGURATION

//**GREENHOUSE CONFIGURATION
// Greenhous Activity on/off
bool greenhouseOn = 1;
// Greenhouse trigger treshhold
uint16_t limiarEstufa = 70;
// New Greenhouse Logic
// Variable for Greenhouse Rele control
bool isSpraying = false;
// Interval with valve On/Off
unsigned long intervalOn = 3000;   // 3 segundos
unsigned long intervalOff = 57000; // 57 segundos
unsigned long estufaSprayStart = 0;
bool overTargetHumidity;

bool fansOn = false;
uint8_t fansTreshold = 35;
bool fanSystemOn = true;

//**PLANTS IRRIGATION CONFIGURATION
// Automatic Irrigation System On/Off
bool autoIr = 1;
int error;
// External Request for Irrigation
int externalIrReqID = -1;   // Updatable by google sheets
uint8_t externalIrReqRelay; // The rele requiested for irrigtion
time_t epochstartCycleTime; // Armazena o início de cada ciclo de irrigação
//**Times for individual Canteiros
// Definindo o número de relés
unsigned long tIrCant1 = 50000;  // 45 sec default
unsigned long tIrCant2 = 600000; // 10 min default
unsigned long tIrCant3 = 600000; // 10 min default
unsigned long tIrCant4 = 600000; // 10 min default
unsigned long tIrCant5 = 600000; // 10 min default
unsigned long tIrGH = 1200000;   // 20 min default
// Between Irrigation Time for each Canteiro - Updatable
unsigned long betweenIrIntervalCant1 = 21600000;  // 4 Times a day. Updat latter, 3 times only in sunny hours.
unsigned long betweenIrIntervalCant2 = 151200000; // 42 hours
unsigned long betweenIrIntervalCant3 = 151200000; // 42 hours
unsigned long betweenIrIntervalCant4 = 151200000; // 42 hours
unsigned long betweenIrIntervalCant5 = 151200000; // 42 hours
unsigned long betweenIrIntervalGH = 172800000;    // 48 hours

// Definir intervalos de irrigação
unsigned long intervaloEntreIrrigacoes[NUM_RELES] = {betweenIrIntervalGH, betweenIrIntervalCant1, betweenIrIntervalCant2, betweenIrIntervalCant3, betweenIrIntervalCant4, betweenIrIntervalCant5}; // Update to { betweenIrIntervalCant11, betweenIrIntervalCant12, ... }
// Variáveis para armazenar o último tempo que cada relé foi acionado
unsigned long timer[NUM_RELES] = {0, 0, 0, 0, 0, 0}; // store last irrigation time to compare with intervalo[]
unsigned long tempoIrrigacao[NUM_RELES];
time_t lastIrrigatedEpoch[NUM_RELES];
// int releState;

// Variável para controlar o estado dos relés (ligado/desligado)
bool releAtivo[NUM_RELES] = {false, false, false, false, false, false};
bool isReleOn = false;
unsigned long intervaloIr = 0;
unsigned long tempoDecorrido = 0;
uint8_t rele;
// Timers for the irrigation logic as it runs only one each time
unsigned long previousMillis = 0;
unsigned long irrigationStart = 0;

//--Tarefas
// This is the control of the task line
// Estrutura para representar uma tarefa na fila
struct Tarefa
{
  int rele;              // Identificador do relé (0 a 4)
  unsigned long duracao; // Duração que o relé ficará ligado
  int releState;
  bool ativa; // Flag para saber se a tarefa é de ativação ou desativação
};

// Definindo a fila com tamanho fixo (pode ser adaptada)
const int MAX_TAREFAS = 10;
Tarefa fila[MAX_TAREFAS]; // Array para a Fila de tarefas
int inicioFila = 0;       // Índice do início da fila
int fimFila = 0;          // Índice do fim da fila

// LCD Configuration
// LCD Display Adress
LCD_I2C lcd(0x27, 16, 2); // Endereço padrão da maioria dos módulos PCF8574
// Temporização de tela
unsigned long tempoAnterior = 0;
const unsigned long intervaloTela = 3000; // Tempo para alternar telas (em ms)
// Variável para armazenar a tela atual
int telaAtual = 0;
uint8_t lastShiftData = 0;
unsigned long transformEpoch;

// methods to be used:

// Enviador de Dados
//**Laravel API --
void sendLaravelLiveData();
bool sendLaravelAverageData(const Dados &dados);
void receberConfigMasterLaravel();
// Update External Irrigation Request
bool unqueCommand(int externalIrReqID);
bool reciveQueue();

// Relays control
void shiftData(int number);
void relayControl(int relayHexNum);
void fanControl(bool command);

// Connect Wifi
void connectWiFi();
bool testInternetConnection();

//**Sensors
// Mux Readings
void muxReadings();
// //BMP280
// void bmpReading();
// AHT10 Readings:
void aht10read();
// Leitura Sensores Resistivos
uint16_t readPin();
// dht11
void Load_DHT11_Data();
// Mux port selection code
void selectChannel();

// Method for saving values in RAM for 60 readings
void addReading();
void calculateAverages();
void sendAndClearData();
// void estufaFeedback(int estufaTime);

// Task line for irrigations
void irEpochUpdate(bool epochUpdated);
void loadLastIrEpochs();
void enfileirarTarefa();
Tarefa desenfileirarTarefa();

// LCD MEthods
void switchTela();
void exibirSensores();
void exibirReles();
void exibirConexaoWiFi();
void exibirLastIr(int lastIr);
void memoryStats();
void exibirKpStatus();
void exibirGrennhgouseCycleStatus();

void setup()
{

  // Shift Register Pins
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  // First thing set all the relays off
  shiftData(0);

  Serial.begin(115200);

  // Access to the EEPROM Memory
  EEPROM.begin(EEPROM_SIZE);

  // LCD Initialization
  Wire.begin(50000); // 100khz
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();

  // BMP280 sensor
  // bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);

  /* Default settings from datasheet. */
  // bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
  //                 Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
  //                 Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
  //                 Adafruit_BMP280::FILTER_X16,      /* Filtering. */
  //                 Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // AHT 10 Sensor
  // aht.begin(); disable

  // DHT sensores
  dhtEstufa.begin();

  Load_DHT11_Data();

  // Mux pins as OUTPUTS
  pinMode(MUXA, OUTPUT);
  pinMode(MUXB, OUTPUT);
  pinMode(MUXC, OUTPUT);
  // pinMode(MUXD, OUTPUT); -> used in dht

  // Irrigation system Timer
  tempoIrrigacao[0] = tIrGH;
  tempoIrrigacao[1] = tIrCant1;
  tempoIrrigacao[2] = tIrCant2;
  tempoIrrigacao[3] = tIrCant3;
  tempoIrrigacao[4] = tIrCant4;
  tempoIrrigacao[5] = tIrCant5;

  // WiFi
  connectWiFi();

  // Inicializando Gsheets
  if (WiFi.status() == WL_CONNECTED)
  {
    debugln("\nWi-Fi connected!");
    updated = 1;
  }
  else
  {
    debugln("\nFailed to connect to Wi-Fi!");
  }

  // Set scure wifi conection for data sending
  client.setInsecure();

  // Initialize a NTPClient to get time
  timeClient.begin();

  // Carrega tempos salvos na EEPROM
  loadLastIrEpochs();

  // NEED TO PUT THIS CODE WITH FLAG IN THE LOOP IN CASE THERE IS NO WIFI WHEN TURNING ON
  if (updated)
  {

    unsigned long startAttemptTime = millis(); // Marca o início da tentativa
    const unsigned long timeout = 60000;       // Tempo limite de 1 min
    // Tente sincronizar o tempo
    while (!timeClient.update())
    {

      epochTime = timeClient.getEpochTime();
      if (millis() - startAttemptTime > timeout)
      { // Verifica se o tempo limite foi excedido
        Serial.println("Falha ao sincronizar com o servidor NTP.");
        break; // Sai do loop após o timeout
      }
      delay(500); // Aguarda meio segundo antes de tentar novamente
    }

    // Define a flag se o loop foi concluído com sucesso
    if (millis() - startAttemptTime <= timeout)
    {
      epochTime = timeClient.getEpochTime();
      Serial.print(F("Esse é o epoch no SETUP: "));
      Serial.print(epochTime);
      epochUpdated = true;
    }
  }

  irEpochUpdate(epochUpdated);
}

void loop()
{

  unsigned long currentMillis = millis();

  // Update LCD display
  //  if (currentMillis - tempoAnterior >= intervaloTela) {
  //    tempoAnterior = currentMillis;
  //    telaAtual++;
  //    if (telaAtual > 9) telaAtual = 0;  // Reset at number of screens
  //    lcd.clear();
  //    switchTela();
  //  }

  // Test internet conection pingin google's ip
  if ((currentMillis - lastConectionTest >= 380000) && !isSpraying)
  { // change again to 380000

    // Test internet conection
    updated = testInternetConnection();

    // If fails reconnect
    if (!updated)
    {
      connectWiFi();
    }

    lastConectionTest = currentMillis;
  }

  // NEW Updating Data
  if ((currentMillis - lastUpdate > recivingDataInterval) && (updated && !isSpraying))
  { // 1min teste

    receberConfigMasterLaravel();

    if (externalIrReqID = -1)
    {
      Serial.println("Pedindo comando Externo");
      reciveQueue();
    }

    if (externalIrReqID > -1)
    {
      Serial.println("External Command Update");
      unqueCommand(externalIrReqID);
    }

    lastUpdate = currentMillis;
  }

  // Live data sent to Laravel
  if ((currentMillis - lastReading >= readingInterval) && !isSpraying)
  {

    lastReading = currentMillis;
    muxReadings();
    // bmpReading();
    // aht10read();
    Load_DHT11_Data();
    addReading();

    if (updated)
    {

      // Review this epooch offline logic
      timeClient.update();
      epochTime = timeClient.getEpochTime();

      Serial.printf("Sending Averages:");
      sendLaravelLiveData();
    }
  }

  // Send Average Data Last for EpochLogic
  if ((averageStorage.size() > 0) && (currentMillis - lastAvgSend >= AvgSendInterval) && !isSpraying)
  {

    if (updated)
    {
      lastAvgSend = currentMillis;
      sendAndClearData();
    }
  }

  if (fanSystemOn)
  {

    // Fans Logic
    if (temperatureEstufa >= fansTreshold)
    {

      if (!fansOn)
      {
        fanControl(true);
      }
    }
    else
    {

      if (fansOn)
      {
        fanControl(false);
      }
    }
  }
  else
  {
    if (fansOn)
    {
      fanControl(false);
    }
  }

  // Desliga os reles após o tempo
  for (int i = 0; i < NUM_RELES; i++)
  {
    if (releAtivo[i] && (currentMillis - irrigationStart >= intervaloIr))
    {
      // Tempo esgotado, desativa o relé
      Serial.print(F("Tempo esgotado, desativando relé: "));
      Serial.println(i);

      relayControl(clearAll); // Send the binary number that turns all relays off
      releAtivo[i] = false;
      isReleOn = false;
    }
  }

  // Irrigation Logic
  //  Enfilera os relés após o periodo Setado
  for (int i = 0; i < NUM_RELES; i++)
  {
    // Verifica se o tempo decorrido é maior ou igual ao intervalo definido para esse relé
    if (currentMillis - timer[i] >= intervaloEntreIrrigacoes[i])
    {
      // Verifica se o relé não está ligado, para evitar repetição desnecessária
      if (!releAtivo[i])
      {
        enfileirarTarefa(i);      // Ativa o relé atual e desliga os outros
        timer[i] = currentMillis; // Atualiza o temporizador para esse relé
      }
      break; // Sai do loop para garantir que apenas um relé seja ativado por vez
    }
  }

  // Verifica se há uma tarefa na fila
  if ((inicioFila != fimFila))
  {

    if (isReleOn)
    { // Ends loop here and avoid all Greenhouse Logic!
      return;
    }

    debugln("Iniciando loop da fila");
    debugln("Nenhum rele ligado");
    Tarefa tarefaAtual = desenfileirarTarefa(); // Desenfileira a próxima tarefa

    if (tarefaAtual.rele > -1 && tarefaAtual.rele <= NUM_RELES)
    { // Verifica se a tarefa é para um relé válido
      int rele = tarefaAtual.rele;
      int releState = tarefaAtual.releState;

      if (tarefaAtual.ativa)
      {

        // Make sure only one relay runs each time
        relayControl(clearAll);

        delay(1000);

        // Ativar o relé
        Serial.print("Ativando relé: ");
        Serial.println(rele);

        relayControl(releState); // Send the binary number that turns that rele on

        // update timers
        releAtivo[rele] = true;
        timer[rele] = currentMillis;
        irrigationStart = currentMillis;
        intervaloIr = tarefaAtual.duracao; // Configura a duração da ativação
        isReleOn = true;
        lastIrrigatedEpoch[rele] = epochTime;

        // EEPROM SAVE
        eepromSaveLastIrEpochs(rele, epochTime);
        debugln(F("Numero enviado ao shift Register: "));
        debugln(releState);
        debugln(F("Intervalo de irrigacao cofigurado:"));
        debugln(intervaloIr);
      }
      else
      {
        // Desativar o relé
        Serial.print("Desativando relé: ");
        Serial.println(rele);
        // Turn all reles off
        relayControl(clearAll); // Send the binary number that turns that rele on

        releAtivo[rele] = false;
        isReleOn = false;
      }
    }
  }

  // GREENHOUSE LOGIC --- Only Runs after the relays
  if (greenhouseOn)
  { // Double check on the rele, but it should always be off to get here
    bool humidityChanged = false;
    // Need to put another condition flag, that targets the umidity in the greenhouse over tresshold

    if (humidityEstufa > limiarEstufa)
    {
      if (!overTargetHumidity)
      { // Executa somente se o estado mudou
        overTargetHumidity = true;
        humidityChanged = true;
      }
    }
    else
    {
      if (overTargetHumidity)
      { // Executa somente se o estado mudou
        overTargetHumidity = false;
        humidityChanged = true;
      }
    }

    if (humidityChanged)
    {
      Serial.print("limpando rele do ghouse");
      relayControl(clearAll);  // Executa a função somente em mudanças de estado
      humidityChanged = false; // Reseta a flag
    }
    // if (humidityEstufa > limiarEstufa) {
    //   overTargetHumidity = true;
    //   //Disable relay
    //   shiftData(clearAll);
    // }

    if (!overTargetHumidity)
    {

      if (isSpraying)
      {
        // Válvula ligada, verifica se é hora de desligar
        if (currentMillis - estufaSprayStart >= intervalOn)
        {
          isSpraying = false;               // Desliga a válvula
          estufaSprayStart = currentMillis; // Atualiza o tempo da última mudança
          // Desliga o relé
          debugln(F("Is Spraying OFF"));
          relayControl(clearAll);
        }
      }
      else
      {
        // Válvula desligada, verifica se é hora de ligar
        if (currentMillis - estufaSprayStart >= intervalOff)
        {
          isSpraying = true;                // Liga a válvula
          estufaSprayStart = currentMillis; // Atualiza o tempo da última mudança
          debugln("Is Spraying ON");
          relayControl(greenhouseonState);
        }
      }
    }
  }
}