# Gateway
Here you can find the code related to passing messages to the sensor nodes back
and forth.

## Mosquitto
Mosquitto is an open source (EPL/EDL licensed) message broker that implements
the MQTT protocol versions 3.1 and 3.1.1. MQTT provides a lightweight method of
carrying out messaging using a publish/subscribe model.

### Installation
On Debian like systems do:
`sudo apt-get install mosquitto libmosquitto-dev libmosquittopp-dev mosquitto-clients`

### Topic Messages
Example:
```
mosquitto_pub -t path/to/topic -m "Your Message"
mosquitto_sub -v -t '#'
```
### Documentation
http://mosquitto.org/api/files/mosquitto-h.html
http://git.eclipse.org/c/mosquitto/org.eclipse.mosquitto.git/tree/lib/mosquitto.h
http://git.eclipse.org/c/mosquitto/org.eclipse.mosquitto.git/tree/lib/cpp/mosquittopp.h
