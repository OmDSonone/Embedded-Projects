#include <ESP8266WiFi.h>

const char* ssid = "Om";
const char* password = "12345678";

WiFiServer server(80);

// Slot states and timing
bool slot1 = false, slot2 = false, slot3 = false, slot4 = false;
unsigned long slot1Start = 0, slot2Start = 0, slot3Start = 0, slot4Start = 0;
unsigned long used1 = 0, used2 = 0, used3 = 0, used4 = 0;
int amount1 = 0, amount2 = 0, amount3 = 0, amount4 = 0;

// UPI ID
String upiID = "yourupiid@upi"; // Replace with your actual UPI ID

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected. IP: " + WiFi.localIP().toString());
  server.begin();
}

// Function to generate fixed UPI QR code with 1 rupee
String generateUPIQRCode() {
  String name = "Parking Payment";
  String txnNote = "Parking Slot";
  String amountStr = "1"; // Fixed amount of 1 rupee
  String upiLink = "upi://pay?pa=" + upiID + "&pn=" + name + "&tn=" + txnNote + "&am=" + amountStr + "&cu=INR";
  return upiLink;
}

void sendSlotState() {
  String state = String(slot1) + "," + String(slot2) + "," + String(slot3) + "," + String(slot4);
  Serial.println(state);
}

// Reset function to reset the slot after payment
void resetSlot(int slotNum) {
  switch (slotNum) {
    case 1:
      slot1 = false;
      used1 = 0;
      amount1 = 0;
      break;
    case 2:
      slot2 = false;
      used2 = 0;
      amount2 = 0;
      break;
    case 3:
      slot3 = false;
      used3 = 0;
      amount3 = 0;
      break;
    case 4:
      slot4 = false;
      used4 = 0;
      amount4 = 0;
      break;
  }
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available()) delay(1);

  String request = client.readStringUntil('\r');
  client.flush();

  unsigned long currentMillis = millis();

  // Handle slot toggling and time calculation
  if (request.indexOf("/SLOT1=TOGGLE") != -1) {
    if (!slot1) slot1Start = currentMillis;
    else {
      used1 = (currentMillis - slot1Start) / 1000;
      amount1 = (used1 / 60) + ((used1 % 60) > 0 ? 1 : 0); // 1 rupee per minute
    }
    slot1 = !slot1;
  }

  if (request.indexOf("/SLOT2=TOGGLE") != -1) {
    if (!slot2) slot2Start = currentMillis;
    else {
      used2 = (currentMillis - slot2Start) / 1000;
      amount2 = (used2 / 60) + ((used2 % 60) > 0 ? 1 : 0);
    }
    slot2 = !slot2;
  }

  if (request.indexOf("/SLOT3=TOGGLE") != -1) {
    if (!slot3) slot3Start = currentMillis;
    else {
      used3 = (currentMillis - slot3Start) / 1000;
      amount3 = (used3 / 60) + ((used3 % 60) > 0 ? 1 : 0);
    }
    slot3 = !slot3;
  }

  if (request.indexOf("/SLOT4=TOGGLE") != -1) {
    if (!slot4) slot4Start = currentMillis;
    else {
      used4 = (currentMillis - slot4Start) / 1000;
      amount4 = (used4 / 60) + ((used4 % 60) > 0 ? 1 : 0);
    }
    slot4 = !slot4;
  }

  sendSlotState();

  // Calculate elapsed time for each slot
  int elapsed1 = slot1 ? (currentMillis - slot1Start) / 1000 : 0;
  int elapsed2 = slot2 ? (currentMillis - slot2Start) / 1000 : 0;
  int elapsed3 = slot3 ? (currentMillis - slot3Start) / 1000 : 0;
  int elapsed4 = slot4 ? (currentMillis - slot4Start) / 1000 : 0;

  // Start building the HTTP response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE HTML><html><head><title>Parking Slots</title>");
  client.println("<style>body{font-family:sans-serif;text-align:center;} .slot{padding:20px;margin:10px;display:inline-block;width:140px;color:#fff;border-radius:10px;} .green{background:green;} .red{background:red;} span{display:block;margin-top:5px;font-size:18px;}</style>");
  client.println("<script>");
  client.printf("let times = [%d,%d,%d,%d];\n", elapsed1, elapsed2, elapsed3, elapsed4);
  client.printf("let active = [%d,%d,%d,%d];\n", slot1, slot2, slot3, slot4);
  client.println(R"rawliteral(
    function updateTimers() {
      for (let i = 0; i < 4; i++) {
        let el = document.getElementById("timer" + (i+1));
        if (active[i]) {
          times[i]++;
          let mins = Math.floor(times[i] / 60);
          let secs = times[i] % 60;
          el.innerText = (mins < 10 ? "0" : "") + mins + ":" + (secs < 10 ? "0" : "") + secs;
        } else {
          el.innerText = "";
        }
      }
    }
    setInterval(updateTimers, 1000);
  )rawliteral");
  client.println("</script></head><body>");

  client.println("<h1>Slot Selection</h1>");

  // Slot 1
  client.print("<a href=\"/SLOT1=TOGGLE\"><div class='slot ");
  client.print(slot1 ? "red" : "green");
  client.print("'>Slot 1<span id='timer1'>");
  client.print(slot1 ? "00:00" : "");
  client.print("</span>");
  if (!slot1 && amount1 > 0) {
    client.print("<div>Used ");
    client.print(amount1);
    client.print(" min = &#8377;");
    client.print(amount1);
    client.print("</div>");
    String qrCodeLink1 = generateUPIQRCode(); // Always generate QR for 1 rupee
    client.print("<div><img src='https://api.qrserver.com/v1/create-qr-code/?data=" + qrCodeLink1 + "&size=150x150' /></div>");
    resetSlot(1); // Reset Slot 1 after payment
  }
  client.print("</div></a>");

  // Slot 2
  client.print("<a href=\"/SLOT2=TOGGLE\"><div class='slot ");
  client.print(slot2 ? "red" : "green");
  client.print("'>Slot 2<span id='timer2'>");
  client.print(slot2 ? "00:00" : "");
  client.print("</span>");
  if (!slot2 && amount2 > 0) {
    client.print("<div>Used ");
    client.print(amount2);
    client.print(" min = &#8377;");
    client.print(amount2);
    client.print("</div>");
    String qrCodeLink2 = generateUPIQRCode(); // Always generate QR for 1 rupee
    client.print("<div><img src='https://api.qrserver.com/v1/create-qr-code/?data=" + qrCodeLink2 + "&size=150x150' /></div>");
    resetSlot(2); // Reset Slot 2 after payment
  }
  client.print("</div></a>");

  // Slot 3
  client.print("<a href=\"/SLOT3=TOGGLE\"><div class='slot ");
  client.print(slot3 ? "red" : "green");
  client.print("'>Slot 3<span id='timer3'>");
  client.print(slot3 ? "00:00" : "");
  client.print("</span>");
  if (!slot3 && amount3 > 0) {
    client.print("<div>Used ");
    client.print(amount3);
    client.print(" min = &#8377;");
    client.print(amount3);
    client.print("</div>");
    String qrCodeLink3 = generateUPIQRCode(); // Always generate QR for 1 rupee
    client.print("<div><img src='https://api.qrserver.com/v1/create-qr-code/?data=" + qrCodeLink3 + "&size=150x150' /></div>");
    resetSlot(3); // Reset Slot 3 after payment
  }
  client.print("</div></a>");

  // Slot 4
  client.print("<a href=\"/SLOT4=TOGGLE\"><div class='slot ");
  client.print(slot4 ? "red" : "green");
  client.print("'>Slot 4<span id='timer4'>");
  client.print(slot4 ? "00:00" : "");
  client.print("</span>");
  if (!slot4 && amount4 > 0) {
    client.print("<div>Used ");
    client.print(amount4);
    client.print(" min = &#8377;");
    client.print(amount4);
    client.print("</div>");
    String qrCodeLink4 = generateUPIQRCode(); // Always generate QR for 1 rupee
    client.print("<div><img src='https://api.qrserver.com/v1/create-qr-code/?data=" + qrCodeLink4 + "&size=150x150' /></div>");
    resetSlot(4); // Reset Slot 4 after payment
  }
  client.print("</div></a>");

  client.println("</body></html>");
  client.stop();
}
