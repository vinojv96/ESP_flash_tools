#define DEBUG true

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(11,OUTPUT);
  digitalWrite(11,LOW);

  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);

  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);

  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=3\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}

void loop()
{
  if(Serial1.available()) // check if the esp is sending a message
  {


    if(Serial1.find("+IPD,"))
    {
      delay(1000); // wait for the serial buffer to fill up (read all the serial data)
      // get the connection id so that we can then disconnect
      int connectionId = Serial1.read()-48; // subtract 48 because the read() function returns
      // the ASCII decimal value and 0 (the first decimal number) starts at 48

        Serial1.find("pin="); // advance cursor to "pin="

      int pinNumber = (Serial1.read()-48)*10; // get first number i.e. if the pin 13 then the 1st number is 1, then multiply to get 10
      pinNumber += (Serial1.read()-48); // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then add to the first number

      digitalWrite(pinNumber, !digitalRead(pinNumber)); // toggle pin

      // make close command
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand+=connectionId; // append connection id
      closeCommand+="\r\n";

      sendData(closeCommand,1000,DEBUG); // close connection
    }
  }
}

/*
* Name: sendData
 * Description: Function used to send data to ESP8266.
 * Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
 * Returns: The response from the esp8266 (if there is a reponse)
 */
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";

  Serial1.print(command); // send the read character to the esp8266

  long int time = millis();

  while( (time+timeout) > millis())
  {
    while(Serial1.available())
    {

      // The esp has data so display its output to the serial window
      char c = Serial1.read(); // read the next character.
      response+=c;
    }
  }

  if(debug)
  {
    Serial.print(response);
  }

  return response;
}

