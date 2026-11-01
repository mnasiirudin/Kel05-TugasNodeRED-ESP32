
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define KELOMPOK "klp5"
#define ROOM "ruang3"
#define WIFI_SSID "SSID_LAB"
#define WIFI_PASS "PASSWORD_LAB"
#define BROKER_IP "IPLAPTOP"
#define MQTT_USER "klp5"
#define MQTT_PASS "klp5"

const uint16_t BROKER_PORT = 1883;

// ===== TOPIC & CLIENT ID dibentuk dari KELOMPOK + ROOM (E2, E7) =====
#define PREFIX "pens/" KELOMPOK "/" ROOM
#define CLIENT_ID "esp32-" KELOMPOK "-" ROOM

const char *T_TEMP = PREFIX "/temperature";
const char *T_HUM = PREFIX "/humidity";
const char *T_L1 = PREFIX "/lamp1";
const char *T_L2 = PREFIX "/lamp2";
const char *T_STATUS = PREFIX "/status";
// Nilai tambah: konfirmasi keadaan lampu (retain)
const char *T_L1_STATE = PREFIX "/lamp1/state";
const char *T_L2_STATE = PREFIX "/lamp2/state";

#define POT_TEMP_PIN 36 // pot suhu  -> dianggap 20..40 °C
#define POT_HUM_PIN 39  // pot humid -> dianggap 0..100 %
const int L1_PIN = 14;
const int L2_PIN = 27;

float readPotAvg(int pin)
{
    long sum = 0;
    for (int i = 0; i < 16; i++)
        sum += analogRead(pin);
    return sum / 16.0;
}

void sensorBegin()
{
    analogReadResolution(12);
    analogSetPinAttenuation(POT_TEMP_PIN, ADC_11db);
    analogSetPinAttenuation(POT_HUM_PIN, ADC_11db);
}

bool readSensor(float &t, float &h)
{
    t = 20.0 + (readPotAvg(POT_TEMP_PIN) / 4095.0) * 20.0;
    h = (readPotAvg(POT_HUM_PIN) / 4095.0) * 100.0;
    return true;
}

WiFiClient espClient;
PubSubClient mqtt(espClient);

// ===== INTERVAL (ms) =====
const unsigned long WIFI_RETRY_MS = 15000;
const unsigned long MQTT_RETRY_MS = 5000;
const unsigned long PUBLISH_MS = 5000; // E5

unsigned long lastWifiTry = 0;
unsigned long lastMqttTry = 0;
unsigned long lastPublish = 0;

bool wasWifiConnected = false;
bool wasMqttConnected = false;

bool lamp1On = false;
bool lamp2On = false;
bool lampStateDirty = true; // true = keadaan lampu perlu dikirim ke broker

// ===== E6: hanya "1" / "0" yang diproses, payload lain diabaikan =====
void handleLamp(int pin, const char *name, bool &state, const String &msg)
{
    if (msg == "1")
        state = true;
    else if (msg == "0")
        state = false;
    else
    {
        Serial.printf("⚠️ %s: payload '%s' diabaikan\n", name, msg.c_str());
        return;
    }

    digitalWrite(pin, state ? HIGH : LOW);
    Serial.printf("💡 %s: %s\n", name, state ? "ON" : "OFF");
    lampStateDirty = true; // dikirim dari loop(), bukan dari dalam callback
}

// ===== CALLBACK pesan masuk =====
void onMessage(char *topic, byte *payload, unsigned int length)
{
    String msg;
    for (unsigned int i = 0; i < length; i++)
        msg += (char)payload[i];

    Serial.printf("MQTT >> [%s] %s\n", topic, msg.c_str());

    if (strcmp(topic, T_L1) == 0)
        handleLamp(L1_PIN, "Lampu 1", lamp1On, msg);
    else if (strcmp(topic, T_L2) == 0)
        handleLamp(L2_PIN, "Lampu 2", lamp2On, msg);
}

// ===== E4: dipanggil setiap kali (ulang) tersambung ke broker =====
void onMqttConnected()
{
    Serial.println("MQTT Connection established.");

    mqtt.subscribe(T_L1, 1);
    mqtt.subscribe(T_L2, 1);
    mqtt.publish(T_STATUS, "online", true); // retain

    lampStateDirty = true; // kirim ulang keadaan lampu setelah (re)connect
}

// ===== WIFI (non-blocking) =====
void handleWifi(unsigned long now)
{
    bool conn = (WiFi.status() == WL_CONNECTED);

    if (conn && !wasWifiConnected)
    {
        Serial.printf("✅ WiFi OK | IP: %s | RSSI: %d\n",
                      WiFi.localIP().toString().c_str(), WiFi.RSSI());
        WiFi.setSleep(false);
        WiFi.setTxPower(WIFI_POWER_8_5dBm);
    }
    else if (!conn && wasWifiConnected)
    {
        Serial.println("⚠️ WiFi putus");
        lastWifiTry = now; // beri waktu auto-reconnect bekerja dulu
    }
    wasWifiConnected = conn;

    if (!conn && now - lastWifiTry >= WIFI_RETRY_MS)
    {
        lastWifiTry = now;
        Serial.println("🔄 Mencoba konek WiFi...");
        WiFi.disconnect();
        WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
}

// ===== MQTT (non-blocking: hanya mencoba tiap MQTT_RETRY_MS) =====
bool connectMqtt()
{
    Serial.printf("MQTT: connect ke %s sebagai %s ... ", BROKER_IP, CLIENT_ID);

    // E3: LWT -> topic status, "offline", QoS 0, retain
    bool ok = mqtt.connect(CLIENT_ID, MQTT_USER, MQTT_PASS,
                           T_STATUS, 0, true, "offline");
    if (!ok)
    {
        // rc=4 user/pass salah, rc=5 tidak diizinkan, rc=-2 broker tidak terjangkau
        Serial.printf("gagal, rc=%d\n", mqtt.state());
        return false;
    }

    Serial.println("OK ✅");
    return true;
}

void handleMqtt(unsigned long now)
{
    if (wasMqttConnected && !mqtt.connected())
    {
        wasMqttConnected = false;
        Serial.printf("❌ MQTT putus | state=%d wifi=%d rssi=%d heap=%u\n",
                      mqtt.state(), WiFi.status(), WiFi.RSSI(), ESP.getFreeHeap());
    }

    if (WiFi.status() != WL_CONNECTED)
        return;

    if (!mqtt.connected())
    {
        if (now - lastMqttTry >= MQTT_RETRY_MS)
        {
            lastMqttTry = now;
            if (connectMqtt())
            {
                wasMqttConnected = true;
                onMqttConnected();
            }
        }
    }
    else
    {
        mqtt.loop();
    }
}

// ===== Nilai tambah: konfirmasi keadaan lampu =====
void handleLampState()
{
    if (!lampStateDirty || !mqtt.connected())
        return;

    bool ok1 = mqtt.publish(T_L1_STATE, lamp1On ? "1" : "0", true);
    bool ok2 = mqtt.publish(T_L2_STATE, lamp2On ? "1" : "0", true);
    if (ok1 && ok2)
        lampStateDirty = false;
}

// ===== E5: publish sensor tiap 5 detik pakai millis(), 1 desimal =====
void handlePublish(unsigned long now)
{
    if (now - lastPublish < PUBLISH_MS)
        return;
    lastPublish = now;

    float t, h;
    if (!readSensor(t, h))
    {
        Serial.println("⚠️ Gagal membaca sensor (tidak dikirim)");
        return;
    }

    if (mqtt.connected())
    {
        mqtt.publish(T_TEMP, String(t, 1).c_str());
        mqtt.publish(T_HUM, String(h, 1).c_str());
        Serial.printf("📤 [PUB] Temp: %.1f °C | Hum: %.1f %%\n", t, h);
    }
    else
    {
        Serial.printf("📴 (offline) Temp: %.1f °C | Hum: %.1f %%\n", t, h);
    }
}

void setup()
{
    Serial.begin(115200);
    delay(300);

    pinMode(L1_PIN, OUTPUT);
    pinMode(L2_PIN, OUTPUT);
    digitalWrite(L1_PIN, LOW);
    digitalWrite(L2_PIN, LOW);

    sensorBegin();

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    mqtt.setServer(BROKER_IP, BROKER_PORT);
    mqtt.setCallback(onMessage);
    mqtt.setKeepAlive(15);    // LWT terdeteksi broker sekitar 15-25 detik (T9)
    mqtt.setSocketTimeout(3); // connect() tidak menahan loop lama

    Serial.printf("\n=== Mulai | %s | broker %s:%u ===\n", PREFIX, BROKER_IP, BROKER_PORT);
}

void loop()
{
    unsigned long now = millis();

    handleWifi(now);
    handleMqtt(now);
    handleLampState();
    handlePublish(now);
}
