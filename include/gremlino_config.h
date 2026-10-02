#pragma once

#define GREMLINO_VERSION "1.0.0"

#ifndef GREMLINO_AP_PASSWORD
#define GREMLINO_AP_PASSWORD "gremlino!"
#endif

#ifndef GREMLINO_AP_PREFIX
#define GREMLINO_AP_PREFIX "Gremlino"
#endif

#ifndef GREMLINO_HOSTNAME
#define GREMLINO_HOSTNAME "gremlino"
#endif

#ifndef GREMLINO_BOOT_PIN
#define GREMLINO_BOOT_PIN 0
#endif

constexpr uint16_t GREMLINO_HTTP_PORT = 80;
constexpr uint16_t GREMLINO_DNS_PORT = 53;
constexpr uint32_t GREMLINO_FACTORY_RESET_HOLD_MS = 7000;
constexpr uint32_t GREMLINO_RESTART_DELAY_MS = 900;
