#pragma once

extern GLuint loadDDS(const char* image_path, int* w = nullptr, int* h = nullptr);
extern GLuint loadImageToTexture(const char* image_path);