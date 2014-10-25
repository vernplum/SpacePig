#include <SPI.h>
#include <Wire.h>
#include <Flash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>

#define OLED_MOSI    2
#define OLED_CLK     3
#define OLED_DC      4
#define OLED_RESET   5
#define OLED_CS      6

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


// MY DEFINES
#define MAX_BULLETS 3
#define MAX_ALIEN_BULLETS 6
#define MAX_EXPLOSION_DOTS 15
#define MAX_STARS 15
#define MAX_ALIENS 9
#define BIGNUM true
#define SMALLNUM false

#define CENTERED 100


#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_A8  7040
#define NOTE_B8  7902
#define NOTE_BLANK 9999
#define NOTE_END 9998


int pitches[] PROGMEM = { 
    NOTE_A6, NOTE_E7, NOTE_A6, NOTE_F7, NOTE_E7, NOTE_C7, NOTE_A6, NOTE_E7, NOTE_A6, NOTE_F7, NOTE_E7, NOTE_C7, 
    NOTE_F6, NOTE_C7, NOTE_E7, NOTE_F7, NOTE_E7, NOTE_C7, NOTE_F6, NOTE_C7, NOTE_E7, NOTE_F7, NOTE_E7, NOTE_C7, 
  
    NOTE_C7, NOTE_B6, NOTE_C7, NOTE_D7, NOTE_C7, NOTE_B6, NOTE_G7, NOTE_F7, NOTE_E7, NOTE_F7, NOTE_E7, NOTE_D7, 
    NOTE_C7, NOTE_B6, NOTE_C7, NOTE_D7, NOTE_C7, NOTE_B6, NOTE_C7, NOTE_D7, NOTE_F7, NOTE_A7, NOTE_C8, NOTE_B7,
   
    NOTE_A8, NOTE_FS7, NOTE_DS7, NOTE_C7, NOTE_A7, NOTE_FS6, NOTE_DS6, NOTE_FS6, NOTE_A7, NOTE_FS6, NOTE_A7, NOTE_DS6, 
    NOTE_E6, NOTE_GS6, NOTE_B6, NOTE_GS6, NOTE_B6, NOTE_E7, NOTE_GS6, NOTE_B6, NOTE_E7, NOTE_B6, NOTE_GS7, NOTE_E7,

    NOTE_END
};
 
byte lengths[] PROGMEM = { 
    4, 4, 4, 4, 4, 4, 
    4, 4, 4, 4, 4, 4, 
    4, 4, 4, 4, 4, 4, 
    4, 4, 4, 4, 4, 4, 
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
} ;


static unsigned char bossOctowolfString[] PROGMEM = {  24, 12, 29, 24, 32, 24, 21, 15, 99 };
static unsigned char bossTentaBrainString[] PROGMEM = {  29, 14, 23, 29, 10, 11, 27, 10, 18, 23, 99 };
static unsigned char bossSpaceSlugString[] PROGMEM = {  28, 25, 10, 12, 14, 0, 28, 21, 30, 16, 99 };
static unsigned char bossHelloKittyString[] PROGMEM = {  17, 14, 21, 21, 24, 0, 20, 18, 29, 29, 34, 99 };
static unsigned char cherryString[] PROGMEM = { 12, 17, 14, 27, 27, 34, 0, 29, 18, 22, 14, 36, 99  }; // 99 is end, 0 is space
static unsigned char ctString[] PROGMEM = { 12, 29, 99 };
static unsigned char scoreString[] PROGMEM = { 28, 12, 24, 27, 14, 37, 99 };
static unsigned char levelString[] PROGMEM = { 21, 14, 31, 14, 21, 37, 99 };
static unsigned char oinkString[] PROGMEM = { 24, 18, 23, 20, 36, 99 };
static unsigned char gameOverString[] PROGMEM = { 16, 10, 22, 14, 0, 24,  31, 14, 27, 99 };
static unsigned char getReadyString[] PROGMEM = { 16, 14, 29, 0, 27, 14, 10, 13, 34, 36, 99 };
static unsigned char heartString[] PROGMEM = { 38, 39, 99 };
static unsigned char bossHealthString[] PROGMEM = { 11, 24, 28, 28, 99 };
static unsigned char hyperspaceString[] PROGMEM = { 17, 34, 25, 14, 27, 28, 25, 10, 12, 14, 36, 99 };
static unsigned char bossStageString[] PROGMEM = { 11, 24, 28, 28, 0, 28, 29, 10, 16, 14 , 99 };
static unsigned char bossDefeatedString[] PROGMEM = { 11, 24, 28, 28, 0, 13, 14, 15, 14, 10, 29, 14, 13, 36, 99 };
static unsigned char OinkOink[] PROGMEM = {  24, 18, 23, 20, 36, 0, 24, 18, 23, 20, 36, 99 };
static unsigned char IamSpaceWiggy[] PROGMEM = { 18, 0, 10, 22, 0, 28, 25, 10, 12, 14, 0, 32, 18, 16, 16, 34, 99 };
static unsigned char ILoveYou[] PROGMEM = { 18, 0, 21, 24, 31, 14, 0, 34, 24, 30, 99 };
static unsigned char Nicholas[] PROGMEM = { 23, 18, 12, 17, 24, 21, 10, 28, 99 };
static unsigned char ToSaveThe[] PROGMEM = { 29, 24, 0, 28, 10, 31, 14, 0, 29, 17, 14 , 99 };
static unsigned char UniverseYou[] PROGMEM = { 30, 23, 18, 31, 14, 27, 28, 14, 0, 34, 24, 30, 99 };
static unsigned char MustBeBrave[] PROGMEM = { 22, 30, 28, 29, 0, 11, 14, 0, 11, 27, 10, 31, 14, 99 };
static unsigned char GoodLuck[] PROGMEM = { 16, 24, 24, 13, 0, 21, 30, 12, 20, 99 };
static unsigned char AgainstThe[] PROGMEM = { 10, 16, 10, 18, 23, 28, 29, 0, 29, 17, 14, 99 };
static unsigned char FinalBoss[] PROGMEM = {15, 18, 23, 10, 21, 0, 11, 24, 28, 28, 36, 99 };
static unsigned char Congratulations[] PROGMEM = { 12, 24, 23, 16, 27, 10, 29, 30, 21, 10, 29, 18, 24, 23, 28, 36, 99 };
static unsigned char YouHaveDefeated[] PROGMEM = { 34, 24, 30, 0, 17, 10, 31, 14, 0, 13, 14, 15, 14, 10, 29, 14, 13, 99 };
static unsigned char TheDreaded[] PROGMEM = { 29, 17, 14, 0, 13, 27, 14, 10, 13, 14, 13, 99 };
static unsigned char NowYouHaveSaved[] PROGMEM = { 23, 24, 32, 0, 34, 24, 30, 0, 17, 10, 31, 14, 0, 28, 10, 31, 14, 13, 99 };
static unsigned char TheUniverse[] PROGMEM = { 29, 17, 14, 0,30, 23, 18, 31, 14, 27, 28, 14, 99 };
static unsigned char AndAllThePigs[] PROGMEM = { 10, 23, 13, 0, 10, 21, 21, 0, 29, 17, 14, 0, 25, 18, 16, 28, 99 };
static unsigned char AreHappy[] PROGMEM = { 10, 27, 14, 0, 17, 10, 25, 25, 34, 99 };
static unsigned char TheEnd[] PROGMEM = { 29, 17, 14, 0, 14, 23, 13, 99 };
static unsigned char SpacePig[] PROGMEM = { 28, 25, 10, 12, 14, 0, 25, 18, 16, 99 };
static unsigned char ForYour8th[] PROGMEM = { 15, 24, 27, 0, 34, 24, 30, 27, 0, 8, 29, 17, 99 };
static unsigned char Birthday[] PROGMEM = { 11, 18, 27, 29, 17, 13, 10, 34, 99 };
static unsigned char TenthOctober2014[] PROGMEM = { 1, 24, 29, 17, 0, 24, 12, 29, 24, 11, 14, 27, 0, 2, 24, 1, 4, 99 };
static unsigned char Perfect[] PROGMEM = { 25, 14, 27, 15, 14, 12, 29, 36, 99 };
static unsigned char GraphicsBy[] PROGMEM = { 16, 27, 10, 25, 17, 18, 12, 28, 99 };
static unsigned char GameDesignBy[] PROGMEM = { 16, 10, 22, 14, 0, 13, 14, 28, 18, 16, 23, 99 };
static unsigned char MusicBy[] PROGMEM = { 22, 30, 28, 18, 12, 99 };
static unsigned char ProgrammingBy[] PROGMEM = { 25, 27, 24, 16, 27, 10, 22, 22, 18, 23, 16, 0, 11, 34, 99 };
static unsigned char edquek[] PROGMEM = { 14, 13, 26, 30, 14, 20, 40, 17, 24, 29, 22, 10, 18, 21, 41, 12, 24, 22, 99 };
static unsigned char LoveFromPapi[] PROGMEM = { 21, 24, 31, 14, 0, 15, 27, 24, 22, 0, 25, 10, 25, 10, 99 };
static unsigned char BlankLine[] PROGMEM = { 99 };
static unsigned char YouGot[] PROGMEM = {  34, 24, 30, 0, 16, 24, 29, 99 };
static unsigned char Eat[] PROGMEM = { 14, 10, 29, 99 };
static unsigned char Cherries[] PROGMEM = {  12, 17, 14, 27, 27, 18, 14, 28, 36, 99 };
static unsigned char ExtraLife[] PROGMEM = { 14, 33, 29, 23, 10, 0, 21, 18, 15, 14, 99 };
static unsigned char HighScore[] PROGMEM = { 17, 18, 16, 17, 0, 28, 12, 24, 27, 14, 36, 99 };
static unsigned char EnterName[] PROGMEM = { 14, 23, 29, 14, 27, 0, 18, 23, 18, 29, 18, 10, 21, 28, 99 };
static unsigned char PressFireToPlay[] PROGMEM = { 25, 27, 14, 28, 28, 0, 15, 18, 27, 14, 0, 29, 24, 0, 25, 21, 10, 34, 99 };
static unsigned char ShootAliens[] PROGMEM = { 13, 14, 28, 29, 27, 24, 34, 0, 10, 21, 18, 14, 23, 28, 99 };
static unsigned char SaveTheUniverse[] PROGMEM = { 28, 10, 31, 14, 0, 29, 17, 14, 0, 30, 23, 18, 31, 14, 27, 28, 14, 99 };
static unsigned char SpaceSkeleton[] PROGMEM = { 28, 25, 10, 12, 14, 0, 28, 20, 14, 21, 14, 29, 24, 23, 99 };


class point
{
    public:
    	int x, y;
        int dirx, diry;
        byte movestep;
        bool alive;
};


class explosionDot
{
    public:
        byte x, y, dirx, diry;
        bool alive;
        byte lifespan;
      
        explosionDot()
        {
          lifespan = random(10) + 10;
          alive = false;
        }
      
    void show()
    {      
        x += dirx;
        y -= diry;
            
        drawBigPixel(x, y);
  
        lifespan--;
        
        if (lifespan == 0)
        {
          alive = false;
        }
    }
};



point myAliens[MAX_ALIENS];
explosionDot myExplosionDots[MAX_EXPLOSION_DOTS];
char bossHealth;
byte level;

class star
{   
    public:
        byte x, y, type;
        
    public:
      star() 
      {   
          type = (random(5) + 1);          
          x = random(128);
          y = random(64);
      }
    
      void show()
      {        
          display.drawLine(x, y, x + (type >> 1), y, WHITE);  
          
          x -= type;
          
          if (x <= 0) 
          {   
              y = random(64);
              x = 127;
          }     
      }
};


byte bulletCount = 0;
byte alienBulletCount = 0;
int firingProb;
int firingMaxBullet;
bool flashAlienBullet = true;



enum BulletType
{
    PlayerBullet,
    AlienBullet
};

class bullet
{
    public:
         byte x, y;
         bool alive;
         byte type;
    
    public:  
        void fire(byte x1, byte y1)
        {            
            x = x1;
            y = y1;
            alive = true;
        }
     
        void move()
        {
            switch (type)
            {
                case PlayerBullet:
                    x += 4;
                    
                    if (x >= 127)
                    {
                        alive = false;
                        bulletCount--;
                    }
                    
                    break;
                
                case AlienBullet:
                
                    if (level <= 10) x-= 2;
                    if (level > 10 && level <= 17) x-= 3;
                    if (level > 17) x -= 4;
                    
                    if (x <= 4)
                    {
                        alive = false;
                        alienBulletCount--;
                    }
                                        
                    break;
            }
                      
            
            if (type == AlienBullet)
            {
                drawBigPixel(x, y);
                display.drawPixel(x, y - 1, WHITE);
                display.drawPixel(x + 1, y - 1, WHITE);
                display.drawPixel(x, y + 2, WHITE);
                display.drawPixel(x + 1, y + 2, WHITE);
                display.drawPixel(x- 1, y, WHITE);
                display.drawPixel(x - 1, y + 1, WHITE);
                display.drawPixel(x + 2, y, WHITE);
                display.drawPixel(x + 2, y + 1, WHITE);
            }
            else
            {            
                display.drawFastHLine(x - 3, y, 7, WHITE);
                display.drawFastHLine(x - 3, y + 1, 7, WHITE);
            }
        }
};



byte counterSinceFired = 0;
bullet bullets[MAX_BULLETS];
bullet alienBullets[MAX_ALIEN_BULLETS];


enum GameStage
{
    Attract,
    NewGame,
    GetReady,
    NewLevel,
    InGame,
    LevelCompleted,
    Wiggy,
    Endgame,
    CherryTime,
    PigHit,
    GameOver,
    Credits,
    GotHighScore
};


// PUT GLOBALS HERE
byte pig_x;
byte pig_y;
star ob[MAX_STARS];
GameStage stage = GetReady;
bool altlegsFlag = true;
byte altlegcounter = 0;
byte getReadyCounter = 0;
byte flashCounter = 0;
bool showText = true;
byte lives;

int score;
byte numAliens;
byte aliensInLevel;
int nextCounter;
char periodCounter;
int cherryTimeCounter = 0; //even == cherry time, odd == over
byte eaten;
byte oinkCounter;

const unsigned char space_pig_bmp[] PROGMEM = { 
0x80, 0x01, 0x02, 0xA1, 0xF1, 0xCE, 0x52, 0x09, 0x32, 0x54, 0x05, 0x4A, 0x28, 0x02, 0x01, 0x08,
0x00, 0x79, 0x08, 0x00, 0x85, 0x04, 0x01, 0x2A, 0x02, 0x02, 0x84, 0x05, 0x84, 0x78, 0x09, 0x7A,
0x00, 0x19, 0x09, 0x80, 0x06, 0x0C, 0x00
};  

const unsigned char altlegs[] PROGMEM = {
0x20, 0x09, 0x10, 0x14, 0x2C, 0x23, 0xCB, 0xD0, 0x08, 0x50, 0x04, 0x80
};

const unsigned char  boss1[] PROGMEM = {
0x04, 0x40, 0x06, 0x60, 0x0F, 0xE2, 0x3B, 0x72, 0x79, 0x34, 0xFF, 0xF5, 0xAB, 0xFA, 0x01, 0xFC,
0xAB, 0xF9, 0xFF, 0xFE, 0x7C, 0xF8, 0x01, 0xFC, 0x06, 0xAA, 0x08, 0xA9, 0x01, 0x24, 0x02, 0x12
};

const unsigned char  boss1b[] PROGMEM = {
0x04, 0x40, 0x06, 0x60, 0x2F, 0xE0, 0x79, 0x30, 0x1D, 0xB0, 0x27, 0xF6, 0x0B, 0xF9, 0x05, 0xFE,
0x13, 0xF9, 0x0F, 0xFE, 0x5C, 0xF9, 0x31, 0xFC, 0x02, 0xAB, 0x02, 0xA8, 0x02, 0x96, 0x00, 0x40
};

const unsigned char boss2 [] PROGMEM = {
0x71, 0x01, 0x28, 0x19, 0xBB, 0x2C, 0x65, 0xFF, 0x18, 0x03, 0x9C, 0x90, 0xC7, 0x18, 0xE0, 0x67,
0xFF, 0xC0, 0xD5, 0x29, 0x40, 0x0F, 0xFF, 0xF8, 0x03, 0xFF, 0xC8, 0x01, 0x42, 0x88, 0x06, 0x42,
0x5C, 0x38, 0x34, 0x54, 0x18, 0x2A, 0x40, 0x28, 0x00, 0xE0, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00
};

const unsigned char boss2b [] PROGMEM = {
0x01, 0x01, 0x00, 0x01, 0xBB, 0x00, 0xF1, 0xFF, 0x0E, 0x6B, 0x18, 0x98, 0xA7, 0x5A, 0xEE, 0x07,
0xFF, 0xC0, 0x05, 0x29, 0x43, 0x0C, 0xAA, 0x7E, 0x16, 0x00, 0x43, 0xE2, 0x55, 0x80, 0x61, 0xD5,
0x60, 0xA2, 0x7E, 0x18, 0x0E, 0x21, 0x07, 0x06, 0x40, 0x86, 0x0A, 0x61, 0x85, 0x00, 0xA1, 0x40
};

const unsigned char boss3 [] PROGMEM = {
0x00, 0x00, 0xE0, 0x00, 0x00, 0x71, 0x10, 0x00, 0x00, 0x8A, 0x28, 0x00, 0x00, 0xAA, 0x28, 0x70,
0x00, 0x8A, 0x09, 0x88, 0x00, 0x51, 0x13, 0xA8, 0x00, 0x38, 0xE7, 0x88, 0x0F, 0x3C, 0xEF, 0x70,
0x10, 0x9D, 0xFE, 0x00, 0x20, 0x5F, 0xFE, 0x3C, 0x24, 0x7F, 0xFE, 0x42, 0x24, 0x7F, 0xFF, 0xCA,
0x10, 0xFC, 0x0F, 0xCA, 0x0F, 0xC0, 0xC3, 0xC2, 0x03, 0xB8, 0xC4, 0xFC, 0x07, 0x7D, 0xEF, 0x78,
0x07, 0x9F, 0xFC, 0xF8, 0x03, 0xE0, 0x01, 0xFC, 0x03, 0xFF, 0xFF, 0xFE, 0x01, 0xF7, 0xFF, 0xBE,
0x01, 0xD7, 0xDF, 0x3D, 0x00, 0x97, 0x4E, 0x91, 0x00, 0x62, 0x44, 0x8E, 0x00, 0x01, 0x83, 0x00
};

const unsigned char boss3b [] PROGMEM = {
0x01, 0xC1, 0xC0, 0x00, 0x02, 0x22, 0x20, 0xE0, 0x02, 0xA4, 0x11, 0x10, 0x02, 0x25, 0x11, 0x50,
0x01, 0xE5, 0x13, 0x10, 0x00, 0x72, 0x27, 0xE0, 0x3E, 0x71, 0xCF, 0x00, 0x41, 0x39, 0xCE, 0x00,
0x80, 0xBB, 0xDC, 0x78, 0x80, 0xFF, 0xFC, 0x84, 0xA0, 0xFF, 0xFE, 0x82, 0xA0, 0xFF, 0xFF, 0x22,
0x41, 0xFF, 0xFF, 0x22, 0x3F, 0xC8, 0x4F, 0x82, 0x1F, 0xA8, 0x53, 0xFC, 0x3E, 0x2F, 0xF3, 0xF8,
0x3E, 0xFF, 0xF9, 0xFC, 0x7F, 0xFF, 0xFF, 0xFC, 0x7F, 0xFF, 0xFF, 0xFC, 0xF9, 0xF8, 0xFC, 0xFC,
0xB9, 0xB0, 0x5C, 0x78, 0x98, 0x90, 0x48, 0xB8, 0x70, 0x60, 0x38, 0x88, 0x00, 0x00, 0x10, 0x70
};

const unsigned char boss4  [] PROGMEM = {
0x07, 0x00, 0xE3, 0x80, 0x0F, 0x81, 0x57, 0xC0, 0x0F, 0xFE, 0xAF, 0xC0, 0x0F, 0xFF, 0x5F, 0xC0,
0x1F, 0xFE, 0xA3, 0x40, 0x1F, 0xFF, 0x6A, 0xA0, 0x0F, 0xFF, 0xE3, 0x50, 0x0F, 0xFF, 0xFE, 0xB0,
0x1F, 0xFF, 0xFF, 0x50, 0x1F, 0xFF, 0xFF, 0xA0, 0x1F, 0xFF, 0xFF, 0xE0, 0x1F, 0xFF, 0xFF, 0xE0,
0x23, 0x3F, 0xF3, 0x30, 0x5F, 0x3F, 0xF3, 0xE8, 0x13, 0xFF, 0xFF, 0x20, 0x1F, 0xFC, 0xFF, 0xF0,
0x2D, 0xFF, 0xFF, 0x48, 0x07, 0xFF, 0xFF, 0xC0, 0x0B, 0xFF, 0xFF, 0x20, 0x01, 0xFF, 0xFE, 0x00,
0x03, 0xE0, 0x1F, 0x00, 0x07, 0xF0, 0x3F, 0x80, 0x0F, 0xBF, 0xFB, 0xC0, 0x17, 0x4D, 0x4D, 0xA0,
0x32, 0xCA, 0xCB, 0x30, 0x3F, 0x7D, 0x7D, 0xF0, 0x3E, 0xAA, 0xAA, 0xF0, 0x1B, 0x57, 0x55, 0x60,
0x03, 0xFF, 0xFF, 0x00, 0x03, 0xFF, 0xFF, 0x00, 0x03, 0xFC, 0xFF, 0x00, 0x01, 0xFC, 0xFE, 0x00
};

const unsigned char boss4b  [] PROGMEM = {
0x00, 0x80, 0xE2, 0x00, 0x00, 0xC1, 0x56, 0x00, 0x00, 0xC2, 0xAE, 0x00, 0x00, 0x63, 0x5C, 0xC0,
0x00, 0xFE, 0xA3, 0x40, 0x01, 0x01, 0x6A, 0xA0, 0x03, 0x00, 0xE3, 0x60, 0x02, 0x00, 0x1E, 0xA0,
0x02, 0x00, 0x03, 0x40, 0x04, 0x00, 0x01, 0x80, 0x04, 0x00, 0x00, 0x80, 0x05, 0xE0, 0x1E, 0x80,
0x35, 0x20, 0x12, 0xB0, 0x45, 0x25, 0x12, 0x88, 0x03, 0xE0, 0x1E, 0x80, 0x19, 0x00, 0x01, 0x30,
0x20, 0xD5, 0x5E, 0x08, 0x04, 0x55, 0x50, 0xC0, 0x08, 0x55, 0x50, 0x20, 0x00, 0x3F, 0xE0, 0x00,
0x01, 0x80, 0x06, 0x00, 0x01, 0xBA, 0xE6, 0x00, 0x42, 0x00, 0x01, 0x00, 0x18, 0x7A, 0xF0, 0x00,
0x40, 0x00, 0x00, 0x80, 0x00, 0x3A, 0xE0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5F, 0xE8, 0x40,
0x00, 0x0C, 0xC0, 0x00, 0x00, 0x40, 0x08, 0x00, 0x00, 0x80, 0x04, 0x00, 0x03, 0x80, 0x07, 0x00
};

const unsigned char  cherry[] PROGMEM = {
0x03, 0x04, 0x0C, 0x14, 0xE4, 0xAE, 0xEA, 0x0E, 
};

const unsigned char SpaceWiggy [] PROGMEM = {
0xFC, 0x00, 0x00, 0x86, 0x00, 0x00, 0x82, 0x00, 0x00, 0xC3, 0x00, 0x0F, 0x41, 0x00, 0x19, 0x61,
0x00, 0x33, 0x39, 0xFE, 0x22, 0x0E, 0x01, 0xE6, 0x05, 0x80, 0x84, 0x0C, 0x00, 0x7C, 0x08, 0x03,
0x40, 0x1B, 0x00, 0x60, 0x13, 0x00, 0x20, 0x10, 0x63, 0x20, 0x10, 0x93, 0x20, 0x10, 0xF0, 0x20,
0x10, 0x00, 0x20, 0x18, 0x60, 0x60, 0x0C, 0x00, 0xC0, 0x07, 0xFF, 0x00, 0x03, 0x06, 0x00, 0x06,
0x01, 0x00, 0x1D, 0xFE, 0x80, 0x24, 0x00, 0xF0, 0x4D, 0xFE, 0x48, 0x54, 0x00, 0x64, 0x66, 0x7C,
0xD4, 0x03, 0x01, 0x8C, 0x01, 0xFE, 0x00, 0x00, 0x44, 0x00, 0x01, 0xC7, 0x00, 0x00, 0x00, 0x00
};



unsigned char fpsString[] PROGMEM = { 15, 25, 28, 99 };
long timerCounter;
int fpsCounter, fpsCounterOld;


/*
int freeRam () 
{
    extern int __heap_start, *__brkval; 
    int v; 
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
*/

void setup()   
{                
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC);
    display.display();

    pinMode(10, INPUT);
    pinMode(11, INPUT);
    pinMode(12, INPUT);
    
    for (byte m = 0; m < MAX_BULLETS; m++)
    {
        bullets[m].type = PlayerBullet;
        bullets[m].alive = false;
    }
 
    for (byte m = 0; m < MAX_ALIEN_BULLETS; m++)
    {
        alienBullets[m].type = AlienBullet;
        alienBullets[m].alive = false;
    }
        
    stage = Attract;   
    
       
    int j = 0;
    int notePitch;
    int noteLength;
    
    for (;;)
    {         
        notePitch = pgm_read_word(&(pitches[j]));
        noteLength = pgm_read_byte(&(lengths[j]));
        
        if (notePitch == NOTE_END) 
        {
            j = 0;
            continue;
        }       
        
        if (notePitch != NOTE_BLANK) tone(13, notePitch, noteLength * 85);
        
        delay(noteLength * 85);

        if (digitalRead(12) == HIGH) 
        {
            playFireSound();
            break;  
        }
        
        j++;
    };     
}



int localvec[20];
char initials[3];



void loop() 
{
    FLASH_ARRAY(byte, startCoord3DArray,    //each row must be padded to 18 numbers (or MAX_ALIENS * 2)
        96, 16, 96, 32, 96, 48, 112, 16, 112, 32, 112, 48, 0, 0, 0, 0, 0, 0,    //0 
        96, 16, 96, 32, 96, 48, 112, 16, 112, 32, 112, 48, 0, 0, 0, 0, 0, 0,    //1
        96, 16, 96, 32, 96, 48, 112, 16, 112, 32, 112, 48, 0, 0, 0, 0, 0, 0,    //2
        80, 8, 96, 16, 112, 24, 80, 56, 96, 48, 112, 40, 0, 0, 0, 0, 0, 0,      //3
        96, 16, 112, 16, 120, 32, 112, 48, 96, 48, 88, 32, 0, 0, 0, 0, 0, 0,    //4
        
        110, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                // 5 BOSS 1  
        
        80, 8, 87, 16, 94, 24, 101, 32, 108, 40, 115, 48, 122, 56, 0, 0, 0, 0,  // 6
        112, 22, 96, 11, 80, 22, 96, 32, 80, 42, 96, 53, 112, 42, 0, 0, 0, 0,  //7
        80, 16, 80, 32, 80, 48, 96, 16, 96, 32, 96, 48, 112, 32, 0, 0, 0, 0,  //8
        80, 16, 80, 32, 80, 48, 112, 16, 112, 32, 112, 48, 96, 32, 0, 0, 0, 0,  //9 
        72, 48, 84, 32, 96, 16, 108, 32, 120, 48, 104, 48, 88, 48, 0, 0, 0, 0,  //10 
        
               
        110, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                // 11 BOSS 2
 
        96, 8, 112, 8, 96, 24, 112, 24, 96, 40, 112, 40, 96, 56, 112, 56, 0, 0, //12
        96, 16, 112, 16, 80, 26, 96, 26, 64, 38, 80, 38, 48, 48, 64, 48, 0, 0,  //13
        72, 32, 84, 24, 96, 16, 108, 24, 120, 32, 108, 40, 96, 48, 84, 40, 0, 0,  //14
        64, 24, 80, 16, 96, 16, 112, 24, 112, 40, 96, 48, 80, 48, 64, 40, 0, 0,  //15
        80, 8, 85, 24, 90, 40, 95, 56, 110, 8, 105, 24, 100, 40, 95, 56, 0, 0,  //16 
        
        110, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,               // BOSS3
        
        80, 16, 80, 32, 80, 48, 96, 24, 96, 40, 96, 56, 112, 16, 112, 32, 112, 48,  //18
        80, 16, 80, 32, 80, 48, 96, 16, 96, 32, 96, 48, 112, 16, 112, 32, 112, 48,  //19 
        80, 24, 80, 40, 96, 16, 96, 32, 96, 48, 112, 8, 112, 24, 112, 40, 112, 56,   //20
        80, 28, 76, 44, 72, 58, 96, 22, 92, 38, 88, 52, 112, 16, 108, 32, 104, 48,  //21 
        72, 8, 82, 20, 112, 8, 102, 20, 92, 32, 72, 56, 82, 44, 102, 44, 112, 56,  //22 
        
        110, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                 // BOSS 4
    );



    FLASH_ARRAY(int, levelData,
        6, 6, 6, 6, 6, 1, 7, 7, 7, 7, 7, 1, 8, 8, 8, 8, 8, 1, 9, 9, 9, 9, 9, 1, 
    );
    
    
    FLASH_ARRAY(byte, CherryTimeData,
        32, 0, 0, 35, 0, 0, 38, 0, 0, 38, 0, 0, 35, 0, 0, 32, 0, 0, 29, 0, 0, 26, 0, 0, 22, 0, 0, 32, 0, 0,  
        40, 0, 0, 40, 0, 0, 36, 0, 0, 32, 0, 0, 28, 0, 0, 24, 0, 0, 20, 0, 0, 18, 0, 0, 18, 0, 0, 20,  
        0, 0, 0, 0, 0, 0, 0, 99
    );
    
    FLASH_ARRAY(byte, CherryTimeData2,
        24, 0, 24, 0, 32, 0, 32, 0, 40, 0, 40, 0, 48, 0, 48, 0, 32, 0, 40, 0,  
        48, 0, 48, 0, 48, 0, 56, 0, 40, 0, 40, 0, 48, 0, 48, 0, 32, 0, 32, 0,
        24, 0, 24, 0, 32, 0, 32, 0, 40, 0, 40, 0, 48, 0, 48, 0, 32, 0, 40, 0,
        32, 0, 24, 0, 16, 0, 16, 0, 16, 0, 32, 0, 40, 0, 40, 0, 32, 0, 40, 0,
        0, 0, 0, 0, 0, 0, 0, 99
    );
    
    FLASH_ARRAY(byte, CherryTimeData3,
        56, 52, 58, 44, 40, 36, 32, 28, 24, 20, 
        16, 32, 36, 24, 32, 20, 24, 16, 20, 12, 
        16, 20, 24, 30, 36, 42, 48, 48, 48, 40, 
        32, 24, 24, 24, 24, 36, 36, 40, 48, 56, 
        56, 48, 40, 32, 24, 16, 24, 32, 40, 48, 
        56, 48, 40, 48, 40, 32, 40, 32, 24, 24, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        99
    );
      
    FLASH_ARRAY(int, vecdata,
        0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2,   //0
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,       //1
        -1, -2, -1, -2, -1, -2, -1, -2, -1, -2, -1, -2, -1, -2, -1, -2, -1, -2, -1, -2,  //2      
        -3, -1, -3, -1, -3, -1, -3, -1, -3, -1, -3, -1, -3, -1, -3, -1, -3, -1, -3, -1,    //3
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      //4
        
        -2, -1, -2, -1, -2, -1, -2, -1, -2, -1, -2, 1, -2, 1, 0, 2, 0, 2, 0, 2,    // BOSS 1 OCTOWOLF
        
        0, -2, 0, -2, -1, -2, -1, -2, 0, -2, 0, -2, -1, -2, -1, -2, 0, -2, 0, -2,   //6
        -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, -1, -2, -1, -2, -1, -2, -1, -2, -1,      //7
        -2, -1, -2, -1, -2, -1, -2, -1, -2, -1, -2, 1, -2, 1, 0, 2, 0, 2, 0, 2,  //8
        0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2,     //9
        0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0,        //10
        
        -2, -3, -2, -3, -2, -3, -2, -3, -2, -3, -2, -3, -2, -3, -2, -3, -2, -3, -2, -3,    // BOSS 2 TENTA-BRAIN
        
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      // 12
        -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0,       //13
        -3, 0, -3, 0, -3, 0, -3, 0, -3, 0, -3, -1, -2, -1, -2, -1, -2, -1, -2, -1,    //14
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,     //15
        3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1,         //16
        
        -3, 9, -7, 5, -8, 0, -8, -6, -2, -8, 3, -9, 7, -5, 8, 0, 8, 6, -1, 8,    // BOSS 3 SPACE SLUG
        
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,    -2, 2, -2, 2, -2, 2, -2, 2, -2, 2,  //18
        -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0,        //19
        -3, -2, -3, -2, -3, -2, -3, -2, -3, -2, -3, -2, -3, -2, -3, -2, -3, -2, -3, -2,     //20
        0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2, 0, -2,   //21
        -2, -1,-2, -1,-2, -1,-2, -1,-2, -1,-2, -1,-2, -1,-2, -1,-2, -1,-2, -1,    //22
        
        -4, -2, -4, -2, -4, -2, -4, -2, -4, -2, -4, 3, -4, 3, -4, 3, -4, 3, -4, 3     // FINAL BOSS
    );
    
    display.clearDisplay();
    
  
    
    for (int i = 0; i < MAX_STARS; i++) { ob[i].show(); } // DRAW STARFIELD
                
    flashAlienBullet = !flashAlienBullet;
    
    if (stage == NewGame)
    {
        pig_x = 0;
        pig_y = 32;  
        lives = 5;
        level = 0;
        score = 0;
        stage = NewLevel; 
        bossHealth = 0;
        cherryTimeCounter = 0;
    }
      
    if (stage == NewLevel)
    {
        numAliens = levelData[level]; 
        
        switch (level)
        {
            case 0: case 1: case 2: case 3: case 4:
                firingProb = 150;
                firingMaxBullet = 2;
                break;
            case 6: case 7: case 8: case 9: case 10:
                firingProb = 120;
                firingMaxBullet = 2;
                break;
            case 12: case 13: case 14: case 15: case 16:
                firingProb = 100;
                firingMaxBullet = 3;
                break;
            case 18: case 19: case 20: case 21: case 22:
                firingProb = 80;
                firingMaxBullet = 3;
                break;
            case 5:
                firingProb = 30;
                firingMaxBullet = 3;
                break;
            case 11:
                firingProb = 30;
                firingMaxBullet = 3;
                break;
            case 17:
                firingProb = 20;
                firingMaxBullet = 4;
                break;
            case 23:
                firingProb = 10;
                firingMaxBullet = 4;
                break;               
        }
        
        for (int i = 0; i < levelData[level]; i++)
        {
            myAliens[i].x = 127;
            myAliens[i].y = 32;  
            myAliens[i].dirx = 1;
            myAliens[i].diry = 1;
            myAliens[i].movestep = 0;   // change movesteps here
            myAliens[i].alive = true;
        }
        
        // LEVEL SPECIFIC MOVEMENTS
        if (level == 2 || level == 3)
        {
            for (int i = 3; i < 6; i++)
            {
                myAliens[i].diry = -1;  
            }
        }
        
        
        if (level == 4 || level == 9)
        {
            myAliens[0].dirx *= -1;
            myAliens[2].dirx *= -1;
            myAliens[3].diry *= -1;
            myAliens[5].diry *= -1;              
        }
        
        if (level == 10 || level == 7)
        {
            myAliens[0].movestep = 5;
            myAliens[1].movestep = 5;
            myAliens[2].movestep = 5;
            myAliens[3].movestep = 5;             
        }
        
       
       
       if (level == 16)
        {
          myAliens[4].dirx *= -1;
            myAliens[5].dirx *= -1;              
            myAliens[6].dirx *= -1;
            myAliens[7].dirx *= -1; 
        }
        
        
        if (level == 14)
        {
            myAliens[0].dirx *= -1;
            myAliens[1].dirx *= -1;
            myAliens[2].dirx *= -1;
            myAliens[3].dirx *= -1;  
     
            myAliens[0].diry *= -1;
            myAliens[1].diry *= -1;
            myAliens[2].diry *= -1;
            myAliens[3].diry *= -1;  
        }
        
        
        if (level == 12)
        {
            for (int i = 0; i < levelData[level] ; i++)
            {
                myAliens[i].dirx = random(3) + 1;
                if (random(2) % 2 == 0) myAliens[i].dirx *= -1;
                myAliens[i].diry = random(3) + 1;
                if (random(2) % 2 == 0) myAliens[i].diry *= -1;               
            }  
        }
        
        if (level == 15)
        {
            myAliens[2].dirx *= -1;
            myAliens[3].dirx *= -1;  
            
            myAliens[4].diry *= -1;
            myAliens[5].diry *= -1;
          
            myAliens[6].diry *= -1;
            myAliens[7].diry *= -1; 
            
            myAliens[6].dirx *= -1;
            myAliens[7].dirx *= -1; 
        }
        
        if (level == 18)
        {
            myAliens[3].diry *= -1;
            myAliens[4].diry *= -1;
            myAliens[5].diry *= -1;   
        }



   //     for (int j = 0; j < 24; j++)
     //   {
            for (int i = 0; i < 20; i++)
            {
                localvec[i] = vecdata[(/*j */level * 20) + i];
                
       //         Serial.print(localvec[i]);
         //       Serial.print(", ");
            }
            
           // Serial.println();
      //  }
        
 //       for(;;);
        
  /*      Serial.print(F("Free Ram: "));      
        Serial.println(freeRam());    
    */    
        flashCounter = 0;
        stage = GetReady; 
    }
    
    
    if (stage == LevelCompleted)
    {                
        bulletCount = 0;
        alienBulletCount = 0;
        
        resetBullets();
        
        
        
        getReadyCounter++;
                 
        switch (level)
        {
            case 0: case 1: case 2: case 3: case 4:
            case 6: case 7: case 8: case 9: case 10:
            case 12: case 13: case 14: case 15: case 16:
            case 18: case 19: case 20: case 21: case 22:
                if (getReadyCounter == 45)
                {
                    if (level == 22)
                    {
                        getReadyCounter = 0;
                        stage = Wiggy;
                        break;
                    }
                    
                    level++;
                    getReadyCounter = 0;
                    stage = NewLevel;
                }
            
                break;  
            
            case 5: case 11: case 17: case 23:           
                if (getReadyCounter <= 40)
                {
                    for (int i = 0; i < 25; i++)
                    {
                        display.drawDottedLine(myAliens[0].x, myAliens[0].y, myAliens[0].x + (random(64) - 32), myAliens[0].y + (random(64) - 32), WHITE, random(8) + 8); 
                    }
                    
                    playBossHitSound();           
                }
                
                if (getReadyCounter %4 == 0)
                {
                    explode(myAliens[0].x, myAliens[0].y);
                }
                
                if (getReadyCounter == 40) 
                {
                    myAliens[0].alive = false;
                    playAlienHitSound();
                }
                
                if (getReadyCounter > 40 && getReadyCounter < 100)
                {             
                    drawString(CENTERED, 24, bossDefeatedString); 
                    
                    switch (level)
                    {
                        case 5:
                            drawString(CENTERED, 36, bossOctowolfString);
                            break; 
                        case 11: 
                            drawString(CENTERED, 36, bossTentaBrainString);
                            break;
                        case 17: 
                            drawString(CENTERED, 36, bossSpaceSlugString);
                            break;
                        case 23:
                            drawString(CENTERED, 36, bossHelloKittyString);
                            drawString(CENTERED, 48, SpaceSkeleton);                       
                            break;
                    }
                }
                else if (getReadyCounter > 99 && getReadyCounter < 125 && level != 23)
                {
                    drawString(CENTERED, 32, cherryString);      
                }
                else if (getReadyCounter == 125)
                {                                    
                    if (level == 23)
                    {      
                        getReadyCounter = 0;        
                        score += lives * 500;
                        stage = Endgame;
                    }
                    else
                    {
                        level++;
                        getReadyCounter = 0;      
                        nextCounter = 0;
                        periodCounter = 0;
                        eaten = 0;
                        stage = CherryTime;  
                    } 
                }
                        
                break;  
 
        }
    }
    
    
    if (stage == CherryTime)
    {
        if (++periodCounter == 6)
        {
            periodCounter = 0;

            int nextByte;
            
            switch (cherryTimeCounter)
            {   
                case 0:
                    nextByte = CherryTimeData[nextCounter];
                    break;
                case 2:
                    nextByte = CherryTimeData2[nextCounter];
                    break;
                case 4:
                    nextByte = CherryTimeData3[nextCounter];
                    break;
            }        
            
            nextCounter++;
            
            if (nextByte == 99)
            {
                cherryTimeCounter++;
            }
            else if (nextByte != 0)
            {
                for (int i = 0; i < MAX_ALIENS; i++)
                {
                    if (myAliens[i].alive == false)
                    {                      
                        myAliens[i].alive = true;                        
                        myAliens[i].x = 127;
                        myAliens[i].y = nextByte;
                        myAliens[i].dirx = -4;
                        myAliens[i].diry = 0;
                        break;
                    }                      
                }               
            }            
        }
        
        for (int i = 0; i < MAX_ALIENS; i++)
        {
            if (myAliens[i].alive == true)
            {     
                myAliens[i].x += myAliens[i].dirx; 
                
                if (myAliens[i].x <= 0)
                {
                    myAliens[i].alive = false; 
                    continue;
                } 
                                
                if (myAliens[i].x > 24 || myAliens[i].x < 16)
                    continue;
                else if (myAliens[i].y > (pig_y + 7) || myAliens[i].y < (pig_y - 6))
                    continue;
                else  //eaten!
                {
                    playCherryEatenSound();
                    eaten++;
                    myAliens[i].alive = false; 
                    oinkCounter = 3;
                    score++;
                }
            }                
        }   
              
        if (oinkCounter > 0)
        {
            oinkCounter--;
            drawString(25, pig_y - 10, oinkString);  
        }
                
        if ((cherryTimeCounter % 2) != 0)
        {
            resetAliens();
          
            getReadyCounter++;
   
            drawString(47, 24, YouGot);
            drawNumber(77, 24, eaten, SMALLNUM);
            drawString(CENTERED, 32, Cherries);
                   
            if (getReadyCounter > 75)
            {
                if ((cherryTimeCounter == 1 && eaten > 10) || (cherryTimeCounter == 3 && eaten > 20) || (cherryTimeCounter == 5 && eaten > 30))
                {
                    drawString(CENTERED, 40, ExtraLife);                 
                    if (getReadyCounter == 76) lives++;
                } 
                
                if ((cherryTimeCounter == 1 && eaten == 20) || (cherryTimeCounter == 3 && eaten == 40) || (cherryTimeCounter == 5 && eaten == 60))
                {
                    drawString(CENTERED, 48, Perfect); 
                    drawString(CENTERED, 56, ExtraLife);   
                    
                    if (getReadyCounter == 76) 
                    {
                        score += cherryTimeCounter * 1000;
                        lives++;
                    }
                }
            }
                      
            if (getReadyCounter == 120)
            {
                cherryTimeCounter++;
                nextCounter = 0;
                getReadyCounter = 0;
                eaten = 0;
                stage = NewLevel;
            }
        }
    }
    
   
    
    
    if (stage == Wiggy)
    {
        getReadyCounter++;
        
        
        if (getReadyCounter < 32)
        {
            display.drawBitmap(103, 63 - getReadyCounter, SpaceWiggy, 24, 32, WHITE);
        }
        else if (getReadyCounter >31 && getReadyCounter < 225)
        {
            display.drawBitmap(103, 31, SpaceWiggy, 24, 32, WHITE);
        }
                    
        if (getReadyCounter > 32 && getReadyCounter < 85)
        {
            drawString(CENTERED, 24, OinkOink);
            drawString(CENTERED, 36, IamSpaceWiggy);    
        }
        
        if (getReadyCounter > 84 && getReadyCounter < 130)
        {
            drawString(CENTERED, 24, ILoveYou);
            drawString(CENTERED, 36, Nicholas);   
        }
        
        if (getReadyCounter > 129 && getReadyCounter < 180)
        {
            drawString(CENTERED, 20, ToSaveThe);
            drawString(CENTERED, 30, UniverseYou);  
            drawString(CENTERED, 40, MustBeBrave); 
        }
        
        if (getReadyCounter > 179 && getReadyCounter < 225)
        {
            drawString(CENTERED, 20, GoodLuck);
            drawString(CENTERED, 30, AgainstThe);
            drawString(CENTERED, 40, FinalBoss);     
        }
        
        
        if (getReadyCounter == 255)
        {
            level++;
            stage = NewLevel;
        } 
    }
    
    
    
    if (stage == Endgame)
    {
        getReadyCounter++;
               
        if (getReadyCounter < 32)
        {
            display.drawBitmap(103, 63 - getReadyCounter, SpaceWiggy, 24, 32, WHITE);
        }
        else
        {
            display.drawBitmap(103, 31, SpaceWiggy, 24, 32, WHITE);
        }
      
        if (getReadyCounter % 5 == 0)
        {
            for (int i = 0; i < MAX_STARS; i++) ob[i].type--;      
        } 
        
        if (getReadyCounter == 45)
        {
            getReadyCounter = 0;
            stage = Credits;
        }
    }
    
    
    if (stage == Credits)
    {
        display.drawBitmap(103, 31, SpaceWiggy, 24, 32, WHITE);
        static int y = 63;
        static int page = 0;
        
        if (page == 0)
        {        
            drawString(CENTERED, y, Congratulations);
            drawString(CENTERED, y + 10, BlankLine);
            drawString(CENTERED, y + 20, YouHaveDefeated);
            drawString(CENTERED, y + 30, TheDreaded);
            drawString(CENTERED, y + 40, bossHelloKittyString);
            drawString(CENTERED, y + 50, SpaceSkeleton);
            drawString(CENTERED, y + 60, NowYouHaveSaved);
            drawString(CENTERED, y + 70, TheUniverse);
            drawString(CENTERED, y + 80, AndAllThePigs);
            drawString(CENTERED, y + 90, AreHappy);
            drawString(CENTERED, y + 100, BlankLine);
            drawString(CENTERED, y + 110, BlankLine);
            drawString(CENTERED, y + 120, BlankLine);
            drawString(CENTERED, y + 130, SpacePig);
            drawString(CENTERED, y + 140, ForYour8th);
            drawString(CENTERED, y + 150, Birthday);
            drawString(CENTERED, y + 160, TenthOctober2014);
            drawString(CENTERED, y + 170, BlankLine);
            drawString(CENTERED, y + 180, LoveFromPapi);
            
            if (--y < -190)
            {
                y = 63;
                getReadyCounter = 0;
                page++;
            }
        }
        else
        {
          
            drawString(CENTERED, y, GameDesignBy);
            drawString(CENTERED, y + 10, GraphicsBy);
            drawString(CENTERED, y + 20, MusicBy);
            drawString(CENTERED, y + 30, ProgrammingBy);
            drawString(CENTERED, y + 40, BlankLine);
            drawString(CENTERED, y + 50, edquek);  
            drawString(CENTERED, y + 60, BlankLine);
            drawString(CENTERED, y + 70, BlankLine);
            drawString(CENTERED, y + 80, BlankLine);
            drawString(CENTERED, y + 90, BlankLine);
            drawString(CENTERED, y + 100, TheEnd);
            drawString(CENTERED, y + 110, BlankLine);
            drawString(CENTERED, y + 120, BlankLine);
            drawString(CENTERED, y + 130, BlankLine); 
   
            if (--y == -140)
            {
                y = 63;
                getReadyCounter = 0;
                page = 0;
                stage = GameOver; 
            }         
        }     
    }
    
    
    
    if (stage == GetReady)
    {       
        flashCounter++;
        
        if (flashCounter == 20)
        {
            showText = !showText;
            
            flashCounter = (!showText) ? 12 : 0;
        }
        
        if (showText)
        {
            if (level == 5 || level == 11 || level == 17 || level == 23)
            {
                drawString(CENTERED, 24, hyperspaceString); 
                drawString(CENTERED, 36, bossStageString);
            }
            else
            {  
                drawString(CENTERED, 32, getReadyString);
            }
        }
          
        int r;
   
        
        byte localArray[MAX_ALIENS * 2];
        
              
        for (char i = 0; i < MAX_ALIENS * 2; i++)
        {
            localArray[i] = startCoord3DArray[i + (level * (MAX_ALIENS * 2))];    
        }
                                  
        for (char i = 0; i < levelData[level]; i++)
        {          
            myAliens[i].x -= (myAliens[i].x - localArray[(i << 1) + 0]) >> 1;
            
            r = localArray[(i << 1) + 1];
   
   
            if (myAliens[i].y >= r)
            {
                myAliens[i].y -= (myAliens[i].y - r) >> 1;  
            }
            else
            {
                myAliens[i].y += (r - myAliens[i].y) >> 1;    
            }
        }
 
 
        if (level == 5 || level == 11 || level == 17 || level == 23)
        {
            bossHealth++;
        }
                                     
        getReadyCounter++;
        
        if (getReadyCounter == 45)
        {
            getReadyCounter = 0;
            stage = InGame; 
        }  
        
        if (getReadyCounter % 5 == 0)
        {
            for (int i = 0; i < MAX_STARS; i++)
            {
                if (level == 5 || level == 11 || level == 17 || level == 23)
                {
                    ob[i].type++;  
                }
                else if (level == 6 || level == 12 || level == 18)
                {
                    ob[i].type--;      
                }
            }
        }
    }
    
    /* DRAW THE BOSS HEALTH BAR */
    if (((level == 5 || level == 11 || level == 17 || level == 23) && stage != Endgame && stage != Credits) && stage != LevelCompleted && stage != GotHighScore && stage != GameOver && stage != Attract)
    {
       drawString(16, 59, bossHealthString); 
       display.drawRect(33, 60, 78, 4, WHITE);
       display.drawFastHLine(34, 61, (float)((float)76 / 45) * bossHealth, WHITE);
       display.drawFastHLine(34, 62, (float)((float)76 / 45) * bossHealth, WHITE);   
    }
    

    /* DRAW THE PIG AND ANIMATE HIS LEGS */
    if (++ altlegcounter == 20)
    {
        altlegsFlag = !altlegsFlag;
        altlegcounter = 0;
    }
    
    if (stage != Attract && stage!= GameOver)
    {
        display.drawBitmap(pig_x, pig_y - 6, space_pig_bmp, 24, 13, WHITE);
          
        if (altlegsFlag)
        {  
            display.drawBitmap2(pig_x + 2, pig_y + 1, altlegs, 16, 6, WHITE);
        }
    }
    
             
    for (byte i = 0; i < levelData[level]; i++)
    {
        if (stage != CherryTime)
        {  
            if (myAliens[i].alive)
            {
                switch (level)
                {
                    case 0: case 1: case 2: case 3: case 4:
                    case 6: case 7: case 8: case 9: case 10:
                    case 12: case 13: case 14: case 15: case 16:
                    case 18: case 19: case 20: case 21: case 22:
                        draw12x8Bitmap(myAliens[i].x - 6, myAliens[i].y - 4, (level << 1) + (altlegsFlag ? 1 : 0));
                        break;
                    case 5:
                        display.drawBitmap(myAliens[i].x - 8, myAliens[i].y - 8, (altlegsFlag ? boss1 : boss1b), 16, 16, WHITE);
                        break;   
                    case 11:
                        display.drawBitmap(myAliens[i].x - 12, myAliens[i].y - 8, (altlegsFlag ? boss2 : boss2b), 24, 16, WHITE);
                        break; 
                    case 17:
                        display.drawBitmap(myAliens[i].x - 16, myAliens[i].y - 12, (altlegsFlag ? boss3 : boss3b), 32, 24, WHITE);                                      
                        break;  
                    case 23:                 
                        display.drawBitmap(myAliens[i].x - 16, myAliens[i].y - 16, (altlegsFlag ? boss4 : boss4b), 32, 32, WHITE);
                        break; 
                }
            }
        }
        else    // WE ARE USING THE ALIENS ARRAY TO DRAW THE CHERRIES TO SAVE BYTES       
        {
            if (myAliens[i].alive) display.drawBitmap(myAliens[i].x - 4, myAliens[i].y - 4, cherry, 8, 8, WHITE);
        }  
    }
         
  
  
    if (stage == InGame || stage == CherryTime || stage == GetReady || stage == PigHit || stage == LevelCompleted || stage == NewLevel || stage == Wiggy || stage == Endgame || stage == Credits)
    {    
        for (byte i = 0; i < MAX_EXPLOSION_DOTS; i++)
	{
            if (myExplosionDots[i].alive)
            {
                myExplosionDots[i].show();
            }
        } 
        
        drawString(0, 0, scoreString);
        drawNumber(25, 0, score, BIGNUM);
        drawString(51, 0, levelString);   
        
        if (stage == CherryTime)
        {
            drawString(75, 0, ctString);
        } 
        else
        {
          drawNumber(75, 0, level, SMALLNUM);  
        }
      
        if (lives >= 0 && lives <= 5)
        {
          for (byte i = 0; i < lives; i++)
          {
              drawString(88 + i * 8, 0, heartString);  
          }
        }
        else
        {
            drawString(98, 0, heartString);    
            drawChar(110, 0, 33);
            drawNumber(118, 0, lives, SMALLNUM);
        }
    }
    
  
    
    if (stage != PigHit)
    {    
        if (digitalRead(10) == HIGH && pig_y > 6) pig_y -= 2;   // MOVE UP AND DOWN
        if (digitalRead(11) == HIGH && pig_y < 58) pig_y += 2;
    }    
        
    
    if (stage == InGame)
    {
        bool changeX = false;
        bool changeY = false;
        byte r;
        byte xBound;
        
        switch (level)
        {
            case 0: case 1: case 2: case 3: case 4: case 5: 
                xBound = 64;
                break;
            
            case 6: case 7: case 8: case 9: case 10: case 11: 
                xBound = 53;
                break;
                
            case 12: case 13: case 14: case 15: case 16: case 17:
                xBound = 54;
                break;
                
            case 18: case 19: case 20: case 21: case 22: case 23:
                xBound = 40; 
                break;                
        }
        
        
        for (byte i = 0; i < levelData[level]; i++)   // HAS ANY ALIEN TOUCHED THE EDGE?  
        {        
            byte r = myAliens[i].movestep << 1;
            
            if (myAliens[i].alive)     // IGNORE IF NOT ALIVE
            {   
                if ((myAliens[i].x < xBound && (localvec[r] * myAliens[i].dirx) < 0) || (myAliens[i].x > 124 && (localvec[r] * myAliens[i].dirx) > 0))  
                {  
                    if (level != 3 && level != 4 && level != 13 && level != 15 && level != 12 && level != 22)
                    {
                        changeX = true;  
                        break;
                    }  
                    else 
                    {
                        myAliens[i].dirx *= -1;
                    }
                }
                               
                if (level == 12 || level == 22)
                {                 
                    if (myAliens[i].y < 4) myAliens[i].y = 59;
                    if (myAliens[i].y > 59) myAliens[i].y = 4;
                }               
                else if ((myAliens[i].y < 4 && (localvec[r + 1] * myAliens[i].diry) < 0)  || (myAliens[i].y  > 59 && (localvec[r + 1] * myAliens[i].diry) > 0))  
                { 
                    if (level != 3 && level != 4 && level != 6 && level != 14 && level != 15 && level != 16  && level != 21)
                    {
                        changeY = true;  
                        break;
                    }  
                    else
                    {
                        myAliens[i].diry *= -1;
                    }
                    
                }
            }                
        }    
         
        for (byte i = 0; i < levelData[level]; i++)   // CHANGE THEIR DIRECTIONS
        {                  
            if (myAliens[i].alive)     // IGNORE IF NOT ALIVE
            {         
                if (changeX)  myAliens[i].dirx *= -1;                               
                if (changeY)  myAliens[i].diry *= -1;  
               
                if (++myAliens[i].movestep == 10)    // INC/LOOP THE MOVEMENT VECTOR 
                {
                    myAliens[i].movestep = 0;
                }
                
                r = (myAliens[i].movestep << 1);      // GET THE NEXT MOVEMENT VECTO
                            
                if (level != 12)
                {            
                    myAliens[i].x += localvec[r] * myAliens[i].dirx;
                    myAliens[i].y += localvec[r + 1] * myAliens[i].diry; 
                }
                else
                {            
                    myAliens[i].x += myAliens[i].dirx;
                    myAliens[i].y += myAliens[i].diry; 
                }
            }
        }
               
        changeX = changeY = false;
 
 
        
        counterSinceFired++;
              
        if (digitalRead(12) == HIGH)
        {  
            if (bulletCount < MAX_BULLETS && counterSinceFired > 8)
            {   
                playFireSound(); 
                
                for (byte i = 0; i < MAX_BULLETS; i++)
                {
                    if (bullets[i].alive == false)
                    {
                         bullets[i].fire(25, pig_y);
                         counterSinceFired = 0;
                         bulletCount ++;
                         break;
                    }
                }
            }         
        }
       


        for (int i = 0; i < levelData[level]; i++)
        {
            if (myAliens[i].alive)
            {  
                
                if (random(firingProb) == 0)      // set firing rate for aliens
                {               
                    if (alienBulletCount < firingMaxBullet)
                    {
                        for (byte j = 0; j < MAX_ALIEN_BULLETS; j++)
                        {
                            if (alienBullets[j].alive == false)
                            {
                                 alienBullets[j].fire(myAliens[i].x - 4, myAliens[i].y);   
                                 playAlienFireSound();                          
                                 alienBulletCount++;
                                 goto outer;
                            }
                        }
                    } 
                    
                    outer:
                        continue;  
                } 
            }            
        }
       
        for (byte i = 0; i < MAX_ALIEN_BULLETS; i++)
        {  
 
            if (alienBullets[i].alive)
            {
                alienBullets[i].move();
            }
            else
            {
                continue;
            }         
            
            if (alienBullets[i].x > 20) continue;
            else if (alienBullets[i].y < pig_y - 6) continue;
            else if (alienBullets[i].y > pig_y + 5) continue;
            else // PIG IS HIT!
            {   
                explode(alienBullets[i].x, alienBullets[i].y);     
                stage = PigHit;
                playPigHitSound();
                lives--;
                break;
            }
            
        }      
       
    
        // DRAW ALL THE BULLETS AND THEN DO SOME HIT TESTING
        for (byte i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].alive)
            {
                bullets[i].move();
                
                byte hitWidth, hitHeight;
  
                switch (level)
                {
                    case 0: case 1: case 2: case 3: case 4:
                    case 6: case 7: case 8: case 9: case 10:
                    case 12: case 13: case 14: case 15: case 16:
                    case 18: case 19: case 20: case 21: case 22:
                        hitWidth = hitHeight = 4;
                        break;
                    case 5: case 11: 
                        hitWidth = hitHeight = 8;
                        break;   
                    case 17: 
                        hitWidth = 16;
                        hitHeight = 12;
                        break;
                    case 23:
                        hitWidth = hitHeight = 16;
                        break;  
                }
                            
                for (byte j = 0; j < levelData[level]; j++)
                {
                    if (!myAliens[j].alive) continue;        
                    
                         if (bullets[i].x < myAliens[j].x - hitWidth) continue;
                    else if (bullets[i].x > myAliens[j].x + hitWidth) continue;
                    else if (bullets[i].y < myAliens[j].y - hitHeight) continue; 
                    else if (bullets[i].y > myAliens[j].y + hitHeight) continue;
                    else // HIT!
                    {
                        bullets[i].alive = false;
                        bulletCount--;                     
                        score++;
                                
                        switch (level)
                        {
                            case 0: case 1: case 2: case 3: case 4:
                            case 6: case 7: case 8: case 9: case 10:
                            case 12: case 13: case 14: case 15: case 16:
                            case 18: case 19: case 20: case 21: case 22:
                                playAlienHitSound();
                                myAliens[j].alive = false;                   
                                break;                               
                            case 5:  bossHealth -= 5;  playBossHitSound(); break;   
                            case 11: bossHealth -= 3;  playBossHitSound(); break;   
                            case 17: bossHealth -= 3;  playBossHitSound(); break; 
                            case 23: bossHealth -= 1;  playBossHitSound(); break;      
                        }
                        
                        explode(myAliens[j].x, myAliens[j].y);
       
                        if (level == 5 || level == 11 || level == 17 || level == 23)
                        {
                            if (bossHealth <= 0)
                            {
                                //level++;
                                getReadyCounter = 0;
                                numAliens--;
                                score += level * 1000;
                                stage = LevelCompleted;  
                               //stage = GameOver;     uncomment this and comment above to test initial entry 
                            }    
                        }
                        else
                        {
                            if (--numAliens == 0)
                            {
                                //level++;
                                getReadyCounter = 0;
                                stage = LevelCompleted;                                     
                            }        
                        }
                        
                        break;                    
                    }                
                }               
           }   
       }
   }
    
    
    if (stage == PigHit)
    {
        bulletCount = 0;
        alienBulletCount = 0;
        counterSinceFired = 0;
        
        resetBullets();
        
        display.invertDisplay(getReadyCounter++ % 2 == 0);   
        
        if (getReadyCounter == 23)
        {
            getReadyCounter = 0;
            display.invertDisplay(false); 
            stage = InGame;
            
            if (lives == 0)
            {
                resetAliens();
                stage = GameOver;
            }
        }                  
    }


    
    if (stage == GameOver)
    {
        getReadyCounter++;
        
        if ((level == 5 || level == 11 || level == 17 || level == 23) && lives == 0)
        {
            if (getReadyCounter <= 45)
            {
                if (getReadyCounter %5 == 0)
                {
                    for (int i = 0; i < MAX_STARS; i++)
                    {
                        ob[i].type--;
                    }
                }   
            }          
        }
        
        if (getReadyCounter == 75)
        {
            getReadyCounter = 0;
 
            if (score > readHighScore())
            {
                stage = GotHighScore;  
                counterSinceFired = 10;        
                flashCounter = 0;
                initials[0] = 10;
                initials[1] = 42;
                initials[2] = 42;
            }
            else
            {
                stage = Attract;
            }
        }
               
        drawString(CENTERED, 32, gameOverString);      
    }
    
    
    
    if (stage == GotHighScore)
    {
        getReadyCounter ++;
        counterSinceFired++;
        
        if (++flashCounter == 12) flashCounter = 0;
                      
        drawString(CENTERED, 16, HighScore);
        drawString(CENTERED, 24, EnterName);   
  
        static byte whichLetter = 0;
        
        if (whichLetter == 0)
        {
            if (flashCounter < 6) drawInitialsChar(52, 40, initials[0]);
        }
        else
            drawInitialsChar(52, 40, initials[0]);  
        
        if (whichLetter == 1)
        {
            if (flashCounter < 6) drawInitialsChar(62, 40, initials[1]);   
        }
        else
            drawInitialsChar(62, 40, initials[1]); 
                      
        if (whichLetter == 2){
            if (flashCounter < 6) drawInitialsChar(72, 40, initials[2]);
        }
        else
            drawInitialsChar(72, 40, initials[2]);
            

        if (digitalRead(10) == HIGH && counterSinceFired > 5)  
        {
            counterSinceFired = 0;
            getReadyCounter = 0;
            initials[whichLetter]++;
            if (initials[whichLetter] > 42) initials[whichLetter] = 0;  
        }
        
        if (digitalRead(11) == HIGH && counterSinceFired > 5)
        {
            counterSinceFired = 0;
            getReadyCounter = 0;
            initials[whichLetter]--;
            if (initials[whichLetter] < 0) initials[whichLetter] = 42;  
        }
        
        if (digitalRead(12) == HIGH && counterSinceFired > 5)
        {
            whichLetter++;
            getReadyCounter = 0;
            counterSinceFired = 0;
               
            initials[whichLetter] = 10;   
               
            if (whichLetter == 3)
            {
                getReadyCounter = 0;
                writeHighScore(score);
                stage = Attract;  
            }
         }
       
         if (getReadyCounter == 255)
         {
             getReadyCounter = 0;
             writeHighScore(score);
             stage = Attract; 
         }  
    }
    
    
    if (stage == Attract)
    {
        getReadyCounter++;
        if (++flashCounter == 40) flashCounter = 0;
        
        for (int i = 0; i < 9; i++)
        {
            if (i == 5) 
            {
                continue;  
            }
            
            drawInitialsChar(30 + (i * 8), 0, pgm_read_byte(SpacePig + i));  
        }
        
        
        if (getReadyCounter < 80)
        {
            drawString(CENTERED, 16, ShootAliens);
            drawString(CENTERED, 28, SaveTheUniverse);            
            
            draw12x8Bitmap(40, 40, 0 + (altlegsFlag ? 1 : 0));
            draw12x8Bitmap(60, 40, 2 + (altlegsFlag ? 1 : 0));
            draw12x8Bitmap(80, 40, 4 + (altlegsFlag ? 1 : 0));
        }
        
        if (getReadyCounter > 79 && getReadyCounter < 160)
        {
            drawString(42, 22, Eat);
            drawString(56, 22, Cherries); 
         
            for (int i = 0; i < 3; i++)
            {
                display.drawBitmap(50 + (i * 10), 36, cherry, 8, 8, WHITE); 
            }  
        }
        
        if (getReadyCounter > 159)
        {
            drawString(CENTERED, 16, HighScore);
            drawNumber(57, 27, readHighScore(), BIGNUM);
            
            for (int i = 0; i < 3; i++)
            {
                drawInitialsChar(53 + i * 10, 38, initials[i]);          
            }
        }
        
        if (flashCounter > 20) drawString(CENTERED, 56, PressFireToPlay);
              
        if (digitalRead(12) == HIGH && getReadyCounter > 20)
       {
           playFireSound();
           getReadyCounter = 0;
           stage = NewGame; 
       } 
    }
      
    display.display();
}



static unsigned char  font[] PROGMEM = 
{
	0xEA, 0xAA, 0xE0,
	0x4C, 0x44, 0xE0,
	0xE2, 0xE8, 0xE0,
	0xE2, 0x62, 0xE0,
	0xAA, 0xE2, 0x20,
	0xE8, 0xE2, 0xE0,
	0xE8, 0xEA, 0xE0,
	0xE2, 0x22, 0x20,
	0xEA, 0xEA, 0xE0,
	0xEA, 0xE2, 0xE0,
	0x4A, 0xEA, 0xA0,
	0xCA, 0xCA, 0xC0,
	0x68, 0x88, 0x60,
	0xCA, 0xAA, 0xC0,
	0xE8, 0xC8, 0xE0,
	0xE8, 0xC8, 0x80,
	0x68, 0xAA, 0xE0,
	0xAA, 0xEA, 0xA0,
	0xE4, 0x44, 0xE0,
	0x62, 0x22, 0xC0,
	0xAA, 0xCA, 0xA0,
	0x88, 0x88, 0xE0,
	0xAE, 0xEA, 0xA0,
	0xCA, 0xAA, 0xA0,
	0x4A, 0xAA, 0x40,
	0xCA, 0xE8, 0x80,
	0x6A, 0xE2, 0x20,
	0xCA, 0xCA, 0xA0,
	0x68, 0xE2, 0xC0,
	0xE4, 0x44, 0x40,
	0xAA, 0xAA, 0xE0,
	0xAA, 0xAA, 0x40,
	0xAA, 0xEE, 0xA0,
	0xAA, 0x4A, 0xA0,
	0xAA, 0x62, 0x60,
	0xE2, 0x48, 0xE0,
        0x44, 0x40, 0x40,
        0x0, 0x40, 0x40,
        0x6F, 0xF7, 0x31, 
        0xCE, 0xEC, 0x80,
        0x4, 0xAE, 0xD6, 
        0x0, 0x0, 0x40, 
        0x0, 0xE0, 0x0, 
};

void drawChar(char x, char y, char which)
{
    for (register byte i = 0; i < 3; i++)
    {
        for (register byte j = 0 ; j < 4; j++)
        {
            uint8_t nextByte = pgm_read_byte(font + (which * 3) + i);
            
            if (nextByte & (128 >> j))
            {
                display.drawPixel(x + j, y + (i << 1), WHITE);
            }
            
            if (nextByte & (8 >> j))
            {
                display.drawPixel(x + j, y  + (i << 1) + 1, WHITE);
            }
        }
    }
}


void drawBigPixel(char x, char y)
{
    display.drawPixel(x, y, WHITE);
    display.drawPixel(x, y + 1, WHITE);
    display.drawPixel(x + 1, y, WHITE);
    display.drawPixel(x + 1, y + 1, WHITE);
}


void drawInitialsChar(char x, char y, char which)
{
    for (register byte i = 0; i < 3; i++)
    {
        for (register byte j = 0 ; j < 4; j++)
        {
            uint8_t nextByte = pgm_read_byte(font + (which * 3) + i);
            
            if (nextByte & (128 >> j))
            {
                drawBigPixel(x + (j * 2), y + (i * 4));
            }
            
            if (nextByte & (8 >> j))
            {
                drawBigPixel(x + (j * 2), y + (i * 4) + 2);
            }
        }
    }
}

void drawString(byte x, byte y, unsigned char* string)
{
    if (y < 0 || y > 63) return;
    
    unsigned char i = 0;
    unsigned char nextByte = 0;
    
    if (x == CENTERED)
    {
        while (1)
        {
            nextByte = pgm_read_byte(string + i++);
            if (nextByte == 99) break;  
        }
    
        i *= 4;
    
        x = 68 - (i >> 1);    
        
        i = 0;
    }
    
    while (1)
    {
        nextByte = pgm_read_byte(string + i++);
        
        if (nextByte == 0) continue; // space
    
        if (nextByte == 99) break;
        
        drawChar(x + ((i - 1) << 2), y, nextByte);
    }  
}

void drawNumber(byte x, byte y, unsigned int a, boolean bigSmall)  // prints out a 5 digit number max 65535 with leading zeroes
{
    unsigned int b;
    byte offset = 0;
    
    for (unsigned int i = bigSmall ? 10000 : 10; i > 0; i /= 10)
    {
        b = a / i;
        a = a - (b * i);
        
  //      if (!bigSmall && i == 10 && b != 0) // omit leading zero for 2 digit number
    //    {
            drawChar(x + offset, y, b);  
            offset += 4;
      //  }
    }   
}

static unsigned char inv[] PROGMEM = 
{
    0x20, 0x41, 0x8, 0x3F, 0xC6, 0xF6, 0xFF, 0xFB, 0xFD, 0xA0, 0x51, 0x98,
    0x20, 0x49, 0x9, 0xBF, 0xDE, 0xF7, 0xFF, 0xF3, 0xFC, 0x20, 0x44, 0x2,
    
    0xF, 0x7, 0xFE, 0xFF, 0xFE, 0x67, 0xFF, 0xF1, 0x98, 0x36, 0xCC, 0x3,
    0xF, 0x7, 0xFE, 0xFF, 0xFE, 0x67, 0xFF, 0xF3, 0x9C, 0x66, 0x63, 0xC,
    
    0x6, 0x0, 0xF0, 0x1F, 0x83, 0x6C, 0x3F, 0xC1, 0x68, 0x20, 0x41, 0x8,
    0x6, 0x0, 0xF0, 0x1F, 0x83, 0x6C, 0x3F, 0xC0, 0x90, 0x16, 0x82, 0x94,
    
    0x9, 0x8, 0x91, 0xFF, 0xF1, 0x68, 0xFF, 0xFC, 0xF3, 0x8F, 0x10, 0x60,
    0x9, 0x8, 0x91, 0xFF, 0xF1, 0x68, 0x1F, 0x87, 0xFE, 0xEF, 0x7C, 0x63,
    
    0xC6, 0x3E, 0xF7, 0x7F, 0xE1, 0xF8, 0x16, 0x8F, 0xFF, 0x89, 0x10, 0x90,
    0x6, 0x8, 0xF1, 0xCF, 0x3F, 0xFF, 0x16, 0x8F, 0xFF, 0x89, 0x10, 0x90,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // BLANK cos BOSS HAVE OWN GRAPHICS
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    
    0x1F, 0x87, 0xFE, 0xDF, 0xBF, 0x9F, 0xFF, 0xFE, 0x8F, 0x22, 0x43, 0xFC,
    0x1F, 0x87, 0xFE, 0xDF, 0xBF, 0x9F, 0xFF, 0xF6, 0xAE, 0x3F, 0xC0, 0x0,

    0x2, 0x0, 0x60, 0x1F, 0xC3, 0xFE, 0xEF, 0xF3, 0xFF, 0x34, 0xAE, 0x25,
    0x3F, 0xC0, 0x60, 0x1F, 0xC3, 0xFE, 0x2F, 0xFF, 0xFF, 0xE4, 0xA0, 0x94,
    
    0x4, 0x0, 0x88, 0x3F, 0x3F, 0x7C, 0xFF, 0xC3, 0xF3, 0x8, 0x80, 0x40,
    0x20, 0x2, 0x11, 0x3F, 0xAF, 0xFC, 0xF7, 0xC3, 0xF2, 0x21, 0x12, 0x0,
    
    0x7F, 0xC5, 0xB4, 0x7F, 0xC3, 0x58, 0xBF, 0xA5, 0x54, 0x19A, 0x3, 0x18,    
    0x7F, 0xCF, 0x7E, 0x7F, 0xCA, 0xB8, 0x3F, 0x85, 0x54, 0x19A, 0x21, 0x50,
    
    0xDF, 0x7, 0x9F, 0x7B, 0x91, 0xFF, 0xF, 0xE, 0xF0, 0x39, 0xFE, 0xF0,
    0x1F, 0x67, 0x9A, 0xFB, 0xA9, 0xFC, 0xF, 0x0, 0xF7, 0xF9, 0xC0, 0xF7,
    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // BLANK cos BOSS HAVE OWN GRAPHICS
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    
    0x80, 0x1C, 0x3, 0x7F, 0xE7, 0x32, 0x3F, 0xC3, 0xC, 0x1F, 0x80, 0x0,
    0x80, 0x1C, 0x3, 0x7F, 0xE4, 0xCE, 0x3F, 0xC2, 0x4, 0x10, 0x80, 0xF0,
    
    0x9, 0x0, 0x61, 0x46, 0x2B, 0xFC, 0x26, 0x43, 0xFC, 0x16, 0x82, 0x10,
    0x6, 0x8, 0x60, 0x46, 0x23, 0xFD, 0x26, 0x43, 0xFC, 0x16, 0x80, 0x84,
    
    0x20, 0x43, 0xFC, 0x2B, 0x40, 0xF0, 0xB, 0x7, 0xFE, 0x49, 0x21, 0x98,
    0x20, 0x43, 0xFC, 0x2B, 0x47, 0xFE, 0x49, 0x20, 0x90, 0x9, 0x1, 0x98,
    
    0x1F, 0x82, 0xEC, 0x66, 0x6E, 0x67, 0xFF, 0xFF, 0xFF, 0x6D, 0xB2, 0x49,
    0x1F, 0x82, 0x64, 0x66, 0x6F, 0x77, 0xFF, 0xFF, 0xFF, 0xDB, 0x69, 0x24,
    
    0xF, 0x3, 0xFC, 0xE, 0xE0, 0x3F, 0x3, 0xF0, 0xFE, 0x3F, 0xC0, 0xF0,
    0xF, 0x3, 0xFC, 0x7E, 0xEF, 0xFF, 0xFF, 0xF7, 0xFE, 0x3F, 0xC0, 0xF0,
    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // BLANK cos BOSS HAVE OWN GRAPHICS
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    0x1F, 0x87, 0xFE, 0xDF, 0xBF, 0x9F, 0xFF, 0xFE, 0x8F, 0x22, 0x43, 0xFC,
    0x1F, 0x87, 0xFE, 0xDF, 0xBF, 0x9F, 0xFF, 0xF6, 0xAE, 0x3F, 0xC0, 0x0,

    0x2, 0x0, 0x60, 0x1F, 0xC3, 0xFE, 0xEF, 0xF3, 0xFF, 0x34, 0xAE, 0x25,
    0x3F, 0xC0, 0x60, 0x1F, 0xC3, 0xFE, 0x2F, 0xFF, 0xFF, 0xE4, 0xA0, 0x94,
    
    0x4, 0x0, 0x88, 0x3F, 0x3F, 0x7C, 0xFF, 0xC3, 0xF3, 0x8, 0x80, 0x40,
    0x20, 0x2, 0x11, 0x3F, 0xAF, 0xFC, 0xF7, 0xC3, 0xF2, 0x21, 0x12, 0x0,
    
    0x7F, 0xC5, 0xB4, 0x7F, 0xC3, 0x58, 0xBF, 0xA5, 0x54, 0x19A, 0x3, 0x18,    
    0x7F, 0xCF, 0x7E, 0x7F, 0xCA, 0xB8, 0x3F, 0x85, 0x54, 0x19A, 0x21, 0x50,
    
    0xDF, 0x7, 0x9F, 0x7B, 0x91, 0xFF, 0xF, 0xE, 0xF0, 0x39, 0xFE, 0xF0,
    0x1F, 0x67, 0x9A, 0xFB, 0xA9, 0xFC, 0xF, 0x0, 0xF7, 0xF9, 0xC0, 0xF7,
    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // BLANK cos BOSS HAVE OWN GRAPHICS
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void draw12x8Bitmap(char x, char y, char which)
{
    for (register byte i = 0; i < 4; i++)
    {
        // draw first byte easy
        
        for (register byte j = 0 ; j < 8; j++)
        {
            uint8_t nextByte = pgm_read_byte((which << 3) + (which << 2) + inv + (i << 1) + i);
            
            if (nextByte & (128 >> j))
            {
                display.drawPixel(x + j, y + (i << 1), WHITE);
            }
        }    
        
        // draw the next upper nybble
        
        for (register byte j = 0 ; j < 4; j++)
        {
            uint8_t nextByte = pgm_read_byte((which << 3) + (which << 2) + inv + (i << 1) + i + 1);
            
            if (nextByte & (128 >> j))
            {
                display.drawPixel(x + j + 8, y + (i << 1), WHITE);
            }
         }
        
        // draw the next lower nybble
        
        for (register byte j = 0 ; j < 4; j++)
        {
            uint8_t nextByte = pgm_read_byte((which << 3) + (which << 2) + inv + (i << 1) + i + 1);
            
            if (nextByte & (8 >> j))
            {
                display.drawPixel(x + j, y + (i << 1) + 1, WHITE);
            }
         }
        
        
        // draw the trailing byte
        for (register byte j = 0 ; j < 8; j++)
        {
            uint8_t nextByte = pgm_read_byte((which << 3) + (which << 2) + inv + (i << 1) + i + 2);
            
            if (nextByte & (128 >> j))
            {
                display.drawPixel(x + j + 4, y + (i << 1) + 1, WHITE);
            }
        }
    }
}



/* PLAY SOUND FUNCTIONS */

void playFireSound()
{
    for (int j = 6000; j > 4500; j -=100)
    {
        tone(13, j, 10);
        delay(5);
    } 
}

void playAlienFireSound()
{
    for (int j = 2000; j < 3500; j +=100)
    {
        tone(13, j, 10);
        delay(5);
    } 
}

int bleep2[] PROGMEM  = { NOTE_A7, NOTE_CS7, NOTE_E7, NOTE_A8, NOTE_E7, NOTE_CS7, NOTE_A7 };
char lengths2[] PROGMEM  = { 1, 1, 1, 1, 1, 1, 1 } ; 
        
void playAlienHitSound()
{    
    for (int j = 0; j < 7; j++)
    {
        tone(13, pgm_read_word(&(bleep2[j])), pgm_read_byte(&(lengths2[j])) * 50);
        delay (35);
    } 
}    
  
  
void playPigHitSound()
{
    tone(13, 10, 100);
    delay (100);
    tone(13, 100, 150);
    delay (150);
    delay(150);
    tone(13, 10, 100);
    delay (100);
    tone(13, 100, 150);
    delay (150);
}


int bleep3[] PROGMEM = { NOTE_C7, NOTE_C8, NOTE_C7, NOTE_C8 };
byte lengths3[] PROGMEM = { 1, 1, 1, 1 } ; 
    
void playCherryEatenSound()
{       
    for (int j = 0; j < 4; j++)
    {
        tone(13, pgm_read_word(&(bleep3[j])), pgm_read_byte(&(lengths3[j])) * 75);
        delay (15);
    }
}  
    
    
void playBossHitSound() 
{
    tone(13, 150, 10);
    delay(5);
}


void resetBullets()
{
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].alive= false;
    }
    
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++)
    {
        alienBullets[i].alive = false;  
    }
}


void resetAliens()
{
    for(int i = 0; i < MAX_ALIENS; i++)
    {
        myAliens[i].alive = false;
    }    
}



void explode (int x, int y)
{
    for (byte k = 0; k < MAX_EXPLOSION_DOTS; k++)
    {
        for (byte l = 0; l < MAX_EXPLOSION_DOTS; l++)
        {
            if (myExplosionDots[l].alive == false)
            {
                myExplosionDots[l].alive = true;
                myExplosionDots[l].x = x;
                myExplosionDots[l].y = y;
                myExplosionDots[l].dirx = random(3) + 1;
                if (random(2) % 2) myExplosionDots[l].dirx *= -1;
                myExplosionDots[l].diry = random(4) + 1;
                if (random(2) % 2) myExplosionDots[l].diry *= -1;
                myExplosionDots[l].lifespan = random(10) + 10;
            }
        }
    }    
}


unsigned int readHighScore()
{   
    for (int i = 0; i < 3; i++)
    {
        initials[i] = EEPROM.read(i + 2);
    }
    
    unsigned int highScore = EEPROM.read(0) << 8;
 
    return (highScore | EEPROM.read(1));       
}


void writeHighScore(unsigned int score)
{
    EEPROM.write(1, score);  
    score = score >> 8;   
    EEPROM.write(0, score);
       
    for (int i = 0; i < 3; i++)
    {
        EEPROM.write(i + 2, initials[i]);
    }
}
