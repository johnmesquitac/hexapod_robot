#include <Ultrasonic.h>
#define pino_trigger 22
#define pino_echo 24
String valores;
String coordenadas_robo;
String coordenadas_alvo;
int x_robo;
int y_robo;
float inclinacao_robo;
int x_alvo;
int y_alvo;
float inclinacao_alvo;
Ultrasonic ultrasonic(pino_trigger, pino_echo);
bool Verifica_Sensor()
{
  long microsec = ultrasonic.timing();
  float centimetros = ultrasonic.convert(microsec, Ultrasonic::CM);
  if (centimetros > 50)
  {
    return true;
  }
  else
  {
    return false;
  }
}
void De_Pe()
{
  Serial1.print("#4GC1\r\n");
  delay(2000);
}
void Andar_Frente()
{
  while(Verifica_Sensor()==false){}
  Serial1.print("#5GC2\r\n"); 
}
void Virar_Esquerda()
{
  Serial1.print("#7GC1\r\n");
}
void Virar_Direita()
{
  Serial1.print("#6GC1\r\n");
}
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++)
  {
    if(data.charAt(i)==separator || i==maxIndex)
    {
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void Distribui_Valores()
{
  coordenadas_robo = getValue(valores,';',0);
  inclinacao_robo = getValue(valores,';',1).toFloat();
  coordenadas_alvo = getValue(valores,';',2);
  inclinacao_alvo = getValue(valores,';',3).toFloat();
  x_robo = getValue(coordenadas_robo,',',0).toInt();
  y_robo = getValue(coordenadas_robo,',',1).toInt();
  x_alvo = getValue(coordenadas_alvo,',',0).toInt();
  y_alvo = getValue(coordenadas_alvo,',',1).toInt();
}
void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
}
void loop()
{
  De_Pe();
  while (true)
  {  
    while(!Serial.available()){}
    valores = Serial.readString();
    Distribui_Valores();
    if (abs(x_robo - x_alvo) < 15  && abs(y_robo - y_alvo) < 15)
    {
      De_Pe();
      delay(2000);
    }
    else
    {
      if (abs(inclinacao_alvo - inclinacao_robo) > 0.5)
      {
        if (inclinacao_alvo < inclinacao_robo) 
        {
          Virar_Esquerda();
        }
        else
        {
          Virar_Direita();
        }      
      }
      else
      {
        Andar_Frente();        
      }
    }
  }  
}