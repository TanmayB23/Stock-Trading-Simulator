#include "stock.h"
#include <curl/curl.h>
#include "nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;
using namespace std;

// Callback function for handling data from API response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* s) {
    s->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Stock::Stock(const string &symb) : symbol(symb), price(0.0) {}
void Stock::updatePrice() {
    string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + ".BSE&apikey=W031PUCHEDHD0V10";  // Replace with your API key
    CURL* curl = curl_easy_init();
    if (curl) {
        string response_string;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            json response = json::parse(response_string);
            try {
                // Fetch latest stock price from the JSON response
                auto timeSeries = response["Time Series (Daily)"];
                auto latest = timeSeries.begin();
                price = stod(latest.value()["4. close"].get<string>());
                cout << "Stock price updated: " << price << "\n";
            } catch (json::exception& e) {
                cerr << "Error parsing JSON response: " << e.what() << "\n";
            }
        } else {
            cerr << "Error fetching stock data\n";
        }
    }
}