#pragma once

// Error codes

/* Failed to initialize glfw */
#define GLIB_FAIL_INIT_GLFW 0x10

/* Failed to read a file */
#define GLIB_FAIL_FILE_READ 0x20

/* Failed to link vertex and fragment shader */
#define GLIB_FAIL_SHADER_LINK 0x30
/* Failed to compile vertex shader */
#define GLIB_FAIL_SHADER_COMPILEv 0x31
/* Failed to compile fragment shader */
#define GLIB_FAIL_SHADER_COMPILEf 0x32

/* Unimplemented Framebuffer type */
#define GLIB_FRAMEBUFFER_UNKNOWN_TYPE 0x40
/* Something went wrong while constructing OpenGL Framebuffer */
#define GLIB_FRAMEBUFFER_NOT_COMPLETE 0x41

/* Failed to load sound file */
#define GLIB_SOUND_FILE_READ_FAIL 0x50
/* Unsupported audio format (mostly occurrs when a sound file has more than 2 channels) */
#define GLIB_SOUND_UNSUPPORTED_FORMAT 0x51
/* Failed to open / access sound file */
#define GLIB_SOUND_FILE_OPEN_FAIL 0x52
/* Internal error */
#define GLIB_SOUND_INTERNAL_ERROR 0x53
/* Device doesn't support the OpenAL Effects extension */
#define GLIB_SOUND_OPENAL_EXT_EFX_NOT_SUPPORTED 0x54