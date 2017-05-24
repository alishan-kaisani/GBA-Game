#define REG_DISPCNT *(unsigned short*) 0x4000000
#define MODE_3 3
#define BG2_EN (1 << 10)

#define COLOR(R,G,B) (((R) & 0x1F) | (((G) & 0x1F) << 5) | (((B) & 0x1F) << 10))
#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define ORANGE COLOR(255,165, 0)
#define INDIGO COLOR(75,0,130)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

#define OFFSET(row, col, rowLen) ((row)*(rowLen) + (col))

//Buttons
#define BUTTONS       *(volatile unsigned short*) 0x4000130
#define BUTTON_A      (1 << 0)
#define BUTTON_B      (1 << 1)
#define BUTTON_START  (1 << 2)
#define BUTTON_SELECT (1 << 3)
#define BUTTON_RIGHT  (1 << 4)
#define BUTTON_LEFT   (1 << 5)
#define BUTTON_UP     (1 << 6)
#define BUTTON_DOWN   (1 << 7)
#define BUTTON_R      (1 << 8)
#define BUTTON_L      (1 << 9)
#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

/* DMA */

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

#define REG_DMA0SAD         *(volatile unsigned int*)0x40000B0 		// source address
#define REG_DMA0DAD         *(volatile unsigned int*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile unsigned int*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(volatile unsigned int*)0x40000BC 		// source address
#define REG_DMA1DAD         *(volatile unsigned int*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile unsigned int*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(volatile unsigned int*)0x40000C8 		// source address
#define REG_DMA2DAD         *(volatile unsigned int*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile unsigned int*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(volatile unsigned int*)0x40000D4 		// source address
#define REG_DMA3DAD         *(volatile unsigned int*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile unsigned int*)0x40000DC       // control register

#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

//Game DEFINES
#define playerSIZEOG 5
#define FIRES_CAP 30

typedef struct {
	int row;
	int col;
    int numGrow;
} playerST;

typedef struct {
    int row[FIRES_CAP];
    int col[FIRES_CAP];
    int ndx;
} fireST;

extern int rd;
extern int cd;

extern unsigned int playerCOLOR;
extern int playerSIZE;
extern int cookieCOL;
extern int cookieROW;
extern fireST fires;
extern unsigned int colors[];
extern int colors_len;
extern unsigned short *videoBuffer;

//Screen Prototypes
void fillScreen3(unsigned short color);
void waitForVblank();
void drawRect(int row, int col, int height, int width, volatile unsigned short color);
void setPixel(int row, int col, unsigned short color);
void drawImage3(int row, int col, int width, int height, const unsigned short *image);
void drawPlayer(playerST player);
void drawCookie();
void drawFire();

//GAME Logistics Prototypes;
void generateCookie(playerST player);
void generateFire();
playerST generatePlayer();
playerST startGame();
void clearFires();
void endGAME();
int gameWonf(playerST player);

//GAME Logic Prototypes
int hitWall(playerST player);
int hitCookie(playerST player);
int hitFire(playerST player);

//GAME Play Prototypes
void growplayer();
