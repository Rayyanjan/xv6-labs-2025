#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Check if character is valid for secret (digit or letter)
int is_valid(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Common words/strings to filter out
int is_common_word(char *str, int len) {
    char *common[] = {"help", "null", "This", "Usage", "secret", "fork", "exec", 
                      "open", "pipe", "file", "many", "Bdaa", "failed", "missing",
                      "runcmd", "redirection", "syntax", "0123456789ABCDEF"};
    
    for (int i = 0; i < 18; i++) {
        int match = 1;
        int j = 0;
        while (common[i][j] != '\0') {
            if (j >= len || str[j] != common[i][j]) {
                match = 0;
                break;
            }
            j++;
        }
        if (match && common[i][j] == '\0' && j == len) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int alloc_size = 10000;
    int found_count = 0;
    
    for (int attempt = 0; attempt < 8; attempt++) {
        char *mem = sbrk(alloc_size);
        if (mem == (char*)-1) {
            continue;
        }
        
        int i = 0;
        while (i < alloc_size && found_count < 5) {
            // Skip invalid chars
            while (i < alloc_size && !is_valid(mem[i])) {
                i++;
            }
            
            if (i < alloc_size) {
                int start = i;
                
                // Count consecutive valid characters
                while (i < alloc_size && is_valid(mem[i])) {
                    i++;
                }
                
                int len = i - start;
                
                // Filter: length 4-30, not a common word, has variety
                if (len >= 4 && len <= 30 && !is_common_word(&mem[start], len)) {
                    // Check for variety (not all same char)
                    int has_variety = 0;
                    for (int j = 1; j < len; j++) {
                        if (mem[start + j] != mem[start]) {
                            has_variety = 1;
                            break;
                        }
                    }
                    
                    if (has_variety) {
                        for (int j = start; j < start + len; j++) {
                            printf("%c", mem[j]);
                        }
                        printf("\n");
                        found_count++;
                    }
                }
            }
        }
        
        if (found_count >= 5) break;
    }
    
    exit(0);
}
