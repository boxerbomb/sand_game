#pragma once
#include "raylib.h"

class Chunk {
public: 
    Chunk(int wx, int wy);
    void SetPixel(int x, int y, int value);
    int GetPixel(int x, int y);
    //GetTexture();
    void Update();
    int worldX;
    int worldY;
    Color chunk_color;

    void UpdateTexture();
    RenderTexture2D tex;

    bool needs_update = false;
    int* chunk_data;
    Chunk* up = nullptr;
    Chunk* down = nullptr;
    Chunk* left = nullptr;
    Chunk* right = nullptr;
};