void http() {

  String payload;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("scraping...");
  Serial.println("scraping..");
  //digitalWrite(2, HIGH);   // turn the onboard LED on

  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    HTTPClient http;

    http.begin("https://www.sir.toscana.it/monitoraggio/stazioni.php?type=idro");   //Specify the URL
    int httpCode = http.GET();                                                      //Make the request

    if (httpCode > 0) {                                                             //Check for the returning code

      payload = http.getString();
      Serial.println(httpCode);
      //Serial.println(payload);

      delay(300);
      lcd.setCursor(0, 1);
      lcd.print("OK!");
      delay(300);
    }
    http.end(); //Free the resources

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("parsing...");

    payload.replace("\"", ""); //remove quotations"
    payload.replace("&nbsp;", " "); //remove &nbsp

    //token = payload.substring(10414, 11211);      //first attemp super unreliable

    /*                                              //second attemp unreliable
      String omb_tur = getValue(payload, ',', 3180);
      String omb_si = getValue(payload, ',', 2991);
      float omb_tur_mapserv = omb_tur.toFloat();
      float omb_si_mapserv = omb_si.toFloat();
    */

    ind = payload.indexOf("TOS03005641"); //third attemp based on indexof and getvalue function ...OK
    String omb_si = getValue_ind(payload, ',', 8);
    Serial.print("Ponte del Garbo SI: ");
    Serial.print(omb_si);
    Serial.println(" [m szi] - Δh1");
    ind = payload.indexOf("TOS03005641");
    String omb_si_var = getValue_ind(payload, ',', 10);
    Serial.print("var: ");
    Serial.print(omb_si_var);
    Serial.println(" Δh1");

    ind = payload.indexOf("TOS01005672");
    String omb_buon = getValue_ind(payload, ',', 8);
    Serial.print("Buonconvento: ");
    Serial.print(omb_buon);
    Serial.println(" [m szi] - Δh1");
    ind = payload.indexOf("TOS01005672");
    String omb_buon_var = getValue_ind(payload, ',', 10);
    Serial.print("var: ");
    Serial.print(omb_buon_var);
    Serial.println(" Δh1");

    ind = payload.indexOf("TOS01005791");
    String omb_sass = getValue_ind(payload, ',', 8);
    Serial.print("Sasso: ");
    Serial.print(omb_sass);
    Serial.println(" [m szi] - Δh1");
    ind = payload.indexOf("TOS01005791");
    String omb_sass_var = getValue_ind(payload, ',', 10);
    Serial.print("var: ");
    Serial.print(omb_sass_var);
    Serial.println(" Δh1");

    ind = payload.indexOf("TOS03005831");
    String omb_tur = getValue_ind(payload, ',', 8);
    Serial.print("Ponte Tura: ");
    Serial.print(omb_tur);
    Serial.println(" [m szi] - Δh1");
    ind = payload.indexOf("TOS03005831");
    String omb_tur_var = getValue_ind(payload, ',', 10);
    Serial.print("var: ");
    Serial.print(omb_tur_var);
    Serial.println(" Δh1");

    ind = payload.indexOf("TOS01005822");
    String omb_ist = getValue_ind(payload, ',', 8);
    Serial.print("Istia: ");
    Serial.print(omb_ist);
    Serial.println(" [m szi] - Δh1");
    ind = payload.indexOf("TOS01005822");
    String omb_ist_var = getValue_ind(payload, ',', 10);
    Serial.print("var: ");
    Serial.print(omb_ist_var);
    Serial.println(" Δh1");

    ind = payload.indexOf("TOS16005841");
    String omb_berr = getValue_ind(payload, ',', 8);
    Serial.print("Berrettino: ");
    Serial.print(omb_berr);
    Serial.println(" [m szi] - Δh1");
    ind = payload.indexOf("TOS16005841");
    String omb_berr_var = getValue_ind(payload, ',', 10);
    Serial.print("var: ");
    Serial.print(omb_berr_var);
    Serial.println(" Δh1");


    ind = payload.indexOf("riferiti");
    String tim = getValue_ind(payload, ' ', 3);
    Serial.print("last read: ");
    Serial.println(tim);



    omb_si_float = omb_si.toFloat();     //float my strings in an electronic river
    omb_si_var_float = omb_si_var.toFloat();

    omb_buon_float = omb_buon.toFloat();
    omb_buon_var_float = omb_buon_var.toFloat();

    omb_sass_float = omb_sass.toFloat();
    omb_sass_var_float = omb_sass_var.toFloat();

    omb_tur_float = omb_tur.toFloat();
    omb_tur_var_float = omb_tur_var.toFloat();

    omb_ist_float = omb_ist.toFloat();
    omb_ist_var_float = omb_ist_var.toFloat();

    omb_berr_float = omb_berr.toFloat();
    omb_berr_var_float = omb_berr_var.toFloat();

    /*
        int dat_len = dat.length() + 1;

        char dat_array[dat_len];

        dat.toCharArray(dat_array, dat_len);
    */
    tim_float = tim.toFloat();


    omb_si_led = mapf(omb_si_float, 0.3, 2.0, 0.0, 255.0); //floating point mapped from 0.3 is the "alarm" level
    omb_buon_led = mapf(omb_buon_float, 0.3, 2.0, 0.0, 255.0);
    omb_sass_led = mapf(omb_sass_float, 0.3, 2.0, 0.0, 255.0);
    omb_tur_led = mapf(omb_tur_float, 0.3, 2.0, 0.0, 255.0);
    omb_ist_led = mapf(omb_ist_float, 0.3, 2.0, 0.0, 255.0);
    omb_berr_led = mapf(omb_berr_float, 0.3, 2.0, 0.0, 255.0);

    omb_si_var_led = mapf(omb_si_var_float, 0, 1.0, 1000.0, 50.0); //floating point mapped from level of variation in 1h
    omb_buon_var_led = mapf(omb_buon_var_float, 0, 1.0, 1000.0, 50.0);
    omb_sass_var_led = mapf(omb_sass_var_float, 0, 1.0, 1000.0, 50.0);
    omb_tur_var_led = mapf(omb_tur_var_float, 0, 1.0, 1000.0, 50.0);
    omb_ist_var_led = mapf(omb_ist_var_float, 0, 1.0, 1000.0, 50.0);
    omb_berr_var_led = mapf(omb_berr_var_float, 0, 1.0, 1000.0, 50.0);

    omb_si_led = constrain(omb_si_led, 0, 255);  // limits range of sensor values to between 0 and 255 to avoid neg values
    omb_buon_led = constrain(omb_buon_led, 0, 255);
    omb_sass_led = constrain(omb_sass_led, 0, 255);
    omb_tur_led = constrain(omb_tur_led, 0, 255);
    omb_ist_led = constrain(omb_ist_led, 0, 255);
    omb_berr_led = constrain(omb_berr_led, 0, 255);

    omb_si_var_led = constrain(omb_si_var_led, 50, 1000);  // limits range of sensor values to between 0 and 1000 to avoid neg values?
    omb_buon_var_led = constrain(omb_buon_var_led, 50, 1000);
    omb_sass_var_led = constrain(omb_sass_var_led, 50, 1000);
    omb_tur_var_led = constrain(omb_tur_var_led, 50, 1000);
    omb_ist_var_led = constrain(omb_ist_var_led, 50, 1000);
    omb_berr_var_led = constrain(omb_berr_var_led, 50, 1000);

    omb_si_led_scal = fscale( 0, 255, 0, 255, omb_si_led, -2); //fscale to avoid exponential led brightness
    omb_buon_led_scal = fscale( 0, 255, 0, 255, omb_buon_led, -2);
    omb_sass_led_scal = fscale( 0, 255, 0, 255, omb_sass_led, -2);
    omb_tur_led_scal = fscale( 0, 255, 0, 255, omb_tur_led, -2);
    omb_ist_led_scal = fscale( 0, 255, 0, 255, omb_ist_led, -2);
    omb_berr_led_scal = fscale( 0, 255, 0, 255, omb_berr_led, -2);


    lcd.setCursor(0, 1);
    lcd.print("OK!");
    delay(300);

    //print debug
    Serial.println("-led vals-");
    Serial.println(omb_si_led);
    Serial.println(omb_buon_led);
    Serial.println(omb_sass_led);
    Serial.println(omb_tur_led);
    Serial.println(omb_ist_led);
    Serial.println(omb_berr_led);
    Serial.println("-fscaled led vals-");
    Serial.println(omb_si_led_scal);
    Serial.println(omb_buon_led_scal);
    Serial.println(omb_sass_led_scal);
    Serial.println(omb_tur_led_scal);
    Serial.println(omb_ist_led_scal);
    Serial.println(omb_berr_led_scal);

    Serial.println("mill led vars-");
    Serial.println(omb_si_var_led);
    Serial.println(omb_buon_var_led);
    Serial.println(omb_sass_var_led);
    Serial.println(omb_tur_var_led);
    Serial.println(omb_ist_var_led);
    Serial.println(omb_berr_var_led);

  }
}
