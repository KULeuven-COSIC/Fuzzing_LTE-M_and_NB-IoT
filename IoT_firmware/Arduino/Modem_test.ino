#include <MKRNB.h>

#define ITER_NUM 10000

void setup() {
  Serial.begin(9600);
  while (!Serial);
  //MODEM.debug();
  MODEM.begin();
  while (!MODEM.noop());
}

String send_cmd(String cmd, bool expect_non_empty_response, int max_time=10000) {
    String response;
    MODEM.send(cmd);
    Serial.print("Command: ");
    Serial.print(cmd);
    Serial.print("; ");
    for (int i = 0; i < 3; i++) { // Necessary as sometimes MODEM.waitForResponse returns an empty string and the real response is given after next command
      int ret = MODEM.waitForResponse(max_time, &response);
      if (ret == 1) {
        Serial.print("Response: ");
        Serial.println(response);
      } else {
        Serial.print("No response (ret=");
        Serial.print(ret);
        Serial.println(")");
        break;
      }
      if (!response.isEmpty() || !expect_non_empty_response) break;
      delay(100);
    }
    return response;
}

void find_networks() {
    send_cmd("AT+CFUN=1", false);
    delay(1000);
    send_cmd("AT+COPS=?",true,180000);
}

void connect_to_network() {
    delay(1000);
    send_cmd("AT+CFUN=1", false);
    delay(1000);
    send_cmd("AT+COPS=1,2,\"00101\"", false, 100000);
    delay(100000);
    send_cmd("AT+COPS?", true);
}

bool apply()
{
  Serial.print("Applying changes and saving configuration: ");
  MODEM.send("AT+CFUN=15");
  MODEM.waitForResponse(5000);
  delay(5000);

  do {
    delay(1000);
    MODEM.noop();
  } while (MODEM.waitForResponse(1000) != 1);

  Serial.println("done.");
  
  return true;
}

void loop() {
  // connect_to_network();
  // while(true);
  //find_networks();
  // while(true);
  send_cmd("AT+URAT=7",false);
  apply();

  const String MODULE_NAME = "<LTE_FUZZER> ";
  const String NEW_ITER = "LTE_NEW_ITERATION";
  const String LTE_CONNECTED = "LTE_CONNECTED";
  const String LTE_NOT_CONNECTED = "LTE_NOT_CONNECTED";
  const String LTE_CRASH = "LTE_CRASH";

  String response;
  for (int i = 0; i < ITER_NUM; i++) {
    if (!MODEM.noop()) {
      Serial.print(MODULE_NAME); Serial.println(LTE_CRASH);
      MODEM.hardReset();
      Serial.println("Hard reset successful");
      MODEM.begin();
      continue;
    }
    Serial.print(MODULE_NAME); Serial.print(NEW_ITER); Serial.print(" "); Serial.println(i);
    response = send_cmd("AT+CFUN?", true);
    //if (response.isEmpty()) continue;
    response = send_cmd("AT+CFUN=1", false);
    delay(100);
    response = send_cmd("AT+CFUN?", true);
    //if (response.isEmpty()) continue;
    //response = send_cmd("AT+COPS=1,2,\"00101\",9", false);
    delay(3000);
    response = send_cmd("AT+COPS?", true);
    //if (response.isEmpty()) continue;
    if (response.lastIndexOf("00101") != -1) {
      Serial.print(MODULE_NAME); Serial.println(LTE_CONNECTED);
    } else {
      Serial.print(MODULE_NAME); Serial.println(LTE_NOT_CONNECTED);
    }
    send_cmd("AT+CFUN=4", false);
    delay(2000);
    send_cmd("AT+CFUN?", true);
    //if (response.isEmpty()) continue;
    send_cmd("AT+COPS?", true);
  }

  while (true);
}

