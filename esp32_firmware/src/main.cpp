#include <Arduino.h>
#include <WiFi.h>

#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/bool.h>

// ---- A ADAPTER ----
const char* WIFI_SSID = "Roots 05";
const char* WIFI_PASS = "Roots@05";
const char* AGENT_IP  = "192.168.1.37";   // IP de ton PC (hostname -I)
const uint16_t AGENT_PORT = 8888;

const int LED_PIN = 2; // LED bleue integree

rcl_subscription_t subscriber;
std_msgs__msg__Bool msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define RCCHECK(fn) { rcl_ret_t rc = fn; if (rc != RCL_RET_OK) { error_loop(); } }

void error_loop() {
  while (1) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

void led_callback(const void *msgin) {
  const std_msgs__msg__Bool *led_msg = (const std_msgs__msg__Bool *)msgin;
  digitalWrite(LED_PIN, led_msg->data ? HIGH : LOW);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);

  IPAddress agent_ip;
  agent_ip.fromString(AGENT_IP);
  set_microros_wifi_transports((char*)WIFI_SSID, (char*)WIFI_PASS, agent_ip, AGENT_PORT);

  delay(2000);

  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "esp32_led_node", "", &support));

  RCCHECK(rclc_subscription_init_default(
      &subscriber,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
      "/led_cmd"));

  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &led_callback, ON_NEW_DATA));
}

void loop() {
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
}
/*


// La LED intégrée sur la majorité des ESP32 WROOM est sur la broche GPIO 2.
// Si tu branches une LED externe, change ce numéro par la broche utilisée (ex: 23).
#define LED_PIN 2

void setup() {
  // Initialisation de la communication série pour le débogage
  Serial.begin(115200);
  
  // Configuration de la broche de la LED en mode Sortie
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("--- Test de la LED ESP32 demarre ---");
}

void loop() {
  // Allume la LED
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED : ALLUMEE");
  delay(1000); // Attend 1 seconde

  // Éteint la LED
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED : ETEINTE");
  delay(1000); // Attend 1 seconde
}*/