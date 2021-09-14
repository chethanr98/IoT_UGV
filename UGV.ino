#include<TinyGPS++.h>
#include<SoftwareSerial.h>

SoftwareSerial Ps(50,51);
SoftwareSerial Bl(52,53);
TinyGPSPlus gps;

#define enFL 7
#define enFR 6
#define enRL 5
#define enRR 4
#define FL1 22
#define FL2 23
#define FR1 24
#define FR2 25
#define RL1 26
#define RL2 27
#define RR1 28
#define RR2 29

#define LED 13
#define echo 2
#define trig 3

#define ts A0
#define gs A1

int i,tem,gasr;
char dc;
int durt,dist;
float temv,gasc,gasv,RS;
int Lat,Long;

void setup()
{
  pinMode(enFL,OUTPUT);
  pinMode(enFR,OUTPUT);
  pinMode(enRL,OUTPUT);
  pinMode(enRR,OUTPUT);
  pinMode(FL1,OUTPUT);
  pinMode(FL2,OUTPUT);
  pinMode(FR1,OUTPUT);
  pinMode(FR2,OUTPUT);
  pinMode(RL1,OUTPUT);
  pinMode(RL2,OUTPUT);
  pinMode(RR1,OUTPUT);
  pinMode(RR2,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(ts,INPUT);
  pinMode(gs,INPUT);
  
  Serial.begin(9600);
  Ps.begin(9600);
  Bl.begin(9600);
}

void MOV()
 {
  switch(dc)
  {
    case 'B': 
      digitalWrite(FL1,LOW);
      digitalWrite(FL2,HIGH);  
      digitalWrite(FR1,LOW);
      digitalWrite(FR2,HIGH);
      digitalWrite(RL1,LOW);
      digitalWrite(RL2,HIGH);  
      digitalWrite(RR1,LOW);
      digitalWrite(RR2,HIGH);
      if(i!=256)
        acc();
      break;
    case 'F': 
      digitalWrite(FL2,LOW);
      digitalWrite(FL1,HIGH);  
      digitalWrite(FR2,LOW);
      digitalWrite(FR1,HIGH);
      digitalWrite(RL2,LOW);
      digitalWrite(RL1,HIGH);
      digitalWrite(RR2,LOW);
      digitalWrite(RR1,HIGH);
      if(i!=256)
        acc();
      break;
    case 'R':
      digitalWrite(FL2,LOW);
      digitalWrite(FL1,HIGH);
      digitalWrite(FR1,LOW);
      digitalWrite(FR2,HIGH); 
      digitalWrite(RL2,LOW);
      digitalWrite(RL1,HIGH);
      digitalWrite(RR1,LOW);
      digitalWrite(RR2,HIGH);
      if(i!=256)
        acc();
      break;
    case 'L': 
      digitalWrite(FL1,LOW);
      digitalWrite(FL2,HIGH);
      digitalWrite(FR2,LOW);
      digitalWrite(FR1,HIGH);
      digitalWrite(RL1,LOW);
      digitalWrite(RL2,HIGH);
      digitalWrite(RR2,LOW);
      digitalWrite(RR1,HIGH);
      if(i!=256)
        acc();
      break;
    case 'S': OFF();
    default: OFF();
  }
}

void acc()
{
  for(i=0;i<256;i++)
  {
    analogWrite(enFL,i);
    analogWrite(enFR,i);
    analogWrite(enRL,i);
    analogWrite(enRR,i);
    delay(20);
  }
}

void OFF()
{
  digitalWrite(FL1,LOW);
  digitalWrite(FL2,LOW);
  digitalWrite(FR1,LOW);
  digitalWrite(FR2,LOW);
  digitalWrite(RL1,LOW);
  digitalWrite(RL2,LOW);
  digitalWrite(RR1,LOW);
  digitalWrite(RR2,LOW);
}

void Ultra()
{
  digitalWrite(LED,LOW);
  digitalWrite(trig,HIGH);
  delay(50);
  digitalWrite(trig,LOW);
  durt=pulseIn(echo,HIGH);
  dist=(durt/2)/29.1;
  if(dist<10)
  {
    digitalWrite(LED,HIGH);
    dc='B';
    MOV();
    temp();
    gas();
    pos();
    dc='S';
  }
  delay(50);
  digitalWrite(LED,LOW);
}

void temp()
{
  temv=analogRead(ts);
  tem=temv/1024*500;
  Serial.write(tem);
  delay(100);
}

void gas()
{
  gasv=analogRead(gs);
  gasc=gasv*5.0/1024;
  RS=(5-gasc)/gasc;
  gasr=RS/2.03;
  Serial.write(gasr);
  delay(100);
}

void pos()
{
  if(Ps.available() && gps.encode(Ps.read()))
  { 
    if(gps.location.isValid())
    {
      Lat=gps.location.lat();
      Long=gps.location.lng();
      Serial.write(Lat);
      delay(100);
      Serial.write(Long);
    }
  }
}

void loop()
{
  if(Bl.available())
    dc=Bl.read();
  
  temp();
  gas();
  pos();
  
  MOV();
  Ultra();
}
