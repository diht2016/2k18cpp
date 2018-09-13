#include <string.h>

#include "sortable-text.cpp"

bool beginningComparator(char* a, char* b) {
  int pos = 0;
  while (true) {
    if (a[pos] == b[pos]) {
      if (a[pos] == '\0') {
        // equal strings
        return false;
      }
      pos++;
      continue;
    }
    
    return (a[pos] < b[pos]);
  }
}

bool endComparator(char* a, char* b) {
  int apos = strlen(a) - 1;
  int bpos = strlen(b) - 1;
  
  while (apos >= 0 && bpos >= 0) {
    // skip spaces and punctuation
    if ((unsigned char) a[apos] < '\x40') {
      apos--;
      continue;
    }
    if ((unsigned char) b[bpos] < '\x40') {
      bpos--;
      continue;
    }
    
    if (a[apos] == b[bpos]) {
      apos--;
      bpos--;
      continue;
    }
    
    // a case when both characters are second parts of two-byte UTF-8 characters
    if ((a[apos] & 0x80) && !(a[apos] & 0x40) &&
        (b[bpos] & 0x80) && !(b[bpos] & 0x40)) {
      if (apos > 0 && bpos > 0) {
        // comparing first parts first
        if (a[apos - 1] != b[bpos - 1]) {
          return (a[apos - 1] < b[bpos - 1]);
        }
      }
    }
    
    return (a[apos] < b[bpos]);
  }
  
  return (apos >= 0);
}

int main() {
  SortableText text("original-lines.txt");
  
  text.sort(beginningComparator);
  text.save("beginning-sorted-lines.txt");
  
  text.sort(endComparator);
  text.save("end-sorted-lines.txt");
  
}