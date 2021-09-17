#include "SheetsWeather.h"



SheetsWeather::SheetsWeather()
{}

SheetsWeather::~SheetsWeather()
{}

void SheetsWeather::write_data() 
{
	Serial.print("Connecting to ");
	Serial.print(server);

	WiFiClient client;
	int retries = 5;
	while (!!!client.connect(server, 80) && (retries-- > 0)) {
		Serial.print(".");
	}
	Serial.println();
	if (!!!client.connected()) {
		Serial.println("Failed to connect...");
	}

	Serial.print("Request resource: ");
	Serial.println(resource);

	String jsonObject = String("{\"value1\":\"") + _indication_sensor.DHT_temperature +"C"+ "\",\"value2\":\"" + _indication_sensor.DHT_humidity+"%"+ 
		+ "\",\"value3\":\"" + _indication_sensor.BME_pressure+"hPa" + "\"}";


	client.println(String("POST ") + resource + " HTTP/1.1");
	client.println(String("Host: ") + server);
	client.println("Connection: close\r\nContent-Type: application/json");
	client.print("Content-Length: ");
	client.println(jsonObject.length());
	client.println();
	client.println(jsonObject);

	int timeout = 5 * 10;           
	while (!!!client.available() && (timeout-- > 0)) {
		delay(100);
	}
	if (!!!client.available()) {
		Serial.println("No response...");
	}
	while (client.available()) {
		Serial.write(client.read());
	}

	Serial.println("\nclosing connection");
	client.stop();
}

void SheetsWeather::update()
{
	if (millis() > update_time)
	{
		write_data();
		update_time = delay_update + millis();
	}
}