//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "ProiectPMP"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,14,0,0,0,57,0,10,8,1,
  1,0,32,81,11,11,5,31,105,110,
  105,116,0,1,0,48,81,11,11,5,
  31,112,108,97,121,0,7,36,3,84,
  23,6,31,26,5,11,1,1,19,49,
  26,8,5,31,97,110,105,109,97,116,
  105,111,110,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t button_1; // =1 if button pressed, else =0 
  uint8_t button_2; // =1 if button pressed, else =0 
  char edit_1[11];  // string UTF8 end zero  
  uint8_t button_3; // =1 if button pressed, else =0 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

/////////////////////////////////////////////
//      other includes and variables       //
/////////////////////////////////////////////
#include <SPI.h>
#include <TimerOne.h>

int slaveSelect = 10;
uint8_t row[5];
int delayTime = 100;
unsigned long del=100;
int on=1;
unsigned long StartTime= millis();
unsigned long CurrentTime= millis();
unsigned long ledTime= millis();
unsigned long timpoff= millis();
unsigned long timpon = millis();

int semnals=0;
int semnal=1;
int count=0;
int mode=0;
unsigned long mesaj[20][5]={
  {0x3c,0x20,0x20,0x20,0x3c},
  {0x38,0x28,0x38,0x28,0x24},
  {0x10,0x10,0x10,0x10,0x10},
  {0x3c,0x20,0x3c,0x04,0x3c},
  {0x3e,0x08,0x08,0x08,0x08},
  {0x10,0x10,0x10,0x10,0x10},
  };

///litere
unsigned long a[5]={0x30,0x48,0x78,0x48,0x48};
unsigned long b[5]={0x70,0x48,0x70,0x48,0x70};
unsigned long c[5]={0x78,0x40,0x40,0x40,0x78};
unsigned long d[5]={0x70,0x48,0x48,0x48,0x70};
unsigned long e[5]={0x78,0x40,0x78,0x40,0x78};
unsigned long f[5]={0x70,0x40,0x60,0x40,0x40};
unsigned long g[5]={0x78,0x40,0x58,0x48,0x78};
unsigned long h[5]={0x48,0x48,0x78,0x48,0x48};
unsigned long i[5]={0x20,0x20,0x20,0x20,0x20};
unsigned long j[5]={0x10,0x10,0x10,0x50,0x70};
unsigned long k[5]={0x48,0x50,0x60,0x50,0x50};
unsigned long l[5]={0x40,0x40,0x40,0x40,0x78};
unsigned long m[5]={0x41,0x2a,0x2a,0x24,0x24};//m...
unsigned long n[5]={0x44,0x64,0x54,0x4c,0x44};//n...
unsigned long o[5]={0x78,0x48,0x48,0x48,0x78};
unsigned long p[5]={0x70,0x50,0x70,0x40,0x40};
unsigned long r[5]={0x70,0x50,0x70,0x50,0x48};
unsigned long s[5]={0x78,0x40,0x78,0x08,0x78};
unsigned long t[5]={0x7c,0x10,0x10,0x10,0x10};
unsigned long u[5]={0x48,0x48,0x48,0x48,0x78};
unsigned long v[5]={0x44,0x44,0x44,0x28,0x10};
unsigned long x[5]={0x44,0x28,0x10,0x28,0x44};
unsigned long y[5]={0x44,0x28,0x10,0x08,0x04};
unsigned long z[5]={0x78,0x08,0x10,0x20,0x78};
bool initializat=false; 
int fillEmpty=0;
int as=0;
bool cleared = true;

/////////////////////////////////////////////
//   end other includes and variables      //
/////////////////////////////////////////////

void set( unsigned long a[5]){
  for(int ij =0;ij<5;ij++)
  row[ij]=a[ij];
  }

  
void checkit(int count){
  
    if(row[count]==0x80){
  if(count==0)
    row[4]=0x80;
    else
    row[count-1]=0x80;
    row[count]=0x00;
    }
    
  }
void scrie(){
  for(int i=3;i<8;i++){
   digitalWrite(slaveSelect, LOW);  
    SPI.transfer(row[i-3]); 
    digitalWrite(slaveSelect, HIGH);
    digitalWrite(i,HIGH);
    digitalWrite(i,LOW);     
  }               
  }
void sterge(){
  for(int i=0;i<5;i++)
  row[i]=0x00;
  }
void aprinde(){
    //row[4]=0x40;
    for(int i=0;i<5;i++)
    row[i]=0xff;
    }
    
void start(){
  if(millis()-StartTime>200UL){
    StartTime=millis();
    if(on==1)
    {
      aprinde();
      on=0;
      }
      else if(on==0)
      {
        sterge();
        on=1;
        }
    }
}

   void firma(){
    
    set(mesaj[0]);
    for(int i =0; i<9;i++)
      for(int j = 0; j<5; j++)
      {
        mesaj[i][j]= mesaj[i][j]<<1;
        if(mesaj[i+1][j] & 0x80){
          mesaj[i][j] = mesaj[i][j] | 0x01;
          }
        }
        for(int j =0; j<5; j++){
          mesaj[9][j] = mesaj[9][j]<<1;
        if(mesaj[0][j] & 0x100){
          mesaj[9][j] = mesaj[9][j] | 0x01;
          }
          }
      }

      void initFirma(char msj[11]){
        for(int ij =0; ij <10;ij++)
        switch(msj[ij]){
          case 'a': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=a[ji];
          break;
           case 'b': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=b[ji];
          break;
           case 'c': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=c[ji];
          break;
           case 'd': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=d[ji];
          break;
           case 'e': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=e[ji];
          break;
           case 'f': 
           for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=f[ji];
          break;
           case 'g': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=g[ji];
          break;
           case 'h': 
           for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=h[ji];
          break;
           case 'i': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=i[ji];
          break;
           case 'j': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=j[ji];
          break;
           case 'k': 
           for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=k[ji];
          break;
           case 'l': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=l[ji];
          break;
           case 'm': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=m[ji];
          break;
           case 'n': 
            for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=n[ji];
          break;
           case 'o': 
          for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=o[ji];
          break;
           case 'p': 
           for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=p[ji];
          break;
                 
           case 'r': 
         for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=r[ji];
          break;
           case 's': 
          for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=s[ji];
          break;
           case 't': 
          for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=t[ji];
          break;
           case 'u': 
           for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=u[ji];
          break;
           case 'x': 
           for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=x[ji];
          break;
           case 'y': 
          for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=y[ji];
          break;
           case 'z': 
           for(int ji=0;ji<5;ji++)
            mesaj[ij][ji]=z[ji];
          break;
          default:
            for(int ji = 0; ji<5;ji++)
             mesaj[ij][ji]=0;
          }
          
        }

 void fillRandom(){
  int i = random(0,5);
  int j=random(0,8);
  int ok=1;
  int rcount=0;
  while(ok==1){
    if(row[i]==0xff)
    {
      i++;
      if(i==5) i=0;
      rcount++;
      }
      if(rcount==5)
      ok=0;
      if((row[i] & (0x01<<j)) ==0){
        ok=0;
        }
        else
        {
          j++;
          if(j==8)
          j=0;
        }
    }
  row[i]=row[i]|0x01<<j;
  }

    
 void emptyRandom(){
  int i = random(0,5);
  int j=random(0,8);
  int ok=1;
  int rcount=0;
  while(ok==1){
    if(row[i]==0x00)
    {
      i++;
      if(i==5) i=0;
      rcount++;
      }
      for(int k=0;k<5;k++)
      
      if(rcount==5)
      ok=0;
      if((row[i] & (0x01<<j))){
        ok=0;
        }
        else
        {
          j++;
          if(j==8)
          j=0;
        }
    }
  row[i]=row[i]^(0x01<<j);
  }
 
void plimbare(){
  if(semnal)
  {
    row[count]=0x01;
    semnal=0;
    }
    else{
      row[count-1]=0x00;
      if(count==0)
      row[4]=0x00;
  row[count]=row[count]<<1;
  if(semnals)
  {
    count++;
    if(count==5)
    count=0;
    row[count]=0x01;
    semnals=0;
    }
  if(row[count]==0x80){
    if(count==0)
    row[4]=0x80;
    else
    row[count-1]=0x80;
    row[count]=0x00;
    semnals=1;
    }
    }
  }
       

void setup() 
{
  RemoteXY_Init (); 
  pinMode(slaveSelect, OUTPUT);
  SPI.begin();
  StartTime = millis();
  Timer1.initialize(5000);
  Timer1.attachInterrupt(scrie);
  SPI.beginTransaction(SPISettings(10000000, LSBFIRST, SPI_MODE0));
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
  
  randomSeed(analogRead(0));   
}

void loop() 
{ 
  RemoteXY_Handler ();
    if(RemoteXY.button_3==1){
    if(millis() - timpoff >40){
      timpoff=millis();
      timpon=millis();

      mode++;
      if(mode>4)
      mode=0;
      cleared=false;
    }
  }
  
   if(RemoteXY.button_1==1){
    initFirma(RemoteXY.edit_1);
    }
    if(RemoteXY.button_2==1){
      
    if(millis() - timpoff >40){
      timpoff=millis();
      timpon=millis();
      mode=5;
    }
  }
  
  if(millis()-CurrentTime>del){
    StartTime=millis();
    CurrentTime=millis();

     switch(mode){
      case 0: if(cleared==false){sterge(); cleared=true;} plimbare();
      break;
      case 1:if(cleared==false){sterge(); cleared=true;} if(fillEmpty==0)
     fillRandom();
     else
     emptyRandom();
     if(row[0]==0xff && row[1]==0xff && row[2]==0xff && row[3]==0xff && row[4]==0xff)
     fillEmpty=1;
     if(row[0]==0x00 && row[1]==0x00 && row[2]==0x00 && row[3]==0x00 && row[4]==0x00)
     fillEmpty=0;
      break;
      case 2:if(cleared==false){sterge(); cleared=true;} aprinde();
      break;
      case 3:if(cleared==false){sterge(); cleared=true;} if(as) {as=0; aprinde();} else {as=1; sterge();}
      break;
      case 4: sterge();
      break;
      case 5:if(cleared==false){sterge(); cleared=true;} firma();
      default: if(cleared==false){sterge(); cleared=true;} start(); 
      break;
      }
    }
}
