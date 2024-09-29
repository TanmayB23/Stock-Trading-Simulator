# Stock Trading Simulator README

## Project Title
Stock Trading Simulator

## Overview
The Stock Trading Simulator is a command-line application that allows users to simulate trading stocks. Users can log in, create accounts, deposit and withdraw money, and buy or sell stocks. The application fetches real-time stock prices using RESTful APIs.

## Technologies Used
- **C++**: The primary programming language for development.
- **cURL**: Used for making HTTP requests to external APIs for retrieving stock prices.
- **nlohmann/json**: A C++ library for parsing JSON data, facilitating the handling of API responses.
- **SQLite**: A lightweight database used for storing user accounts and transaction details.

## Features
- **User Authentication**: Users can log in or create a new account.
- **Stock Trading**: Buy and sell stocks using real-time market prices.
- **Account Management**: Users can deposit and withdraw funds, and check their account balance.

## Setup Instructions
1. **Install Required Libraries**:
   - Install cURL and SQLite on your system.
   - Ensure you have the nlohmann/json library available.

2. **Build the Project**:
   Use the following command to compile the code:
   ```bash
   g++ -o stock_simulation src/main.cpp src/stock.cpp src/user.cpp src/database.cpp -I./include -I"C:\curl\include" -L"C:\curl\lib" -L"C:\sqlite" -lcurl -lsqlite3
   ```

3. **Run the Application**:
   Execute the compiled binary:
   ```bash
   ./stock_simulation
   ```

## Usage
- Upon starting, users are prompted to log in or create an account.
- Users can deposit money, withdraw funds, buy stocks, sell stocks, and check their balance through a simple menu interface.

## API Integration
The project fetches stock prices using the Alpha Vantage API, which provides daily stock price information. Users can input stock symbols to trade stocks.

## Conclusion
This Stock Trading Simulator provides an interactive way to learn about stock trading, utilizing modern programming practices in C++. It offers a hands-on experience in simulating market conditions while managing user accounts and transactions efficiently. 

## Acknowledgments
- [Alpha Vantage API](https://www.alphavantage.co/)
- [nlohmann/json GitHub Repository](https://github.com/nlohmann/json) 
- [SQLite Documentation](https://www.sqlite.org/docs.html) 
