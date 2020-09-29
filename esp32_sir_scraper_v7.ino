#include <WiFi.h>
#include <HTTPClient.h>
#include <String.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32_MailClient.h>
#define emailSenderAccount    "qweqwe@gmail.com"
#define emailSenderPassword   "qweqwe@"
#define emailRecipient        "qweqwe@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 SIR scraper"

//core0
TaskHandle_t led;

//vars .yes a lot.
int lcdColumns = 16;
int lcdRows = 2;

float omb_si_float;
float omb_tur_float;
float omb_buon_float;
float omb_sass_float;
float omb_ist_float;
float omb_berr_float;

float omb_si_var_float;
float omb_tur_var_float;
float omb_buon_var_float;
float omb_sass_var_float;
float omb_ist_var_float;
float omb_berr_var_float;

float tim_float;

// long previousMillis = 0;
long previousMillis_http = 0;
long previousMillis_mail = 0;
long interval_http = 300000;
long interval_mail = 43200000;
// long interval_lcd = 1000;

String payload;

String omb_tur;
String omb_si;
String omb_buon;
String omb_sass;
String omb_ist;
String omb_berr;

String omb_tur_var;
String omb_si_var;
String omb_buon_var;
String omb_sass_var;
String omb_berr_var;
String omb_ist_var;


int omb_tur_led;
int omb_si_led;
int omb_buon_led;
int omb_sass_led;
int omb_berr_led;
int omb_ist_led;

int omb_tur_var_led;
int omb_si_var_led;
int omb_buon_var_led;
int omb_sass_var_led;
int omb_berr_var_led;
int omb_ist_var_led;

int omb_si_led_scal;
int omb_buon_led_scal;
int omb_sass_led_scal;
int omb_tur_led_scal;
int omb_berr_led_scal;
int omb_ist_led_scal;

int ind;

char body[300];

//wifi
const char* ssid = "qweqwe";
const char* password = "qweqwe";

//memory trick? from https://thingpulse.com/esp32-how-to-use-psram/
void logMemory() {
  log_d("Used PSRAM: %d", ESP.getPsramSize() - ESP.getFreePsram());
}
//init mail
SMTPData smtpData;

//init lcd
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);



//getvalue with ind from indexof (finded idro station)
String getValue_ind(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;
  for (int i = ind; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//double floating map from https://gist.github.com/nadavmatalon/71ccaf154bc4bd71f811289e78c65918
double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//fscale from https://playground.arduino.cc/Main/Fscale/
float fscale( float originalMin, float originalMax, float newBegin, float
              newEnd, float inputValue, float curve) {
  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;
  // condition curve parameter
  // limit range
  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;
  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function
  /*
    Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution
    Serial.println();
  */
  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }
  // Zero Refference the values
  OriginalRange = originalMax - originalMin;
  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }
  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float
  /*
    Serial.print(OriginalRange, DEC);
    Serial.print("   ");
    Serial.print(NewRange, DEC);
    Serial.print("   ");
    Serial.println(zeroRefCurVal, DEC);
    Serial.println();
  */
  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }
  if (invFlag == 0) {
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
  }
  else     // invert the ranges
  {
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }
  return rangedValue;
}

//PWM leds from https://robojax.com/learn/arduino/?vid=robojax_ESP32_LED_fade

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0 //si
#define LEDC_CHANNEL_1     1 //buon
#define LEDC_CHANNEL_2     2 //sass
#define LEDC_CHANNEL_3     3 //tur
#define LEDC_CHANNEL_4     4 //ist
#define LEDC_CHANNEL_5     5 //berr

// bit precission for LEDC timer
#define LEDC_TIMER  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN0            14 //si
#define LED_PIN1            27 //buon
#define LED_PIN2            16 //sass
#define LED_PIN3            17 //tur
#define LED_PIN4            25 //ist
#define LED_PIN5            26 //berr


void setup() {
  Serial.begin(115200);

  //memory trick?
  logMemory();
  byte* psdRamBuffer = (byte*)ps_malloc(500000);
  logMemory();
  free(psdRamBuffer);
  logMemory();

  //leds
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER);
  ledcAttachPin(LED_PIN0, LEDC_CHANNEL_0);

  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER);
  ledcAttachPin(LED_PIN1, LEDC_CHANNEL_1);

  ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER);
  ledcAttachPin(LED_PIN2, LEDC_CHANNEL_2);

  ledcSetup(LEDC_CHANNEL_3, LEDC_BASE_FREQ, LEDC_TIMER);
  ledcAttachPin(LED_PIN3, LEDC_CHANNEL_3);

  ledcSetup(LEDC_CHANNEL_4, LEDC_BASE_FREQ, LEDC_TIMER);
  ledcAttachPin(LED_PIN4, LEDC_CHANNEL_4);

  ledcSetup(LEDC_CHANNEL_5, LEDC_BASE_FREQ, LEDC_TIMER);
  ledcAttachPin(LED_PIN5, LEDC_CHANNEL_5);

  //leds on core0
  xTaskCreatePinnedToCore(
    led_code, /* Task function. */
    "led",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &led,    /* Task handle to keep track of created task */
    0);        /* pin task to core 0 */

  lcd.begin();
  lcd.backlight();
  delay(3000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connett..");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("connett...");
    delay(300);
  }
  Serial.println("Conness!");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conness!");
  delay(300);

  http(); //scrape + parse on boot
  delay(300);
  mail(); // mail on boot if theres conditions
}


//for leds on core0
void led_code( void * pvParameters ) {
leds:
  Serial.print("leds running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    leds();
  }
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long currentMillis_http = millis();
  unsigned long currentMillis_mail = millis();

  if (currentMillis_http - previousMillis_http > interval_http) {
    previousMillis_http = currentMillis_http;
    http();
  }
  if (currentMillis_mail - previousMillis_mail > interval_mail) {
    previousMillis_mail = currentMillis_mail;
    mail();
  }

  lcd.setCursor(0, 0);
  lcd.print("last update:");
  lcd.setCursor(0, 1);
  lcd.print(tim_float);
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Ponte del Garbo SI: ");
  lcd.setCursor(0, 1);
  lcd.print(omb_si_float);
  lcd.setCursor(5, 1);
  lcd.print(" m szi");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("            ");
  lcd.setCursor(0, 1);
  lcd.print(omb_si_var_float);
  lcd.setCursor(5, 1);
  lcd.print(" var");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Buonconvento: ");
  lcd.setCursor(0, 1);
  lcd.print(omb_buon_float);
  lcd.setCursor(5, 1);
  lcd.print(" m szi");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("            ");
  lcd.setCursor(0, 1);
  lcd.print(omb_buon_var_float);
  lcd.setCursor(5, 1);
  lcd.print(" var");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Sasso: ");
  lcd.setCursor(0, 1);
  lcd.print(omb_sass_float);
  lcd.setCursor(5, 1);
  lcd.print(" m szi");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("            ");
  lcd.setCursor(0, 1);
  lcd.print(omb_sass_var_float);
  lcd.setCursor(5, 1);
  lcd.print(" var");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Istia: ");
  lcd.setCursor(0, 1);
  lcd.print(omb_ist_float);
  lcd.setCursor(5, 1);
  lcd.print(" m szi");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("            ");
  lcd.setCursor(0, 1);
  lcd.print(omb_ist_var_float);
  lcd.setCursor(5, 1);
  lcd.print(" var");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Ponte Tura: ");
  lcd.setCursor(0, 1);
  lcd.print(omb_tur_float);
  lcd.setCursor(5, 1);
  lcd.print(" m szi");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("            ");
  lcd.setCursor(0, 1);
  lcd.print(omb_tur_var_float);
  lcd.setCursor(5, 1);
  lcd.print(" var");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Berrettino: ");
  lcd.setCursor(0, 1);
  lcd.print(omb_berr_float);
  lcd.setCursor(5, 1);
  lcd.print(" m szi");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("            ");
  lcd.setCursor(0, 1);
  lcd.print(omb_berr_var_float);
  lcd.setCursor(5, 1);
  lcd.print(" var");
  delay(2000);
  lcd.clear();

}
