//#include <WiFi.h>
//#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Your WiFi credentials
const char* ssid = "joni";
const char* password = "bitagure123";

// Telegram bot token
const String token = "1451659082:AAFwMYzJ1za1_YiXSqjqWpppAr9oXzlfYio";

// Your Telegram user ID
const long user_id = 733099479;

// LED pin
const int ledPin = 2;

// Telegram bot object
UniversalTelegramBot bot(token, ssid, password);

// Function to turn the LED on
void turnOnLED() {
  digitalWrite(ledPin, HIGH);
}

// Function to turn the LED off
void turnOffLED() {
  digitalWrite(ledPin, LOW);
}

// Function to handle Telegram messages
void handleMessage(String message) {
  if (message == "/led_on") {
    // Turn on the LED
    turnOnLED();
    bot.sendMessage(user_id, "LED is turned on");
  } else if (message == "/led_off") {
    // Turn off the LED
    turnOffLED();
    bot.sendMessage(user_id, "LED is turned off");
  } else {
    // Reply with a message
    bot.sendMessage(user_id, "I don't understand your message");
  }
}

void setup() {
  // Set the LED pin as output
  pinMode(ledPin, OUTPUT);

  // Connect to the WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the Telegram bot
  bot.begin();
}

void loop() {
  // Check for new messages
  while (bot.getUpdates()) {
    // Handle each message
    for (int i = 0; i < bot.getUpdatesCount(); i++) {
      handleMessage(bot.getMessage(i).text);
    }
  }
}
