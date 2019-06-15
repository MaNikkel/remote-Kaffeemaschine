#include <Ethernet.h>

#define botao 2
#define rele 8

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 25, 100 };
EthernetServer servidor(80);
String readString;


int estadoBotao = 0, estadoRele = 0, estadoAnterior = 0;

void setup() {
  pinMode(botao, INPUT);
  pinMode(rele, OUTPUT);
  //attachInterrupt(digitalPinToInterrupt(botao), acionaBotaoHigh, FALLING);
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  servidor.begin();
  
}

void loop() {
 digitalWrite(rele, estadoRele);
 estadoBotao = digitalRead(botao);
 if(estadoBotao == HIGH && estadoBotao != estadoAnterior) estadoRele = !estadoRele;
 else if(estadoBotao == LOW && estadoBotao != estadoAnterior) estadoRele = !estadoRele;
 estadoAnterior = estadoBotao;

 EthernetClient cliente = servidor.available();
 if (cliente) {
    while (cliente.connected()) {
      if (cliente.available()) {
        char c = cliente.read();
  
        if (readString.length() < 100) {
          readString += c;             
        }
 
        if (c == '\n') {
          cliente.println("HTTP/1.1 200 OK");
          cliente.println("Content-Type: text/html");
          cliente.println();
           
          cliente.println("<HTML>");
          cliente.println("<BODY>");
          cliente.println("<H1>Cafeteirazita</H1>");
          cliente.println("<hr />");
          cliente.println("<br />");
          cliente.println("<p>Cafeteira <a href=\"/?ligar\"><button>LIGA</button></a><a href=\"/?desligar\"><button>DESLIGA</button></a>");
          cliente.println("</BODY>");
          cliente.println("</HTML>");
           
          delay(1);
          cliente.stop();
           
          if(readString.indexOf("?ligar") > 0)
          {
            estadoRele = HIGH;
          }
          else {
            if(readString.indexOf("?desligar") > 0)
            {
              estadoRele = LOW;
            }
          }
          readString="";    
        }
      }
    }
  }
 
 
 
 

}
