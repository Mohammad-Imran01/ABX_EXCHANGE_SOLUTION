# axb-exchange-solution

A C++ application to request, receive, and parse ticker data, then write the received data to a JSON file.

## Overview

This project is a C++ application that interacts with a stock exchange API to:
1. Request ticker data.
2. Receive and parse the data.
3. Write the parsed data to a JSON file.

The core functionality is encapsulated in the `exchangehandler` class, which handles all tasks. The class is instantiated and used in `main.cpp`.

Additionally, the repository includes a pre-built binary zip file (`bin-ABXExchange.zip`) that contains the application binary and a Node.js server for easy deployment and execution.

## Prerequisites

To build and run this project from source, you need the following installed:

- **Qt 6**: The application uses Qt for certain functionalities.
- **MinGW 6.2+**: Required for compiling the C++ code on Windows.

If you want to run the pre-built binary, no additional setup is required. Simply download and extract the zip file.

## Pre-Built Binary

You can directly run the application without building it from source. The pre-built binary and Node.js server are available in the following zip file:

[Download bin-ABXExchange.zip](https://github.com/Mohammad-Imran01/ABX_EXCHANGE_SOLUTION/blob/master/bin-ABXExchange.zip)

### Steps to Run the Pre-Built Binary:
1. Download and extract the `bin-ABXExchange.zip` file.
2. Navigate to the extracted folder.
3. Run the application binary (e.g., `ABXExchange.exe`).
4. The Node.js server is included in the zip file for additional functionality (if needed).

## Building the Project from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/Mohammad-Imran01/ABX_EXCHANGE_SOLUTION.git
   cd ABX_EXCHANGE_SOLUTION
