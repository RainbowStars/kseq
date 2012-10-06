#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/libkern.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/conf.h>
#include <sys/systm.h>
#include <sys/uio.h>

static struct cdev *seq_dev;

static d_read_t  seq_read;
static d_write_t seq_write;
//static d_ioctl_t seq_ioctl;

static struct cdevsw seq_cdevsw = {
    .d_version  = D_VERSION,
    .d_read     = seq_read,
    .d_write    = seq_write,
//    .d_ioctl    = seq_ioctl,
    .d_name     = "seq"
};

static uint32_t seq_counter = 0;
static int error = 1;

static int seq_read(struct cdev *dev __unused, struct uio *uio, int flags __unused) {
    if (error == 0) {
        error = 1;
        return 0;
    } else
        error = 0;
    
    void *buffer;
    buffer = malloc(sizeof(uint32_t), M_TEMP, M_ZERO);
    sprintf(buffer, "%d\n", seq_counter);
    error = uiomove(buffer, sizeof(uint32_t), uio);
    
    seq_counter++;
    uio->uio_resid = 0;
    
    return error;
}

static int seq_write(struct cdev *dev __unused, struct uio *uio, int flags __unused) {
    uio->uio_resid = 0;
    seq_counter = 0;
    return (0);
}

/*
static int seq_ioctl(struct cdef *dev __unused, u_long cmd, caddr_t data __unused, int flags __unused, struct thread *td) {
    return (0);
}
*/

static int seq_modevent(module_t mod __unused, int type, void *data __unused) {
    switch(type) {
        case MOD_LOAD:
            if(bootverbose)
                printf("seq: <seq device>\n");
            seq_dev = make_dev_credf(MAKEDEV_ETERNAL_KLD, &seq_cdevsw, 0, NULL, UID_ROOT, GID_WHEEL, 0666, "seq");
            break;
        case MOD_UNLOAD:
            destroy_dev(seq_dev);
            break;
        case MOD_SHUTDOWN:
            break;
        default:
            return (EOPNOTSUPP);
    }
    return (0);
}

DEV_MODULE(seq, seq_modevent, NULL);
MODULE_VERSION(seq, 0);
