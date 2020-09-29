//from https://www.survivingwithandroid.com/send-email-using-esp32-smtp-server/ and understand use of sprintf with c++ reference
void mail() {
  if (omb_tur_var_float > 0.8 || omb_si_var_float > 0.8 || omb_sass_var_float > 0.8 || omb_buon_var_float > 0.8 || omb_ist_var_float > 0.8 || omb_berr_var_float > 0.8) {  //"alarm" levels the river

    sprintf(body, "<h2>livelli</h2>  <p>Ponte del Garbo SI: <strong>%.2f </strong>var&nbsp;<strong>%.2f </strong></p>  <p>Buonconvento: <strong>%.2f </strong>var&nbsp;<strong>%.2f</strong></p>  <p>Sasso: <strong>%.2f </strong>var&nbsp;<strong>%.2f</strong></p>  <p>Istia: <strong>%.2f </strong>var&nbsp;<strong>%.2f</strong></p>  <p>Ponte Tura: <strong>%.2f </strong>var&nbsp;<strong>%.2f</strong></p>  <p>Berrettino: <strong>%.2f </strong>var&nbsp;<strong>%.2f</strong></p>", omb_si_float, omb_si_var_float, omb_buon_float, omb_buon_var_float, omb_sass_float, omb_sass_var_float, omb_ist_float, omb_ist_var_float, omb_tur_float, omb_tur_var_float, omb_berr_float, omb_berr_var_float);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("mailing...");
    Serial.println("mailing...");
    delay(1000);

    smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
    smtpData.setSender("ESP32", emailSenderAccount);
    smtpData.setPriority("High");
    smtpData.setSubject(emailSubject);
    smtpData.setMessage(body, true);
    smtpData.addRecipient(emailRecipient);
    //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");

    if (!MailClient.sendMail(smtpData))
      Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    smtpData.empty();

    lcd.setCursor(0, 1);
    lcd.print("OK!");
    delay(300);
  }
}
