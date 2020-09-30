<p><img style="display: block; margin-left: auto; margin-right: auto;" src="https://content.instructables.com/ORIG/FY9/GUX4/KFJOP2U3/FY9GUX4KFJOP2U3.jpg?auto=webp&amp;frame=1&amp;width=755&amp;height=1024&amp;fit=bounds&amp;md=9749158fa3ed00d48641066ed1561940" alt="" width="755" height="700" /></p>

Hello World! This is my first Git rep! I'm Marco from Grosseto Italy, i'm not a developer, i'm not an electronic engineer but i'm an environmental guide in our local natural park (Maremma natural park).

In my cooperative we work a lot with canoe tours , naturalistic and calm tours on the Ombrone river for birdwatching and a bit of sense relaxing.

But the Ombrone river is also famous for its characteristic kind of flow: from summer "zero" to autumn-winter flow of several thousand cubic meters of water per hour.

For this reason, the Ombrone river is constantly monitored by the excellent regional hydrometric service(SIR), with a fantastic site that makes the collected data available.

Now we are waiting for the end of the season and for the first autumn floods for remove the dock and the canoes...

But when we start to talk about scraping parsing esp32ing etc? one moment more

During the italian lockdown i've done the first arduino blink lesson, cool... and i've contiunued to play around with sensors and arduino C. So if you find my code crude or full of nonsense logical gaps consider this!

I did a lot of things just to waste time but with this constant risk of flooding I decided to do something cool: thanks to an ESP32 with an arduino form factor (Wemos d1 r32) which I kept wrapped for months because I thought "difficult", I really had a lot of satisfactions:

I have done a scraper that bring and parse data from a SIR webpage and display the variables obtained on a cheap i2c LCD, and as brightness-frequency blinking on some leds on a realistic map.

so if a flood comes from the mountain side I will be warned by the increasing brightness (level change) and by the increasing frequency (level change in one hour) of the leds on the map... but the ESP32 can also sends emails! So why not do a function for this?

lets go to see the code below now!

Supplies:
ESP32 based board: I have used Wemos d1 r32 cause have female pins and it's easier to handle
leds, red is more "alert like"
I2C 16x2 LCD iv'e only a blue one but the green is more stylish
cheap cork board
night time (optional)

online i have found only vapourous tutorials that uses external sites to parse data, so i tried to test the power of the esp32 cores parsing the data directly on the board... without problems!

so if you view the SIR page source can understand how its works: luckily they uses a "comma separated like" values for display their data, so with this code I search for an hydrographic station (code TOSnumber) and count the number of commas (as separator) that contain the value i want (and store it as variable), then remap it as led brightness and time in millis for "variation level" blinking

The core of this scraper is in http tab:

    ind = payload.indexOf("TOSstation_number");  //get the position (ind) from where start to count commas
    String my_var = getValue_ind(payload, ',', 8); //where 8 is the eighth cell defined by commas (separator)
    
and the getValue_ind function is in the main tab

    String getValue_ind(String data, char separator, int index) {
      int found = 0;
      int strIndex[] = {0, -1};
      int maxIndex = data.length() - 1;
      for (int i = ind; i <= maxIndex && found <= index; i++) { //from ind position
        if (data.charAt(i) == separator || i == maxIndex) {
          found++;
          strIndex[0] = strIndex[1] + 1;
          strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
      }
      return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
    } 

adapted from https://stackoverflow.com/a/14824108

the code asks for the page every 15min, it is about 44Kb, i don't think is a big load for the servers.. To avoid too much scrolling i have divided the program in different functions, read the comments


i've used the tabs feature of the arduino IDE so i have different .ino files.

this is the latest version of the code, there's a lot of efforts, so i'll appreciate any suggestion!

- for the straightening of the led brightness for low values, 
- for the "map" function for floating point variables 
- and for the use of the second core on the ESP32, read on the comments on the .ino files on github!

as upload settings i've used Tools: Partition Scheme: NO OTA to have more space for this huge code

have fun!
https://www.instructables.com/id/ESP32-Scraper-parser-mailer-and-Live-Mapper/

