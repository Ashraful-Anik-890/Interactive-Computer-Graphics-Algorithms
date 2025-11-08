#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Window dimensions
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 800

// Colors (RGBA)
typedef struct {
    Uint8 r, g, b, a;
} Color;

Color COLOR_BACKGROUND = {26, 32, 44, 255};
Color COLOR_AXIS = {74, 85, 104, 255};
Color COLOR_GREEN = {0, 255, 0, 255};
Color COLOR_YELLOW = {255, 255, 0, 255};
Color COLOR_BLUE = {59, 130, 246, 255};
Color COLOR_RED = {239, 68, 68, 255};
Color COLOR_WHITE = {255, 255, 255, 255};
Color COLOR_GRAY = {156, 163, 175, 255};

// Point structure
typedef struct {
    int x;
    int y;
} Point;

// Global variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int originX, originY;

// Function prototypes
bool init();
void close_sdl();
void clearCanvas();
void plot(int x, int y, Color color);
void drawLineBresenham(int x0, int y0, int x1, int y1, Color color);
void drawCircleMidpoint(int xc, int yc, int r, Color color);
void plotSymmetric(int xc, int yc, int x, int y, Color color);
Point translate(Point p, int tx, int ty);
Point rotate_point(Point p, double angle);
Point reflectY(Point p);
void drawTriangle(Point p1, Point p2, Point p3, Color color);
void drawText(const char* text, int x, int y);
void drawUI();
void handleInput();

// Initialize SDL
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Interactive Computer Graphics Algorithms",
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             WINDOW_WIDTH, WINDOW_HEIGHT,
                             SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    originX = CANVAS_WIDTH / 2;
    originY = CANVAS_HEIGHT / 2;

    return true;
}

// Close SDL
void close_sdl() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Clear canvas and draw axes
void clearCanvas() {
    // Draw background
    SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND.r, COLOR_BACKGROUND.g, 
                          COLOR_BACKGROUND.b, COLOR_BACKGROUND.a);
    SDL_Rect canvas_rect = {0, 0, CANVAS_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &canvas_rect);

    // Draw axes
    SDL_SetRenderDrawColor(renderer, COLOR_AXIS.r, COLOR_AXIS.g, 
                          COLOR_AXIS.b, COLOR_AXIS.a);
    
    // X-axis
    SDL_RenderDrawLine(renderer, 0, originY, CANVAS_WIDTH, originY);
    
    // Y-axis
    SDL_RenderDrawLine(renderer, originX, 0, originX, WINDOW_HEIGHT);
}

// Plot a pixel (2x2 for visibility)
void plot(int x, int y, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // Convert coordinates (flip Y axis for mathematical coordinates)
    int screenX = originX + x;
    int screenY = originY - y;
    
    // Draw 2x2 pixel for visibility
    SDL_Rect pixel = {screenX - 1, screenY - 1, 2, 2};
    SDL_RenderFillRect(renderer, &pixel);
}

// Bresenham's Line Algorithm
void drawLineBresenham(int x0, int y0, int x1, int y1, Color color) {
    int dx = abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;
    
    while (true) {
        plot(x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// Plot 8-way symmetric points for circle
void plotSymmetric(int xc, int yc, int x, int y, Color color) {
    plot(xc + x, yc + y, color);
    plot(xc - x, yc + y, color);
    plot(xc + x, yc - y, color);
    plot(xc - x, yc - y, color);
    plot(xc + y, yc + x, color);
    plot(xc - y, yc + x, color);
    plot(xc + y, yc - x, color);
    plot(xc - y, yc - x, color);
}

// Midpoint Circle Algorithm
void drawCircleMidpoint(int xc, int yc, int r, Color color) {
    int x = 0;
    int y = r;
    int p = 1 - r;

    plotSymmetric(xc, yc, x, y, color);

    while (x < y) {
        x++;
        if (p < 0) {
            p = p + 2 * x + 1;
        } else {
            y--;
            p = p + 2 * x + 1 - 2 * y;
        }
        plotSymmetric(xc, yc, x, y, color);
    }
}

// Translation
Point translate(Point p, int tx, int ty) {
    Point result = {p.x + tx, p.y + ty};
    return result;
}

// Rotation (angle in degrees)
Point rotate_point(Point p, double angle) {
    double rad = angle * M_PI / 180.0;
    double s = sin(rad);
    double c = cos(rad);
    
    int x_new = (int)round(p.x * c - p.y * s);
    int y_new = (int)round(p.x * s + p.y * c);
    
    Point result = {x_new, y_new};
    return result;
}

// Reflection about Y-axis
Point reflectY(Point p) {
    Point result = {-p.x, p.y};
    return result;
}

// Draw a triangle
void drawTriangle(Point p1, Point p2, Point p3, Color color) {
    drawLineBresenham(p1.x, p1.y, p2.x, p2.y, color);
    drawLineBresenham(p2.x, p2.y, p3.x, p3.y, color);
    drawLineBresenham(p3.x, p3.y, p1.x, p1.y, color);
}

// Draw simple UI panel
void drawUI() {
    // Draw UI background
    SDL_SetRenderDrawColor(renderer, 31, 41, 55, 255);
    SDL_Rect ui_rect = {CANVAS_WIDTH, 0, WINDOW_WIDTH - CANVAS_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &ui_rect);
    
    // Draw border
    SDL_SetRenderDrawColor(renderer, COLOR_GRAY.r, COLOR_GRAY.g, COLOR_GRAY.b, COLOR_GRAY.a);
    SDL_RenderDrawLine(renderer, CANVAS_WIDTH, 0, CANVAS_WIDTH, WINDOW_HEIGHT);
}

// Main function
int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    // Default values for demonstrations
    int line_x0 = 10, line_y0 = 12, line_x1 = 26, line_y1 = 22;
    int circle_xc = -3, circle_yc = -3, circle_r = 8;
    int trans_ax = 0, trans_ay = 0;
    int trans_bx = 1, trans_by = 1;
    int trans_cx = 5, trans_cy = 2;

    // Initial display
    clearCanvas();
    drawUI();
    SDL_RenderPresent(renderer);

    printf("\n=== Interactive Computer Graphics Algorithms ===\n");
    printf("Commands:\n");
    printf("  1 - Draw Bresenham's Line (default: (10,12) to (26,22))\n");
    printf("  2 - Draw Midpoint Circle (default: center (-3,-3), radius 8)\n");
    printf("  3 - Apply 2D Transformations to Triangle\n");
    printf("  C - Clear Canvas\n");
    printf("  Q - Quit\n");
    printf("\nPress a key to start...\n\n");

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        quit = true;
                        break;
                    
                    case SDLK_1:
                        printf("Drawing Bresenham's Line from (%d,%d) to (%d,%d)\n", 
                               line_x0, line_y0, line_x1, line_y1);
                        drawLineBresenham(line_x0, line_y0, line_x1, line_y1, COLOR_GREEN);
                        SDL_RenderPresent(renderer);
                        break;
                    
                    case SDLK_2:
                        printf("Drawing Midpoint Circle: center (%d,%d), radius %d\n", 
                               circle_xc, circle_yc, circle_r);
                        drawCircleMidpoint(circle_xc, circle_yc, circle_r, COLOR_YELLOW);
                        SDL_RenderPresent(renderer);
                        break;
                    
                    case SDLK_3: {
                        printf("Applying 2D Transformations:\n");
                        Point A = {trans_ax, trans_ay};
                        Point B = {trans_bx, trans_by};
                        Point C = {trans_cx, trans_cy};
                        
                        // Draw initial triangle (blue)
                        printf("  Initial triangle: A(%d,%d) B(%d,%d) C(%d,%d)\n", 
                               A.x, A.y, B.x, B.y, C.x, C.y);
                        drawTriangle(A, B, C, COLOR_BLUE);
                        
                        // (i) Translate by (5, 1)
                        A = translate(A, 5, 1);
                        B = translate(B, 5, 1);
                        C = translate(C, 5, 1);
                        printf("  After translation (5,1): A(%d,%d) B(%d,%d) C(%d,%d)\n", 
                               A.x, A.y, B.x, B.y, C.x, C.y);
                        
                        // (ii) Rotate 90 degrees
                        A = rotate_point(A, 90.0);
                        B = rotate_point(B, 90.0);
                        C = rotate_point(C, 90.0);
                        printf("  After 90° rotation: A(%d,%d) B(%d,%d) C(%d,%d)\n", 
                               A.x, A.y, B.x, B.y, C.x, C.y);
                        
                        // (iii) Reflect about y-axis
                        A = reflectY(A);
                        B = reflectY(B);
                        C = reflectY(C);
                        printf("  After Y-axis reflection: A(%d,%d) B(%d,%d) C(%d,%d)\n", 
                               A.x, A.y, B.x, B.y, C.x, C.y);
                        
                        // Draw final transformed triangle (red)
                        drawTriangle(A, B, C, COLOR_RED);
                        SDL_RenderPresent(renderer);
                        break;
                    }
                    
                    case SDLK_c:
                        printf("Clearing canvas...\n");
                        clearCanvas();
                        drawUI();
                        SDL_RenderPresent(renderer);
                        break;
                }
            }
        }
        
        SDL_Delay(16); // ~60 FPS
    }

    close_sdl();
    return 0;
}
