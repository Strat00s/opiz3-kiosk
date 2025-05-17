#include <fstream>
#include <string>
#include <stdio.h>

#include <wiringPi.h>
#include <softPwm.h>


#define PWM_PIN 3
#define EN_PIN 14

#define FIFO_PATH "/tmp/bckl_ctrl"

#define EXIT -2


// Main function for controlling pins based on brigtness
void setBrightness(int brt) {
    // clamp
    if (brt > 100)
        brt = 100;

    // turn off fully when below 0
    if (brt < 0) {
        digitalWrite(EN_PIN, 0);
        softPwmWrite(PWM_PIN, 100);
    }
    else {
        digitalWrite(EN_PIN, 1);
        softPwmWrite(PWM_PIN, 100 - brt);
    }
}

// Initialize wiringPi and used pins
bool init() {
    if (wiringPiSetup() != 0)
        return false;

    pinMode(EN_PIN, OUTPUT);
    pinMode(PWM_PIN, OUTPUT);

    if (softPwmCreate(PWM_PIN, 0, 100) != 0)
        return false;

    return true;
}

int main () {
    if (!init()) {
        printf("Failed to initialize wiringPi\n");
        return 1;
    }

    setBrightness(100);

    // read until EXIT
    while (true) {
        // open pipe
        std::ifstream fifo_file(FIFO_PATH);
        if (!fifo_file.is_open()) {
            printf("Failed to open pipe\n");
            return 2;
        }
        printf("Opened pipe %s\n", FIFO_PATH);

        std::string line = "";
        int value = 100;

        // wait for new data in pipe
        while (std::getline(fifo_file, line)) {
            printf("Received: %s\n", line.c_str());
            try {
                value = std::stoi(line);
                if (value == EXIT)
                    break;
                setBrightness(value);
            }
            catch (...) {
                printf("Exception\n");
            }
        }

        // close pipe
        fifo_file.close();
        printf("Pipe closed\n");
        
        if (value == EXIT)
            break;
    }

    printf("Exiting...");

    return 0;
}