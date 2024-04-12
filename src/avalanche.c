#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<cglm/cglm.h>

float lerp(float x, float x0, float x1, float y0, float y1) return (y0*(x1-x)+y1*(x-x0))/(x1-x0);

int main(int argc, char **argv) {
    if(SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window *window = SDL_CreateWindow("Avalanche", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_SHOWN);
        if(window) {
            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer) {
                mat3 view, inv;
                glm_mat3_identity(view);

                vec3 v1, v2;
                int mouseX, mouseY;
                SDL_Event event;

                int nvertices = 1;
                float *vertices = malloc(3*sizeof(float));
                vertices[nvertices*3-1] = 1.f;
                float *newVertices;

                float angle, angleMn = 0.f, angleMx = 2.f*M_PI;
                float xMn, xMx, yMn, yMx;
                float randomX, randomY;
                float x0, x1, y0, y1;
                float distanceOver, distanceUnder;
                int intersections;

                int lockedMode = SDL_FALSE;
                int running = SDL_TRUE;
                while(running) {
                    if(running == 1) {
                        SDL_GetMouseState(&mouseX, &mouseY);
                        vertices[nvertices*3-3] = mouseX;
                        vertices[nvertices*3-2] = mouseY;

                        glm_mat3_inv(view, inv);
                        glm_mat3_mulv(inv, vertices+(nvertices-1)*3, vertices+(nvertices-1)*3);

                        if(running == 1 && lockedMode) {
                            vertices[nvertices*3-3] = SDL_roundf(vertices[nvertices*3-3]/64.f)*64.f;
                            vertices[nvertices*3-2] = SDL_roundf(vertices[nvertices*3-2]/64.f)*64.f;
                        }
                    }

                    glm_mat3_mulv(view, vertices, v2);
                    if(running >= 2) {
                        if(SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE)) {
                            fprintf(stderr, "ERROR: SDL_SetRenderDrawColor() failed.\n");
                            running = SDL_FALSE;
                        }
                        if(SDL_RenderDrawLine(renderer, SDL_cosf(angle)*SDL_sqrtf(959.f*959.f+539.f*539.f)+v2[0], -SDL_sinf(angle)*SDL_sqrtf(959.f*959.f+539.f*539.f)+v2[1], -SDL_cosf(angle)*SDL_sqrtf(959.f*959.f+539.f*539.f)+v2[0], SDL_sinf(angle)*SDL_sqrtf(959.f*959.f+539.f*539.f)+v2[1])) {
                            fprintf(stderr, "ERROR: SDL_RenderDrawLine() failed.\n");
                            running = SDL_FALSE;
                        }
                    }

                    if(SDL_SetRenderDrawColor(renderer, 255, 127, 0, SDL_ALPHA_OPAQUE)) {
                        fprintf(stderr, "ERROR: SDL_SetRenderDrawColor() failed.\n");
                        running = SDL_FALSE;
                    }
                    for(int i = 1; i < nvertices; i++) {
                        glm_vec3_copy(v2, v1);
                        glm_mat3_mulv(view, vertices+i*3, v2);

                        if(SDL_RenderDrawLine(renderer, v1[0], v1[1], v2[0], v2[1])) {
                            fprintf(stderr, "ERROR: SDL_RenderDrawLine() failed.\n");
                            running = SDL_FALSE;
                        }
                    }

                    SDL_RenderPresent(renderer);

                    SDL_Delay(1000.f/60.f);

                    if(SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE)) {
                        fprintf(stderr, "ERROR: SDL_SetRenderDrawColor() failed.\n");
                        running = SDL_FALSE;
                    }
                    if(SDL_RenderClear(renderer)) {
                        fprintf(stderr, "ERROR: SDL_RenderClear() failed.\n");
                        running = SDL_FALSE;
                    }

                    while(SDL_PollEvent(&event)) {
                        switch(event.type) {
                        case SDL_MOUSEBUTTONDOWN:
                            if(running < 2) {
                                switch(event.button.button) {
                                case SDL_BUTTON_LEFT: //Adds a point
                                    vertices = realloc(vertices, (++nvertices)*3*sizeof(float));
                                    vertices[nvertices*3-1] = 1.f;
                                    break;
                                case SDL_BUTTON_RIGHT: //Toggles locked mode
                                    lockedMode = !lockedMode;
                                    break;
                                default:
                                    break;
                                }
                            }
                            break;
                        case SDL_MOUSEWHEEL: //Zooms on a point
                            glm_translate2d_x(view, vertices[nvertices*3-3]);
                            glm_translate2d_y(view, vertices[nvertices*3-2]);
                            glm_scale2d_uni(view, SDL_powf(2.f, event.wheel.y));
                            glm_translate2d_x(view, -vertices[nvertices*3-3]);
                            glm_translate2d_y(view, -vertices[nvertices*3-2]);
                            break;
                        case SDL_KEYDOWN:
                            switch(event.key.keysym.scancode) {
                            case SDL_SCANCODE_A: //Translates to the left
                                glm_translate2d_x(view, 64.f);
                                break;
                            case SDL_SCANCODE_D: //Translates to the right
                                glm_translate2d_x(view, -64.f);
                                break;
                            case SDL_SCANCODE_W: //Translates to the top
                                glm_translate2d_y(view, 64.f);
                                break;
                            case SDL_SCANCODE_S: //Translates to the bottom
                                glm_translate2d_y(view, -64.f);
                                break;
                            case SDL_SCANCODE_ESCAPE: //Resets the camera
                                glm_mat3_identity(view);
                                break;
                            case SDL_SCANCODE_BACKSPACE: //Deletes the last point
                                if(nvertices > 1 && running < 2) vertices = realloc(vertices, (--nvertices)*3*sizeof(float));
                                break;
                            case SDL_SCANCODE_EQUALS: //Deletes all the points
                                if(nvertices > 1) vertices = realloc(vertices, (nvertices = 1)*3*sizeof(float));
                                if(running >= 2) {
                                    free(newVertices);
                                    newVertices = NULL;
                                    angleMn = 0.f;
                                    angleMx = 2.f*M_PI;
                                    running = 1;
                                }
                                break;
                            case SDL_SCANCODE_RETURN: //Starts the computations
                                if(nvertices > 3) {
                                    if(running == 1) {
                                        newVertices = malloc(nvertices*3*sizeof(float));

                                        printf("L'avalanche originale est delimitee par les points suivants:\n");
                                        for(int i = 0; i < nvertices; i++) printf("\t(%f, %f)\n", vertices[i*3], vertices[i*3+1]);
                                    }

                                    printf("\nIteration %d: angle = approx. %.8f\n", running, angle = (angleMn+angleMx)/2.f);

                                    glm_rotate2d_make(inv, angle);
                                    glm_translate2d_x(inv, -vertices[0]);
                                    glm_translate2d_y(inv, -vertices[1]);

                                    glm_vec3_copy(vertices, vertices+(nvertices-1)*3);
                                    for(int i = 0; i < nvertices; i++) glm_mat3_mulv(inv, vertices+i*3, newVertices+i*3);

                                    printf("L'avalanche est delimitee par les points suivants apres changement de repere:\n");
                                    for(int i = 0; i < nvertices; i++) printf("\t(%f, %f)\n", newVertices[i*3], newVertices[i*3+1]);

                                    xMn = newVertices[0], xMx = newVertices[0], yMn = newVertices[1], yMx = newVertices[1];
                                    for(int i = 1; i < nvertices-1; i++) {
                                        xMn = fmin(xMn, newVertices[i*3]);
                                        xMx = fmax(xMx, newVertices[i*3]);
                                        yMn = fmin(yMn, newVertices[i*3+1]);
                                        yMx = fmax(yMx, newVertices[i*3+1]);
                                    }

                                    distanceOver = 0.f;
                                    distanceUnder = 0.f;

                                    for(int i = 0; i < 1000000; i++) {
                                        randomX = ((float)rand()/(float)(RAND_MAX))*(xMx-xMn)+xMn;
                                        randomY = ((float)rand()/(float)(RAND_MAX))*(yMx-yMn)+yMn;

                                        intersections = 0;

                                        for(int j = 0; j < nvertices-1; j++) {
                                            x0 = newVertices[j*3];
                                            y0 = newVertices[j*3+1];
                                            x1 = newVertices[j*3+3];
                                            y1 = newVertices[j*3+4];

                                            if(((x0<randomX && randomX<x1) || (x1<randomX && randomX<x0)) && lerp(randomX, x0, x1, y0, y1) > randomY) intersections++;
                                        }

                                        if(intersections%2) {
                                            if(randomY > 0) distanceOver += SDL_fabsf(randomX)+SDL_fabsf(randomY);
                                            else if(randomY < 0) distanceUnder += SDL_fabsf(randomX)+SDL_fabsf(randomY);
                                        }
                                    }
                                    printf("distanceOver: %f, distanceUnder: %f\n", distanceOver, distanceUnder);

                                    if(distanceOver>distanceUnder) angleMx = angle;
                                    else if(distanceOver<distanceUnder) angleMn = angle;

                                    running++;
                                }
                                break;
                            default:
                                break;
                            }
                            break;
                        case SDL_QUIT: //Exits
                            printf("\nFinal angle = approx. %f\n", (angleMn+angleMx)/2.f);
                            running = SDL_FALSE;
                            break;
                        default:
                            break;
                        }
                    }
                }

                free(newVertices);
                free(vertices);

                SDL_DestroyRenderer(renderer);
            }
            else fprintf(stderr, "ERROR: SDL_CreateRenderer() failed.\n");

            SDL_DestroyWindow(window);
        }
        else fprintf(stderr, "ERROR: SDL_CreateWindow() failed.\n");
    }
    else fprintf(stderr, "ERROR: SDL_Init() failed.\n");

    SDL_Quit();

    return EXIT_SUCCESS;
}
