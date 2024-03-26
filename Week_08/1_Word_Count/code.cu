#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <string.h>

#define N 1024 // Maximum length of the sentence
#define MAX_WORD_LENGTH 64 // Assuming the word will not be longer than this

__constant__ char d_word[MAX_WORD_LENGTH];

__device__ bool isWordMatch(const char *sentence, int startIdx) {
    for (int i = 0; d_word[i] != '\0'; ++i) {
        if (sentence[startIdx + i] != d_word[i]) {
            return false;
        }
    }
    // Check if the next character is a space or end of the sentence, ensuring it's a complete word match
    char nextChar = sentence[startIdx + strlen(d_word)];
    return nextChar == ' ' || nextChar == '\0';
}

__global__ void countWordOccurrences(const char *sentence, int sentenceLength, unsigned int *count) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    if (i < sentenceLength) {
        // Check if current position is the start of a word (preceded by a space if not at the beginning) and matches the target word
        if ((i == 0 || sentence[i - 1] == ' ') && isWordMatch(sentence, i)) {
            atomicAdd(count, 1);
        }
    }
}

int main() {
    const char sentence[N] = "this is a test sentence with test as a test word";
    const char word[MAX_WORD_LENGTH] = "test"; // The word to find
    char *d_sentence;
    unsigned int count = 0, *d_count;

    int sentenceLength = strlen(sentence);

    cudaMalloc((void**)&d_sentence, sentenceLength + 1);
    cudaMalloc((void**)&d_count, sizeof(unsigned int));

    cudaMemcpy(d_sentence, sentence, sentenceLength + 1, cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(d_word, word, MAX_WORD_LENGTH); // Copy word to constant memory
    cudaMemcpy(d_count, &count, sizeof(unsigned int), cudaMemcpyHostToDevice);

    int blockSize = 256;
    int numBlocks = (sentenceLength + blockSize - 1) / blockSize;
    countWordOccurrences<<<numBlocks, blockSize>>>(d_sentence, sentenceLength, d_count);

    cudaMemcpy(&count, d_count, sizeof(unsigned int), cudaMemcpyDeviceToHost);

    printf("The word '%s' occurs %u times in the sentence.\n", word, count);

    cudaFree(d_sentence);
    cudaFree(d_count);

    return 0;
}
