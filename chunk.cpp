#include "chunk.hpp"
#include "constants.hpp"
#include <iostream>

Chunk::Chunk(int wx, int wy){

	worldX = wx;
	worldY = wy;

	chunk_data = new int[CHUNK_SIZE*CHUNK_SIZE];
	for(int y=0; y<CHUNK_SIZE; y++){
		for(int x=0; x<CHUNK_SIZE; x++){
			int data_index = (y*CHUNK_SIZE) + x;
			chunk_data[data_index] = 0;
		}
	}

	unsigned char r = GetRandomValue(100, 255);
	unsigned char g = GetRandomValue(0, 255);
	unsigned char b = GetRandomValue(0, 255);

	chunk_color = Color();
	chunk_color.r = r;
	chunk_color.g = r;
	chunk_color.b = r;
	chunk_color.a = 255;

	//printf("Creating Chunk with color %d,%d,%d \n", r,g,b);
	int chunk_start_x = worldX * CHUNK_SIZE;
	int chunk_end_x = (worldX+1) * CHUNK_SIZE - 1;
	int chunk_start_y = worldY * CHUNK_SIZE;
	int chunk_end_y = (worldY+1) * CHUNK_SIZE - 1;
	//printf("Active Chunk X[%d->%d] Y[%d->%d]\n",chunk_start_x, chunk_end_x, chunk_start_y, chunk_end_y);

	tex = LoadRenderTexture(CHUNK_SIZE, CHUNK_SIZE);
	UpdateTexture();
}

void Chunk::UpdateTexture(){
	BeginTextureMode(tex);
	ClearBackground(RAYWHITE);

	for(int y=0; y<CHUNK_SIZE; y++){
		for(int x=0; x<CHUNK_SIZE; x++){
			int data_index = (y*CHUNK_SIZE) + x;
			if(chunk_data[data_index] == 1){
				DrawPixel(x, CHUNK_SIZE-y-1, BLACK); 
			}
		}
	}
	
	EndTextureMode();
}

void Chunk::SetPixel(int x, int y, int value){

	int inside_chunk_x = x - (worldX * CHUNK_SIZE);
	int inside_chunk_y = y - (worldY * CHUNK_SIZE);

	//printf("Inside Chunk: %d, %d\n",inside_chunk_x, inside_chunk_y);

	int data_index = (inside_chunk_y*CHUNK_SIZE) + inside_chunk_x;
    chunk_data[data_index] = value;

    needs_update = true;

    return;
}

int Chunk::GetPixel(int x, int y){
	int data_index = (y*CHUNK_SIZE) + x;
    return chunk_data[data_index];
}