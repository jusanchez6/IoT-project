#include <gpiod.h>
#include <unistd.h>

#define CHIP "/dev/gpiochip0"
#define GPIO11 17
#define GPIO13 27

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *line11, *line13;

    chip = gpiod_chip_open(CHIP);
    line11 = gpiod_chip_get_line(chip, GPIO11);
    line13 = gpiod_chip_get_line(chip, GPIO13);

    gpiod_line_request_output(line11, "blink", 0);
    gpiod_line_request_output(line13, "blink", 0);

    while (1) { // bucle infinito
        gpiod_line_set_value(line11, 1);
        gpiod_line_set_value(line13, 0);
        usleep(500000);

        gpiod_line_set_value(line11, 0);
        gpiod_line_set_value(line13, 1);
        usleep(500000);
    }

    gpiod_line_release(line11);
    gpiod_line_release(line13);
    gpiod_chip_close(chip);

    return 0;
}
