# Inferno

Inferno is a Slackbot-enabled LED flame. It's designed for use in a modded 100% Soft's [Dumpster Fire - This is Fine vinyl figure](https://100soft.shop/products/dumpster-fire-this-is-fine-vinyl-figure) and for integration with Slack-based alerting. In addition to Slack integration, Inferno also provides a web server.

<img src="https://github.com/nullpainter/inferno/blob/main/images/front.jpg" width="49%" alt="Front" />
<img src="https://github.com/nullpainter/inferno/blob/main/images/back.jpg" width="49%" alt="Back" />

## Usage

When first turned on, Inferno creates a WiFi AP called `Inferno`. Connect to this in order to select an internet-enabled WiFi network for Inferno to join to. 

### Slack 

Assuming a Slack bot has been created with the name `@Inferno`, the following commands are honoured:

* `@Inferno on` turn on flame 
* `@Inferno off` turn off flame, enable monitoring mode

Additionally, if the text `[Alerting]` or `Triggered: ` appears in any Slack channel that the Slack bot is a member of, Inferno turns on for a predefined period of time.

### Web server

Inferno creates a web server on port 80. Issuing a GET will list the available commands:

```

    ,.   (   .      )        .      "
   ("     )  )'     ,'        )  . (`     '`
 .; )  ' (( (" )    ;(,     ((  (  ;)  "  )"
 _"., ,._'_.,)_(..,( . )_  _' )_') (. _..( '..
 
GET  /status        get LED status
POST /off           turn LEDs off
POST /on            turn LEDS on
POST /monitor       enable monitoring
```

## Building

Inferno is built with the Ar

### Slack configuration

Inferno expects the following files to be present in flash:

* `data/secrets/slackBotId.txt`
* `data/secrets/slackToken.txt`

These files should contain a single line containg the Slack bot id and OAuth tokens, respectively.

### LED pins

LED pin assignment is defined in `Inferno.h`. Pin to GPIO mappings can be found [here](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/).

The ESP8266 should be configured to run at 160 Mhz as per BearSSL's instructions. This is due to the time taken to process SSL queries.

### Libraries

Inferno was created using Arduino libraries. The following libraries are required:

* [WIFIManager](https://github.com/tzapu/WiFiManager)
* [ESP8266WiFi](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)
* [TaskScheduler](https://github.com/arkhipenko/TaskScheduler)
* [ArduinoWebSockets](https://github.com/gilmaimon/ArduinoWebsockets)
* [ArduinoJson](https://arduinojson.org/)


## Caveats

I am not a professional C/C++ developer. Apologies for any shameful practices.
