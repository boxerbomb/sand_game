#include "raylib.h"

#include <iostream>
#include <cmath>


#include "chunk.hpp"
#include "ChunkManager.hpp"
#include "constants.hpp"



const int screenWidth = 400;
const int screenHeight = 400;
const int screenCenterX = screenWidth/2;
const int screenCenterY = screenHeight/2;





class GameWindow {
public:
    GameWindow(){
        world_center.x = 0;
        world_center.y = 0;
    }
    Vector2 window_pos_to_world_pos(Vector2 mouse_pos){
        Vector2 world_pos;
        //world_pos.x = (int)(world_center.x - ( screenWidth/2 ) / pixel_scaler + mouse_pos.x );
        //world_pos.y = (int)(world_center.y - ( screenHeight/2 ) / pixel_scaler + mouse_pos.y);
        world_pos.x = (int)(world_center.x - ( (screenWidth/pixel_scaler)/2 ) + (mouse_pos.x/pixel_scaler) ) + (CHUNK_SIZE/2);
        world_pos.y = (int)(world_center.y - ( (screenHeight/pixel_scaler)/2 ) + (mouse_pos.y/pixel_scaler) ) + (CHUNK_SIZE/2);
        return world_pos;
    }

    ChunkManager chunk_man;
    int pixel_scaler = 2;
private:
    Vector2 world_center;
    std::vector<int> chunk_ids;
};


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    GameWindow game_window;
    Vector2 window_mouse_pos;
    game_window.chunk_man.chunks_in_view.push_back(game_window.chunk_man.active_chunk);

    //SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        window_mouse_pos = GetMousePosition();
        Vector2 world_pos = game_window.window_pos_to_world_pos(window_mouse_pos);

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            for (const auto& pair : game_window.chunk_man.incomplete_chunks_hashmap) {
              std::cout << "Key: " << pair.first.first <<","<<pair.first.second<< std::endl;
            }
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){


              int r = 5;
              double PIi = 3.1415926535;
              double i, angle, x1, y1;

              for(i = 0; i < 360; i += 0.1)
              {
                    angle = i;
                    x1 = r * cos(angle * PIi / 180);
                    y1 = r * sin(angle * PIi / 180);
                    game_window.chunk_man.SetPixel(world_pos.x + x1,world_pos.y + y1, 1);
                    //putpixel(x + x1, y + y1, color);
              }



            //printf("World Pos: %f, %f\n", world_pos.x, world_pos.y);
            //printf("Chunk ID:  %d, %d\n", (int)std::floor(float(world_pos.x)/CHUNK_SIZE), (int)std::floor(float(world_pos.y)/CHUNK_SIZE));
            //game_window.chunk_man.SetPixel(world_pos.x,world_pos.y, 1);
            //game_window.chunk_man.GetPixel(world_pos.x,world_pos.y);
        }


        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(SKYBLUE);
             

            for (auto it = begin (game_window.chunk_man.chunks_in_view); it != end (game_window.chunk_man.chunks_in_view); ++it) {
                int chunk_draw_size = CHUNK_SIZE * game_window.pixel_scaler;
                int drawX = screenCenterX + ( (*it)->worldX * chunk_draw_size) - (chunk_draw_size/2);
                int drawY = screenCenterY + ( (*it)->worldY * chunk_draw_size) - (chunk_draw_size/2);


                if((*it)->needs_update){
                    (*it)->UpdateTexture();
                    (*it)->needs_update = false;
                }
                DrawTextureEx((*it)->tex.texture, Vector2{ (float)drawX, (float)drawY}, 0.0, game_window.pixel_scaler , WHITE);
                //DrawTexture( , drawX, drawY, WHITE);
            }

        DrawFPS(0, 0);     
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
