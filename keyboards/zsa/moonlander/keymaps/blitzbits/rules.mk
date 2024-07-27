CONSOLE_ENABLE = no
COMMAND_ENABLE = no
NO_USB_STARTUP_CHECK = yes
TAP_DANCE_ENABLE = yes
NKRO_ENABLE = no
MIDI_ENABLE = no
WAIT_FOR_USB = yes
MOUSEKEY_ENABLE = yes # need to test this with kvm
#define NO_MUSIC_MODE
AUDIO_ENABLE = yes
#LTO_ENABLE = yes

RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE  = yes

SWAP_HANDS_ENABLE = yes
LEADER_ENABLE = no
KEY_LOCK_ENABLE = no


SRC += features/casemodes.c
SRC += features/leader.c
SRC += features/achordion.c
SRC += features/layer_lock.c
