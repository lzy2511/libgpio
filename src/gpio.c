/* libgpio
 *
 * Copyright 2017 Manuel Traut <544088192@qq.com>
 *
 * LGPL licensed
 */
#include <gpio.h>

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SYSFS     "/sys/class/gpio/"
#define EXPORT    SYSFS"export"
#define UNEXPORT  SYSFS"unexport"
#define STR_LEN   64

#ifdef DEBUG
#define debug(...) printf (__VA_ARGS__)
#else
#define debug(...)
#endif

static int gpio_open (unsigned int gpio, int mode);
static int do_export_unexport (unsigned gpio, int export);
static int do_direction (unsigned int gpio, char *direction);

/*-----------------------------------------------------------------------------
 *  lora interface
 *-----------------------------------------------------------------------------*/
#define RDONLY        0
#define RDWR          1

int gpio_irq_timed_wait (unsigned int gpio, int *value, int timeout_ms)
{
    int ret = 0;
    int fd = 0;
    
    struct pollfd irqdesc = {
        .events = POLLPRI | POLLERR ,
    };
    fd = gpio_open(gpio, RDONLY);

    if (fd < 0)
    {
        fprintf (stderr, "%s can't access gpio %d\n", __func__, gpio);
        ret = -1;
        return ret;
    }

    irqdesc.fd = fd;

    ret = poll (&irqdesc, 1, timeout_ms);

    if (ret == -1)
    {
        ret = -errno;
        close (fd);
        fd = -1;
        fprintf (stderr, "wait for irq failed: %d\n", ret);
        return ret;
    }
    close (fd);
    /* timeout */
    if (ret == 0)
    {
        return 1;
    }
    *value = gpio_get_value(gpio);
    return ret;
}


int gpio_irq_wait (unsigned int gpio, int *value)
{
    return gpio_irq_timed_wait (gpio, value, -1);
}

static int set_irq (unsigned int gpio, gpio_irq_mode_e m)
{
    char c[STR_LEN];
    int fd, ret = 0;

    ret = snprintf (c, STR_LEN, SYSFS"gpio%d/edge", gpio);

    if (ret < 0)
    {
        fprintf (stderr, "cannot create edge sysfs string for gpio %d\n", gpio);
        return ret;
    }

    fd = open (c, O_RDWR);

    if (fd < 0)
    {
        ret = -errno;
        fprintf (stderr, "gpio %d open edge fd failed: %d\n", gpio, ret);
        return ret;
    }

    switch (m)
    {
        case GPIO_RISING:
            ret = write (fd, "rising", 6);
            if (ret != 6)
                ret = -1;
            break;
        case GPIO_FALLING:
            ret = write (fd, "falling", 7);
            if (ret != 7)
                ret = -1;
            break;
        case GPIO_BOTH:
            ret = write (fd, "both", 4);
            if (ret != 4)
                ret = -1;
            break;
        case GPIO_NONE:
            ret = write (fd, "none", 4);
            if (ret != 4)
                ret = -1;
            break;
        default:
            fprintf (stderr, "unknown irq mode\n");
            return -1;
    }

    if (ret == -1)
    {
        ret = -errno;
        fprintf (stderr, "set edge on gpio %d failed: %d\n", gpio, ret);
        goto close_out;
    }

    ret = 0;

close_out:
    close (fd);

    return ret;

}

int gpio_enable_irq (unsigned int gpio, gpio_irq_mode_e m)
{
    debug ("%s: %d\n", __func__, gpio);

    return set_irq (gpio, m);
}

static int gpio_open (unsigned int gpio, int mode)
{
    char c[STR_LEN];
    int ret = 0;
    int fd = -1;
    
    ret = snprintf (c, STR_LEN, SYSFS"gpio%d/value", gpio);

    if (ret < 0)
    {
        fprintf (stderr, "cannot create value sysfs string for gpio %d\n", gpio);
        return ret;
    }

    ret = 0;

    switch (mode)
    {
        case RDWR:
            fd = open (c, O_RDWR);
            break;
        case RDONLY:
            fd = open (c, O_RDONLY);
            break;
        default:
            fprintf (stderr, "gpio %d direction not set\n", gpio);
            return -EINVAL;
    }

    if (fd < 0)
    {
        ret = -errno;
        fprintf (stderr, "gpio %d open value fd failed: %d\n", gpio, ret);
    }

    return fd;
}
int gpio_get_value (unsigned int gpio)
{
    int ret = 0;
    char c;
    int fd = -1;
    
    debug ("%s: %d\n", __func__, gpio);

    fd = gpio_open(gpio, RDONLY);

    if (fd < 0)
    {
        ret = -1;
        fprintf (stderr, "%s can't access gpio %d\n", __func__, gpio);
        return ret;
    }

    ret = lseek (fd, 0x00, SEEK_SET);

    if (ret == -1)
    {
        ret = -errno;
        fprintf (stderr, "%s rewind of gpio %d failed %d\n", __func__,
            gpio, ret);

        close (fd);
        return ret;
    }

    ret = read (fd, &c, 1);

    if (ret != 1)
    {
        ret = -errno;
        fprintf (stderr, "%s read value of gpio %d failed %d\n", __func__,
            gpio, ret);

        close (fd);
        return ret;
    }

    ret = 0;

    switch (c)
    {
        case '0':
            ret    = GPIO_LOW;
            break;
        case '1':
            ret   = GPIO_HIGH;
            break;
        default:
            c = -1;
            fprintf (stderr, "%s value %x of gpio %d unknown\n", __func__,
                c, gpio);
            return -EINVAL;
    }
    close (fd);
    return ret;
}

int gpio_set_value (unsigned int gpio, int value)
{
    int ret = 0;
    char c;
    int fd = -1;

    fd = gpio_open(gpio, RDWR);

    if (fd <= 0)
    {
        fprintf (stderr, "%s can't access gpio %d\n", __func__, gpio);
        return ret;
    }
    if (ret)
    {
        fprintf (stderr, "%s can't access gpio %d\n", __func__, gpio);
        return ret;
    }

    switch (value)
    {
        case GPIO_LOW:
            c = '0';
            break;
        case GPIO_HIGH:
            c = '1';
            break;
        default:
            fprintf (stderr, "%s value %d of gpio %d unknown\n", __func__,
                value, gpio);
            return -EINVAL;
    }

    ret = write (fd, &c, 1);

    if (ret != 1)
    {
        ret = -errno;
        fprintf (stderr, "%s write value of gpio %d failed %d\n", __func__,
            gpio, ret);

        close (fd);
        return ret;
    }
    close(fd);
    return 0;
}
int gpio_direction_input(unsigned int gpio)
{
    int ret = 0;
    ret = do_direction (gpio, "in\0");
    return ret;
}
int gpio_direction_output(unsigned int gpio, int value)
{
    int ret = 0;
    ret = do_direction (gpio, "out\0");
    if (ret < 0)
    {
        return ret;
    }
    ret = gpio_set_value(gpio, value);
    return ret;
}
static int do_direction (unsigned int gpio, char *direction)
{
    char c[STR_LEN];
    int fd, ret = 0;

    ret = snprintf (c, STR_LEN, SYSFS"gpio%d/direction", gpio);

    if (ret < 0)
    {
        fprintf (stderr, "cannot create dir sysfs string for gpio %d\n", gpio);
        return ret;
    }

    fd = open (c, O_RDWR);

    if (fd < 0)
    {
        ret = -errno;
        fprintf (stderr, "gpio %d open dir fd failed: %d\n", gpio, ret);
        return ret;
    }

    ret = write (fd, direction, strnlen (direction, 3));

    if (ret != strnlen (direction, 3))
    {
        ret = -errno;
        fprintf (stderr, "set direction: %s on gpio %d failed: %d\n",
            direction, gpio, ret);
        goto close_out;
    }
    ret = 0;

close_out:
    close (fd);
    return ret;
}

int gpio_export(unsigned gpio)
{
    return do_export_unexport(gpio, 1);
}
void gpio_unexport(unsigned gpio)
{
    do_export_unexport(gpio, 0);
}
static int do_export_unexport (unsigned gpio, int export)
{
    char c[STR_LEN];
    ssize_t len;
    int ret = 0;

    int fd;

    if (export)
        fd = open (EXPORT, O_WRONLY);
    else
        fd = open (UNEXPORT, O_WRONLY);

    if (fd == -1)
    {
        ret = errno;
        if (export)
            fprintf (stderr, "open %s failed: %d\n", EXPORT, ret);
        else
            fprintf (stderr, "open %s failed: %d\n", UNEXPORT, ret);
        return -ret;
    }

    ret = snprintf (c, STR_LEN, "%d", gpio);

    if (ret < 0)
    {
        fprintf (stderr, "gpio no %d cannot be converted into a string", gpio);
        goto close_out;
    }

    len = write (fd, c, ret);

    if (len != ret)
    {
        if (export)
            fprintf (stderr, "export gpio no %d failed: %d\n", gpio, errno);
        else
            fprintf (stderr, "unexport gpio no %d failed: %d\n", gpio, errno);

        ret = -errno;
        goto close_out;
    }

    ret = 0;
close_out:
    close (fd);
    return ret;
}

