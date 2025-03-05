## Overview

This project is a C++ application that interacts with a stock exchange API to:
1. Request ticker data.
2. Receive and parse the data.
3. Write the parsed data to a JSON file.

The core functionality is encapsulated in the `exchangehandler` class, which handles all tasks. The class is instantiated and used in `main.cpp`.

## Prerequisites

To build and run this project from source, you need the following installed:

- **Qt 6**: The application uses Qt for certain functionalities.
- **MinGW 6.2+**: Required for compiling the C++ code on Windows.

## Pre-Built Binary

You can directly run the application without building it from source. The pre-built binary and Node.js server are available in the following zip file:

[bin-ABXExchange.zip](https://github.com/Mohammad-Imran01/ABX_EXCHANGE_SOLUTION/bin-ABXExchange.zip)

### Steps to Run the Pre-Built Binary:
1. Download and extract the `bin-ABXExchange.zip` file.
2. Navigate to the extracted folder.
3. Run the application binary (e.g., `ABXExchange.exe`).
4. Run the command `node main.js` to start the ABX exchange server.  
   **Note:** Ensure that you have Node.js version 16.17.0 or higher installed on your system.

## Building the Project from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/Mohammad-Imran01/ABX_EXCHANGE_SOLUTION.git
   cd ABX_EXCHANGE_SOLUTION
