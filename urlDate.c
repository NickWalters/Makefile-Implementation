
#include "bake.h"




static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data)
{
    (void)ptr;
    (void)data;
    /* we are not interested in the headers itself,
     so we only return the size we would have saved ... */
    return (size_t)(size * nmemb);
}



time_t urlDate(char * URL)
{
    CURL *curl;
    CURLcode res;
    long filetime = -1;
    const char *filename = strrchr(URL, '/') + 1;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        /* No download if the file */
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        /* Ask for filetime */
        curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
        
        res = curl_easy_perform(curl);
        
        if(CURLE_OK == res) {
            res = curl_easy_getinfo(curl, CURLINFO_FILETIME, &filetime);
            if((CURLE_OK == res) && (filetime >= 0)) {
                time_t file_time = (time_t)filetime;
                printf("filetime %s: %s\n", filename, ctime(&file_time));
                struct tm * modDate = localtime(&file_time);
                time_t changeDate = mktime(modDate);
                return changeDate;
            }
        }
        else {
            /* we failed */
            fprintf(stderr, "Cannot open URL. curl told us %d\n", res);
        }
        
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return -1;
}

/*
int main(int argc, char *argv[]){
    urlDate("http://teaching.csse.uwa.edu.au/units/CITS2002/workshops/mycal.c");
}
 */
