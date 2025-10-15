#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Game of Life config
const int CELL_SIZE = 4;  // Smaller cells
const int GRID_WIDTH = SCREEN_WIDTH / CELL_SIZE;   // 32
const int GRID_HEIGHT = SCREEN_HEIGHT / CELL_SIZE; // 16

// Bit-packed grids: each bit is a cell
// Each row has GRID_WIDTH cells, so number of bytes per row = ceil(GRID_WIDTH/8)
const int BYTES_PER_ROW = (GRID_WIDTH + 7) / 8;

uint8_t currentGrid[GRID_HEIGHT][BYTES_PER_ROW];
uint8_t nextGrid[GRID_HEIGHT][BYTES_PER_ROW];

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
  randomSeed(analogRead(0));
  initializeGrid();
}

void loop() {
  drawGrid();

  bool stuck = !updateGrid();

  delay(200);

  if (stuck) {
    delay(500);
    wipeScreen();
    delay(500);
    initializeGrid();
  }
}

// Set or clear a cell in the grid
void setCell(uint8_t grid[][BYTES_PER_ROW], int x, int y, bool state) {
  if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return;
  uint8_t mask = 1 << (x % 8);
  if (state) {
    grid[y][x / 8] |= mask;
  } else {
    grid[y][x / 8] &= ~mask;
  }
}

// Get a cell's state from the grid
bool getCell(uint8_t grid[][BYTES_PER_ROW], int x, int y) {
  if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return false;
  uint8_t mask = 1 << (x % 8);
  return (grid[y][x / 8] & mask) != 0;
}

// Set up the grid randomly
void initializeGrid() {
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      setCell(currentGrid, x, y, random(2));
    }
  }
}

// Draw current state to the OLED display
void drawGrid() {
  display.clearDisplay();
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      if (getCell(currentGrid, x, y)) {
        display.fillRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, SSD1306_WHITE);
      }
    }
  }
  display.display();
}

// Returns true if grid changed this generation
bool updateGrid() {
  bool changed = false;

  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      int neighbors = countNeighbors(x, y);
      bool currentState = getCell(currentGrid, x, y);
      bool newState = currentState
                      ? (neighbors == 2 || neighbors == 3)
                      : (neighbors == 3);
      setCell(nextGrid, x, y, newState);

      if (newState != currentState) {
        changed = true;
      }
    }
  }

  // Copy nextGrid into currentGrid
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int b = 0; b < BYTES_PER_ROW; b++) {
      currentGrid[y][b] = nextGrid[y][b];
    }
  }

  return changed;
}

// Count living neighbor cells
int countNeighbors(int x, int y) {
  int count = 0;
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if (dx == 0 && dy == 0) continue;
      int nx = x + dx;
      int ny = y + dy;
      if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT) {
        if (getCell(currentGrid, nx, ny)) count++;
      }
    }
  }
  return count;
}

// Screen wipe
void wipeScreen() {
  for (int y = 0; y < SCREEN_HEIGHT; y += 4) {
    display.fillRect(0, y, SCREEN_WIDTH, 5, SSD1306_WHITE);
    display.display();
    delay(5);
  }

  delay(100);
  display.clearDisplay();
  display.display();
}
