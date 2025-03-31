#include "ChunkManager.hpp"


ChunkManager::ChunkManager(){
    active_chunk = CreateNewChunk(0,0,nullptr,nullptr,nullptr,nullptr);
}

void ChunkManager::SetPixel(int x, int y, int value){
    //printf("ChunkManager::SetPixel(%d, %d, %d)\n",x,y,value);
    setActiveChunk(x,y);
    active_chunk->SetPixel(x, y, value);
}

int ChunkManager::GetPixel(int x, int y){
    //printf("ChunkManager::GetPixel(%d, %d)\n",x,y);
    setActiveChunk(x,y);
    return active_chunk->GetPixel(x,y);
}

Chunk* ChunkManager::search_for_incomplete_chunk(int wx, int wy){
    std::pair<int, int> key = {wx, wy};

    Chunk* return_chunk;

    
    try {
        return_chunk = incomplete_chunks_hashmap.at(key);
    } catch (const std::out_of_range& e) {
        return nullptr;
    }

    return return_chunk;

}

void ChunkManager::add_incomplete_chunk(Chunk* chunk_to_add){
    std::pair<int, int> key = {chunk_to_add->worldX, chunk_to_add->worldY};

    incomplete_chunks_hashmap[key] = chunk_to_add;

}

void ChunkManager::evaluate_completeness(Chunk* my_chunk){

    if (((my_chunk->up == nullptr) | (my_chunk->down == nullptr) | (my_chunk->left == nullptr) | (my_chunk->right == nullptr)) == false){
        std::pair<int, int> key = {my_chunk->worldX, my_chunk->worldY};
        incomplete_chunks_hashmap.erase(key);
        my_chunk->chunk_color.r = 0;
        my_chunk->chunk_color.g = 128;
        my_chunk->chunk_color.b = 0;
        my_chunk->UpdateTexture();
    }

    
}

Chunk* ChunkManager::CreateNewChunk(int wx, int wy, Chunk* known_up, Chunk* known_down, Chunk* known_left, Chunk* known_right){
    Chunk* new_chunk = new Chunk(wx, wy);
    
    if(known_up == nullptr){
        Chunk* found_chunk = search_for_incomplete_chunk(wx, wy-1);
        new_chunk->up = found_chunk;

        if(found_chunk != nullptr){
            found_chunk->down = new_chunk;
            evaluate_completeness(found_chunk);
        }

    }else{
        new_chunk->up = known_up;
    }

    if(known_down == nullptr){
        Chunk* found_chunk = search_for_incomplete_chunk(wx, wy+1);
        new_chunk->down = found_chunk;

        if(found_chunk != nullptr){
            found_chunk->up = new_chunk;
            evaluate_completeness(found_chunk);
        }

    }else{
        new_chunk->down = known_down;
    }

    if(known_left == nullptr){
        Chunk* found_chunk = search_for_incomplete_chunk(wx-1, wy);
        new_chunk->left = found_chunk;

        if(found_chunk != nullptr){
            found_chunk->right = new_chunk;
            evaluate_completeness(found_chunk);
        }

    }else{
        new_chunk->left = known_left;
    }

    if(known_right == nullptr){
        Chunk* found_chunk = search_for_incomplete_chunk(wx+1, wy);
        new_chunk->right = found_chunk;

        if(found_chunk != nullptr){
            found_chunk->left = new_chunk;
            evaluate_completeness(found_chunk);
        }


    }else{
        new_chunk->right = known_right;
    }

    if ((new_chunk->up == nullptr) | (new_chunk->down == nullptr) | (new_chunk->left == nullptr) | (new_chunk->right == nullptr)) {
        add_incomplete_chunk(new_chunk);
    }else{
        new_chunk->chunk_color.r = 0;
        new_chunk->chunk_color.g = 128;
        new_chunk->chunk_color.b = 0;
        new_chunk->UpdateTexture();
    }

    return new_chunk;
    // Search for neighbors here, and connect   
}

void ChunkManager::setActiveChunk(int x, int y){
    int chunk_start_x = active_chunk->worldX * CHUNK_SIZE;
    int chunk_end_x = (active_chunk->worldX+1) * CHUNK_SIZE - 1;
    int chunk_start_y = active_chunk->worldY * CHUNK_SIZE;
    int chunk_end_y = (active_chunk->worldY+1) * CHUNK_SIZE - 1;

    // printf("Active Chunk X Limits %d->%d\n",chunk_start_x, chunk_end_x);
    // printf("Active Chunk Y Limits %d->%d\n",chunk_start_y, chunk_end_y);

    if(x < chunk_start_x){
        if(active_chunk->left == nullptr){
            //active_chunk->left = new Chunk(active_chunk->worldX-1, active_chunk->worldY);
            active_chunk->left = CreateNewChunk(active_chunk->worldX-1, active_chunk->worldY, nullptr, nullptr, nullptr, active_chunk);
            evaluate_completeness(active_chunk);
            chunks_in_view.push_back(active_chunk->left);
        }
        active_chunk = active_chunk->left;
        setActiveChunk(x,y);
    }

    if(x > chunk_end_x){
        if(active_chunk->right == nullptr){
            //active_chunk->right = new Chunk(active_chunk->worldX+1, active_chunk->worldY);
            active_chunk->right = CreateNewChunk(active_chunk->worldX+1, active_chunk->worldY, nullptr, nullptr, active_chunk, nullptr);
            evaluate_completeness(active_chunk);
            chunks_in_view.push_back(active_chunk->right);
        }
        active_chunk = active_chunk->right;
        setActiveChunk(x,y);
    }

    if(y < chunk_start_y){
        if(active_chunk->up == nullptr){
            //active_chunk->up = new Chunk(active_chunk->worldX, active_chunk->worldY-1);
            active_chunk->up = CreateNewChunk(active_chunk->worldX, active_chunk->worldY-1, nullptr, active_chunk, nullptr, nullptr);
            evaluate_completeness(active_chunk);
            chunks_in_view.push_back(active_chunk->up);
        }
        active_chunk = active_chunk->up;
        setActiveChunk(x,y);
    }

    if(y > chunk_end_y){
        if(active_chunk->down == nullptr){
            //active_chunk->down = new Chunk(active_chunk->worldX, active_chunk->worldY+1);
            active_chunk->down = CreateNewChunk(active_chunk->worldX, active_chunk->worldY+1, active_chunk, nullptr, nullptr, nullptr);
            evaluate_completeness(active_chunk);
            chunks_in_view.push_back(active_chunk->down);
        }
        active_chunk = active_chunk->down;
        setActiveChunk(x,y);
    }

}