#ifndef MK64_DIAGNOSTIC_OPTIONS_H
#define MK64_DIAGNOSTIC_OPTIONS_H
/* One build policy for the boot menu and the logger itself. A saved config
 * or controller chord must not override a logging-disabled build. */
#define MK64_ENABLE_LOGGER_OPTIONS 0
#endif
