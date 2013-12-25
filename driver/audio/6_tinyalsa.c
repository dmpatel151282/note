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
源码：
  pcm.c             for control interface
  mixer.c           for pcm interface
  tinyplay.c        for play
  tinycap.c         for capture
  tinymixer.c       for mixer

---------------------------------------------------
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

struct pcm *pcm_open(unsigned int card, unsigned int device,
        unsigned int flags, struct pcm_config *config);
flags:
PCM_OUT        
PCM_IN    PCM_MMAP PCM_NOIRQ PCM_NORESTART       
          
int pcm_is_ready(struct pcm *pcm);

struct pcm_params
struct pcm_params *pcm_params_get(unsigned int card, unsigned int device,
        unsigned int flags);
int pcm_get_config(struct pcm *pcm, struct pcm_config *config);
int pcm_set_config(struct pcm *pcm, struct pcm_config *config);

int pcm_write(struct pcm *pcm, const void *data, unsigned int count);
int pcm_read(struct pcm *pcm, void *data, unsigned int count);

int pcm_start(struct pcm *pcm);
int pcm_stop(struct pcm *pcm);

int pcm_close(struct pcm *pcm);
