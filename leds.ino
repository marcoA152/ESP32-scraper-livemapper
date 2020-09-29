void leds() {

  ledcWrite(LEDC_CHANNEL_0, omb_si_led_scal);
  ledcWrite(LEDC_CHANNEL_1, omb_buon_led_scal);
  ledcWrite(LEDC_CHANNEL_2, omb_sass_led_scal);
  ledcWrite(LEDC_CHANNEL_3, omb_tur_led_scal);
  ledcWrite(LEDC_CHANNEL_4, omb_ist_led_scal);
  ledcWrite(LEDC_CHANNEL_5, omb_berr_led_scal);
  delay(30); //without this esp32 crashes after boot (?)

  if (omb_si_var_float > 0.2) {            //blink if is over alarm level of 1h variation
    ledcWrite(LEDC_CHANNEL_0, omb_si_led_scal);
    delay(omb_si_var_led);
    ledcWrite(LEDC_CHANNEL_0, 0);
    delay(omb_si_var_led);
  }
  if (omb_buon_var_float > 0.2) {
    ledcWrite(LEDC_CHANNEL_1, omb_buon_led_scal);
    delay(omb_buon_var_led);
    ledcWrite(LEDC_CHANNEL_1, 0);
    delay(omb_buon_var_led);
  }
  if (omb_sass_var_float > 0.2) {
    ledcWrite(LEDC_CHANNEL_2, omb_sass_led_scal);
    delay(omb_sass_var_led);
    ledcWrite(LEDC_CHANNEL_2, 0);
    delay(omb_sass_var_led);
  }
  if (omb_tur_var_float > 0.2) {
    ledcWrite(LEDC_CHANNEL_3, omb_tur_led_scal);
    delay(omb_tur_var_led);
    ledcWrite(LEDC_CHANNEL_3, 0);
    delay(omb_tur_var_led);
  }
  if (omb_ist_var_float > 0.2) {
    ledcWrite(LEDC_CHANNEL_4, omb_ist_led_scal);
    delay(omb_ist_var_led);
    ledcWrite(LEDC_CHANNEL_4, 0);
    delay(omb_ist_var_led);
  }
  if (omb_berr_var_float > 0.2) {
    ledcWrite(LEDC_CHANNEL_5, omb_berr_led_scal);
    delay(omb_berr_var_led);
    ledcWrite(LEDC_CHANNEL_5, 0);
    delay(omb_berr_var_led);
  }
}
