#ifndef CONFIG_H
#define CONFIG_H

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_CLK_PIN    18
#define OLED_MOSI_PIN   23
#define OLED_DC_PIN     21
#define OLED_CS_PIN     22
#define OLED_RESET_PIN  -1

// ================= MQTT =================

#define MQTT_SERVER     "c72603c7220246669502e424fc44418a.s1.eu.hivemq.cloud"
#define MQTT_PORT       8883

#define MQTT_CLIENT_ID  "ai_health_monitor_yichi_01"

#define MQTT_TOPIC      "health/monitor/data"

#endif