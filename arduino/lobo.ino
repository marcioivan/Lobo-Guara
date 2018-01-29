#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


const int proximity_pin = 16; //d0
const int latido_pin = 5; //d1
const int olhobranco_pin = 4; //d2
const int buttoncabeca_pin = 14; //d5
const int buttoncorpo_pin = 12; //d6
const int buttonpata_pin = 13; //d7
const int buttonrabo_pin = 15; //d8
int proximity_last = 0;
int cabeca_last = 0;
int corpo_last = 0;
int pata_last = 0;
int rabo_last = 0;

//WiFI
#define WIFI_SSID       "joaquim"
#define WIFI_PASS       "1234567oitonovezero"

//Adafruit IO
#define AIO_SERVER       "io.adafruit.com"
#define AIO_SERVERPORT   1883
#define AIO_USERNAME     "krautz"
#define AIO_KEY          "1619a03e813b499987b0ebeb3f94e503"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish proximity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/proximity");
Adafruit_MQTT_Publish buttoncabeca = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/cabeca");
Adafruit_MQTT_Publish buttoncorpo = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/corpo");
Adafruit_MQTT_Publish buttonpata = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pata");
Adafruit_MQTT_Publish buttonrabo = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/rabo");
Adafruit_MQTT_Subscribe latido = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/latido");
void MQTT_connect();

void setup()
{
  pinMode(proximity_pin, INPUT);
  pinMode(latido_pin, OUTPUT);
  pinMode(olhobranco_pin, OUTPUT);
  pinMode(buttoncabeca_pin, INPUT);
  pinMode(buttoncorpo_pin, INPUT);
  pinMode(buttonpata_pin, INPUT);
  pinMode(buttonrabo_pin, INPUT);

  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mqtt.subscribe(&latido);
  digitalWrite (olhobranco_pin, HIGH);
}

void loop()
{
  MQTT_connect();

  if(digitalRead(proximity_pin) == LOW && proximity_last == 0)
  {
    proximity.publish(1);
    //digitalWrite(olhobranco_pin, LOW);
    //digitalWrite(latido_pin, HIGH);
    Serial.println("proximity HIGH");
    proximity_last = 1;
  }

  if(digitalRead(proximity_pin) == HIGH && proximity_last == 1)
  {
    proximity.publish(0);
    //digitalWrite(latido_pin, LOW);
    //digitalWrite(olhobranco_pin, HIGH);
    Serial.println("proximity LOW");
    proximity_last = 0;
    delay(100);
  }

  if(digitalRead(buttoncabeca_pin) == HIGH && cabeca_last == 0)
  {
    buttoncabeca.publish(1);
    Serial.println("buttoncabeca HIGH");
    cabeca_last = 1;
  }

  if(digitalRead(buttoncabeca_pin) == LOW && cabeca_last == 1)
  {
    buttoncabeca.publish(0);
    Serial.println("buttoncabeca LOW");
    cabeca_last = 0;
    delay(100);
  }

  if(digitalRead(buttoncorpo_pin) == HIGH && corpo_last == 0)
  {
    buttoncorpo.publish(1);
    Serial.println("buttoncorpo HIGH");
    corpo_last = 1;
  }

  if(digitalRead(buttoncorpo_pin) == LOW && corpo_last == 1)
  {
    buttoncorpo.publish(0);
    Serial.println("buttoncorpo LOW");
    corpo_last = 0;
  }

  if(digitalRead(buttonpata_pin) == HIGH && pata_last == 0)
  {
    buttonpata.publish(1);
    Serial.println("buttonpata HIGH");
    pata_last = 1;
  }

  if(digitalRead(buttonpata_pin) == LOW && pata_last == 1)
  {
    buttonpata.publish(0);
    Serial.println("buttonpata LOW");
    pata_last = 0;
  }

  if(digitalRead(buttonrabo_pin) == HIGH && rabo_last == 0)
  {
    buttonrabo.publish(1);
    Serial.println("buttonrabo HIGH");
    rabo_last = 1;                                                                                                                  delay(300);
  }

  if(digitalRead(buttonrabo_pin) == LOW && rabo_last == 1)
  {
    buttonrabo.publish(0);
    Serial.println("buttonrabo LOW");
    rabo_last = 0;                                                                                                                      delay(300);
  }

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100)))
  {
    if (subscription == &latido)
    {
      Serial.print("latido: ");
      Serial.println((char *)latido.lastread);
      if (strcmp((char *)latido.lastread, "1") == 0)
      {
        digitalWrite(latido_pin, HIGH);
        digitalWrite(olhobranco_pin, LOW);
      }
      if (strcmp((char *)latido.lastread, "0") == 0)
      {
        digitalWrite(latido_pin, LOW);
        digitalWrite(olhobranco_pin, HIGH);
      }
    }

    if (subscription == &proximity)
    {
      Serial.print("proximity: ");
      Serial.println((char *)proximity.lastread);
      if (strcmp((char *)proximity.lastread, "1") == 0)
      {
        digitalWrite(latido_pin, HIGH);
        digitalWrite(olhobranco_pin, LOW);
      }
      if (strcmp((char *)proximity.lastread, "0") == 0)
      {
        digitalWrite(latido_pin, LOW);
        digitalWrite(olhobranco_pin, HIGH);
      }
    }
  }
}

void MQTT_connect()
{
  int8_t ret;

  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0)
    {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
