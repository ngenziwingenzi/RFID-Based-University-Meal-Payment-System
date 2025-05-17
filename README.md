# RFID-Based University Meal Payment System

This project is a smart meal payment system designed for universities using RFID cards and ESP8266 modules. Students scan their RFID cards at a meal station to deduct their meal balance, while administrators can refill balances using a separate admin RFID scanner. The backend is powered by Django and the system communicates over Wi-Fi, ensuring real-time updates without physical USB connections.

## 🔧 Technologies Used

### ⚙️ Backend
- **Django** (Python): Manages student profiles, card balances, transaction logs, and API endpoints.
- **SQLite**: Lightweight database for development (can be switched to PostgreSQL in production).
- **REST API**: Two endpoints for student scans and admin refills.

### 💡 Microcontroller & Embedded
- **ESP8266 (NodeMCU)**: Microcontroller handling RFID scanning and Wi-Fi communication.
- **MFRC522 RFID Reader**: Scans student/admin RFID cards.
- **C++ with Arduino Framework**: Logic for sending HTTP requests from ESP modules.

### 🌐 Communication
- **Wi-Fi (Local Network)**: ESP8266 modules communicate with the Django server using HTTP POST requests.
- **PlatformIO in VS Code**: Used for uploading firmware and monitoring serial output.

## 📁 Project Structure

Updated mealpaymentproject/
│
├── esp8266_devices/             # Firmware for ESP8266 modules
│   ├── admin_device/           # Handles RFID card refill actions
│   └── student_device/         # Handles RFID meal scan actions
│
├── meal_system/                # Django backend project
│   ├── meal_system/            # Django settings, URLs, and WSGI config
│   └── meals/                  # Core app handling students and logs
│       └── models.py           # Defines Student and Log models
│
├── db.sqlite3                  # SQLite database (used during development)
├── manage.py                   # Django project manager
├── venv/                       # Python virtual environment
└── README.md                   # Project documentation (to be added)


## 🚀 How It Works

1. **Admin Device** scans a refill card and sends UID + amount to the backend.
2. **Student Device** scans student cards and deducts a meal cost.
3. Backend validates and updates the balance; logs the transaction.

## 📲 Endpoints

- `POST /api/refill/` → Accepts `{ "uid": "<admin_uid>", "amount": <int> }`
- `POST /api/scan/` → Accepts `{ "uid": "<student_uid>" }`

## 🛠 Setup Guide

1. **Django Server**:
   - Install dependencies: `pip install -r requirements.txt`
   - Run server: `python manage.py runserver 0.0.0.0:8000`
2. **ESP8266**:
   - Update `serverName` in `.ino` files to match your PC's IP (e.g., `http://192.168.0.105:8000/api/...`)
   - Upload code via PlatformIO or Arduino IDE.
3. **RFID Cards**:
   - Pre-register student/admin cards in Django admin or database.
   - Scan to refill or deduct meals.

## 🧪 Testing

- Use Django admin at `http://<your-ip>:8000/admin/` to view students and balances.
- Monitor ESP serial output using PlatformIO terminal.
- Test POST requests with Postman or Python scripts in `/scripts`.

## 🧑‍💻 Authors

Built by a team in Darho's IoT and Embedded Systems Class.
**Cynthia**,
**Sam Pray** &
**ngenziwingenzi**

