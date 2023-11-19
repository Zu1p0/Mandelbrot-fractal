#include <SDL.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 800;
const int MAX_ITERATIONS = 100;

SDL_Renderer* renderer;
SDL_Window* window;

// Define the region of the complex plane to display
double minX = -2.0;
double maxX = 2.0;
double minY = -2.0;
double maxY = 2.0;

void drawMandelbrot() {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            // Map pixel coordinates to the complex plane
            double zx = minX + (maxX - minX) * x / WIDTH;
            double zy = minY + (maxY - minY) * y / HEIGHT;

            double cx = zx;
            double cy = zy;

            int iteration = 0;

            while (iteration < MAX_ITERATIONS) {
                double xtemp = zx * zx - zy * zy + cx;
                zy = 2.0 * zx * zy + cy;
                zx = xtemp;

                // Check if the point is outside the Mandelbrot set
                if (zx * zx + zy * zy > 4.0) {
                    break;
                }

                iteration++;
            }

            // Map the number of iterations to a color
            Uint8 r = static_cast<Uint8>(iteration % 256);
            Uint8 g = static_cast<Uint8>(iteration * 2 % 256);
            Uint8 b = static_cast<Uint8>(iteration * 5 % 256);

            // Set the pixel color
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

// Function to handle zooming
void zoom(double factor, double x, double y ) {
    // Map pixel coordinates to the complex plane
    double centerX = minX + (maxX - minX) * x / WIDTH;
    double centerY = minY + (maxY - minY) * y / HEIGHT;

    //double centerX = 0.5 * (minX + maxX);
    //double centerY = 0.5 * (minY + maxY);
	std::cout << centerX << " " << centerY << std::endl;

    double newWidth = (maxX - minX) * factor;
    double newHeight = (maxY - minY) * factor;

    minX = centerX - 0.5 * newWidth;
    maxX = centerX + 0.5 * newWidth;
    minY = centerY - 0.5 * newHeight;
    maxY = centerY + 0.5 * newHeight;
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Mandelbrot Fractal");

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                // Zoom in when left mouse button is clicked
                zoom(0.5, event.button.x, event.button.y);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
                // Zoom out when right mouse button is clicked
                zoom(2.0, event.button.x, event.button.y);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawMandelbrot();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
