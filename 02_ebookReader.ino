#include <M5StickCPlus2.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "BaoNguyen";
const char* password = "so40ngo115";

const char* url = "https://webcuatui.com/demo.html";

String ebookContent = "";
int currentPage = 0;
const int charsPerPage = 150; // Maximum characters per page

void fetchData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      ebookContent = http.getString();
    } else {
      ebookContent = "Failed to fetch data";
    }
    http.end();
  } else {
    ebookContent = "WiFi not connected";
  }
}

void displayPage(int page) {
  M5.Lcd.fillScreen(TFT_WHITE);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.setCursor(0, 0);

  int startChar = page * charsPerPage;
  int endChar = startChar + charsPerPage;

  if (startChar < ebookContent.length()) {
    String pageContent = ebookContent.substring(startChar, endChar);
    // Display the content with line spacing
    int cursorY = 0;
    for (int i = 0; i < pageContent.length(); i++) {
      if (pageContent[i] == '\n' || (i > 0 && i % 20 == 0)) { // 20 characters per line as an example
        cursorY += 16 + 2; // Text height + line spacing (2 pixels)
        M5.Lcd.setCursor(0, cursorY);
      }
      M5.Lcd.print(pageContent[i]);
    }
  } else {
    M5.Lcd.print("End of content");
  }
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Lcd.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("\nConnected");

  // Fetch data from URL
  fetchData();

  // Display the first page
  displayPage(currentPage);
}

void loop() {
  // Update button states
  M5.update();

  // Check if button A is pressed (next page)
  if (M5.BtnA.wasPressed()) {
    currentPage++;
    displayPage(currentPage);
  }

  // Check if button B is pressed (previous page)
  if (M5.BtnB.wasPressed()) {
    if (currentPage > 0) {
      currentPage--;
    }
    displayPage(currentPage);
  }
}

