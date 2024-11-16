Keylogger Program
This project is a simple keylogger that runs in the background and logs keystrokes to a file, then uploads the logs to a remote server periodically. It also adds itself to the Windows autostart to ensure it runs on system startup.

Features
Key Logging: Captures keystrokes, including special characters when the Shift key is pressed.
Log Uploading: Logs are uploaded to a remote server over HTTP.
Autostart: Adds the program to the system's startup registry so that it runs every time the computer is rebooted.
Console Hiding: Hides the console window to avoid detection.
Installation
Clone the repository to your local machine.
Open the project in a C development environment (such as Visual Studio).
Ensure that wininet.lib is correctly linked to the project for HTTP functionality.
Usage
1. Compile the Program
To compile the program, use a C compiler that supports Windows development. Make sure to link wininet.lib in your project settings.

2. Running the Program
The program runs as a background process and logs keystrokes.
Logs are saved to C:\ProgramData\system_logs.txt.
The logs are uploaded to a remote server at a specified URL (replace "HERE YOUR SERVER" with your server's actual address).
It will attempt to add itself to the Windows startup registry to ensure it starts with Windows.
3. Logs Upload Interval
The program uploads logs at regular intervals (in this case, every 50,000 iterations of the loop). This interval can be adjusted in the code.

4. Autostart
The program automatically registers itself to run on system startup by modifying the Windows registry key:

mathematica
Copy code
HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
The entry is added with the name SystemService, pointing to the executable file.

Notes
Do NOT use this code for malicious activities.
The code is intended for educational purposes and as an example of keylogging behavior.