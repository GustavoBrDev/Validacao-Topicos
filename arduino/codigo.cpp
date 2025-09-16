#include <PubSubClient.h>
#include <WiFi.h>


const char* REDE = "Wokwi-GUEST";
const char* SENHA = "";

const char* BROKER = "broker.hivemq.com";
const char* TOPICO_BASE = "ifsc/ctds/topicos/validacao/gustavo-stinghen";

const unsigned long LAMPADA_SALA = 18;
const unsigned long LAMPADA_COZINHA = 19;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  Serial.begin(115200);

  pinMode(LAMPADA_SALA, OUTPUT);
  pinMode(LAMPADA_COZINHA, OUTPUT);
  digitalWrite(LAMPADA_SALA, LOW);
  digitalWrite(LAMPADA_COZINHA, LOW);

  connectWifi();

  client.setServer(BROKER, 1883);
  client.setCallback(callback);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  if (!client.connected()) {
    reconnectMqtt();
  } else {
    client.loop(); 
  }

  publicaDados();
}

void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Conectando ao WiFi: ");
  Serial.println(REDE);

  WiFi.mode(WIFI_STA);
  WiFi.begin(REDE, SENHA);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("WiFi conectado. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("Falha ao conectar WiFi");
  }
}

void reconnectMqtt() {

  if (WiFi.status() != WL_CONNECTED) return;
  if (client.connected()) return;

  String clientId = "Wokwi-Client-Gustavo-Stinghen";

  if (client.connect(clientId.c_str())) {

    String tSala = String(TOPICO_BASE) + "/lampadas/sala";
    String tCozinha = String(TOPICO_BASE) + "/lampadas/cozinha";
    client.subscribe(tSala.c_str());
    client.subscribe(tCozinha.c_str());
  } else {
    Serial.print(" falhou, rc=");
    Serial.println(client.state());
    delay(2000);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  message.trim();

  String tSala = String(TOPICO_BASE) + "/lampadas/sala";
  String tCozinha = String(TOPICO_BASE) + "/lampadas/cozinha";
  String top = String(topic);

  String up = message;
  up.toUpperCase();

  if (top == tSala) {
    if (up == "ON") {
      digitalWrite(LAMPADA_SALA, HIGH);
    } else if (up == "OFF") {
      digitalWrite(LAMPADA_SALA, LOW);
    }
  } else if (top == tCozinha) {
    if (up == "ON") {
      digitalWrite(LAMPADA_COZINHA, HIGH);
    } else if (up == "OFF") {
      digitalWrite(LAMPADA_COZINHA, LOW);
    }
  }
}

void publicaDados() {
  if (!client.connected()) return;

  String topicSala = String(TOPICO_BASE) + "/status/lampadas/sala";
  String topicCoz  = String(TOPICO_BASE) + "/status/lampadas/cozinha";

  char payloadSala[2];
  char payloadCozinha[2];
  int estadoSala = digitalRead(LAMPADA_SALA);
  int estadoCozinha = digitalRead(LAMPADA_COZINHA);
  payloadSala[0] = estadoSala ? '1' : '0';
  payloadSala[1] = '\0';
  payloadCozinha[0] = estadoCozinha ? '1' : '0';
  payloadCozinha[1] = '\0';

  client.publish(topicSala.c_str(), payloadSala);
  client.publish(topicCoz.c_str(), payloadCozinha);
}
