#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>

const char* ssid = "<Wifi SSID>";     // Replace with your Wi-Fi SSID
const char* password = "<Wifi Password>";           // Replace with your Wi-Fi Password

// CoinMarketCap API key
const char* apiKey = "<CoinMarketCap API key>"; // Replace with your CoinMarketCap API key

// CoinMarketCap API URL for fetching BTC, ETH, SOL prices
const char* apiURL = "https://pro-api.coinmarketcap.com/v1/cryptocurrency/listings/latest?limit=10&convert=USD";

TFT_eSPI tft = TFT_eSPI();

// Fetch and parse API response
bool fetchCryptoPrices(DynamicJsonDocument& doc) {
  HTTPClient http;
  http.begin(apiURL);
  http.addHeader("X-CMC_PRO_API_KEY", apiKey);  // Add API Key to request header
  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) { // Success
    String payload = http.getString();
    Serial.println("Fetching Price from API, payload is: " + payload);

    // Parse JSON response
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print("JSON Deserialization failed: ");
      Serial.println(error.f_str());
      http.end();
      return false;
    }

    http.end();
    return true;
  } else {
    Serial.println("Error: Failed to fetch data from CoinMarketCap.");
    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);
    http.end();
    return false;
  }
}

// Function to get price of a cryptocurrency by symbol
float getPriceByName(const String& symbol, DynamicJsonDocument& doc) {
  JsonArray data = doc["data"].as<JsonArray>(); // Get the array of cryptos
  for (JsonObject crypto : data) {
    String symbolFromAPI = crypto["symbol"].as<String>();
    if (symbolFromAPI == symbol) {
      return crypto["quote"]["USD"]["price"].as<float>(); // Return price in USD
    }
  }
  return -1; // Return -1 if the symbol is not found
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize TFT display
  tft.init();
  tft.setRotation(1); // Adjust rotation as needed
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  tft.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
    Serial.print(".");
  }
  tft.println("\nWi-Fi Connected!");
  delay(1000);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Create a JSON document object
    DynamicJsonDocument doc(8192);

    if (fetchCryptoPrices(doc)) { // Fetch and parse API response
      // Get prices for BTC, ETH, and SOL
      float btcPrice = getPriceByName("BTC", doc);
      float ethPrice = getPriceByName("ETH", doc);
      float solPrice = getPriceByName("SOL", doc);

      // Display prices on TFT
      tft.fillScreen(TFT_BLACK);

    // Bitcoin
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(55, 30);
    tft.println("Bitcoin:");
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(55, 55);
    tft.println("$" + String(btcPrice, 2));

    // Ethereum
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(55, 90);
    tft.println("Ethereum:");
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setCursor(55, 115);
    tft.println("$" + String(ethPrice, 2));

    // Solana
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(55, 150);
    tft.println("Solana:");
    tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
    tft.setCursor(55, 175);
    tft.println("$" + String(solPrice, 2));

    } else {
      tft.fillScreen(TFT_RED);
      tft.setCursor(10, 50);
      tft.println("API Error!");
    }

  } else {
    tft.fillScreen(TFT_RED);
    tft.setCursor(20, 50);
    tft.setTextSize(3);
    tft.println("Wi-Fi Disconnected!");
  }

  Serial.println("Waiting for 270 sec");
  delay(270000); // Update every 270 sec
}
