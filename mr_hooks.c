/*
 * This file contains device specific hooks.
 * Always enclose hooks to #if MR_DEVICE_HOOKS >= ver
 * with corresponding hook version!
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>

#include <log.h>

#if MR_DEVICE_HOOKS >= 1
int mrom_hook_after_android_mounts(const char *busybox_path, const char *base_path, int type)
{
    return 0;
}
#endif /* MR_DEVICE_HOOKS >= 1 */


#if MR_DEVICE_HOOKS >= 2
// Screen gets cleared immediatelly after closing the framebuffer on this device,
// give user a while to read the message box until it dissapears.
void mrom_hook_before_fb_close(void)
{
    usleep(800000);
}
#endif /* MR_DEVICE_HOOKS >= 2 */


#if MR_DEVICE_HOOKS >= 3
static time_t gettime(void)
{
    struct timespec ts;
    int ret;

    ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    if (ret < 0) {
        ERROR("clock_gettime(CLOCK_MONOTONIC) failed: %s\n", strerror(errno));
        return 0;
    }

    return ts.tv_sec;
}

static int wait_for_file(const char *filename, int timeout)
{
    struct stat info;
    time_t timeout_time = gettime() + timeout;
    int ret = -1;
    int waited = 0;

    while (gettime() < timeout_time && ((ret = stat(filename, &info)) < 0))
    {
        waited = 1;
        usleep(30000);
    }

    if(ret >= 0)
        return waited;
    else
        return ret;
}

// Some hammerhead kernels are too fast and mmcblk initialization
// occurs a bit too late, wait for it.
void tramp_hook_before_device_init(void)
{
    // boot
    const char *filename = "/sys/devices/msm_sdcc.1/mmc_host/mmc1/mmc1:0001/block/mmcblk0/mmcblk0p19/uevent";
    int ret;

    INFO("Waiting for file %s\n", filename);
    ret = wait_for_file(filename, 5);
    if(ret > 0)
    {
        // had to wait, make sure the init is complete
        usleep(100000);
    }
    else if(ret < 0)
    {
        ERROR("Timeout while waiting for %s!\n", filename);
    }
}
#endif /* MR_DEVICE_HOOKS >= 3 */
