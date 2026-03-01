#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>

#include "tsensor.h"

#define TSENSOR_NODE            "/dev/ot_tsensor"

static volatile sig_atomic_t stop = 0;

static const struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"enable", required_argument, NULL, 's'},
    {"monitor", required_argument, NULL, 'm'},
    {"status", no_argument, NULL, 'r'},
    {NULL, 0, NULL, 0}
};

/**
 * @brief 程序使用方法
 * 
 * @param[in] name 程序名
 */
static void help(const char* name)
{
    printf("用法: %s [OPTION]... [-s on|off]\n", name);
    printf("    -m, --monitor single|loop 单次/循环; 功能说明: 温度循环监测使能\n");
    printf("    -s, --enable on|off 开/关; 功能说明: Tsensor 使能控制\n");
    printf("    -r, --status 读取当前tsensor配置");
}

/**
 * @brief 解析温度循环监测使能参数
 * 
 * @param name 温度循环监测使能参数名称
 * @return __u8 采集模式; 默认单次采集
 * @retval 0: 单次采集模式
 * @retval 1: 循环采集模式
 */
static __u8 parse_monitor_state(const char* name)
{
    if (strcmp(name, "single") == 0)
        return 0;
    if (strcmp(name, "loop") == 0)
        return 1;

    return 0;
}

/**
 * @brief 解析Tsensor使能参数
 * 
 * @param name Tsensor状态名称
 * @return __u8 使能状态; 默认开启
 * @retval 0: 关闭
 * @retval 1: 开启
 */
static __u8 parse_enable_state(const char* name)
{
    if (strcmp(name, "on") == 0)
        return 1;
    if (strcmp(name, "off") == 0)
        return 0;

    return 1;
}

static void handle_signal(int sig)
{
    switch (sig)
    {
    case SIGINT:
    case SIGTERM:
        stop = 1;
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    int fd, ret, opt;
    __u8 val;
    char buf[32];

    fd = open(TSENSOR_NODE, O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while ((opt = getopt_long(argc, argv, "hs:m:r", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'h':
            help(argv[0]);
            exit(EXIT_SUCCESS);
            break;
        case 's':
            val = parse_enable_state(optarg);
            if (ioctl(fd, SET_TSENSOR_EN, &val) < 0)
            {
                perror("SET_TSENSOR_EN");
                goto ioctl_err;
            }
            break;
        case 'm':
            val = parse_monitor_state(optarg);
            if (ioctl(fd, SET_TSENSOR_MONITOR_EN, &val) < 0)
            {
                perror("SET_TSENSOR_MONITOR_EN");
                goto ioctl_err;
            }
            break;
        case 'r': 
            if (ioctl(fd, GET_TSENSOR_EN, &val) < 0)
            {
                perror("GET_TSENSOR_EN");
                goto ioctl_err;
            }
            printf("Tsensor: %u (0为关闭, 1为开启)\n", val);
            if (ioctl(fd, GET_TSENSOR_MONITOR_EN, &val) < 0)
            {
                perror("GET_TSENSOR_MONITOR_EN");
                goto ioctl_err;
            }
            printf("Tsensor温度循环监测: %u (0为单次采集模式, 1为循环采集模式)\n", val);
            break;
        default:
            break;
        }
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    while (!stop)
    {
        ret = read(fd, buf, sizeof(buf) - 1);
        if (ret < 0)
        {
            perror("read");
            goto read_err;
        }
        buf[ret] = '\0';
        
        printf("\n当前芯片温度为 %s℃\n", buf);
        sleep(1);
    }

    close(fd);

    exit(EXIT_SUCCESS);

ioctl_err:
read_err:
    close(fd);

    exit(EXIT_FAILURE);
}
