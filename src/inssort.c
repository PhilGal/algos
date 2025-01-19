#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(char *a, char *b) {
  char tmp = *a;
  *a = *b;
  *b = tmp;
}

void insertion_sort(char items[], int n) {
  int j;
  int cnt = 0;
  for (int i = 1; i < n; i++) {
    j = i;
    while (j > 0 && items[j] < items[j - 1]) {
      swap(&items[j], &items[j - 1]);
      j -= 1;
      cnt++;
    }
  }
  printf("iterations: %d\n", cnt);
}

struct MemoryStruct {
  char *memory;
  size_t size;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userdata) {
  size_t realsize = size * nmemb; // print response to stdout
   struct MemoryStruct *mem = (struct MemoryStruct *)userdata;
  // allocate a necessary size buf and set it to our buf.
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

int main(int argc, char *argv[]) {

  struct MemoryStruct chunk = {.memory = malloc(1), .size = 0};

  CURL *curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, "https://duckduckgo.com");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  CURLcode res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    fprintf(stderr, "failed: %s\n", curl_easy_strerror(res));
  }
  printf("%s\n", chunk.memory);

  char arr[chunk.size + 1];
  strcpy(arr, chunk.memory);
  insertion_sort(arr, chunk.size);
  printf("sorted string: %s\n\a", arr);
  curl_easy_cleanup(curl);
  free(chunk.memory);
  curl_global_cleanup();
}
