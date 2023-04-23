#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <linux/input.h>

struct abs {
    int x;
    int y;
    int xmax;
    int ymax;
};

int event(int type, int code, int value, struct input_event event) {
    if (event.type == type && event.value == value && event.code == code) {
        return 1;
    }

    return 0;
}

int duration(int start, int stop) {
    return stop - start;
}

int main(int argc, char * argv[]) {
    struct input_event ev;
    struct abs cord;
    int timeout_ms = 10000;
    struct pollfd fds[1];
    int x = 0, y = 0;
    int ret;
    int status[6];
    int ti = 0;
    char command[50];
    char input_dev[] = "/dev/input/event4\0";
    int start = 0, stop, mode = 0;
    cord.x = cord.xmax = 1080;
    cord.y = cord.ymax = 2264;

    if (argc >= 2) {
        strcpy(input_dev, argv[1]);
        if (argc == 7) {
            cord.xmax = strtol(argv[2], NULL, 10);
            cord.ymax = strtol(argv[3], NULL, 10);
            cord.x = strtol(argv[4], NULL, 10);
            cord.y = strtol(argv[5], NULL, 10);
            mode = strtol(argv[6], NULL, 10);
        }
    }

    printf("%s %d %d  %d %d  %d %d\n", input_dev, cord.xmax, cord.ymax, cord.x, cord.y, cord.xmax / cord.x, cord.ymax / cord.y);

    fds[0].fd = open(input_dev, O_RDONLY | O_NONBLOCK);
    if (fds[0].fd < 0) {
        printf("error unable open for reading '%s'\n", input_dev);
        return (0);
    }
    fds[0].events = true;
    while (true) {
        ret = poll(fds, 1, timeout_ms);
        if (ret > 0) {
            if (fds[0].revents) {
                ssize_t r = read(fds[0].fd, & ev, sizeof(ev));

                if (r < 0) {
                    printf("error %d\n", (int) r);
                    break;
                } else {

                    if (ev.type == 3 && ev.code == 53) {
                        status[0] = ev.value;
                    }
                    if (ev.type == 3 && ev.code == 54) {
                        status[1] = ev.value;
                    }
                    if (ev.type == 3 && ev.code == 47) {
                        status[2] = ev.value;
                    }
                    if (ev.type == 3 && ev.code == 57) {
                        status[3] = ev.value;
                    }
                    if (status[2] == 1 && status[3] != -1) {
                        status[5] = 1;
                    } else {
                        status[5] = 0;
                    }

                    if (mode == 0) {
                        x = status[0] / (cord.xmax / cord.x);
                        y = status[1] / (cord.ymax / cord.y);
                    } else if (mode == 1) {
                        x = cord.x - (status[0] / (cord.xmax / cord.x));
                        y = status[1] / (cord.ymax / cord.y);
                    } else if (mode == 2) {
                        x = (status[0] / (cord.xmax / cord.x));
                        y = cord.y - (status[1] / (cord.ymax / cord.y));
                    } else if (mode == 3) {
                        x = cord.x - (status[0] / (cord.xmax / cord.x));
                        y = cord.y - (status[1] / (cord.ymax / cord.y));
                    } else if (mode == 4) {
                        y = (status[0] / (cord.xmax / cord.x));
                        x = (status[1] / (cord.ymax / cord.y));
                    } else if (mode == 5) {
                        y = cord.x - (status[0] / (cord.xmax / cord.x));
                        x = (status[1] / (cord.ymax / cord.y));
                    } else if (mode == 6) {
                        y = (status[0] / (cord.xmax / cord.x));
                        x = cord.y - (status[1] / (cord.ymax / cord.y));
                    } else if (mode == 7) {
                        y = cord.x - (status[0] / (cord.xmax / cord.x));
                        x = cord.y - (status[1] / (cord.ymax / cord.y));
                    }

                    if (ev.type == 3) {
                        if (ev.code == 53) {}
                        if (ev.code == 54) {
                            if (status[5] == 0) {
                                sprintf(command,"xdotool mousemove %d %d",x,y);system(command);
                                printf("%d %d \n", x, y);
                            } else if (status[5] == 1) {
                                printf("hold %d %d \n", x, y); sprintf(command,"xdotool mousemove %d %d mousedown 1",x,y);system(command);
                            }
                        }
                    } else if (event(1, 330, 1, ev) == 1) {
                        start = ev.time.tv_sec;
                        printf("start %d\n", start);
                        ti++;
                    }

                    if (event(1, 330, 0, ev) == 1) {
                        stop = ev.time.tv_sec;
                        printf("stop %d  %d\n", stop, duration(start, stop));
                        if (duration(start, stop) < 1) {
                            printf("\t\t(left click)\n");system("xdotool click 1");
                        } else if (duration(start, stop) >= 1 && duration(start, stop) <= 2) {
                            printf("\t\t(right click)\n");system("xdotool click 3");
                        }
                    }
                }
            } else {
                printf("error\n");
            }
        } else {
            printf("timeout...Exiting");
            break;
        }
    }

    close(fds[0].fd);
    return 0;
}
