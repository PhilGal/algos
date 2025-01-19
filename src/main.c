#include <stdio.h>
#include <curl/curl.h>

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *userdata) {
  fwrite(ptr, size, nmemb, stdout); // print response to stdout
  return size * nmemb;
}

int main() {
  CURL *curl  = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, "https://duckduckgo.com");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  CURLcode res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    fprintf(stderr, "failed: %s\n", curl_easy_strerror(res));
  }

  curl_easy_cleanup(curl);

  return 0;
}
