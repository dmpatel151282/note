tinyalsa

部分支持PCM Interface 和 Control Interface

1. libtinyalsa.so

2. tinyplay         
Usage: tinyplay file.wav [-D card] [-d device] [-p period_size]
                         [-n n_periods]
3. tinycap          
Usage: tinycap file.wav [-D card] [-d device] [-c channels] [-r rate]
                        [-b bits] [-p period_size] [-n n_periods]
4. tinypcminfo  
Usage: tinypcminfo -D card -d device

-D 0 (card0) -d 0 (device0) 对应： 
                              PCM out: pcmC0D0p
                              PCM in:  pcmC0D0c
-p : period_size
-n : period_count
-b : Sample bits

----------------------------------------------------
external/tinyalsa/
源码：
  pcm.c                 for pcm interface
  mixer.c               for control interface
  tinyplay.c            for tinyplay
  tinycap.c             for tinycap
  tinymixer.c           for mixer

  tinyalsa/asoundlib.h

---------------------------------------------------
pcm 接口

struct pcm_config
channels: unsigned int
rate: unsigned int
period_size: unsigned int
period_count: unsigned int
format: enum pcm_format
start_threshold: unsigned int
stop_threshold: unsigned int
silence_threshold: unsigned int
avail_min: int

struct pcm
fd: int
flags: unsigned int
running: int :1
underruns: int
buffer_size: unsigned int
boundary: unsigned int
config: pcm_config
mmap_status: snd_pcm_mmap_status
mmap_control: snd_pcm_mmap_control
sync_ptr: snd_pcm_sync_ptr

1. struct pcm *pcm_open(unsigned int card, unsigned int device,
        unsigned int flags, struct pcm_config *config);
flags:
PCM_OUT        
PCM_IN    PCM_MMAP PCM_NOIRQ PCM_NORESTART       
          
2. int pcm_is_ready(struct pcm *pcm);

3. unsigned int pcm_get_buffer_size(struct pcm *pcm);

4.1 int pcm_write(struct pcm *pcm, const void *data, unsigned int count);
  static unsigned int pcm_format_to_bits(enum pcm_format format);
  ioctl(pcm->fd, SNDRV_PCM_IOCTL_WRITEI_FRAMES, &x)

4.2 int pcm_read(struct pcm *pcm, void *data, unsigned int count);

5. int pcm_close(struct pcm *pcm);

struct pcm_params
struct pcm_params *pcm_params_get(unsigned int card, unsigned int device,
        unsigned int flags);
int pcm_get_config(struct pcm *pcm, struct pcm_config *config);
int pcm_set_config(struct pcm *pcm, struct pcm_config *config);

int pcm_start(struct pcm *pcm);
int pcm_stop(struct pcm *pcm);

------------------------------------------------------------------------
mixer 接口 

struct mixer
fd: int
count: unsigned int
ctl: struct mixer_ctl *
info: struct snd_ctl_elem_info *

struct mixer_ctl
mixer: struct mixer *
ename: char **
info: struct snd_ctl_elem_info *

struct snd_ctl_elem_info

enum mixer_ctl_type
MIXER_CTL_TYPE_BOOL,
MIXER_CTL_TYPE_INT,
MIXER_CTL_TYPE_ENUM,
MIXER_CTL_TYPE_BYTE,
MIXER_CTL_TYPE_IEC958,
MIXER_CTL_TYPE_INT64,
MIXER_CTL_TYPE_UNKNOWN,
MIXER_CTL_TYPE_MAX,

1. struct mixer *mixer_open(unsigned int card)

2. static void tinymix_set_value(struct mixer *mixer, unsigned int id, unsigned int value)
  struct mixer_ctl *mixer_get_ctl(struct mixer *mixer, unsigned int id)
  enum mixer_ctl_type mixer_ctl_get_type(struct mixer_ctl *ctl)
  unsigned int mixer_ctl_get_num_values(struct mixer_ctl *ctl)
  int mixer_ctl_set_value(struct mixer_ctl *ctl, unsigned int id, int value)

3. void mixer_close(struct mixer *mixer)

struct mixer_ctl *mixer_get_ctl_by_name(struct mixer *mixer, const char *name);

mixer设置
(1) by name
  "Speaker Function"

