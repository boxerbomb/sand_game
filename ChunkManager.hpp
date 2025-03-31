#include <unordered_map>
#include <vector>
#include <bits/stdc++.h>
#include "chunk.hpp"
#include "constants.hpp"


// This code is taken from here.
// This is a custom hash function to be used by an unordered map
// It allows for std::pairs to be used as keys
//https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};


class ChunkManager{
    public:
        ChunkManager();
        std::vector<Chunk*> chunks_in_view;
        void setActiveChunk(int x, int y);
        Chunk* CreateNewChunk(int wx, int wy, Chunk* known_up, Chunk* known_down, Chunk* known_left, Chunk* known_right);
        Chunk* search_for_incomplete_chunk(int wx, int wy);
        void add_incomplete_chunk(Chunk* chunk_to_add);
        void evaluate_completeness(Chunk* my_chunk);

        std::unordered_map<std::pair<int, int> , Chunk*, pair_hash> incomplete_chunks_hashmap;

        void SetPixel(int x, int y, int value);
        int GetPixel(int x, int y);

        Chunk* active_chunk = nullptr;
};