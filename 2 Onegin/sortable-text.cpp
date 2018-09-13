#include <algorithm>
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class SortableText {
 public:
  SortableText(char* file_name) {
    buildFromFile(file_name);
  }
  
  void sort(bool(*comparator)(char*, char*)) {
    std::sort(lines.begin(), lines.end(), comparator);
  }
  
  void save(char* file_name) {
    // "wb" instead of "w" to process writing '\n' correctly
    FILE *file = fopen(file_name, "wb");
    assert(file != NULL);
    
    char lf = '\n';
    for (size_t i = 0; i < lines.size(); i++) {
      fwrite(lines[i], 1, strlen(lines[i]), file);
      fwrite(&lf, 1, 1, file);
    }
    
    fclose(file);
  }
  
 private:
  void buildFromFile(char* file_name) {
    FILE *file = fopen(file_name, "r");
    assert(file != NULL);
    
    // getting the size of the file
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    size_t buffer_size = length + 1;
    buffer = (char*) calloc(buffer_size, 1);
    fread(buffer, 1, length, file);
    
    lines.push_back(buffer);
    for (size_t i = 0; i < length; i++) {
      if (buffer[i] == '\n') {
        buffer[i] = '\0';
        if (i != length - 1) {
          lines.push_back(&buffer[i + 1]);
        }
      }
    }
    
    fclose(file);
  }
  
 private:
  size_t length;
  char* buffer;
  std::vector<char*> lines;
};