/*
 * This file contains device specific hooks.
 * Always enclose hooks to #if MR_DEVICE_HOOKS >= ver
 * with corresponding hook version!
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>

#if MR_DEVICE_HOOKS >= 1

int mrom_hook_after_android_mounts(const char *busybox_path, const char *base_path, int type)
{
    return 0;
}

#endif /* MR_DEVICE_HOOKS >= 1 */

#if MR_DEVICE_HOOKS >= 2

void mrom_hook_before_fb_close(void)
{
    // Screen gets cleared immediatelly after closing the framebuffer on this device,
    // give user a while to read the message box until it dissapears.
    usleep(800000);
}

#endif /* MR_DEVICE_HOOKS >= 2 */
